#pragma once
#include "Arduino.h"


class Button
{
public:
	Button(uint16_t buttonPin, bool pullUp = false);
	~Button();

	bool isTrigered(bool state);
	bool isRelised();
	bool isPushed();
	bool ifToogled(uint16_t milliseconds);
	bool isToogled(uint16_t milliseconds);

private:
	uint16_t buttonPin;
	bool prevState;
	uint64_t prevMillis;
	uint64_t delayToogle;
	bool toogled;
};

