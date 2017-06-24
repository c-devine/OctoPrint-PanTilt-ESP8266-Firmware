/*
 * ConfigManager.cpp
 *
 *  Created on: Jun 18, 2017
 *      Author: Chris
 */

#include "Arduino.h"
#include "FS.h"
#include "ConfigManager.h"
#include "ArduinoJson-v5.8.4.h"

ConfigManager::ConfigManager() {

	// start the file system
	Serial.println("Starting SPIFFS");
	if (!SPIFFS.begin()) {
		Serial.println();
		Serial.println("Problem starting SPIFFS.");
		return;
	}
}

ConfigManager::~ConfigManager() {

	SPIFFS.end();
}

bool ConfigManager::loadConfig(config_t *config) {

	Serial.println("Loading config file.");

	File configFile = SPIFFS.open(CONFIG_FILE, "r");
	if (!configFile) {
		Serial.println("Failed to open config file.");
		return false;
	}

	size_t size = configFile.size();
	std::unique_ptr<char[]> buf(new char[size]);

	configFile.readBytes(buf.get(), size);
	//Serial.println(buf.get());

	StaticJsonBuffer<400> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file.");
		return false;
	}

	strncpy(config->hostname, json["hostname"], 16);
	strncpy(config->remoteAp, json["remote_ap"]["name"], 16);
	strncpy(config->remotePassword, json["remote_ap"]["password"], 16);

	config->panMin = json["pan"]["min"];
	config->panMax = json["pan"]["max"];
	config->panUs = json["pan"]["us"];
	config->tiltMin = json["tilt"]["min"];
	config->tiltMax = json["tilt"]["max"];
	config->tiltUs = json["tilt"]["us"];

	if (config->panMin == 0 && config->panMax == 0) {
		config->panMin = 1000;
		config->panMax = 2000;
		config->panUs = 1500;
		config->tiltMin = 1000;
		config->tiltMax = 2000;
		config->tiltUs = 1500;
	}

	return true;
}

void ConfigManager::saveConfig(config_t *config) {

	StaticJsonBuffer<400> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["hostname"] = config->hostname;

	JsonObject& remote = root.createNestedObject("remote_ap");
	remote["name"] = config->remoteAp;
	remote["password"] = config->remotePassword;

	JsonObject& pan = root.createNestedObject("pan");
	pan["min"] = config->panMin;
	pan["max"] = config->panMax;
	pan["us"] = config->panUs;

	JsonObject& tilt = root.createNestedObject("tilt");
	tilt["min"] = config->tiltMin;
	tilt["max"] = config->tiltMax;
	tilt["us"] = config->tiltUs;

	File configFile = SPIFFS.open(CONFIG_FILE, "w");
	Serial.print("Saving configuration.");
	root.printTo(Serial);
	root.printTo(configFile);
	Serial.println("");

	configFile.close();
}

