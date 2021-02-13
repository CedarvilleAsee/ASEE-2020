#ifndef GLOBALS
#define GLOBALS

//Line following
int sensors[8]     = {0};
int firstLineIndex = -1;
int lastLineIndex  = -1;
int amountSeen     = 0;
float TimeInState  = 0;
float TimeInLastState = 0;
int CurrentState = 1;
int substateFlag = 0;

//delta time global
unsigned long CurrentTime = 0.0;

Servo clawMotor;
bool clawOpen = false;

PT6961 display(DIN, CLOCK, CS);

#endif
