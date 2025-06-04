# Securodoor

Securodoor is a wireless smart doorbell system that uses a esp32-cam in order to monitor your porch!!

## Why did i build this

Welp I just moved into a new house and as expected, there isnt any smart doorbell or anything, so I decided to make my own, at my own ease. I wanted it to be able to send messages to my phone when I have someone at the door and the microphone is so I can hear the other person on the end

## Any cool ideas I had in this project

Since the esp32 cam doesnt have its own microphone module, I decided to use a microphone amplifier and merge the Audio from the mic and the Video from the camera in order to have A/V for my footage

## My images!!

Schematic:
![Schematic](https://hc-cdn.hel1.your-objectstorage.com/s/v3/68e2fbb1c290d37b46c5d4ba38b14a61768253b1_image.png)

PCB:
![PCB](https://hc-cdn.hel1.your-objectstorage.com/s/v3/bf3249f4e07fa2c3167fb059ce48c55a4c53417a_image.png)

3D Model of PCB:
![3d Model](https://hc-cdn.hel1.your-objectstorage.com/s/v3/404d0d77049ab98243a4a0ee38572da262918797_image.png)

I realized that I didn't need a PCB since I wont be using it and instead handwiring everything a bit too late :D

## BOM
---
Product Name|Price|Link|Quantity
Esp32-cam|$6.15|https://www.aliexpress.com/item/1005003472117545.html?spm=a2g0o.productlist.main.1.1a70LFBALFBA1p&algo_pvid=72696f4e-266f-48a9-9997-3a669fbe0acf&algo_exp_id=72696f4e-266f-48a9-9997-3a669fbe0acf-0&pdp_ext_f=%7B%22order%22%3A%222384%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21CAD%212.40%211.70%21%21%211.71%211.21%21%40210313e917489206007412128e3eda%2112000025941403906%21sea%21CA%210%21ABX&curPageLogUid=gFy8A0obYYaC&utparam-url=scene%3Asearch%7Cquery_from%3A|1
Open Push Button|$0.9|https://www.aliexpress.com/item/4000330112297.html?src=bing&aff_short_key=UneMJZVf&aff_platform=true&isdl=y|1
Red Led (I have this)|$5.95|https://www.amazon.com/Novelty-Place-Colors-Yellow-Lights/dp/B086V2M2TT/ref=sr_1_5?crid=UTO5G5OYSZBC&dib=eyJ2IjoiMSJ9.FU1W2K5Fk4I8It6kU5ok8bzE8LV1mg8EppzcEsQs-kGUMxIzzNIZJenl-7IBDZEBQYFVvXhDcxRD082skllTknS6_F7E_4JSym702ZV8YKKsx81FIuXIaR8EEstOB1KvBq4xrzeqK732ZXem58KsjtjUqrQcmJpq-po1RavjD5LaPibk4EgjpknQKT8SidhpGo0kQKTfIVwatbF9T_6jorx6ynkMsTCvcJDLmtfm497oVJIthu1OGUuZBMxEmWPo3Sk1QfmbfgvrrPi68nW8Le898trL_lPLvYXuXBFVbfo.zjf00hv36kOZy-ZC8Ahlg1pRgX6WG-RE57PO14htiAw&dib_tag=se&keywords=red%2Band%2Bblue%2Bled%2Bdiodes&qid=1748920890&sprefix=red%2Band%2Bblue%2Bled%2Bdiodes%2Caps%2C87&sr=8-5&th=1|1
Blue LED (I have this)|$5.95|https://www.amazon.com/Novelty-Place-Colors-Yellow-Lights/dp/B086V2M2TT/ref=sr_1_5?crid=UTO5G5OYSZBC&dib=eyJ2IjoiMSJ9.FU1W2K5Fk4I8It6kU5ok8bzE8LV1mg8EppzcEsQs-kGUMxIzzNIZJenl-7IBDZEBQYFVvXhDcxRD082skllTknS6_F7E_4JSym702ZV8YKKsx81FIuXIaR8EEstOB1KvBq4xrzeqK732ZXem58KsjtjUqrQcmJpq-po1RavjD5LaPibk4EgjpknQKT8SidhpGo0kQKTfIVwatbF9T_6jorx6ynkMsTCvcJDLmtfm497oVJIthu1OGUuZBMxEmWPo3Sk1QfmbfgvrrPi68nW8Le898trL_lPLvYXuXBFVbfo.zjf00hv36kOZy-ZC8Ahlg1pRgX6WG-RE57PO14htiAw&dib_tag=se&keywords=red%2Band%2Bblue%2Bled%2Bdiodes&qid=1748920890&sprefix=red%2Band%2Bblue%2Bled%2Bdiodes%2Caps%2C87&sr=8-5&th=1|1
USB-C Breakout Board|$2.62|https://www.aliexpress.com/item/1005007382623157.html?spm=a2g0o.productlist.main.32.5d5cHfcBHfcBey&algo_pvid=cd91f053-8489-471f-a669-2f400fd3111d&algo_exp_id=cd91f053-8489-471f-a669-2f400fd3111d-31&pdp_ext_f=%7B%22order%22%3A%22165%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%2111.24%213.21%21%21%2180.50%2122.98%21%402101e9a217489210424232121e409a%2112000040516227671%21sea%21CA%210%21ABX&curPageLogUid=OAmWYXXI2BTZ&utparam-url=scene%3Asearch%7Cquery_from%3A|1
Max9814 Amplifier|$1.86|https://www.aliexpress.com/item/1005006072505538.html?spm=a2g0o.productlist.main.6.2b142437j6bhTN&aem_p4p_detail=2025060220253011721976106197750004164266&algo_pvid=82ee39e8-c7c9-4a29-bb8b-62ecb09c7f6b&algo_exp_id=82ee39e8-c7c9-4a29-bb8b-62ecb09c7f6b-5&pdp_ext_f=%7B%22order%22%3A%229%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%212.36%211.86%21%21%2116.88%2113.30%21%402101e9ec17489211305031150e2fe0%2112000035601559129%21sea%21CA%210%21ABX&curPageLogUid=Kss9a65vAzQ1&utparam-url=scene%3Asearch%7Cquery_from%3A&search_p4p_id=2025060220253011721976106197750004164266_2|1
5V Passive Buzzer|$7.28|https://www.amazon.com/Gikfun-Terminals-Passive-Electronic-Arduino/dp/B01GJLE5BS/ref=sr_1_13_sspa?crid=1XXCJRFNZ8ARP&dib=eyJ2IjoiMSJ9.Zln0sMHMaaAvurM-_sBMhEMmhcumn-a3laQ4PirSVp6lsV0DsZ2BbeCO6p-QssUooqW2tME4FRQycLhvgS36X1BJUnnOtzeX5q7VJyyvEqKz3KNuTsaBNklpigxE3Ve8GOmrhGMq3HOuMpYs1OVDK60LCu_5p6iZQMFG7_vn_K2XejkdP006Hlvsbi9bNSIilVbmkKRgHfECAwIURhnpXSCk3mxQMDoD05dgcpLwfs4.8Mn4IaHMfYMarUNGaa7L8V8Lb8dRCpvShf_g2b4HZ7U&dib_tag=se&keywords=5v+passive+buzzer&qid=1748921437&sprefix=5v+passive+buzze%2Caps%2C156&sr=8-13-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9tdGY&psc=1|1
1k Ohm Resistor|$0.1|https://www.digikey.ca/en/products/detail/yageo/CFR-25JT-52-1K/13921014|2
10k Ohm Resistor|$3.99|https://us.amazon.com/California-JOS-Carbon-Resistor-Tolerance/dp/B0BR68QQPF?sr=8-3|2
5.1k Ohm Resistor|$6.05|https://www.amazon.com/Projects-5-1k-Resistors-Choose-Quantity/dp/B0185FD0WG|2
---
