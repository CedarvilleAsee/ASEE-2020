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

bool test = false;
bool first = true;

void loop() {
  
  readLine();
  if(digitalRead(BUTTON_2) == 0){
    CurrentState = 1;
    writeToWheels(0,0);  
  }
  
  //Debug Section
  //Debug();
  /*if(first){
    first=false;
    SetDelta();
  }
  if(test == false){
    test = rotateTurn(DeltaTime());
  }
  return;*/
  if(amountSeen>maxSeen) maxSeen=amountSeen;
  display.sendNum(CurrentState);
  //waiting state
  if(CurrentState == 1){

    
    if(digitalRead(BUTTON_1) == 0){
      TimeInState = 0;
      SetDelta();
      CurrentState++;
    }
    openClaw();
  }
  //-------------------------------Drive center of line and turn off the mothership-------------------------------
  else if(CurrentState == 2){
    
    TimeInState += DeltaTime();
    SetDelta();
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS*2, _LEFT, 3, 2);
    if(TimeInState > 3000){
      TimeInState = 0;
      CurrentState++; 
    }
  }
  //-------------------------------pick up the first puck on the Outside housing. Stage should be short-------------------------------------
  else if(CurrentState==3){
    
    TimeInState += DeltaTime();
    SetDelta();
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, _LEFT, 2, 1);
    if (analogRead(LEFT_PUCK) <= PUCK_RECIEVED){
      closeClaw();
      CurrentState++;
      TimeInState=0;
      /*if(millis() - lastLoopTime >= TIME_IN_HOLDER){
        CurrentState++;
        TimeInState = 0;
      }
    }
    else{
        lastLoopTime = millis();*/
    }
  }
  //-----------------------------------------turn to miss the goal and pick up the next puck of the Inside housing ---------------------------------------
  else if(CurrentState == 4){
    if(TimeInState>100) openClaw();
    
    TimeInState += DeltaTime();
    SetDelta();
     favorLineFollow(FULL_SPEED, LINE_STRICTNESS, _LEFT, 8);
    //Exit Condition
    if(analogRead(RIGHT_PUCK) <= PUCK_RECIEVED){
      favorLineFollow(FULL_SPEED/2, LINE_STRICTNESS, _LEFT, 7, 6);
      closeClaw();
      TimeInState=0;
      CurrentState++;
      maxSeen=0;
      /*if(millis()-lastLoopTime>=TIME_IN_HOLDER){
        TimeInState = 0;
        CurrentState++;
      }
    }else{
      lastLoopTime=millis();*/
    }
  }
  //---------------------------------------------------drive centered on the line back to mothership------------------------------------
  else if(CurrentState==5){
    
    TimeInState += DeltaTime();
    SetDelta();
    favorLineFollow(FULL_SPEED, 2*LINE_STRICTNESS, _LEFT, 1, 2);
    if(amountSeen==0){
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
