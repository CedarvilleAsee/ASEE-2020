//Retrieval bots
//Has a "whisker" used to detect mothership
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
  //attachInterrupt(WHISKER, whiskerInteruptHandler, HIGH);

  unsigned int Time = millis();
  while(millis()<Time+1234){
    display.sendNum(millis());
  }
}

void loop() {
  
  int w = digitalRead(WHISKER);
  display.sendDigit(w, 1);

  readLine();
  if(digitalRead(BUTTON_2) == 0){
    CurrentState = 1;
    writeToWheels(0,0);  
  }

  TimeInState += DeltaTime();
  SetDelta();

  display.sendDigit(CurrentState, 0);
  //display.sendDigit(0, 1);
  
  //waiting state
  if(CurrentState == 1){
    if(digitalRead(BUTTON_1) == 0){
      interrupts();
      TimeInState = 0;
      substateFlag = 0;
      SetDelta();
      CurrentState++;
    }
    openClaw();
  }
  //-------------------------------Turn left off the mothership-------------------------------
  else if(CurrentState == 2){
    if(substateFlag == 0){
      favorLineFollow(FULL_SPEED, LINE_STRICTNESS,_RIGHT, 3, 2);
      if(amountSeen == 0){
        substateFlag = 1;  
      }
    }
    else if(substateFlag == 1){
      writeToWheels((FULL_SPEED),(FULL_SPEED));
      if(amountSeen == 7){
        substateFlag = 2;  
      }
    }
    else if(substateFlag == 2){
      writeToWheels((FULL_SPEED - 50),(FULL_SPEED - 50));
      if(amountSeen == 0){
        substateFlag = 3;
      }
    }
    else if(substateFlag == 3){
      //90 degree turn
      writeToWheels((FULL_SPEED - 50), -100);
      //Exit Conditions
      if(sensors[3] == HIGH){
        substateFlag = 0;
        TimeInState = 0;
        CurrentState++; 
      }
    }
  }
  //-------------------------------pick up the first puck on the Outside housing. Stage should be short-------------------------------------
  else if(CurrentState == 3){
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 2, 1);
    if (analogRead(LEFT_PUCK) <= PUCK_RECIEVED){
      //display.sendNum(TimeInState);
      closeClaw();
      CurrentState++;
      TimeInState=0;
    }
  }
  //---------------------------turn to miss the goal and pick up the next puck of the Inside housing ---------------------------
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
  //------------------------------------------Pause State between picking up the last puck -----------------------------------
  else if(CurrentState == 5){
    if(TimeInState > 100 && TimeInState < 200){
      openClaw();  
    }
    favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, 4, 3);
    if(TimeInState > 500){
      closeClaw();
      CurrentState++;
      TimeInLastState += TimeInState;
      TimeInState = 0;
      substateFlag = 0;
    }  
  }
  //---------------------------------------------------drive until in front of the mothership the exicute a reverse 90 turn ---------------------------------
  else if(CurrentState == 6){
    //Whisker will be high before and after hitting the mothership
    //if(digitalRead(WHISKER) == HIGH && substateFlag == 0){
    if(whisker() && substateFlag == 0){
      TimeInState = 0;
      substateFlag = 1;
    } //now in interupt
    //else if(digitalRead(WHISKER) == LOW && substateFlag == 1 && TimeInState >= 600){
    else if(!whisker() && substateFlag == 1 && TimeInState >= 500){
      //start turn
      Reverse90Turn(_LEFT); //Function Blocks does not return till turn is complete
      CurrentState++;
      TimeInState = 0;
      substateFlag = 0;
      SetDelta();
    }
    else{
      favorLineFollow(FULL_SPEED, LINE_STRICTNESS, false, -1);
    }
  }
  //---------------------------Drive straight into mothership-------------------------------------
  else if(CurrentState == 7){
    display.sendDigit(substateFlag,1);
    //Drive until it seens the line on the board
    if(substateFlag == 0){
      writeToWheels(FULL_SPEED,FULL_SPEED);
      if(amountSeen >= 7){
        substateFlag++;  
      }
    }
    //Drives in over the line
    else if(substateFlag == 1){
        writeToWheels(FULL_SPEED,FULL_SPEED);
        if(amountSeen == 0){
          substateFlag++;  
        }
    }
    //Drives until it sees the line on the ramp
    else if(substateFlag == 2){
      writeToWheels(FULL_SPEED/2,FULL_SPEED/2);
      if(amountSeen >= 2){
        substateFlag++;
        TimeInState = 0;
      }
    }
    //Follow the line up on the mothership
    else{
      if(TimeInState >= 400){
        straightLineFollow(FULL_SPEED/4, LINE_STRICTNESS - 10);
        //favorLineFollow(FULL_SPEED/2,LINE_STRICTNESS - 10,_RIGHT, 3, 4);
        if(senseEnd()){
          delay(500);
          TimeInState  = 0;
          CurrentState = 1;
          substateFlag = 0;
          writeToWheels(0,0);
          openClaw(20);
        }
      }
    }
  }
  // this stage should not be used emergency stage
  else{
    display.sendDigit(9, 0);
  }
}
