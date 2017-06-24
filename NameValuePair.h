/*
 * NameValuePair.h
 *
 *  Created on: May 15, 2017
 *      Author: Chris
 */

#ifndef NAMEVALUEPAIR_H_
#define NAMEVALUEPAIR_H_

#include "Arduino.h"

class NameValuePair {
public:
	NameValuePair();
	NameValuePair(String name, String value);
	static NameValuePair *createPairs(String nvPairs[][2]);


	String name;
	String value;
};

#endif /* NAMEVALUEPAIR_H_ */
