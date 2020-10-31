void Debug(){
  
}
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
