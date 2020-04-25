// Code Update As of 4.25.2020
// View comments for functional work


#include <Servo.h>                          //Library used to control servos
#include <ComponentObject.h>                //Used for the range sensors
#include <RangeSensor.h>                    //Used for the range sensors
#include <SparkFun_VL53L1X.h>               //Used for the range sensors
#include <vl53l1x_class.h>                  //Used for the range sensors
#include <vl53l1_error_codes.h>             //Used for the range sensors

//Global variables---------------------------------------------------------------------------------------------------------------------------------------------------
Servo rampServo;

boolean redRun;                             //True if the red button was pressed, false if the blue button was pressed.

unsigned long redTime;                      //The time in which the red gate first switched open to closed
unsigned long blueTime;                     //The time in which the blue gate first switched open to closed

SFEVL53L1X redGate; // distance sensors??
SFEVL53L1X blueGate;

//Handle multiple tabs-----------------------------------------------------------------------------------------------------------------------------------------------
#include "A_definitions.h"                  //The definitions used by the program. If you need to change a number, it's probably in here. This must go before any other #include

#include "B_functions.h"                    //Functions called thorughout the program

void setup() 
{
//  Serial.begin(115200);
//  Serial.println(F("Serial OK"));
    pinMode(LED_BUILTIN, OUTPUT);
    SERVO_1.attach(9);
    SERVO_CENTER.attach(10);
    SERVO_2.attach(11);
    RAMP_SERVO.attach(12)

  Wire.begin();                             //Open i2c for distance sensors
  redGate.begin();                          //Initialize red gate sensor
  blueGate.begin();                         //Initialize blue gate sensor

  initializePins();
}

int loopCounter = 0;
void loop() 
{
  //Don't do anything until the prestart button is pressed
  if (loopCounter == 0) {
    waitForPrestart();
    setupStateRed();              //Get the timing of the red gate
    setupStateBlue();             //Get the timing of the blue gates
    selectRedOrBlue(); 
    loopCounter++;
  }
 //Wait for the red or blue button to be pressed
  sendGoToGetters();              //Tell the retrieval robots to go
  launchProperPucks();          //Launch the pucks in appropriate order
}
