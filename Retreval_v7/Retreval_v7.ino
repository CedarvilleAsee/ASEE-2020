// Retrieval bot Drive By Red Goal 4/4/2020 10:01

#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "lineFollowing.h"
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

  //Initialize the holder
  clawMotor.attach(PUCK_CLAW);
  clawMotor.write(120);
}

void loop() {
  
  readLine();
  if(digitalRead(BUTTON_2) == 0){
    CurrentState = 1;
    writeToWheels(0,0);  
  }
  
  display.sendNum(CurrentState);
  TimeInState += DeltaTime();
  SetDelta();
  //waiting state
  if(CurrentState == 1){
    if(digitalRead(BUTTON_1) == 0){
      TimeInState = 0;
      SetDelta();
      CurrentState++;
    }
    openClaw();
  }
  //-------------------------------Turn left off the mothership-------------------------------
  else if(CurrentState == 2){
    writeToWheels(FULL_SPEED,FULL_SPEED/2);
    if(sensors[3] == HIGH){
      TimeInState = 0;
      CurrentState++; 
    }
  }
  //-------------------------------pick up the first puck on the Outside housing. Stage should be short-------------------------------------
  else if(CurrentState==3){
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS,false, 2, 1);
    if (analogRead(LEFT_PUCK) <= PUCK_RECIEVED){
      closeClaw();
      CurrentState++;
      TimeInState=0;
    }
  }
  //-----------------------------------------turn to miss the goal and pick up the next puck of the Inside housing ---------------------------------------
  else if(CurrentState == 4){
    if(TimeInState>100) openClaw();
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 8);
    if(analogRead(RIGHT_PUCK) <= PUCK_RECIEVED){
      closeClaw();
      TimeInState=0;
      CurrentState++;
    }
  }
  //---------------------------------------------------drive until in front of the mothership--------------------------------
  else if(CurrentState==5){
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, -1, 0);
    if(TimeInState>5000){
      TimeInState = 0;
      CurrentState++;
    }
  }
  //---------------------------Turn to the left to face mothership
  else if(CurrentState==6){
    writeToWheels(FULL_SPEED,-FULL_SPEED/2);
    if(TimeInState>1000){
      CurrentState++;
      TimeInState=0;
    }
  }
  //---------------------------Drive straight into mothership
  else if(CurrentState==7){
    if(TimeInState<700) writeToWheels(FULL_SPEED,FULL_SPEED);
    else{
      favorLineFollow(FULL_SPEED,LINE_STRICTNESS,false,3,4);
      if(amountSeen==0){
        TimeInState=0;
        CurrentState=1;
        writeToWheels(0,0);
      }
    }
  }
  // this stage should not be used emergency stage
  else{
    display.sendNum(7);
  }
}