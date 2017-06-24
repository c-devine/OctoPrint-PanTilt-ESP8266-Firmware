/*
 * ConfigManager.h
 *
 *  Created on: Jun 18, 2017
 *      Author: Chris
 */

#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#define CONFIG_FILE 	"/config.json"

typedef struct {
	char hostname[16];
	char remoteAp[16];
	char remotePassword[16];
	uint16_t panUs;
	uint16_t panMin;
	uint16_t panMax;
	uint16_t tiltUs;
	uint16_t tiltMin;
	uint16_t tiltMax;

} config_t;


class ConfigManager {
public:
	ConfigManager();
	virtual ~ConfigManager();
	bool loadConfig(config_t *config);
	void saveConfig(config_t *config);
};

#endif /* CONFIGMANAGER_H_ */
