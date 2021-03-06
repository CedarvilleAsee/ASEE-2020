#ifndef PINS
#define PINS

//display
const int CS    = PC13;
const int CLOCK = PC14;
const int DIN   = PC15;

// Buttons
const int BUTTON_2 = PB4;//stopbutton
const int BUTTON_1 = PB5;//gobutton

// Wheels
const int WHEEL_DIR_LB = PB14;//BIN1
const int WHEEL_DIR_LF = PB15;//BIN2
const int WHEEL_DIR_RB = PB13;//AIN1
const int WHEEL_DIR_RF = PB12;//AIN2

const int WHEEL_SPEED_L = PB1; //PWMB
const int WHEEL_SPEED_R = PB0; //PWMA


// Line sensors

//left to right is 7 to 0
int LINE_SENSOR[] = {PA4, PA5, PA6, PA7, PB3, PA15, PB10, PB11};
//int LINE_SENSOR[] = {PB11, PB10, PA15, PB3, PA7, PA6, PA5, PA4};

// Puck sensors
const int LEFT_PUCK = PA0;
const int RIGHT_PUCK = PA1;
const int PUCK_CLAW = PA2;

//Whisker Sensor
const int WHISKER = PB9;

#endif
