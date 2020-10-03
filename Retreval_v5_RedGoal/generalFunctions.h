bool delayState(int ms) {
  static int milliseconds = -1;
  if(milliseconds == -1) {
    milliseconds = millis();
  }
  else if(millis() - milliseconds >= ms) {
    milliseconds = -1;
    return true;
  }
  return false;
}
void closeClaw(){
  if(clawOpen){
    clawOpen=false;
    //Code to close the holder
    clawMotor.write(0);
  }
}
void openClaw(){
  if(!clawOpen){
    clawOpen=true;
    //Code to open the holder
    clawMotor.write(70);
  }
}

