//Pin definitions----------------------------------------------------------------------------------------------------------------------------------------------------

//Servo pins are defined B_functions.h as (servoname).attach(pinNumber)

#define redButton 35                          //The red start button
#define blueButton 36                         //The blue start button
#define prestartButton 30                     //The pre-start button

//These are likely going away
//#define redMiniRobotStartLED -1               //The LED that signals the red mini robot to start
//#define blueMiniRobotStartLED -1              //The LED that signals the blue mini robot to start

#define redStatusLED 38                       //Shows the status of the red gate
#define blueStatusLED 40                      //Shows the status of the blue gate

#define puckSensor1 -1                        //Sensor that sees puck 1
#define puckSensor2 -1                        //Sensor that sees puck 2
#define puckSensor3 -1                        //Sensor that sees puck 3
#define puckSensor4 -1                        //Sensor that sees puck 4

#define xshutPin 41                           //Pin that disables one of the distance sensors to change the other's address


//Constants for running----------------------------------------------------------------------------------------------------------------------------------------------
#define TIME_OPEN 3000                          //The amount of time in millis the gate stays open
#define TIME_CLOSED 2000                        //The amount of time in millis the gate stays closed
#define CLOSE_MARGIN 0                          //The amount of time to wait after the gate begins opening
#define OPEN_MARGIN 0                           //The amount of time to not fire right before gate closes

//I think these are going to be removed
//#define RED_GATE_OPEN_DIST -1                 //The value the distance sensor will see when the gate is open
//#define RED_GATE_CLOSED_DIST -1               //The value the distance sensor will see when the gate is closed

//#define BLUE_GATE_OPEN_DIST -1                //The value the distance sensor will see when the gate is open
//#define BLUE_GATE_CLOSED_DIST -1              //The value the distance sensor will see when the gate is closed


//I don't remember what these are for, looks like the distance of the actual gate? So if you see further than this, the gate must be open? 
#define RED_DISTANCE 300
#define BLUE_DISTANCE -1

//Servo Constants----------------------------------------------------------------------------------------------------------------------------------------------------
//#define rampLaunchServoAngle 50
