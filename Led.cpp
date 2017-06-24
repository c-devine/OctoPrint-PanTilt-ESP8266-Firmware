#include "Led.h"
#include "math.h"

Led::Led(uint8_t pin) {
	_on = false;
	_pin = pin;
	pinMode(_pin, OUTPUT);
	digitalWrite(_pin, HIGH);
}

Led::~Led() {
	// TODO Auto-generated destructor stub
}

void Led::toggle() {
	if (_on) {
		off();
	} else {
		on();
	}
}

void Led::on() {
	digitalWrite(_pin, LOW);
	_on = true;
}

void Led::off() {
	digitalWrite(_pin, HIGH);
	_on = false;
}

void Led::flash(uint16_t count, uint32_t delayMs) {

	for (uint32_t i = 0; i< count; i++)
	{
		on();
		delay(delayMs);
		off();
		delay(delayMs);
	}
}

//void Led::breath(uint32_t millis) {
	 // float val = (exp(sin(millis/2000.0*PI)) - 0.36787944)*108.0;
	  //analogWrite(_pin, val);
	 // delay(30);
	  //Serial.println(val);
//}
