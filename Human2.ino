// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Human2.ino
    Created:	25.03.2019 17:35:22
    Author:     DESKTOP-VD35GQH\Кисуля
*/


#include "Button.h"
#include "Menu.h"
#include "Motor.h"
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "Config.h"


#define RELE_FORW_PIN 12
#define RELE_BACK_PIN 13
#define RELE1_POWER_PIN 10
#define RELE2_POWER_PIN 11
#define SPEED_DETECTION_PIN 2
#define CURRENT_DETECTOR_PIN A0

Motor myMotor(RELE_FORW_PIN,RELE_BACK_PIN, SPEED_DETECTION_PIN, RELE1_POWER_PIN, RELE2_POWER_PIN, CURRENT_DETECTOR_PIN);

/*Private function declaration*/
void saveToEeprom(void);
void readFromEeprom(void);
void updateLcd(void);
bool errorHandler(void);
void Handler(void);
void animation(void);

void(*resetFunc) (void) = 0;//declare reset function at address 0

/*
	 _____________________________
	|       display 16x2          |
	|_____________________________|

		but1	but2	but3

	but1 : startBackward/stop/left
	but2 : stop/menu
	but3 : startForward/stop/right
*/

#define RS 8
#define ED 7
#define D4 6
#define D5 5
#define D6 4
#define D7 3
LiquidCrystal lcd(RS, ED, D4, D5, D6, D7);

//define buttons pins
#define BUTTON1_PIN A3
#define BUTTON2_PIN A2
#define BUTTON3_PIN A1

Button but1(BUTTON1_PIN, true);
Button but2(BUTTON2_PIN, true);
Button but3(BUTTON3_PIN, true);

int maxStepsForward = 400;
int maxStepsBackward = 400;

/*Menu implementation

	MainScreen
	|->Menu
		|->max.dist.forward
		|->max.dist.backward
		|->save conf.

*/
char fStr[20];
char sStr[20];

IMenu* iMenu;
Menu menu("Menu\0", fStr, sStr);
MenuItemInt mMaxForw("Max >>>\0",  fStr, sStr, &maxStepsForward,  100000, 1, "stp");
MenuItemInt mMaxBack("Max <<<\0", fStr, sStr, &maxStepsBackward, 100000, 1, "stp");
MenuItemEvent mSave("Save to EEPROM\0", fStr, sStr, saveToEeprom, "save");

MainScreen mainScreen(&menu, fStr, sStr);
IMenu * m = &mainScreen;

bool isStart = false;	//true when start moves

unsigned long int animationTime = 0;

// variables to save into eeprom
int forwLimiter;
int backLimiter;


// The setup() function runs once each time the micro-controller starts
void setup()
{
	myMotor.stop();
	
	readFromEeprom();

	lcd.begin(16, 2);

	/*add menu items into the menu*/
	menu.addItem(&mMaxForw);
	menu.addItem(&mMaxBack);
	menu.addItem(&mSave);

	lcd.clear();
	delay(10);
	m->show();
	updateLcd();
	//attach external interrupt for speed sensor
#ifdef ENABLE_INTERRUPT
	attachInterrupt(0, Handler, FALLING);
#endif // ENABLE_INTERRUPT


	//self test request
#ifdef SELF_TEST
	lcd.clear();
	strcpy(fStr, " Do self test?  \0");
	strcpy(sStr, "      Ok      \0");
	updateLcd();
	bool flagChoise = false;
	while (!flagChoise) {
		if (but2.isTrigered(LOW)) {
			strcpy(sStr, " Doing self test   ");
			lcd.clear();
			updateLcd();
			delay(100);
			myMotor.selfTest();
			flagChoise = true;
			strcpy(sStr, " SelfTest done!  ");
			lcd.clear();
			updateLcd();
			delay(800);
		}
	}
#endif // SELF_TEST
		
	
	//if have errors
	if (errorHandler()) {
		while (1);
		//while (!but2.isTrigered(LOW));
		//myMotor.clearError();
	}

	lcd.clear();
	delay(100);
	m->show();
	updateLcd();
	delay(100);
}

// Add the main program code into the continuous loop() function
void loop()
{
	if (isStart)
	{
		if (but1.isTrigered(HIGH) || but2.isTrigered(HIGH) || but3.isTrigered(HIGH))
		{
			//stop
			myMotor.stop();
			delay(100);
			lcd.begin(16, 2);
			lcd.clear();
			m->show();
			strcpy(sStr, "stoped       \0");
			delay(100);
			updateLcd();
			delay(200);
		}

#ifdef ENABLE_ANIMATION
		//move direction animation
		animation();
#endif // ENABLE_ANIMATION

		

		if (myMotor.isRunning() == STOPED) {
			isStart = false;
			delay(100);
			lcd.begin(16, 2);
			lcd.clear();
			delay(100);
			m->show();
			sprintf(sStr, "stoped %u      \0", myMotor.getStepsCounter());
			updateLcd();
			delay(200);
		}
	}
	else
	{
		//Button left pushed
		if (but1.isTrigered(LOW))
		{
			if (m == &mainScreen)
			{
				delay(200);
				lcd.clear();
				delay(100);
				isStart = true;
				strcpy(sStr, "running back  \0");
				updateLcd();
				delay(100);
				myMotor.setBackLimiter(maxStepsBackward);
				myMotor.moveBack();
				return;
			}
			lcd.clear();
			m->left();
			updateLcd();
		}
		//Button left toogled 1sec
		if ((m == &mMaxBack || m == &mMaxForw) && but1.isToogled(1000))
		{
			lcd.clear();
			m->left();
			updateLcd();
			delay(100);
		}
		//Button midle pushed
		if (but2.isTrigered(LOW))
		{
			lcd.clear();
			delay(50);
			m->open(&m);
			updateLcd();
		}
		//Button right pushed
		if (but3.isTrigered(LOW))
		{
			if (m == &mainScreen)
			{

				delay(200);
				lcd.clear();
				delay(100);
				isStart = true;
				strcpy(sStr, "running forw  \0");
				updateLcd();
				delay(100);
				myMotor.setForwLimiter(maxStepsForward);
				myMotor.moveForw();
				return;
			}
			lcd.clear();
			m->right();
			updateLcd();
		}
		//Button right toogled 1sec
		if ((m == &mMaxBack || m == &mMaxForw) && but3.isToogled(1000))
		{
			lcd.clear();
			m->right();
			updateLcd();
			delay(100);
		}
	}

	//error handler
	if (errorHandler()) {
		while (!but2.isTrigered(LOW));
		myMotor.clearError();
		lcd.clear();
		m->show();
		updateLcd();
	}

	//update motor state
	myMotor.update();

	/*
	if (!digitalRead(SPEED_DETECTION_PIN)) {
		//myMotor.detectionEvent();
		Handler();
	}
	*/

}


/*save data into eeprom*/
void saveToEeprom(void)
{
	EEPROM.write(0, true);
	EEPROM.write(1, (maxStepsBackward >> 8));
	EEPROM.write(2, (maxStepsBackward));
	EEPROM.write(3, (maxStepsForward >> 8));
	EEPROM.write(4, (maxStepsForward));

	lcd.clear();
	lcd.write( " __saved__       \0");
	delay(800);
	lcd.clear();
	m->show();
	//updateLcd();
}

/*read data from eeprom*/
void readFromEeprom(void)
{
	maxStepsBackward = ((EEPROM.read(1) << 8) | (EEPROM.read(2)));
	maxStepsForward = ((EEPROM.read(3) << 8) | (EEPROM.read(4)));
}

/*update LCD*/
void updateLcd()
{
	
	cli();
	lcd.print(fStr);
	lcd.setCursor(0, 1);
	lcd.print(sStr);
	sei();

}


bool errorHandler(void)
{
#ifdef ERRORS_ENABLE
	if (myMotor.isError() != NO_ERROR) {
		myMotor.stop();
		delay(100);
		lcd.begin(16, 2);
		strcpy(fStr, "  ERROR!    \0");
		int errorType = myMotor.isError();
		if (errorType == ERROR_SPEED_NO_RESPONSE) {
			//sprintf(sStr, "%u no speed    \0", myMotor.getStepsCounter());
			strcpy(sStr, "no speed response \0");
		}
		else if (errorType == ERROR_RELE) {
			strcpy(sStr, "rele problems \0");
		} 
		else if (errorType == ERROR_CURRENT_SENSOR) {
			strcpy(sStr, "no current     \0");
		}
		else if (errorType == ERROR_RELE_STUCK) {
			strcpy(sStr, "rele stuck!!!     \0");
		}

		lcd.clear();
		delay(50);
		updateLcd();
		delay(100);
		return true;
	}
#endif // ERRORS_ENABLE
	
	return false;
}


//external interrupt handler
void Handler(void)
{
	//tempValIRQ++;
	myMotor.detectionEvent();
}

void animation(void)
{
	static unsigned int counterAnim = 1;
	if (millis() - animationTime > 500) {
		for (int i = 0; i < 20; ++i) {
			sStr[i] = ' ';
		}
		if (myMotor.isRunning() == RUNNING_FORW) {
			sStr[counterAnim] = '>';
			sStr[counterAnim + 1] = '>';
			counterAnim = (counterAnim + 1);
			if (counterAnim == 15) counterAnim = 0;
		} else if(myMotor.isRunning() == RUNNING_BACK) {
			sStr[counterAnim] = '<';
			sStr[counterAnim + 1] = '<';
			counterAnim = (counterAnim - 1);
			if (counterAnim == 0) counterAnim = 15;
		}
		
		animationTime = millis();
		updateLcd();
	}
}