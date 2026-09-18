# future_sanchar-jaal

Future_sanchar-jaal is a custom, open-source digital walkie-talkie powered by ESP32-S3. Unlike traditional analog radios those suffer from static or require a central router to work, this device uses the ESP-NOW v2 protocol to create a direct p2p 2.4GHz network. 

## What makes it unique??
It is unique because it used i2s components alongside a massive 1440 bytes transmission buffer which helps to deliver a crisp voice with an ultra-low latency. Also it uses esp32-S3-wroom-1u which has ipex antenna connector in it which has drastically helped to increase the range of walkie talkie. It provides upto 800+ meters range under normal conditions and if with obstacles it provides upto 200 meters range. 

## Why I made it??

Recently my country Nepal suffered a devastating flood which ruined so many human lives. So I thought that if I could make a Lo-Ra walkie-talkie, I could warn the people using the walkie-talkie along the long distance. Also in Nepal, due to difficult geography, the cellular network coverage could be unreliable when trekking and traveling through rural areas. In conclusion, I made this walkie-talkie to solve the real-world problem and help communicate over long distances easily. 

## How to use it?
1. Turn on the slide switch on the front side of the walkie-talkie.
2. The LED will flash Blue when booting and OLED will display "Sanchar-Jaal Boot".
3. Once the screen displays "Talk!! I am Listening..", you are ready and connected to the network.
4. Then press and hold the Push to Talk button in the front then the led will turn RED, and the screen will display "Transmitting.....!". Then you should speak near the microphone grill.
5. We will have paired devices, so when one talks the LED will turn GREEN and audio will automatically play through the speaker in front.
6. If the walkie talkie glitches then there is reset button in the back why can press that to fix the walkie talkie.
7. If the battery finishes then you can charge it through usb c in the left of walkie talkie
8. You can adjust the volume also from the onboard potentiometer to make the desired volume.

## Assembly Instructions:
### 3d print enclosure:
- All cad files are provided in the github repo.
- there are 3 separate cad files to be printed top lid, bottom lid and the base.
- You should print those 3 files separately

### PCB & soldering:
- All required files are provided in Production folder inside pcb_sanchar.
- solder all the smd components first as indicated in silkscreen
- then solder the usb-c breakout board to its assembly place in the left of walkie talkie using a wire i.e connect In+ and In- of tp4056 to the breakout board VBUS and GND respectively.
- then secure all the components in their respective mounts as in CAD. 

### Flashing firmware:
- Connect your FDTI adapter to J1 programming header on the board.
- Open the firmware file in the arduino ide.
- Hit upload -- when the console says connecting hold the BOOT button in the pcb and Press and release the RESET button then release the boot button.
- Wait for upload to finish then press BOOT button to boot the device.
- Then to pair the two devices check the serial monitor and find the MAC-address of DEVICE 1 and paste it into the code for DEVICE 2 (and vice versa). 
  


## Assets:
### Cad Screenshots:
<img width="316" height="590" alt="Screenshot 2026-09-18 at 1 24 17 PM" src="https://github.com/user-attachments/assets/34574f40-5bb5-4184-9ca5-edd4f708430b" />
<img width="411" height="610" alt="Screenshot 2026-09-18 at 1 24 32 PM" src="https://github.com/user-attachments/assets/ace37eee-b63c-4803-b10f-c32d28163cbe" />
<img width="465" height="538" alt="Screenshot 2026-09-18 at 1 24 58 PM" src="https://github.com/user-attachments/assets/8180cb06-f0eb-4e40-94ad-417f508d5f57" />
<img width="610" height="669" alt="Screenshot 2026-09-18 at 1 25 28 PM" src="https://github.com/user-attachments/assets/f38873c1-7ed7-460c-97f3-3647c18d7e0c" />


### CAD link:
fusion360 ::: [Sanchar-jaal-cad](https://a360.co/4xBj2PT)


### Pcb:
<img width="729" height="440" alt="Screenshot 2026-09-18 at 1 17 54 PM" src="https://github.com/user-attachments/assets/f95da22a-30eb-4403-ba9a-d29eb7432903" />
<img width="742" height="422" alt="Screenshot 2026-09-18 at 1 18 21 PM" src="https://github.com/user-attachments/assets/0b07a30f-3cd8-40fa-851f-65cbcc6cf2a6" />
<img width="706" height="389" alt="Screenshot 2026-09-18 at 1 18 38 PM" src="https://github.com/user-attachments/assets/381ef9db-8324-46b5-b09f-e9b20f264022" />


### Schematic:
<img width="891" height="816" alt="Screenshot 2026-09-18 at 1 19 46 PM" src="https://github.com/user-attachments/assets/fb17d69a-fa3b-459f-be97-f8b5fab07137" />



