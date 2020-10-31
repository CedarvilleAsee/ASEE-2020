//Helper functions---------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * Returns true if, based on internal timing, the red gate is open
 */
boolean redIsOpen()
{
  return (((millis() - redTime) % 5000) > 2000);
}

/*
 * Returns true if, based on internal timing, the blue gate is open
 */
boolean blueIsOpen()
{
  return (((millis() - blueTime) % 5000) > 2000);
}

/*
 * Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
 * Returns true if the puck can safely be launched (enough time after the gate opened and before it closes)
 */
boolean redSafeToLaunch()
{
  if((((millis() - redTime) % 5000) < (CLOSE_MARGIN + 2000)) || (((millis() - redTime) % 5000) > (5000 - OPEN_MARGIN)))          
  {  
    return false;
  }
  return true;
}

/*
 * Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
 * Returns true if the puck can safely be launched (enough time after the gate opened and before it closes)
 */
boolean blueSafeToLaunch()
{
  if((((millis() - blueTime) % 5000) < (CLOSE_MARGIN + 2000)) || (((millis() - blueTime) % 5000) > (5000 - OPEN_MARGIN)))
  {
    return false;
  }
  return true;
}

/*
 * Flashes the red and blue LED to show the internal timing of the red and blue gate. If the microcontroller thinks the gate is open, the respective LED will be on.
 * Useful to see if the internal timing is accurate.
 */
void updateStateLED()
{
  digitalWrite(redStatusLED, redIsOpen());
  digitalWrite(blueStatusLED, blueIsOpen());
}


/*
 * Check that sensors are giving values that make sense, and if any of them are not, print an error to the serial monitor, flash an LED, and stop the code.
 */
void checkSensorValidity()
{
  Serial.println(F("Checking sensor validity..."));
  redGate.startRanging();
  blueGate.startRanging();
  delay(500);
  if(redGate.getDistance() == 0)      //If sensor always reads 0, it is messed up
  {
    Serial.println(F("Red sensor reading 0 always. Check wiring and sensor clearance"));
    while(1)
    {
      digitalWrite(redStatusLED, ((millis() % 100) > 50));         //Flash LED and stop program
    }
  }
  if(blueGate.getDistance() == 0)    //If sensor always reads 0, it is messed up
  {
    while(1)
    {
      Serial.println(F("Blue sensor reading 0 always. Check wiring and sensor clearance"));
      digitalWrite(blueStatusLED, ((millis() % 100) > 50));      //Flash LED and stop program
    }
  }
  Serial.println(F("Sensors appear valid!"));
}

/*
 * Launch the red puck when the gate is safely open. This method checks if the gate is in the proper position, and once it is, launches the puck
 */
//GET DONE - Needs electrical to be complete
void launchRedPuck()
{
  //FIXME: avoid blocking code, will need to call this function in the outer loop (don't want blue launch delayed by red)
  /*while(!redSafeToLaunch())
  {
    //Do nothing until safe to launch
  }*/
  //Melt the wire
  if(redSafeToLaunch()){
    //send the combination to
    //In1:H, In2:H, PWN:H/L = Low, Low
  }
}

/*
 * Launch the blue puck when the gate is safely open. This method checks if the gate is in the proper position, and once it is, launches the puck
 */
//GET DONE - Needs electrical to be complete
void launchBluePuck()
{
  while(blueSafeToLaunch() == false)
  {
    //Do nothing until safe to launch
  }
  //Melt the wire
}


//Main functions-----------------------------------------------------------------------------------------------------------------------------------------------------

/*
 * Setup all pins as inputs and outputs
 */
void initializePins()
{
    Serial.println(F("Initializing pins..."));
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(redButton, INPUT_PULLUP);
    pinMode(blueButton, INPUT_PULLUP);
    pinMode(prestartButton, INPUT_PULLUP);
    
    pinMode(redMiniRobotStartLED, OUTPUT);
    pinMode(blueMiniRobotStartLED, OUTPUT);

    pinMode(redStatusLED, OUTPUT);
    pinMode(blueStatusLED, OUTPUT);

    pinMode(xshutPin, OUTPUT);
    
    Serial.println(F("Initializing finished!"));
}

/*
 * Initialize all sensors and detect errors if they come up. Indicate these errors by printing them to the serial monitor and flashing an LED. Stops code if an error occurs.
 */
void initializeSensors()
{
  Serial.println(F("Initializing sensors..."));
  if(redGate.begin() != 0)        //Redgate.begin() returns 0 if sensor started properly
  {
    Serial.println(F("Red gate sensor failed to start. Check wiring?"));
    while(1)                      //Stop the program and flash the led fast.
    {
      digitalWrite(redStatusLED, ((millis() % 100) > 50));
    }
  }
  /*pinMode(xshutPin, OUTPUT);
  blueGate.setI2CAddress(0x54);
  pinMode(xshutPin, INPUT);
  if(blueGate.begin() != 0)
  {
    Serial.println(F("Blue gate sensor failed to start. Check wiring?"));
    while(1)
    {
      digitalWrite(blueStatusLED, ((millis() % 100) > 50));
    }
  }*/
  Serial.println("Initializing complete!");
}

/*
 * Wait for the prestart button to be pressed before looking at the gates for their status and timing. This prevents false readings during setup.
 */
void waitForPrestart()
{
  Serial.println(F("Waiting for prestart..."));
  while(digitalRead(prestartButton) == HIGH)    // While the prestart button has not been pressed
  {
    //do nothing
  }
  Serial.println(F("Start button pressed!"));
  checkSensorValidity();
}

/*
 * Get the timing for the red gate and its status.
 */
void setupStateRed()
{
  Serial.println(F("Checking red gate state..."));
  redGate.startRanging();                       //Enable reading from the sensor
  int distance = redGate.getDistance();
  while(distance < RED_DISTANCE)                // Wait for the gate to open
  {
    distance = redGate.getDistance();
  }
  while(distance > RED_DISTANCE)                // While gate is open
  {
    redTime = millis();
    distance = redGate.getDistance();
  }
  redGate.clearInterrupt();
  redGate.stopRanging();
  Serial.println(F("Red gate state established!"));
}

/*
 * Get the timing for the red gate and its status.
 */
void setupStateBlue()
{
  Serial.println(F("Checking blue gate state..."));
  int distance = blueGate.getDistance();
  while(distance < BLUE_DISTANCE)                // Wait for the gate to open
  {
    distance = blueGate.getDistance();
  }
  while(distance > BLUE_DISTANCE)                // While gate is open
  {
    blueTime = millis();
    distance = blueGate.getDistance();
  }
  Serial.println(F("Blue gate state established!"));
}

/*
 * Wait for the red or blue button to be pressed. Save whichever button is pressed, because that is the color that gets launched first.
 */
void selectRedOrBlue()
{
  Serial.println(F("Waiting for red or blue to be selected..."));
  while(true)                               // Run this code until a break. A break will happen when a selection is made.
  {
    if(digitalRead(redButton) == LOW)       // If the red button is pressed
    {
      Serial.println(F("Red selected"));
      redRun = true;
      break;
    }

    if(digitalRead(blueButton) == LOW)      // If the blue button is pressed
    {
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
//GET DONE
void sendGoToGetters()                      //activate side robots
{
  digitalWrite(redMiniRobotStartLED, HIGH);
  digitalWrite(blueMiniRobotStartLED, HIGH);
}

/*
 * Do not do anything until the pucks have all been returned. Once all of the pucks have been returned, continue running code after a 100 ms delay
 */
void waitForReturn()
{
  while(digitalRead(puckSensor1) && digitalRead(puckSensor2) && digitalRead(puckSensor3) && digitalRead(puckSensor4))
  {
    digitalWrite(redStatusLED, redIsOpen());
    digitalWrite(blueStatusLED, blueIsOpen());
  }
  delay(100);
}

/*
 * Launch whichever pucks needs to be launched first, then launch the other pucks. 
 */
void launchProperPucks()
{
  if(redRun)
  {
    //ideally 
    launchRedPuck();
    delay(100);       
    launchBluePuck();
  }
  else
  {
    launchBluePuck();
    delay(100);
    launchRedPuck();  
  }
}
