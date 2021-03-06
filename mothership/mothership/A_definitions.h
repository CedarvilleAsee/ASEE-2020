//Pin definitions----------------------------------------------------------------------------------------------------------------------------------------------------
#define redButton 32                          //The red start button
#define blueButton 30                         //The blue start button
#define prestartButton -1                     //The pre-start button

//These are likely going away (need to be removed from code)
#define redMiniRobotStartLED -1               //The LED that signals the red mini robot to start
#define blueMiniRobotStartLED -1              //The LED that signals the blue mini robot to start

#define redStatusLED 40                       //Shows the status of the red gate
#define blueStatusLED 38                      //Shows the status of the blue gate

#define robotSensor1 -1                        //Sensor that sees robot 1
#define robotSensor2 -1                        //Sensor that sees robot 2

#define xshutPin 41                           //Pin that disables one of the distance sensors to change the other's address

#define servoPin 9                            //Pin the servo is plugged into

/*
#define redLaunchIn1  -1                      //the following pins are used for controlling the motor controller
#define redLaunchIn2  -1                      //red pins map to the A set
#define redLaunchPWM  -1
#define redLaunchOut1    -1
#define redLaunchOut2 -1

#define blueLaunchIn1 -1                      //blue pins map to the B set
#define blueLaunchIn2 -1                      
#define blueLaunchPWM  -1
#define blueLaunchOut1    -1
#define blueLaunchOut2 -1
*/

//Constants for running----------------------------------------------------------------------------------------------------------------------------------------------
#define TIME_OPEN 3000                          //The amount of time in millis the gate stays open
#define TIME_CLOSED 2000                        //The amount of time in millis the gate stays closed
#define CLOSE_MARGIN 0                          //The amount of time to wait after the gate begins opening
#define OPEN_MARGIN 0                           //The amount of time to not fire right before gate closes

#define RED_DISTANCE 300
#define BLUE_DISTANCE 300

#define MELT_WIRE_TIME 200


#define extendedPosition 130                     //The position the servo is in when the arms are extended (to knock down ramp)
#define compressedPosition 25                    //The position the servo is in when the arms are not extended (starting position)


//Motor controller constants----------------------------------------------------------------------------------------
//have to control motor controller pins directly.
//The motor controller has been changed to enable more direct control
//Can control up to 8 conections
//all pwm pins should be on to enable launch
//**Red Pucks**
#define in1A1st 36 //Red L
#define in2A1st 34 //Red R
#define pwmA1st 8 //Enables Red L and Red R
//**Blue Pucks**
#define in1B1st 26 //Blue L
#define in2B1st 28 //Blue R
#define pwmB1st 7 //Enable Blue L and Blue R
//Blue L, mapped to out1A2nd
#define in1A2nd 37 //Black
#define in2A2nd 35 //unused launch
#define pwmA2nd 2 //Enable Black
//BlueR, mapped to out1B2nd
#define in1B2nd 27 //unused
#define in2B2nd 29 //unused
#define pwmB2nd 3 //enable unused

//Standby
#define STBY1st 33
#define STBY2nd 31
