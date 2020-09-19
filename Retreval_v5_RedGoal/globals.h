#ifndef GLOBALS
#define GLOBALS

//Line following
int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
float TimeInState = 0;
int CurrentState = 1;

//booleans
bool turning = false;
bool pickingUp = false;
bool atWall = false;
bool doPickup = false;

bool atNextLine = false;
//bool LeftMode = true;
//bool RightMode = false;

//mission globals
int missionNum = 1;
char redPath[40];
char neutralPath[40];
char redPickup[40];
char neutralPickup[40];
int neutralSteps;
int redSteps;

//delta time global
unsigned long CurrentTime = 0.0;

int sensorCounter = 0;

int pickupIndex = 0;
int redIndex = 0;
int neutralIndex = 0;
int racquetballIndex = 0;
PT6961 display(DIN, CLOCK, CS);

int mil = 0;

//Puck Grabbing
unsigned long lastLoopTime = 0;

#endif
