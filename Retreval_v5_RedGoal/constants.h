#ifndef CONSTANTS

#define CONSTANTS

//Puck Constants
const int PUCK_RECIEVED = 300;

//speed constants
const int TURN_SPEED = 65;
const int FULL_SPEED = 120;
const int MAX_SPEED = 80;
const int LINE_STRICTNESS = 20;
const int CENTER_1 = 5;
const int CENTER_2 = 6;

//Turning constants
const int TURN_AMOUNT  = 4;

//message constants
const int PICKUP_EMPTY = 0;
const int PICKUP_RIGHT = 1;
const int PICKUP_LEFT = 2;
const int DEPOSITING = 3;
const int DONE = 4;
const int RACQUETBALL = 5;


//direction constants
const char L = 'l';
const char R = 'r';
const char F = 'f';
const char B = 'b';
const char E = '0';

// Dasch constants
const bool WHEEL_FORWARDS   = true;
const bool WHEEL_BACKWARDS  = false;

const int DO_DUMP = 53;
const int DONT_DUMP = 8;

const int PERSON_CLOSE_RIGHT = 2700;//2000right old
const int PERSON_CLOSE_LEFT = 2600;//1600left old

const int WALL_CLOSE = 1000;//2000 wall
const int AT_FORK = 3600;//3600 fork

const int RIGHT_ARM_UP = 159;//157 up
const int RIGHT_ARM_START = 142;//135 start
const int RIGHT_ARM_WAIT = 120;//120 wait
const int RIGHT_ARM_DOWN = 77;//77 down

const int LEFT_ARM_UP = 33;//33 up
const int LEFT_ARM_START = 48;//48 start
const int LEFT_ARM_WAIT = 72;//72 wait
const int LEFT_ARM_DOWN = 115;//115 down

const int RIGHT_CLAW_OPEN = 95;
const int RIGHT_CLAW_CLOSE = 47;//47

const int LEFT_CLAW_OPEN = 60;//85 open
const int LEFT_CLAW_CLOSE = 110;//110;


#endif
