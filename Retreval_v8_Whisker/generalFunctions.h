void closeClaw(){
  if(clawOpen){
    clawOpen = false;
    //Code to close the holder
    clawMotor.write(CLOSED_ANGLE);
  }
}
void openClaw(){
  if(!clawOpen){
    clawOpen = true;
    //Code to open the holder
    clawMotor.write(OPEN_ANGLE);
  }
}

//Returns true if sensors are 00111100
bool senseEnd(){
  if(sensors[0]==LOW) return false;
  if(sensors[1]==LOW) return false;
  if(sensors[2]==HIGH) return false;
  if(sensors[3]==HIGH) return false;
  if(sensors[4]==HIGH) return false;
  if(sensors[5]==HIGH) return false;
  if(sensors[6]==LOW) return false;
  if(sensors[7]==LOW) return false;
  return true;
}
