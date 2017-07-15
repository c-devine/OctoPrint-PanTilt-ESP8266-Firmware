# PanTilt-ESP8266
# OctoPrint-PanTilt-ESP8266-Firmware

ESP8266 firmware to be used with the [OctoPrint-PanTilt-ESP8266](https://github.com/c-devine/OctoPrint-PanTilt-ESP8266) project.

This project was built with the Sloeber eclipse IDE and the esp8266 libraries.


### Make SPIFFS file
*Some Path To Exe*\mkspiffs.exe 
-c ${workspace_loc:/PanTilt-ESP8266}/data -p 256 -b 8192 -s 3125248 ${workspace_loc:/PanTilt-ESP8266}/Release/PanTilt-ESP8266.spiffs.bin

### Upload SPIFFS file
*Some Path To Exe*\esptool.exe
-cd nodemcu -cb 921600 -cp COM6 -vv -ca 0x100000 -cf ${workspace_loc:/PanTilt-ESP8266}/Release/PanTilt-ESP8266.spiffs.bin

## Snapshots

<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/webcam.png?raw=true" width="320" height="240">
<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/board_bottom.png?raw=true" width="320" height="240">
<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/board_top.png?raw=true" width="320" height="240">


## Setup

See [OctoPrint-PanTilt-ESP8266](https://github.com/c-devine/OctoPrint-PanTilt-ESP8266) for setup.

