#include "Motor.h"



Motor::Motor(uint16_t forwRele, uint16_t backRele, uint16_t holePin, uint16_t power1Rele, uint16_t power2Rele, uint16_t currentDetector) :
	_forwRele(forwRele),
	_backRele(backRele),
	_speedSensor(holePin),
	_power1Rele(power1Rele),
	_power2Rele(power2Rele),
	_currentDetector(currentDetector)
{
	/*configurate output pins*/
	pinMode(_forwRele, OUTPUT);
	digitalWrite(_forwRele, LOW);
	pinMode(_backRele, OUTPUT);
	digitalWrite(_backRele, OFF);
	pinMode(_power1Rele, OUTPUT);
	digitalWrite(_backRele, OFF);
	pinMode(_power2Rele, OUTPUT);
	digitalWrite(_backRele, OFF);
	/*configurate input pins*/
	pinMode(_currentDetector, INPUT);
	digitalWrite(_currentDetector, LOW);
	pinMode(_speedSensor, INPUT);
	digitalWrite(_speedSensor, LOW);

	_errorState = NO_ERROR;
	_motorState = STOPED;
}


Motor::~Motor()
{
}

void Motor::update()
{
	if (_motorState != STOPED) {
		// if no response from speed sensor
#ifdef ENABLE_SPEED_SENSOR

		if ((millis() - _prevTime) > HOLE_DEAD_TIME) {
			_prevTime = millis();
			if (_eventFlag) {
				_eventFlag = false;
				
			}
			else {
				stop();
				_errorState = ERROR_SPEED_NO_RESPONSE;
				
			}
		}

#endif // ENABLE_SPEED_SENSOR

#ifdef DISTANCE_LIMITTERS_ENABLE
		// stops if we overrun steps limiter
		if (_motorState == RUNNING_FORW && _stepsCounter >= _forwLimiter) {
			stop();
		}
		if (_motorState == RUNNING_BACK && _stepsCounter >= _backLimiter) {
			stop();
		}
#endif // DEBUG

		
	}
	else {
#ifdef STOP_CURRENT_CHECK_ENABLE
		//check if we have current when stoped
		if (millis() - _stopTime > CURRENT_TEST_DELAY) {
			if (getCurrent()) {
				_errorState = ERROR_RELE_STUCK;
			}
		}
#endif // STOP_CURRENT_CHECK_ENABLE

	}
}

void Motor::moveForw()
{
	digitalWrite(_power1Rele, ON);
	digitalWrite(_power2Rele, ON);
	delay(10);
	digitalWrite(_forwRele, ON);
	_motorState = RUNNING_FORW;
	_stepsCounter = 0;
	_prevTime = millis();
	_eventFlag = true;
}

void Motor::moveBack()
{
	digitalWrite(_power1Rele, ON);
	digitalWrite(_power2Rele, ON);
	delay(10);
	digitalWrite(_backRele, ON);
	_motorState = RUNNING_BACK;
	_stepsCounter = 0;
	_prevTime = millis();
	_eventFlag = true;
}

void Motor::setForwLimiter(int value)
{
	_forwLimiter = value;
}

void Motor::setBackLimiter(int value)
{
	_backLimiter = value;
}

void Motor::detectionEvent()
{
	if (_motorState == STOPED)
		return;
	_stepsCounter++;
	_eventFlag = true;
	//_prevTime = millis();
}

void Motor::selfTest()
{
	//if we have current when reles is off - get error
	stop();
	digitalWrite(_power1Rele, ON);
	digitalWrite(_backRele, ON);
	delay(500);
	if (getCurrent()) {
		_errorState = ERROR_RELE;
	}
	digitalWrite(_power1Rele, OFF);
	digitalWrite(_backRele, OFF);
	delay(500);
	digitalWrite(_power2Rele, ON);
	digitalWrite(_forwRele, ON);
	delay(500);
	if (getCurrent()) {
		_errorState = ERROR_RELE;
	}
	stop();
	delay(500);
	moveForw();
	delay(200);
	if (!getCurrent()) {
		_errorState = ERROR_CURRENT_SENSOR;
	}
	delay(100);
	stop();
}

void Motor::stop()
{
	digitalWrite(_forwRele, OFF);
	digitalWrite(_backRele, OFF);
	delay(10);
	digitalWrite(_power1Rele, OFF);
	digitalWrite(_power2Rele, OFF);
	_motorState = STOPED;
	delay(300);
	_stopTime = millis();
}

void Motor::clearError()
{
	_errorState = NO_ERROR;
}

int Motor::isRunning()
{
	return _motorState;
}

int Motor::isError()
{
	return _errorState;
}

unsigned int Motor::getStepsCounter()
{
	return (_stepsCounter);
}

bool Motor::getCurrent()
{
	int trueStates = 0;
	int falseStates = 0;
	for (int i = 0; i < 6; ++i) {
		if (analogRead(_currentDetector) > 300) {
			trueStates++;
		}
		else {
			falseStates++;
		}
		delay(6);
	}
	return trueStates > falseStates ? true : false;
}
