# PanTilt-ESP8266
# OctoPrint-PanTilt-ESP8266-Firmware

ESP8266 firmware to be used with the [OctoPrint-PanTilt-ESP8266](https://github.com/c-devine/OctoPrint-PanTilt-ESP8266) project.

This project was built with the Sloeber eclipse IDE and the esp8266 libraries.


### Make SPIFFS file
*Path To MKSPIFFS*\mkspiffs.exe 
-c ${workspace_loc:/PanTilt-ESP8266}/data -p 256 -b 8192 -s 3125248 ${workspace_loc:/PanTilt-ESP8266}/Release/PanTilt-ESP8266.spiffs.bin

### Upload SPIFFS file
*Path To ESPTOOL*\esptool.exe
-cd nodemcu -cb 921600 -cp COM6 -vv -ca 0x100000 -cf ${workspace_loc:/PanTilt-ESP8266}/binaries/PanTilt-ESP8266.spiffs.bin

### Flash Firmware
*Path to ESPTOOL*/esptool.exe -cd nodemcu -cb 921600 -cp COM6 -ca 0x00000 -cf *Project Path*/binaries/PanTilt-ESP8266.bin 

## Snapshots

<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/webcam.png?raw=true" width="320" height="240">
<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/board_bottom.png?raw=true" width="320" height="240">
<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-ESP8266-Firmware/snapshots/assets/img/board_top.png?raw=true" width="320" height="240">
<img src="https://raw.githubusercontent.com/c-devine/OctoPrint-PanTilt-Nano/snapshots/assets/img/harness.png?raw=true" width="320" height="240">


## Setup

See [OctoPrint-PanTilt-ESP8266](https://github.com/c-devine/OctoPrint-PanTilt-ESP8266) for setup.

