#ifndef LINEFOLLOWING
#define LINEFOLLOWING

// Populates the sensors[] variable so that we know amountSeen
void readLine() {
  amountSeen = 0;
  lastLineIndex = -1;
  for(int i = 7; i >= 0; --i) {

    sensors[i] = digitalRead(LINE_SENSOR[i]);

    if(sensors[i] == HIGH) {
      if(lastLineIndex == -1) {
        lastLineIndex = i;
      }
      ++amountSeen;
      firstLineIndex = i;
    }
  }

  sensorCounter = 0;
  
  //for (int i = 0; i < 8; i++ ){
  //  sensorCounter += sensors[i]<<i;
  //}
  
    //display.sendNum((amountSeen * 100) +(lastLineIndex)*10 + firstLineIndex);
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
  if(ls > 255){ls = 255;} 
  if(rs > 255){rs = 255;}
  if(rs < 0){rs = -255;}
  if(ls < 0){ls = -255;} 

  analogWrite(WHEEL_SPEED_L, abs(ls));
  analogWrite(WHEEL_SPEED_R, abs(rs));
}

bool lineFollow(int ts, int strictness, int cen1 = 4, int cen2 = 3) {
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
}

#endif
