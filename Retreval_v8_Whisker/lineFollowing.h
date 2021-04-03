#ifndef LINEFOLLOWING
#define LINEFOLLOWING

#include "Time.h"

// Populates the sensors[] variable so that we know amountSeen
void readLine() {
  amountSeen = 0;
  lastLineIndex = -1;
  for(int i = 7; i >= 0; i--) {

    sensors[i] = digitalRead(LINE_SENSOR[i]);

    if(sensors[i] == HIGH) {
      if(lastLineIndex == -1) {
        lastLineIndex = i;
      }
      amountSeen++;
      firstLineIndex = i;
    }
  }

  //Debug print out to display. writes the sensors into a pattern around the outer eadge of to digits of the seven segment display.
  if(Debug){
    int lowerBits = 0x00;
    int upperBits = 0x00;
    
    if(sensors[7] == 1){ upperBits += 0x08;  }
    if(sensors[6] == 1){ upperBits += 0x10;  }
    if(sensors[5] == 1){ upperBits += 0x20;  }
    if(sensors[4] == 1){ upperBits += 0x01;  }
    
    if(sensors[3] == 1){ lowerBits += 0x01;  }
    if(sensors[2] == 1){ lowerBits += 0x02;  }
    if(sensors[1] == 1){ lowerBits += 0x04;  }
    if(sensors[0] == 1){ lowerBits += 0x08;  }
  
    display.sendDigit_Advanced(0xC4, upperBits);
    display.sendDigit_Advanced(0xc6, lowerBits);
  }
}

void writeWheelDirection(bool ldir, bool rdir) {
  digitalWrite(WHEEL_DIR_LF, ldir);
  digitalWrite(WHEEL_DIR_LB, !ldir);
  digitalWrite(WHEEL_DIR_RF, rdir);
  digitalWrite(WHEEL_DIR_RB, !rdir);
}

void writeToWheels(int ls, int rs) {
  if(ls < 0) {
    digitalWrite(WHEEL_DIR_LF, false); //right backwards
    digitalWrite(WHEEL_DIR_LB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_LF, true); //right forwards
    digitalWrite(WHEEL_DIR_LB, false);
  }
  if(rs < 0) {
    digitalWrite(WHEEL_DIR_RF, false); //right backwards
    digitalWrite(WHEEL_DIR_RB, true);
  }
  else {
    digitalWrite(WHEEL_DIR_RF, true); //right forwards
    digitalWrite(WHEEL_DIR_RB, false);
  }

  //Make sure it does not exceed max
  if(rs < -255){rs = 254;}
  if(ls < -255){ls = 254;}  
  if(ls > 255){ls = 255;} 
  if(rs > 255){rs = 255;}
  
  //display.sendNum(rs);

  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

void favorLineFollow(int ts, int strictness, bool favorRight = false, int cen = 3,int stable = -7){
  //Sees no lines, use what the center is to guess a direction to turn
  if(amountSeen == 0){
    if(cen<4){//Turn left
      writeToWheels(ts+4*strictness,ts-4*strictness);
    }else{//Turn right
      writeToWheels(ts-4*strictness,ts+4*strictness);
    }
    return;
  }
  //-7 is a sentitent value for same as cen
  if(stable==-7) stable=cen;
  //Diff is the difference between the central index (cen) and the index of the line it's following
  int diff;
  if(favorRight)
    if(abs(lastLineIndex - cen) > abs(lastLineIndex - stable)) diff = lastLineIndex - stable;
    else diff = lastLineIndex - cen;
  else
    if(abs(firstLineIndex - cen) > abs(firstLineIndex - stable)) diff = firstLineIndex - stable;
    else diff = firstLineIndex - cen;
  
  //If diff is negative, it will turn to the left, if postive: turn left
  int rightSpeed = ts + diff * strictness;
  int leftSpeed  = ts - diff * strictness;
  writeToWheels(leftSpeed,rightSpeed);
}

bool Reverse90Turn(bool turnRight){
  SetDelta();
  int leftWheelSpeed;
  int rightWheelSpeed;
  if(!turnRight){
    leftWheelSpeed = -FULL_SPEED/4;
    rightWheelSpeed = -(FULL_SPEED);
  }
  else{
    leftWheelSpeed = -(FULL_SPEED);
    rightWheelSpeed = -FULL_SPEED/4;
  }
  while(DeltaTime() < 1400){
    writeToWheels(leftWheelSpeed, rightWheelSpeed);
  }
  return true;
}

#endif
