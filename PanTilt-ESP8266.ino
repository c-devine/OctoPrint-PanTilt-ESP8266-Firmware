#include "Arduino.h"
#include "ConfigManager.h"
#include "PanTilt-ESP8266.h"
#include "Led.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "DynamicRequestHandler.h"
#include "Servo.h"

Led led = Led(LED_BUILTIN);
Led led8266 = Led(LED_8266);
ConfigManager configManager;
config_t config;
Servo pan;
Servo tilt;
ESP8266WebServer server(80);
enum server_mode serverMode = MODE_OFF;

void setup() {

    Serial.begin(115200);

	led.off();
	led8266.off();

	// use flash pin to clear out stored data
	pinMode(FLASH_PIN, INPUT);

	// load config
	if (!configManager.loadConfig(&config)) {
		Serial.println("Error loading configuration file.");
		return;
	}

	// set initial state of servos
	pan.attach(PAN_PIN, config.panMin, config.panMax);
	tilt.attach(TILT_PIN, config.tiltMin, config.tiltMax);
	pan.writeMicroseconds(config.panUs);
	tilt.writeMicroseconds(config.tiltUs);

	// start the web server
	startWebServer();

	// attempt to connect if remote AP configured
	if (strlen(config.remoteAp) != 0) {
		Serial.print(
				"Remote network found in configuration, attempting to connect to: ");
		Serial.println(config.remoteAp);
		if (startStationMode(config.remoteAp, config.remotePassword,
				config.hostname))
			return;
		else {
			// error, so flash 8266 led 10 times rapidly
			led8266.flash(10, 100);
			Serial.print("Connection failed, starting AP mode.");
		}

	}

	// if we get to here, then start up AP mode
	if (!startApMode()) {
		Serial.println("Failed to start Access Point.");
		return;
	}

}

void startWebServer() {

// start the web server
	server.begin();

	String nvp[][2] = { { "TITLE", "Configure Local Network" }, { "WELCOME",
			"Welcome" }, { "HOSTNAME", config.hostname }, { "PANMIN", String(
			config.panMin) }, { "PANMAX", String(config.panMax) }, { "TILTMIN",
			String(config.tiltMin) }, { "TILTMAX", String(config.tiltMax) } };
	DynamicRequestHandler *handler = new DynamicRequestHandler(nvp, 7, SPIFFS,
	SETUP_FILE, "/setup", 0);
	server.addHandler(handler);
	server.on("/setup_network", handleSetup);
	server.on("/set_values", handleCommand);
	server.onNotFound(handleNotFound);
}

bool startApMode() {

	WiFi.mode(WIFI_AP);

	if (!WiFi.softAP(LOCAL_AP_NAME, LOCAL_AP_PASSWORD))
		return false;

	IPAddress ip = WiFi.softAPIP();
	serverMode = MODE_AP;

	Serial.print("Access Point started, please connect to network: ");
	Serial.println(LOCAL_AP_NAME);
	Serial.print("Connect to: http://");
	Serial.print(ip);
	Serial.println("/setup to configure local network.");

	return true;
}

bool startStationMode(char *ssid, char *password, char *hostname) {

	String message = "Connecting to network";
	message += ssid;
	message += " : ";
	message += password;
	Serial.println(message);

// if already in station mode, disconnect
	if (WiFi.getMode() == WIFI_STA)
		WiFi.disconnect();

	WiFi.mode(WIFI_STA);
	WiFi.hostname(hostname);
	WiFi.begin(ssid, password);

	for (int i = 0; i < 20; i++) {
		Serial.print(".");
		if (WiFi.status() == WL_CONNECTED) {
			message = "Connected to network, server IP address is : ";
			message += WiFi.localIP().toString();
			Serial.println(message);
			serverMode = MODE_STATION;
			message = "Host name = ";
			message += WiFi.hostname();
			Serial.println(message);

			// turn on the 8266 led
			led8266.on();
			return true;
		}
		delay(1000);
	}

	return false;

}

void handleNotFound() {

	server.send(200, "text/plain", "hello");
}

void handleSetup() {

	String ssid = server.arg("ssid");
	String password = server.arg("password");
	String hostname = server.arg("hostname");
	uint16_t panMin = server.arg("panMin").toInt();
	uint16_t panMax = server.arg("panMax").toInt();
	uint16_t tiltMin = server.arg("tiltMin").toInt();
	uint16_t tiltMax = server.arg("tiltMax").toInt();

	char *cssid = const_cast<char*>(ssid.c_str());
	char *cpassword = const_cast<char*>(password.c_str());
	char *chostname = const_cast<char*>(hostname.c_str());

// set the new values
	strncpy(config.remoteAp, cssid, 16);
	strncpy(config.remotePassword, cpassword, 16);
	strncpy(config.hostname, chostname, 16);

	config.panMin = panMin;
	config.panMax = panMax;
	config.tiltMin = tiltMin;
	config.tiltMax = tiltMax;

// save the new config and reboot
	configManager.saveConfig(&config);

	// send message to client about connection attempt
	String message = "Attempting to connect to : ";
	message += cssid;
	message += "\nDevice will reboot in 5 seconds.";
	Serial.println(message);
	server.send(200, "text/plain", message);
	delay(5000);
	led.off();

	Serial.println("Rebooting....");
	ESP.restart();

}

// http://wifipantilt/set_values?command=set&panUs=1400&tiltUs=1500
void handleCommand() {

	String command = server.arg("command");
	if (command.equals("setUs")) {
		uint16_t panUs = server.arg("panUs").toInt();
		uint16_t tiltUs = server.arg("tiltUs").toInt();
		pan.writeMicroseconds(panUs);
		tilt.writeMicroseconds(tiltUs);
		config.panUs = panUs;
		config.tiltUs = tiltUs;
		configManager.saveConfig(&config);
	}
	server.send(200, "text/plain", "OK");
}

uint16_t getFlashDelay(server_mode mode) {

	uint16_t delay = 500;
	switch (mode) {
	case MODE_OFF:
		delay = 500;
		break;
	case MODE_AP:
		delay = 100;
		break;
	case MODE_STATION:
		delay = 1000;
		break;
	}
	return delay;
}

void reset() {

	led8266.on();
	memset(&config, 0, sizeof(config_t));
	strncpy(config.hostname, HOSTNAME, 16);
	configManager.saveConfig(&config);
	Serial.println("Rebooting....");
	delay(1000);
	ESP.restart();
}

uint32_t millisCounter;
uint32_t currentMillis;

void loop() {

	// look for "flash" button push - reset configuration
	if (digitalRead(FLASH_PIN) == LOW) {
		reset();
	}

	currentMillis = millis();
	if (currentMillis - millisCounter > getFlashDelay(serverMode)) {

		led.toggle();
		millisCounter = millis();
	}

	if (serverMode != MODE_OFF)
		server.handleClient();
}
