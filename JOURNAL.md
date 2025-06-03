---
title: "Securodoor"
author: "Kashyap S"
description: "A custom doorbell with camera footage and custom sounds"
created_at: "2025-05-30"
---

## May 30, 2025

I got some planning done for my BOM ie what parts I plan on getting.
For the trasmitter (the outdoor component):
- ESP32-CAM Module
- Push Button
- MAX98357A I2S Amp
- 8Ω 2W Speaker
- MT3608 Boost Converter
- TP4056 Module with Protection
- 3.7V 1000mAh LiPo Battery
- Dupont Wires

For the indoor reciever component:
- ESP32 Dev Board
- MAX98357A I2S Amp
- 8Ω 2W Speaker
- MT3608 Boost Converter
- TP4056 Module with Protection
- 3.7V 1000mAh LiPo Battery
- 3d printed casing
- Dupont Wires

## Jun 1, 2025

I realized that building an extra indoor component would be useless since I already have a doorbell at home. So I want to change up the way it works. Essentially, the outdoor component will stay the same, except there will be a cutout at the bottom right above my current doorbell button. So when I press the button on the new doorbell a stick (or something rigid) will push the old button and make a sound inside the house, and I can just record the footage from when it was pressed.

I also want to use IFTTT to send a message to my phone when someone is at the door, just so I can make it work a bit better.

Here is the updated BOM without the need for an indoor reciever component:
- ESP-32 CAM
- FTDI Programming Board
- 5V Active Buzzer
- USB-C Breakout Board
- Open Push Button
- 2x LED (5mm Red)
- MAX9814 Microphone Amplifier Module
- 2x 1k ohm resistor (for LED)
- 2x 5.1k ohm resistor (for the breakout board)
- 10k ohm resistor (for button pull-up)

I plan on using the FTDI just so i can flash the code onto the esp32

**It is 7:40 PM and I'm happy to say that I have FINALLY finished my PCB and Schematic**

Schematic:
![Schematic](https://hc-cdn.hel1.your-objectstorage.com/s/v3/68e2fbb1c290d37b46c5d4ba38b14a61768253b1_image.png)

PCB:
![PCB](https://hc-cdn.hel1.your-objectstorage.com/s/v3/bf3249f4e07fa2c3167fb059ce48c55a4c53417a_image.png)

3D Model of PCB:
![3d Model](https://hc-cdn.hel1.your-objectstorage.com/s/v3/404d0d77049ab98243a4a0ee38572da262918797_image.png)


## Jun 2, 2025
Today, I'll start with the firmware (probably going to use arduino ide and then flashing it to the FTDI board and then flash to the esp-32 cam).

It is 10:19 PM and I've officially completed my CAD design for this project!!

CAD:

https://hc-cdn.hel1.your-objectstorage.com/s/v3/8f79267e691d3053632b8f7fadb7e0d79c361e40_image.png

**Total Hours Spent: 8**
