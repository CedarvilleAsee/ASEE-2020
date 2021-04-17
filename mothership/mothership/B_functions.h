//Helper functions---------------------------------------------------------------------------------------------------------------------------------------------------
//None of these need to be changed, scroll to Main Functions

/*
 * Returns true if, based on internal timing, the red gate is open
 */
boolean redIsOpen(){
  return (((millis() - redTime) % 5000) > 2000);
}

/*
 * Returns true if, based on internal timing, the blue gate is open
 */
boolean blueIsOpen(){
  return (((millis() - blueTime) % 5000) > 2000);
}

/*
 * Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
 * Returns true if the puck can safely be launched (enough time after the gate opened and before it closes)
 */
boolean redSafeToLaunch(){
  if((((millis() - redTime) % 5000) < (CLOSE_MARGIN + 2000)) || (((millis() - redTime) % 5000) > (5000 - OPEN_MARGIN)))          {  
    return false;
  }
  return true;
}

/*
 * Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
 * Returns true if the puck can safely be launched (enough time after the gate opened and before it closes)
 */
boolean blueSafeToLaunch(){
  if((((millis() - blueTime) % 5000) < (CLOSE_MARGIN + 2000)) || (((millis() - blueTime) % 5000) > (5000 - OPEN_MARGIN))){
    return false;
  }
  return true;
}

/*
 * Flashes the red and blue LED to show the internal timing of the red and blue gate. If the microcontroller thinks the gate is open, the respective LED will be on.
 * Useful to see if the internal timing is accurate.
 */
void updateStateLED(){
  digitalWrite(redStatusLED, redIsOpen());
  digitalWrite(blueStatusLED, blueIsOpen());
}


/*
 * Check that sensors are giving values that make sense, and if any of them are not, print an error to the serial monitor, flash an LED, and stop the code.
 */
void checkSensorValidity(){
  #ifndef NOSENSOR
  Serial.println(F("Checking sensor validity..."));
  redGate.startRanging();
  blueGate.startRanging();
  delay(500);
  if(redGate.getDistance() == 0){      //If sensor always reads 0, it is messed up
    Serial.println(F("Red sensor reading 0 always. Check wiring and sensor clearance"));
  //TODO: try ussing sensor.begin to fix sensor...
    while(1){
      digitalWrite(redStatusLED, ((millis() % 100) > 50));         //Flash LED and stop program
    }
  }
  if(blueGate.getDistance() == 0){    //If sensor always reads 0, it is messed up
    while(1){
      Serial.println(F("Blue sensor reading 0 always. Check wiring and sensor clearance"));
      digitalWrite(blueStatusLED, ((millis() % 100) > 50));      //Flash LED and stop program
    }
  }
  Serial.println(F("Sensors appear valid!"));
  #endif
}

void enableLaunch(){ 
  digitalWrite(redEnablePin, HIGH);
  digitalWrite(blueEnablePin, HIGH);
  digitalWrite(enableBlackPin, HIGH);
  //digitalWrite(pwmB2nd, HIGH);   //Unused enable
  digitalWrite(STBY1st, HIGH);
  digitalWrite(STBY2nd, HIGH);                       
}

void disableLaunch(){ 
  digitalWrite(redEnablePin, LOW);
  digitalWrite(blueEnablePin, LOW);
  digitalWrite(enableBlackPin, LOW);
  //digitalWrite(pwmB2nd, LOW); //unused enable
  digitalWrite(STBY1st, LOW);
  digitalWrite(STBY2nd, LOW) ;                              
}

//red left puck
void launchRedL()     { digitalWrite(redLeft, HIGH); }
void disconnectRedL() { digitalWrite(redLeft, LOW);  }

//red right puck
void launchRedR()     { digitalWrite(redRight, HIGH); }
void disconnectRedR() { digitalWrite(redRight, LOW);  }

//black puck
void launchBlack()    { digitalWrite(black, HIGH); }
void disconnectBlack(){ digitalWrite(black, LOW ); } 

//blue left puck
void launchBlueL()    { digitalWrite(blueLeft, HIGH); }
void disconnectBlueL(){ digitalWrite(blueLeft, LOW);  }

//blue right puck
void launchBlueR()    { digitalWrite(blueRight, HIGH); }
void disconnectBlueR(){ digitalWrite(blueRight, LOW);  }


/*
 * Launch the red puck when the gate is safely open. This method checks if the gate is in the proper position, and once it is, launches the puck
 */
//GET DONE - Needs electrical to be complete
void launchRedPuck(){
  #ifndef NOSENSOR
  while(redSafeToLaunch() == false){
    //Do nothing until safe to launch
  }
  #endif
  launchRedL();
  delay(MELT_WIRE_TIME);
  disconnectRedL();
  launchRedR();
  delay(MELT_WIRE_TIME);
  disconnectRedR();
}

/*
 * Launch the blue puck when the gate is safely open. This method checks if the gate is in the proper position, and once it is, launches the puck
 */
//GET DONE - Needs electrical to be complete
void launchBluePuck(){
  #ifndef NOSENSOR
  while(blueSafeToLaunch() == false){
    //Do nothing until safe to launch
  }
  #endif
  launchBlueL();
  delay(MELT_WIRE_TIME);
  disconnectBlueL();
  launchBlueR();
  delay(MELT_WIRE_TIME);
  disconnectBlueR();
}

void launchBlackPuck(){
  #ifndef NOSENSOR
  while(redSafeToLaunch() == false){
    //Do nothing until safe to launch
  }
  #endif
  launchBlack();
  delay(MELT_WIRE_TIME);
  disconnectBlack();
}

//Main functions-----------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * Setup all pins as inputs and outputs
 */
void initializePins(){
    Serial.println(F("Initializing pins..."));
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(redButton, INPUT_PULLUP);
    pinMode(blueButton, INPUT_PULLUP);
    //pinMode(prestartButton, INPUT_PULLUP);
    
    pinMode(redMiniRobotStartLED, OUTPUT);
    pinMode(blueMiniRobotStartLED, OUTPUT);

    pinMode(redStatusLED, OUTPUT);
    pinMode(blueStatusLED, OUTPUT);

    rampServo.attach(servoPin);
    rampServo.write(compressedPosition);

    pinMode(redLeft, OUTPUT);
    pinMode(redRight, OUTPUT);
    pinMode(redEnablePin, OUTPUT);
    pinMode(blueLeft, OUTPUT);
    pinMode(blueRight, OUTPUT);
    pinMode(blueEnablePin, OUTPUT);

    pinMode(black, OUTPUT);
    pinMode(in2A2nd, OUTPUT);
    pinMode(enableBlackPin, OUTPUT);
    pinMode(in1B2nd, OUTPUT);
    pinMode(in2B2nd, OUTPUT);
    //pinMode(pwmB2nd, OUTPUT); //unused enable

    pinMode(STBY1st, OUTPUT);
    pinMode(STBY2nd, OUTPUT);
    
    Serial.println(F("Initializing finished!"));
}

/*
 * Initialize all sensors and detect errors if they come up. Indicate these errors by printing them to the serial monitor and flashing an LED. Stops code if an error occurs.
 */
void initializeSensors(boolean stopIfFail){
  #ifndef NOSENSOR
  Serial.println(F("Initializing sensors..."));
  pinMode(xshutPin, OUTPUT);
  delay(20);
  blueGate.setI2CAddress(0x55);
  delay(20);
  Serial.println(redGate.getI2CAddress());
  pinMode(xshutPin, INPUT);
  delay(20);
  Serial.println(blueGate.getI2CAddress());
  if(redGate.begin() != 0){        //Redgate.begin() returns 0 if sensor started properly
    Serial.println(F("Red gate sensor failed to start. Check wiring?"));
    if(stopIfFail == true)
    {
      while(1){                      //Stop the program and flash the led fast.
        digitalWrite(redStatusLED, ((millis() % 100) > 50));
      }
    }
  }
  if(blueGate.begin() != 0){
    Serial.println(F("Blue gate sensor failed to start. Check wiring?"));
    if(stopIfFail == true)
    {
      while(1){
        digitalWrite(blueStatusLED, ((millis() % 100) > 50));
      }
    }
  }
  //redGate.setDistanceModeShort();
  //blueGate.setDistanceModeShort();
  Serial.println("Initializing complete!");
  #endif
}

/*
 * Wait for the prestart button to be pressed before looking at the gates for their status and timing. This prevents false readings during setup.
 */
void waitForPrestart(){
  Serial.println(F("Waiting for prestart..."));
  while(digitalRead(prestartButton) == HIGH){    // While the prestart button has not been pressed
    Serial.print(F("Waiting for prestart...   Red sensor: "));
    Serial.print(redGate.getDistance());
    Serial.print(F("        Blue sensor: "));
    Serial.println(blueGate.getDistance());
  }
  Serial.println(F("Start button pressed!"));
  checkSensorValidity();
}

/*
 * Get the timing for the red gate and its status.
 */
void setupStateRed(){
  Serial.println(F("Checking red gate state..."));
  redGate.startRanging();                       //Enable reading from the sensor
  int distance = redGate.getDistance();
  while(distance < RED_DISTANCE){                // Wait for the gate to open
    distance = redGate.getDistance();
    Serial.println("RED (waiting for open): " + distance);
  }
  while(distance > RED_DISTANCE){                // While gate is open
    redTime = millis();
    distance = redGate.getDistance();
    Serial.println("RED (waiting for close): " + distance); 
  }
  redGate.clearInterrupt();
  redGate.stopRanging();
  Serial.println(F("Red gate state established!"));
}

/*
 * Get the timing for the red gate and its status.
 */
void setupStateBlue(){
  Serial.println(F("Checking blue gate state..."));
  int distance = blueGate.getDistance();
  while(distance < BLUE_DISTANCE){                // Wait for the gate to open
    distance = blueGate.getDistance();
    Serial.println("Blue (waiting for open): " + distance);
    digitalWrite(redStatusLED, redIsOpen());
  }
  while(distance > BLUE_DISTANCE){                // While gate is open
    blueTime = millis();
    distance = blueGate.getDistance();
    Serial.println("Blue (waiting for close): " + distance);
    digitalWrite(redStatusLED, redIsOpen());
  }
  Serial.println(F("Blue gate state established!"));
}

/*
 * Wait for the red or blue button to be pressed. Save whichever button is pressed, because that is the color that gets launched first.
 */
void selectRedOrBlue(){
  Serial.println(F("Waiting for red or blue to be selected..."));
  while(true){                               // Run this code until a break. A break will happen when a selection is made.
    if(digitalRead(redButton) == LOW){       // If the red button is pressed
      Serial.println(F("Red selected"));
      redRun = true;
      break;
    }

    if(digitalRead(blueButton) == LOW){      // If the blue button is pressed
      Serial.println(F("Blue selected"));
      redRun = false;
      break;
    }
    
    digitalWrite(redStatusLED, redIsOpen());
    digitalWrite(blueStatusLED, blueIsOpen());
  }
}

/*
 * Move the servo that pushes the ramps down.
 * Pushes the ramps down. The puck grabbing robots detect this and go.
 */
void sendGoToGetters(){                      //activate side robots
  rampServo.write(extendedPosition);
  delay(2000);
  rampServo.write(compressedPosition);
}

/*
 * Do not do anything until the pucks have all been returned. Once all of the pucks have been returned, continue running code after a 100 ms delay
 */
void waitForReturn(){
  while(digitalRead(robotSensor1) && digitalRead(robotSensor2)){
    digitalWrite(redStatusLED, redIsOpen());
    digitalWrite(blueStatusLED, blueIsOpen());
  }
  delay(100);
}

/*
 * Launch whichever pucks needs to be launched first, then launch the other pucks. 
 */
void launchProperPucks(){
  if(redRun){
    launchRedPuck();
    delay(100);       
    launchBluePuck();
  }
  else{
    launchBluePuck();
    delay(100);
    launchRedPuck();  
  }
}
