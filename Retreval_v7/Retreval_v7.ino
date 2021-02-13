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
bool test = true;

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
    
    if(amountSeen == 0){
      substateFlag = 1;
      writeToWheels((FULL_SPEED),(FULL_SPEED)/10);
    }
    else if(substateFlag == 1 && sensors[3] == HIGH){
    //if(sensors[3] == HIGH){
      substateFlag = 0;
      TimeInState = 0;
      CurrentState++; 
    }
    else{
      writeToWheels(FULL_SPEED,(4*FULL_SPEED)/5);  
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
    if(TimeInState > 100){ 
      openClaw();
      favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 8);
    }
    else{
      favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 3);
    }
    if(analogRead(RIGHT_PUCK) <= PUCK_RECIEVED){
      closeClaw();
      TimeInLastState = TimeInState;
      TimeInState = 0;
      CurrentState++;
    }
  }
  //---------------------------------------------------drive until in front of the mothership--------------------------------
  else if(CurrentState == 5){
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 0, 0);
    if(TimeInState > (5600 - TimeInLastState)){ //3700
      TimeInState = 0;
      CurrentState++;
    }
  }
  //---------------------------Turn to the left to face mothership
  else if(CurrentState == 6){
      Reverse90Turn(_LEFT); //Function Blocks does not return till turn is complete
      CurrentState++;
      TimeInState = 0;
      substateFlag = 0;
      SetDelta();
  }
  //---------------------------Drive straight into mothership
  else if(CurrentState == 7){
    if(substateFlag == 0){
      writeToWheels(FULL_SPEED,FULL_SPEED);
      if(amountSeen >= 7){
        substateFlag++;  
      }
    }
    else if(substateFlag == 1){
        writeToWheels(FULL_SPEED,FULL_SPEED);
        if(amountSeen == 0){
          substateFlag++;  
        }
    }
    else if(substateFlag == 2){
      writeToWheels(FULL_SPEED,FULL_SPEED);
      if(amountSeen >= 1){
        substateFlag++;
      }
    }
    else{
      favorLineFollow(FULL_SPEED/2,LINE_STRICTNESS,_LEFT,3,4);
      if(amountSeen == 0){
        TimeInState  = 0;
        CurrentState = 1;
        substateFlag = 0;
        writeToWheels(0,0);
      }
    }
  }
  // this stage should not be used emergency stage
  else{
    display.sendNum(7);
  }
}
