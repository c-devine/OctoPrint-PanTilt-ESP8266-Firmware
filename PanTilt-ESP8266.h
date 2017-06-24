/*
 * PanTilt_ESP8266.h
 *
 *  Created on: May 6, 2017
 *      Author: Chris
 */

#ifndef PANTILT_ESP8266_H_
#define PANTILT_ESP8266_H_


#define VERSION 1.0
#define SETUP_FILE 			"/setup.html"
#define HOSTNAME 			"wifipantilt"
#define LOCAL_AP_NAME 		"wifipantiltAP"
#define LOCAL_AP_PASSWORD	"password"

#define FLASH_PIN			0
#define LED_8266			2

#define PAN_PIN 5
#define TILT_PIN 4


enum server_mode {
	MODE_OFF, MODE_AP, MODE_STATION
};


bool startApMode();
bool startStationMode(char *ssid, char *password, char *hostname);
void handleSetup();
void handleCommand();
void handleNotFound();
void startWebServer();
void reset();
uint16_t getFlashDelay(server_mode mode);

#endif /* PANTILT_ESP8266_H_ */
