#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoWebsockets.h>
#include "driver/i2s.h"
#include <SPIFFS.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#define I2S_WS 25
#define I2S_SD 33
#define I2S_SCK 32
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE 16000
#define I2S_SAMPLE_BITS 16
#define I2S_READ_LEN (16 * 1024)
#define RECORD_TIME 10
#define I2S_CHANNEL_NUM 1
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8) * RECORD_TIME

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

IPAddress local_IP(192, 168, 1, 200); // static ip i want
IPAddress gateway(192, 168, 1, 1); // router ip
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

const char* ifttt_host = "maker.ifttt.com";
const char* ifttt_key = "YOUR_IFTTT_KEY";
const char* ifttt_event = "doorbell_pressed";

const int BUTTON_PIN = 2;
const int NETWORK_LED = 14;
const int POWER_LED = 15;

int buttonState = HIGH;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

WebServer server(80);
bool isRecording = false;
bool hasRecording = false;
String lastRecordingTime = "";

uint8_t* audioBuffer;
size_t audioBufferSize = 0;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(NETWORK_LED, OUTPUT);
  pinMode(POWER_LED, OUTPUT);
  
  digitalWrite(POWER_LED, HIGH);
  digitalWrite(NETWORK_LED, LOW);
  
  Serial.println("Initializing ESP32-CAM Smart Doorbell with Audio...");
  
  initI2S();
  initCamera();
  connectToWiFi();
  startWebServer();
  
  Serial.print("Doorbell ready at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.println("Audio/Video recording enabled");
}

void loop() {
  server.handleClient();
  checkWiFiConnection();
  handleButtonPress();
  delay(10);
}

void initI2S() {
  Serial.println("Initializing I2S for audio...");
  
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };
  
  esp_err_t result = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (result != ESP_OK) {
    Serial.printf("Failed to install I2S driver: %d\n", result);
    return;
  }
  
  result = i2s_set_pin(I2S_PORT, &pin_config);
  if (result != ESP_OK) {
    Serial.printf("Failed to set I2S pins: %d\n", result);
    return;
  }
  
  Serial.println("I2S initialized successfully");
}

void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 15;
    config.fb_count = 1;
  }
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    flashErrorLED();
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);
    s->set_brightness(s, 1);
    s->set_saturation(s, -2);
  }
  
  Serial.println("Camera initialized successfully");
}

void connectToWiFi() {
  Serial.print("Setting static IP to: ");
  Serial.println(local_IP);
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed");
  }
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    digitalWrite(NETWORK_LED, !digitalRead(NETWORK_LED));
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(NETWORK_LED, HIGH);
  } else {
    Serial.println("Failed to connect to WiFi");
    digitalWrite(NETWORK_LED, LOW);
  }
}

void startWebServer() {
  server.on("/", HTTP_GET, []() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Smart Doorbell - Live View</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #1a1a1a; color: white; }
        .container { max-width: 800px; margin: 0 auto; }
        .header { text-align: center; margin-bottom: 20px; }
        .video-container { position: relative; background: #000; border-radius: 10px; overflow: hidden; margin-bottom: 20px; }
        .video-stream { width: 100%; height: auto; display: block; }
        .controls { display: flex; justify-content: center; gap: 10px; margin: 20px 0; }
        .btn { padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; }
        .btn-primary { background: #007bff; color: white; }
        .btn-success { background: #28a745; color: white; }
        .btn-danger { background: #dc3545; color: white; }
        .status { text-align: center; margin: 10px 0; padding: 10px; border-radius: 5px; }
        .status.recording { background: #dc3545; }
        .status.idle { background: #28a745; }
        .audio-controls { text-align: center; margin: 20px 0; }
        audio { width: 100%; max-width: 400px; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔔 Smart Doorbell</h1>
            <p>Live Audio/Video Feed</p>
        </div>
        
        <div class="video-container">
            <img class="video-stream" src="/stream" alt="Video Stream">
        </div>
        
        <div class="status idle" id="status">
            System Ready - Monitoring Door
        </div>
        
        <div class="controls">
            <button class="btn btn-primary" onclick="refreshStream()">Refresh Stream</button>
            <button class="btn btn-success" onclick="startRecording()">Start Recording</button>
            <button class="btn btn-danger" onclick="stopRecording()">Stop Recording</button>
        </div>
        
        <div class="audio-controls">
            <h3>Latest Audio Recording</h3>
            <audio controls id="audioPlayer">
                <source src="/audio" type="audio/wav">
                Your browser does not support audio playback.
            </audio>
        </div>
    </div>

    <script>
        function refreshStream() {
            document.querySelector('.video-stream').src = '/stream?' + new Date().getTime();
        }
        
        function startRecording() {
            fetch('/start-recording')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('status').textContent = 'Recording Audio/Video...';
                    document.getElementById('status').className = 'status recording';
                });
        }
        
        function stopRecording() {
            fetch('/stop-recording')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('status').textContent = 'Recording Stopped - Processing...';
                    document.getElementById('status').className = 'status idle';
                    setTimeout(() => {
                        document.getElementById('audioPlayer').load();
                    }, 2000);
                });
        }
        
        setInterval(refreshStream, 30000);
        
        setInterval(() => {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    if (data.recording) {
                        document.getElementById('status').textContent = 'Recording in Progress...';
                        document.getElementById('status').className = 'status recording';
                    } else {
                        document.getElementById('status').textContent = 'System Ready - Monitoring Door';
                        document.getElementById('status').className = 'status idle';
                    }
                });
        }, 2000);
    </script>
</body>
</html>
    )rawliteral";
    server.send(200, "text/html", html);
  });
  
  server.on("/stream", HTTP_GET, []() {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      server.send(500, "text/plain", "Camera capture failed");
      return;
    }
    
    server.sendHeader("Content-Type", "image/jpeg");
    server.sendHeader("Content-Length", String(fb->len));
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
    
    esp_camera_fb_return(fb);
  });
  
  server.on("/start-recording", HTTP_GET, []() {
    startAudioRecording();
    server.send(200, "text/plain", "Recording started");
  });
  
  server.on("/stop-recording", HTTP_GET, []() {
    stopAudioRecording();
    server.send(200, "text/plain", "Recording stopped");
  });
  
  server.on("/audio", HTTP_GET, []() {
    if (hasRecording && audioBuffer != nullptr) {
      server.sendHeader("Content-Type", "audio/wav");
      server.sendHeader("Content-Length", String(audioBufferSize));
      server.send_P(200, "audio/wav", (const char*)audioBuffer, audioBufferSize);
    } else {
      server.send(404, "text/plain", "No audio recording available");
    }
  });
  
  server.on("/status", HTTP_GET, []() {
    String json = "{\"recording\":" + String(isRecording ? "true" : "false") + 
                  ",\"hasAudio\":" + String(hasRecording ? "true" : "false") + 
                  ",\"lastRecording\":\"" + lastRecordingTime + "\"}";
    server.send(200, "application/json", json);
  });
  
  server.begin();
  Serial.println("Web server started");
}

void startAudioRecording() {
  if (isRecording) return;
  
  Serial.println("Starting audio recording...");
  isRecording = true;
  //audio buffer
  if (audioBuffer != nullptr) {
    free(audioBuffer);
  }
  audioBuffer = (uint8_t*)malloc(FLASH_RECORD_SIZE);
  
  if (audioBuffer == nullptr) {
    Serial.println("Failed to allocate audio buffer");
    isRecording = false;
    return;
  }
  
  // record the audio
  size_t bytesRead = 0;
  size_t totalBytesRead = 0;
  
  while (totalBytesRead < FLASH_RECORD_SIZE && isRecording) {
    i2s_read(I2S_PORT, audioBuffer + totalBytesRead, I2S_READ_LEN, &bytesRead, portMAX_DELAY);
    totalBytesRead += bytesRead;
  }
  
  audioBufferSize = totalBytesRead;
  hasRecording = true;
  lastRecordingTime = String(millis());
  
  Serial.printf("Audio recording completed: %d bytes\n", audioBufferSize);
}

void stopAudioRecording() {
  if (!isRecording) return;
  
  isRecording = false;
  Serial.println("Audio recording stopped");
}

void handleButtonPress() {
  int reading = digitalRead(BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        Serial.println("Doorbell button pressed!");
        handleDoorbellTrigger();
      }
    }
  }
  
  lastButtonState = reading;
}

void handleDoorbellTrigger() {
  flashLEDs();
  
  if (!isRecording) {
    startAudioRecording();
    
    delay(10000);
    stopAudioRecording();
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    sendIFTTTNotification();
  }
}

void flashLEDs() {
  for(int i = 0; i < 3; i++) {
    digitalWrite(NETWORK_LED, LOW);
    digitalWrite(POWER_LED, LOW);
    delay(200);
    digitalWrite(NETWORK_LED, HIGH);
    digitalWrite(POWER_LED, HIGH);
    delay(200);
  }
}

void flashErrorLED() {
  for(int i = 0; i < 10; i++) {
    digitalWrite(POWER_LED, !digitalRead(POWER_LED));
    delay(200);
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost! Attempting to reconnect...");
    digitalWrite(NETWORK_LED, LOW);
    
    WiFi.begin(ssid, password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      digitalWrite(NETWORK_LED, !digitalRead(NETWORK_LED));
      delay(500);
      retries++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(NETWORK_LED, HIGH);
    }
  }
}

void sendIFTTTNotification() {
  Serial.println("Sending IFTTT notification...");
  
  HTTPClient http;
  String url = "http://" + String(ifttt_host) + "/trigger/" + String(ifttt_event) + "/with/key/" + String(ifttt_key);
  String cameraURL = "http://" + WiFi.localIP().toString();
  String postData = "{\"value1\":\"" + cameraURL + "\",\"value2\":\"Audio/Video Available\"}";
  
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  
  int httpResponseCode = http.POST(postData);
  
  if (httpResponseCode == 200) {
    Serial.println("Notification sent successfully!");
    for(int i = 0; i < 2; i++) {
      digitalWrite(NETWORK_LED, LOW);
      delay(100);
      digitalWrite(NETWORK_LED, HIGH);
      delay(100);
    }
  } else {
    Serial.printf("Error sending notification: %d\n", httpResponseCode);
  }
  
  http.end();
}