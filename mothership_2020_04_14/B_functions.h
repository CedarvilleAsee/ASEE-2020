bool prestartPressed;
bool choiceMade = false;


//Helper functions---------------------------------------------------------------------------------------------------------------------------------------------------
boolean redIsOpen()
{
  return (((millis() - redTime) % 5000) > 2000);
}

boolean blueIsOpen()
{
  return (((millis() - blueTime) % 5000) > 2000);
}

//Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
boolean redSafeToLaunch()
{
  if((((millis() - redTime) % 5000) <= (2000 + CLOSE_MARGIN)) || (((millis() - redTime) % 5000) >= (5000 - OPEN_MARGIN)))          
  {  
    return false;
  }
  return true;
}

//Check if the gate is within the defined launchable period (gate is open and has been open for CLOSE_MARGIN and will not be closed within OPEN_MARGIN)
boolean blueSafeToLaunch()
{
  if((((millis() - blueTime) % 5000) <= (CLOSE_MARGIN + 2000)) || (((millis() - blueTime) % 5000) >= (5000 - OPEN_MARGIN)))
  {
    return false;
  }
  return true;
}

void updateStateLED()
{
  digitalWrite(redStatusLED, redIsOpen());
  digitalWrite(blueStatusLED, blueIsOpen());
}


//Main functions-----------------------------------------------------------------------------------------------------------------------------------------------------
void initializePins()                           //Setup all pins
{
    pinMode(redButton, INPUT_PULLUP);
    pinMode(blueButton, INPUT_PULLUP);
    pinMode(prestartButton, INPUT_PULLUP);
    
    pinMode(redMiniRobotStartLED, OUTPUT);
    pinMode(blueMiniRobotStartLED, OUTPUT);
}

// Don't look at gates at all until prestart button is pressed. This prevents people setting up from getting in the way and causing a false reading.
void waitForPrestart()
{
  while((digitalRead(prestartButton) == HIGH) && !prestartPressed))    // While the prestart button has not been pressed
  {
    startTime = millis();
  }
  if (digitalRead(prestartButton) == LOW)
    prestartPressed = true;
  
}

// Get the timing and state of the red gate
void setupStateRed()
{
  int distance = redGate.getDistance();
  while(distance <= RED_DISTANCE)                // Wait for the gate to open
  {
    distance = redGate.getDistance();
  }
  while(distance > RED_DISTANCE)                // While gate is open
  {
    redTime = millis();
    distance = redGate.getDistance();
  }
}

// Get the timing and state of the blue gate
void setupStateBlue()
{
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
}

void selectRedOrBlue()
{
  while(!choiceMade)                               // Run this code until a break. A break will happen when a selection is made.
  {
    if(digitalRead(redButton) == LOW)       // If the red button is pressed
    {
      redRun = true;
      choiceMade = true;
      break;
    }

    if(digitalRead(blueButton) == LOW)      // If the blue button is pressed
    {
      redRun = false;
      choiceMade = true;
      break;
    }
  }
}

void sendGoToGetters()                      //activate side robots
{
  
}

void waitForReturn()
{
  while(digitalRead(puckSensor1) || digitalRead(puckSensor2) || digitalRead(puckSensor3) || digitalRead(puckSensor4))// low active
  {
    // Do nothing until all of the pucks have returned
  }
  delay(100);
}

void launchRedPuck()
{
  while (puckSensor1 == 1 || puckSensor2 == 1) {
    if (redSafeToLaunch()) {
      digitalWrite(SERVO_1_CLOCKWISE, true);
      delay(10);
      digitalWrite(SERVO_2_CLOCKWISE, true);
      delay(10);
      digitalWrite(SERVO_CENTER_CLOCKWISE, true); // launch with red pucks
    }
  }
}

void launchBluePuck()
{
  while (puckSensor1 == 1 || puckSensor2 == 1) {
    if (blueSafeToLaunch()) {
      digitalWrite(SERVO_1_CCLOCKWISE, true);
      delay(10);
      digitalWrite(SERVO_2_CCLOCKWISE, true);
    }
  }
}

void launchProperPucks()
{
  waitForReturn();
  if(redRun)
  {
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
