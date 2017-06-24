/*
 * NameValuePair.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Chris
 */

#include "NameValuePair.h"

NameValuePair::NameValuePair() {
	this->name = "";
	this->value = "";
}
NameValuePair::NameValuePair(String name, String value) {
	this->name = name;
	this->value = value;
}

NameValuePair* NameValuePair::createPairs(String nvPairs[][2]) {
	Serial.println(sizeof(nvPairs));
	NameValuePair p[1];
	p[0] = NameValuePair(nvPairs[0][0], nvPairs[0][0]);
	return p;
}
