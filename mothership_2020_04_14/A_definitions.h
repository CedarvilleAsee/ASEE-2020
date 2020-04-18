//Pin definitions----------------------------------------------------------------------------------------------------------------------------------------------------
// NEED TO GET ALL PIN NUMBERS
//Servo pins are defined B_functions.h as (servoname).attach(pinNumber)

#define redButton    = 0                          //The red start button
#define blueButton   = 0                         //The blue start button
#define prestartButton   = 0                     //The pre-start button

#define redMiniRobotStartLED     = 0               //The LED that signals the red mini robot to start
#define blueMiniRobotStartLED    = 0              //The LED that signals the blue mini robot to start

#define redStatusLED  = 0                       //Shows the status of the red gate
#define blueStatusLED = 0                      //Shows the status of the blue gate

#define redDistanceSensorPin      = 0               //The pin holding the red side distance sensor
#define blueDistanceSensorPin     = 0              //The pin holding the blue side distance sensor

#define puckSensor1  = 0                        //Sensor that sees puck 1 // NEED PIN NUMBERS
#define puckSensor2  = 0                        //Sensor that sees puck 2
#define puckSensor3  = 0                        //Sensor that sees puck 3
#define puckSensor4  = 0                        //Sensor that sees puck 4


//Constants for running----------------------------------------------------------------------------------------------------------------------------------------------
#define TIME_OPEN  = 3000                          //The amount of time in millis the gate stays open
#define TIME_CLOSED  = 2000                        //The amount of time in millis the gate stays closed
#define CLOSE_MARGIN  = 0010                       //The amount of time to wait after the gate begins opening
#define OPEN_MARGIN  = 0010                        //The amount of time to not fire rigt before gate closes

#define RED_LAUNCHED = 0
#define BLUE_LAUNCHED = 0
//#define RED_GATE_OPEN_DIST -1                 //The value the distance sensor will see when the gate is open
//#define RED_GATE_CLOSED_DIST -1               //The value the distance sensor will see when the gate is closed

//#define BLUE_GATE_OPEN_DIST -1                //The value the distance sensor will see when the gate is open
//#define BLUE_GATE_CLOSED_DIST -1              //The value the distance sensor will see when the gate is closed

#define RED_DISTANCE = 0
#define BLUE_DISTANCE = 0

#define SERVO_1_CLOCKWISE = 0
#define SERVO_1_CCLOCKWISE = 0
#define SERVO_CENTER_CLOCKWISE = 0
#define SERVO_2_CLOCKWISE = 0
#define SERVO_2_CCLOCKWISE = 0

#define RAMP_SERVO_C = xB9
#define RAMP_SERVO_CC = 0

//Servo Constants----------------------------------------------------------------------------------------------------------------------------------------------------
#define rampLaunchServoAngle = 50
