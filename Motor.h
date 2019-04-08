#pragma once
#include <stdio.h>
#include <Arduino.h>
#include "Config.h"

#define DISTANCE_LIMITTERS_ENABLE //uncomment

#define STOPED 0
#define RUNNING_BACK -1
#define RUNNING_FORW 1


//errors list ->
#define NO_ERROR 0
#define ERROR_SPEED_NO_RESPONSE 1
#define ERROR_RELE 2
#define ERROR_CURRENT_SENSOR 3
#define ERROR_RELE_STUCK 4
/*
#define ERROR_POWER_RELE 2
#define ERROR_BACKWARD_RELE 3
#define ERROR_FORWARD_RELE 4
*/
// <- end

//swap to inverte rele logic
#define ON HIGH
#define OFF LOW

class Motor
{
public:
	Motor(uint16_t forwRele, uint16_t backRele, uint16_t holePin, uint16_t power1Rele, uint16_t power2Rele, uint16_t currentDetector);
	~Motor();
	void update();
	void moveForw();
	void moveBack();
	void setForwLimiter(int value);
	void setBackLimiter(int value);
	void detectionEvent();
	void selfTest(); //test all reley modules and speed sensor
	void stop();
	void clearError();
	int isRunning(); //return 0 if stops, -1 if go back, 1 if go forw
	int isError();
	unsigned int getStepsCounter();

	unsigned int _stepsCounter;
	bool _eventFlag;
	bool getCurrent();

private:

	

	int _motorState;
	int _errorState;
	uint16_t _currentDetector;
	uint16_t _power1Rele;
	uint16_t _power2Rele;
	uint16_t _speedSensor;
	uint16_t _forwRele;
	uint16_t _backRele;
	int _forwLimiter;
	int _backLimiter;
	unsigned long int _prevTime;
	unsigned long int _stopTime;
	
};

