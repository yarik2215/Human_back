#pragma once

/*This file contains options to configurate the programm*/

#define SELF_TEST //uncomment to enable self test
#define ERRORS_ENABLE //uncomment to enable errors reports
//#define STOP_CURRENT_CHECK_ENABLE //uncomment to enable checking if we have current when we stoped
#define CURRENT_TEST_DELAY 10000 //time to wait before check current on relays in milliseconds

//#define ENABLE_ANIMATION //uncomment to enable animation while working

#define HOLE_DEAD_TIME 150	//in milliseconds
#define ENABLE_SPEED_SENSOR //uncomment to enable speed sensor test
#define ENABLE_INTERRUPT //interrupt for speed sensor