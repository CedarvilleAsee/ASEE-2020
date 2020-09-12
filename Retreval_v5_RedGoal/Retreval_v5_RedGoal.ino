// Retrieval bot Drive By Red Goal 4/4/2020 10:01

#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "lineFollowing.h"
//#include "turning.h"
#include "subStates.h"
#include "Time.h"

void setup() {
  display.initDisplay();
  display.sendNum(1234, 1);
  // initialize line sensors
  for(int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }
  

 // initialize motor controllers
  pinMode(WHEEL_DIR_LB, OUTPUT);
  pinMode(WHEEL_DIR_LF, OUTPUT);
  pinMode(WHEEL_DIR_RB, OUTPUT);
  pinMode(WHEEL_DIR_RF, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);


  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);

  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work
}

void loop() {
  
  readLine();
  
  //waiting state
  if(CurrentState == 1){

    display.sendNum(CurrentState);
    if(digitalRead(BUTTON_1) == 0){
      TimeInState = 0;
      SetDelta();
      CurrentState++;
    }
  }
  //-------------------------------Drive center of line and turn off the mothership-------------------------------
  else if(CurrentState == 2){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    
    favorLineFollow(FULL_SPEED*6/7, LINE_STRICTNESS, true, 6);
    if(TimeInState > 3000){
      TimeInState = 0;
      CurrentState++; 
    }
  }
  //-------------------------------pick up the first puck on the Outside housing. Stage should be short-------------------------------------
 //Venteral Side up: 0 is right sensor 7 is the left
 //Top Down 0 on left, 7 on right.
  else if(CurrentState == 3){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    lineFollow(FULL_SPEED, LINE_STRICTNESS,6,5);
    if (analogRead(LEFT_PUCK) <= PUCK_RECIEVED){
      CurrentState++;
      TimeInState = 0;
    }
  }
  //-----------------------------------------turn to miss the goal and pick up the next puck of the Inside housing ---------------------------------------
  else if(CurrentState == 4){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    lineFollow(FULL_SPEED, LINE_STRICTNESS,2,1);  
    //Exit Condition
    if(analogRead(RIGHT_PUCK) <= PUCK_RECIEVED){
      TimeInState = 0;
      CurrentState++;
    }
  }
  //---------------------------------------------------drive centered on the line back to mothership------------------------------------
  else if(CurrentState == 5){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    
    favorLineFollow(FULL_SPEED *7/8, LINE_STRICTNESS, true, 2);
    if(amountSeen == 0){
      writeToWheels(0,0);
      TimeInState = 0;
      CurrentState = 1; 
    }
  }
  // this stage should not be used emergency stage
  else{
    display.sendNum(6);
  }
}
