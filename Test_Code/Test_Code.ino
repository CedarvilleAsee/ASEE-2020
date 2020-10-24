// CREATE FUNCTION, DEVICE TESTING CODE
// TEST MODE USES BLUE GATE
#include <Servo.h>                          //Library used to control servos

// for range sensors
#include <ComponentObject.h>                //Used for the range sensors
#include <RangeSensor.h>                    //Used for the range sensors
#include <SparkFun_VL53L1X.h>               //Used for the range sensors
#include <vl53l1x_class.h>                  //Used for the range sensors
#include <vl53l1_error_codes.h>             //Used for the range sensors
#include <Wire.h>
#include "Adafruit_VL6180X.h"

// for display - wire already included for i2c transmission
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Include definitions and functions------------------------------------------------------------------------------------------------------------------------------------
#include "A_definitions.h"                  //The definitions/constants/global variables used by the program.
#include "B_functions.h"                    //Functions called thorughout the program

void setup() 
{
    Serial.begin(115200); // start serial transmission
    Serial.println(F("Serial Connection Successful"));

    // LEDS used to indicate open and closed gates
    pinMode(redStatusLED, OUTPUT); // to know when gates are open or closed - for blue gate
    pinMode(blueStatusLED, OUTPUT);

    initializePins(); // sets up servos and sensors

    Wire.begin();                             //Open i2c for distance sensors 
//    sendGoToGetters(); // currenly just releases ramp
//    waitForPrestart();

    if (fullTestMode) {
      selectRedOrBlue();
      setupStateRed();              //Get the timing of the red gate
    }
    setupStateBlue();             //Get the timing of the blue gates
}

void loop() 
{
    updateStateLED();
    launchProperPucks();          //Launch the pucks in appropriate order
}
