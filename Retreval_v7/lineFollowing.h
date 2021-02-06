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

  //Debug print out to display. Combides sensors into pairs and gives them each there own digit.
  //a one means only one sensor in the pair is on, a 2 means both senors are on, and a 0 mean none of the sensors are on
  if(false){
    int readout = 0;
    readout += sensors[0] + sensors[1];
    readout += (sensors[2] + sensors[3])*10;
    readout += (sensors[4] + sensors[5])*100;
    readout += (sensors[6] + sensors[7])*1000;
    display.sendNum(readout);
    delay(1000);
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

/*bool lineFollow(int ts, int strictness, int cen1 = 4, int cen2 = 3) {
  if (amountSeen == 0) {//what to do if no line is seen
    //floor it and pray? Consider Circling 
    writeToWheels(ts, ts);
  }else{
    int rightSpeed = ts + (lastLineIndex - cen1) * strictness;
    int leftSpeed = ts - (firstLineIndex - cen2) * strictness;
    //display.sendNum( (leftSpeed*1000) + (rightSpeed * 100) + (lastLineIndex)*10 + firstLineIndex);
    writeToWheels(leftSpeed, rightSpeed);
  }
  return false;
}*/


void favorLineFollow(int ts, int strictness, bool favorRight = false, int cen = 3,int stable = -7){
  //Sees no lines, use what the center is to guess a direction to turn
  if(amountSeen == 0){
    //Test: 1 not enough, 7 very sharp
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
    if(abs(lastLineIndex - cen)>abs(lastLineIndex - stable)) diff = lastLineIndex - stable;
    else diff = lastLineIndex - cen;
  else
    if(abs(firstLineIndex - cen)>abs(firstLineIndex - stable)) diff = firstLineIndex - stable;
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
    leftWheelSpeed = -MID_SPEED/4;
    rightWheelSpeed = -(MID_SPEED+10);
  }
  else{
    leftWheelSpeed = -(MID_SPEED+10);
    rightWheelSpeed = -MID_SPEED/4;
  }
  display.sendNum(abs(leftWheelSpeed));
  while(DeltaTime() < 3000){
    writeToWheels(leftWheelSpeed, rightWheelSpeed);
  }
  return true;
}

#endif
