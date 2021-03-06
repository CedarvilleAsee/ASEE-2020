// Retrieval bot 4/4/2020 10:01

#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "lineFollowing.h"
#include "turning.h"
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
  
  //static int state = 1;
  readLine();
  if(digitalRead(BUTTON_2) == 0){
    writeToWheels(0,0);
    CurrentState = 1;  
  }

  //waiting state
  if(CurrentState == 1){

    display.sendNum(CurrentState);
    if(digitalRead(BUTTON_1) == 0){
      CurrentState++;
    }
  }
  //-------------------------------Drive center of line and turn off the mothership-------------------------------
  else if(CurrentState == 2){
    display.sendNum(CurrentState + 100 * substate);
    TimeInState += DeltaTime();
    SetDelta();
    if(substate == 1){
      splitFollow(FULL_SPEED/2, LINE_STRICTNESS, 4, 3, true);
      if(amountSeen <= 2){
        TimeInState = 0;
        substate++;
        SetDelta();
      }
    }
    else if (substate == 2){
      lineFollow(FULL_SPEED*1, LINE_STRICTNESS, 4, 3);
      TimeInState += DeltaTime();
      SetDelta();
      if(TimeInState >= 750){
        substate++;
      }
    }

    //Exit conditions
    else if(substate == 3){
      lineFollow(FULL_SPEED/2, LINE_STRICTNESS, 4, 3);
      TimeInState = 0;
      SetDelta();
      CurrentState ++;
    }
    else{
      lineFollow(FULL_SPEED*.75, LINE_STRICTNESS, 4, 3);
      if(amountSeen >= 4){
        substate++;  
      }
    }
  }
  //-------------------------------pick up the first puck on the inside housing. State should be short-------------------------------------
 //Venteral Side up: 0 is right sensor 7 is the left
 //Top Down 0 on left, 7 on right.
  else if(CurrentState == 3){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    lineFollow(FULL_SPEED, LINE_STRICTNESS,5,6);
    if (TimeInState >= 3100 || RIGHT_PUCK == 1){
      if (CurrentState == 3){
        CurrentState ++;
      }
      TimeInState = 0;
    }
  }
  //-----------------------------------------turn to miss the goal and pick up the next puck of the outside housing ---------------------------------------
  else if(CurrentState == 4){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    //if (TimeInState < 2000){  //haven't passed the goal yet
    lineFollow(FULL_SPEED, LINE_STRICTNESS,1,2,1);
    //}
    //else if (TimeInState < 3000){                   //has passed the goal
    //  lineFollow(FULL_SPEED, LINE_STRICTNESS,2,3);
    //}
    //else{
    if(TimeInState == 3000 || LEFT_PUCK == 1){
      TimeInState = 0;
      lineFollow(FULL_SPEED, LINE_STRICTNESS,3,4);
      CurrentState++;
    }
  }
  //-------------------------------------------- Drive on curved path back to mothership and return to state 1------------------------------------
  else if(CurrentState == 5){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    lineFollow(FULL_SPEED, LINE_STRICTNESS,3,4,2);
    if(TimeInState == 8000){
      CurrentState = 1; 
    }
  }
  // this stage should not be used emergency stage
  else{
    display.sendNum(6);
  }
  
  //display.sendNum(digitalRead(BUTTON_1));
}
