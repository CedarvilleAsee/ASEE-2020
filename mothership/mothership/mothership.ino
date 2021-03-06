
//#define NOSENSOR

#include <Servo.h>                          //Library used to control servos
#include <ComponentObject.h>                //Used for the range sensors
#include <RangeSensor.h>                    //Used for the range sensors
#include <SparkFun_VL53L1X.h>               //Used for the range sensors
#include <vl53l1x_class.h>                  //Used for the range sensors
#include <vl53l1_error_codes.h>             //Used for the range sensors

#include <Wire.h>
#include "SparkFun_VL53L1X.h"


//Global variables---------------------------------------------------------------------------------------------------------------------------------------------------
Servo rampServo;

boolean redRun;                             //True if the red button was pressed, false if the blue button was pressed.

unsigned long redTime;                      //The time in which the red gate was first seen down
unsigned long blueTime;                     //The time in which the blue gate was first seen down

SFEVL53L1X redGate;
SFEVL53L1X blueGate;

//Handle multiple tabs-----------------------------------------------------------------------------------------------------------------------------------------------
#include "A_definitions.h"                  //The definitions used by the program. If you need to change a number, it's probably in here. This must go before any other #include
#include "B_functions.h"                    //Functions called thorughout the program

void setup() 
{
  Serial.begin(115200);
  Serial.println(F("Serial OK"));

  Wire.begin();                             //Open i2c for distance sensors

  initializePins();                         //Set appropriate pins as inputs and outputs
  initializeSensors(true);                      //Setup sensors, including setting their addresses on I2C
  checkSensorValidity();                    //Check for bad sensor input to ensure nothing is obviously wrong
}

void loop() 
{
  
  Serial.println(F("Main loop started"));
  waitForPrestart();            //Don't do anything until the prestart button is pressed
  setupStateRed();              //Get the timing of the red gate
  setupStateBlue();             //Get the timing of the blue gates
  selectRedOrBlue();            //Wait for the red or blue button to be pressed (these act as the start button)
  sendGoToGetters();            //Tell the retrieval robots to go (move the servo to lower the ramp
  //waitForReturn();              //Wait for the retrieval robots to return and drop the pucks into the holes
  //enableLaunch();
  //launchProperPucks();          //Launch the pucks in appropriate order
  Serial.println(F("Main loop finsished"));
  disableLaunch();
  while(1)
  {
    //Stop
  }
 
/*
  //Test code for testing launches
  while(redButton == HIGH)
  {
    
  }
  enableLaunch();
  delay(1000);
  Serial.println("Red button");
  digitalWrite(redStatusLED, HIGH);
  launchRedPuck();
  digitalWrite(redStatusLED, LOW);
  Serial.println("Blue button");
  digitalWrite(blueStatusLED, HIGH);
  launchBluePuck();
  digitalWrite(blueStatusLED, LOW);
  disableLaunch();
  while(1)
  {
    
  }
*/



  
}
