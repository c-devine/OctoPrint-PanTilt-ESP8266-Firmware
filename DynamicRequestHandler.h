/*
 * DynamicRequestHandler.h
 *
 *  Created on: May 13, 2017
 *      Author: Chris
 */

#ifndef DYNAMICREQUESTHANDLER_H_
#define DYNAMICREQUESTHANDLER_H_

#include "Arduino.h"
#include "FS.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "detail/RequestHandlersImpl.h"
#include "NameValuePair.h"

class DynamicRequestHandler: public StaticRequestHandler {
public:

	DynamicRequestHandler(String (*nvp)[2], uint16_t numPairs, FS& fs, const char* path,
			const char* uri, const char* cache_header);bool handle(
			ESP8266WebServer& server, HTTPMethod requestMethod,
			String requestUri) override;

	uint16_t _numPairs = 0;
	String (*_nvp)[2];
};

#endif /* DYNAMICREQUESTHANDLER_H_ */
