ENPH Second Year Summer 
=====

## Description 
Repo to host team code, docs, CAD models, circuit schematics and other useful items. 

## Documentation site 
https://i-jey.github.io/robotsummer/

## Calculation Spreadsheet
https://docs.google.com/spreadsheets/d/1G_nlOqhw7OiXAsn64Kg7ZWVn5ETo55PLTQZf5VaNq4Y/edit#gid=240156634

## First Design Review 
https://docs.google.com/presentation/d/1Tin1bqWngJIaByWks2kfljkIxHQzdjXB50zxXt77TIM/edit#slide=id.g3d10a957f8_2_0

STM32
===

## Key pins
---
|Key Pins   	|Description   	|Notes   	|
|:-:	|:-:	|:-:	|
|A9   	|TX1   	|Connect to RX on the Serial port connector   	|
|A10   	|RX1   	|Connect to TX on the Serial port connector   	|
|B6   	|SCL1   	|OLED display   	|
|B7   	|SDA1   	|OLED display   	|

## Uploading code 
---
If the code fails to upload, the error is usually one of the following: 
1. The platformio.ini is missing the `upload_protocol = serial` and `upload_port = COMX` settings.
2. RX / TX pins have been mixed up from the STM to the serial port connector. They should go to the opposite pins on the serial port connector. 
3. The jumper is not in the correct position. ![](https://raw.githubusercontent.com/i-jey/robotsummer/master/images/blue-pill.jpg)

## Pinout Reference
---
![Pinout](http://wiki.stm32duino.com/images/a/ae/Bluepillpinout.gif)

Electrical Circuits
===

## High-side driver H-Bridge
![](https://raw.githubusercontent.com/i-jey/robotsummer/master/images/high-side-driver-H-bridge.PNG)

Tricks
===
1. Capacitors between power and ground prevent the bluepill from turning off when the H-bridges turn on. A 470uF capacitor keeps the bluepill powered at 3.3V for long enough when the voltage of the rest of the battery drops due to the sudden current draw of the h-bridges.
