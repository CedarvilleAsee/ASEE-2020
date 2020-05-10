//Constants for gate checking
#define TIME_OPEN  3000                          //The amount of time in millis the gate stays open
#define TIME_CLOSED 2000                        //The amount of time in millis the gate stays closed
#define CLOSE_MARGIN  0010                       //The amount of time to wait after the gate begins opening
#define OPEN_MARGIN   0030                        //The amount of time to not fire right before gate closes

// Constants for status display (in association with gates)
#define redStatusLED 38 //yellow light
#define blueStatusLED 40 // green light

//////////////////////////// GLOBAL VARIABLES ///////////////////////////////////
// declare servos
Servo servo1;
Servo centerServo;
Servo servo2;
Servo rampServo;

///////////////// SET THESE VALUES FOR TEST RUNS ////////////////////////////////////

#define GATE_DISTANCE 270

// constants to hold puck sensor status, will need to coordinate with puck sensors
int puckSensor1 = 1;
int puckSensor2 = 1;
int puckSensor3 = 1;
int puckSensor4 = 1;

bool prestartPressed = true;
bool choiceMade = true;

// will need to associate buttons
boolean redRun;                             //True if the red button was pressed, false if the blue button was pressed.
bool fullTestMode = false; // will indicate whether full setup is available, modify before upload

//////////////////////////////////////////////////


unsigned long redTime;                      //The time in which the red gate first switched open to closed
unsigned long blueTime;                     //The time in which the blue gate first switched open to closed

SFEVL53L1X redGateSensor; // distance sensors for gate
SFEVL53L1X blueGateSensor;

//////////// For dispaly, ///////////

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool displayAvailable = false;
