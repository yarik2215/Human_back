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

/*

// класс для работы с кнопками через mcp
class McpButton
{
private:
    uint8_t _pin;
    uint8_t _pullup;
    uint8_t _prevState;
    uint8_t _currState;
    uint64_t _startTime;
    uint64_t _prevMillis;
    uint64_t _lastUpdateTime; //если предыдущая проверка была давно то необходимо обнулить таймеры
    uint8_t _toogled;

public:
    McpButton(uint8_t pin, uint8_t pullup);
    ~McpButton();
    void init();
    bool isTriggered(uint8_t state);
    bool isToogled(uint8_t state, uint32_t milliseconds);
    bool isState(uint8_t state);
    bool getState();
};

McpButton::McpButton(uint8_t pin, uint8_t pullup = PULL_UP_DISABLE) :
_pin(pin),
_pullup(pullup)
{
    
}

McpButton::~McpButton()
{
}

void McpButton::init()
{
    initMcp();
    pinModeMcp(_pin, INPUT, _pullup);
    _currState = _prevState = readMcp(_pin);
}

bool McpButton::isTriggered(uint8_t state)
{
    _currState = readMcp(_pin);
	if (_prevState != _currState) {
		if ((millis() - _prevMillis) > 10) {
			_prevState = _currState;
			if (_currState == state) {
				return true;
			}
		}
	}
	else {
		_prevMillis = millis();
	}
	return false;
}

bool McpButton::isToogled(uint8_t state, uint32_t milliseconds)
{
    if(readMcp(_pin) == state && (millis() - _lastUpdateTime) < 1000) {
        if(millis() - _startTime > milliseconds && !_toogled) {
            _toogled = true;
            return true;
        }
    }
    else {
        _toogled = false;
        _startTime = millis();
    }
    _lastUpdateTime = millis();
    return false;
}

//возвращает состояние (зажата кнопка или нет)
bool McpButton::getState()
{
    return _toogled;
}

//просто считывает состояние кнопки
bool McpButton::isState(uint8_t state)
{
    return (readMcp(_pin) == state);
}
*/
