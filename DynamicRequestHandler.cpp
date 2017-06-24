/*
 * DynamicRequestHandler.cpp
 *
 *  Created on: May 13, 2017
 *      Author: Chris
 */

#include "DynamicRequestHandler.h"

DynamicRequestHandler::DynamicRequestHandler(String (*nvp)[2],
		uint16_t numPairs, FS& fs, const char* path, const char* uri,
		const char* cache_header) :
		StaticRequestHandler(fs, path, uri, cache_header) {
	_numPairs = numPairs;
	_nvp = new String[numPairs][2];

	for (int i = 0; i < numPairs; i++) {
		_nvp[i][0] = String(nvp[i][0]);
		_nvp[i][1] = String(nvp[i][1]);
		Serial.print(_nvp[i][0]); Serial.print(" = "); Serial.println(_nvp[i][1]);
	}

}

bool DynamicRequestHandler::handle(ESP8266WebServer& server,
		HTTPMethod requestMethod, String requestUri) {
	if (!canHandle(requestMethod, requestUri))
		return false;

	DEBUGV("DynamicRequestHandler::handle: request=%s _uri=%s\r\n", requestUri.c_str(), _uri.c_str());

	String path(_path);

	if (!_isFile) {
		// Base URI doesn't point to a file.
		// If a directory is requested, look for index file.
		if (requestUri.endsWith("/"))
			requestUri += "index.htm";

		// Append whatever follows this URI in request to get the file path.
		path += requestUri.substring(_baseUriLength);
	}

	DEBUGV("DynamicRequestHandler::handle: path=%s, isFile=%d\r\n", path.c_str(), _isFile);

	String contentType = getContentType(path);

	// look for gz file, only if the original specified path is not a gz.  So part only works to send gzip via content encoding when a non compressed is asked for
	// if you point the the path to gzip you will serve the gzip as content type "application/x-gzip", not text or javascript etc...
	if (!path.endsWith(".gz") && !_fs.exists(path)) {
		String pathWithGz = path + ".gz";
		if (_fs.exists(pathWithGz))
			path += ".gz";
	}

	File f = _fs.open(path, "r");
	if (!f)
		return false;

	if (_cache_header.length() != 0)
		server.sendHeader("Cache-Control", _cache_header);

	while (f.available()) {
		String line = f.readStringUntil('\n');
		if (line.indexOf("%") != -1) {
			for (int i = 0; i < _numPairs; i++) {
				String name = String("%" + _nvp[i][0] + "%");
				if (line.indexOf(name) != -1) {
					line.replace(name, _nvp[i][1]);
					Serial.println(line);
				}
			}
		}
		server.sendContent(line);
	}

	return true;
}

