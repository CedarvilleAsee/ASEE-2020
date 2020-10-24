#ifndef SUBSTATES
#define SUBSTATES


//fast - followredpathstate, followneutralpathstate

// State 0
//  Waits until the button on board is pushed, go to next statez
/*bool waitState() {
  //for centering robot
  display.sendNum(sensorCounter, 0);
  //keeps the robot still
  writeToWheels(0, 0);
  //when started, copy missions into globals
  if (digitalRead(BUTTON_1) == LOW) {
    //get mission number from switches
    return true;
  }
  return false;
}*/
#endif
