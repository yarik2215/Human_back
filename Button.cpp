#include "Button.h"

Button::Button(uint16_t buttonPin, bool pullUp) :
buttonPin(buttonPin),
prevMillis(0),
delayToogle(0),
toogled(false)
{
	if (pullUp) {
		pinMode(buttonPin, INPUT);
		digitalWrite(buttonPin, HIGH);
	}
	else {
		pinMode(buttonPin, INPUT);
		digitalWrite(buttonPin, LOW);
	}
	prevState = digitalRead(buttonPin);
}


Button::~Button()
{
}


bool Button::isTrigered(bool state)
{
	// TODO: realize deounce
	bool tempState = digitalRead(buttonPin);
	if (prevState != tempState) {
		if ((millis() - prevMillis) > 10) {
			prevState = tempState;
			if (tempState == state) {
				return true;
			}
		}
	}
	else {
		prevMillis = millis();
	}
	return false;
}


bool Button::isRelised()
{
	return false;
}


bool Button::isPushed()
{
	return false;
}

bool Button::ifToogled(uint16_t milliseconds)
{
	if (digitalRead(buttonPin) == LOW) {
		if ((millis() - delayToogle) > milliseconds && !toogled) {
			toogled = true;
			return true;
		}
	}
	else {
		toogled = false;
		delayToogle = millis();
	}
	return false;
}

bool Button::isToogled(uint16_t milliseconds)
{
	ifToogled(milliseconds);
	return toogled;
}
