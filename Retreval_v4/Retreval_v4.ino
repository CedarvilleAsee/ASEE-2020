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
//Testing Git Hub
//Second Try

//Is this comment still here
//here's another one
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
  
  static int state = 1;
  readLine();

  //waiting state
  if(CurrentState == 1){

    display.sendNum(CurrentState);
    if(digitalRead(BUTTON_1) == 0){
      CurrentState++;
    }
  }
  //-------------------------------Drive center of line and turn off the mothership-------------------------------
  else if(CurrentState == 2){
    display.sendNum(CurrentState);
    if(amountSeen > 0){
      lineFollow(FULL_SPEED, LINE_STRICTNESS, 4, 3);
    }
    else if(sensors[4] == 0){
      TimeInState = 1;
      
      writeToWheels(FULL_SPEED, 0);
      
    }
    //Exit conditions
    else{
      lineFollow(FULL_SPEED, LINE_STRICTNESS, 4, 3);
      TimeInState = 0;
      SetDelta();
      if (CurrentState == 2){
        CurrentState ++;
      }
    }
  }
  //-------------------------------pick up the first puck. Stage should be short-------------------------------------
  else if(CurrentState == 3){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
      lineFollow(FULL_SPEED, LINE_STRICTNESS,5,6);
    if (TimeInState >= 3100){
      if (CurrentState == 3){
        CurrentState ++;
      }
      TimeInState = 0;
    }
  }
  //-----------------------------------------turn to miss the goal and pick up the next puck---------------------------------------
  else if(CurrentState == 4){//(too short and tracking wrong side)
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();
    if (TimeInState < 2000){  //haven't passed the goal yet
      lineFollow(FULL_SPEED, LINE_STRICTNESS,4,5);
    }
    else if (TimeInState < 3000){                   //has passed the goal
      lineFollow(FULL_SPEED, LINE_STRICTNESS,2,3);
    }
    else{
      TimeInState = 0;
      lineFollow(FULL_SPEED, LINE_STRICTNESS,3,4);
      CurrentState++;
    }
  }
  //---------------------------------------------------drive centered on the line back to mothership------------------------------------
  else if(CurrentState == 5){
    display.sendNum(CurrentState);
    TimeInState += DeltaTime();
    SetDelta();

    if(TimeInState < 5000){
      lineFollow(FULL_SPEED, LINE_STRICTNESS, 2, 1);  
    }
    else if(TimeInState < 6000){
      writeToWheels(FULL_SPEED/4, FULL_SPEED/2);
    }
    else if(TimeInState < 8000){
        lineFollow(FULL_SPEED, LINE_STRICTNESS, 4, 3);
    }
    //if(TimeInState < 10000){
    else{
        writeToWheels(0, 0);
    }
    
  }
  // this stage should not be used emergency stage
  else{
    display.sendNum(6);
  }
  
  //display.sendNum(digitalRead(BUTTON_1));
  /*switch(state) {

    case 0:
      if(waitState()) {
        state++; // button 1 to start
      }
      break;
    case 1:
      //display.sendNum(1);
      //writeToWheels(100, 100);
      
      lineFollow(FULL_SPEED, LINE_STRICTNESS);
      break;
      
  }*/
}
