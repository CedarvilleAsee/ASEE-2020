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

    redSensor.begin();
    blueSensor.begin();
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
  int distance = redGate.readRange();
  while(distance <= RED_DISTANCE)                // Wait for the gate to open
  {
    distance = redGate.readRange();
  }
  while(distance > RED_DISTANCE)                // While gate is open
  {
    redTime = millis();
    distance = redGate.readRange();
  }
}

// Get the timing and state of the blue gate
void setupStateBlue()
{
  int distance = blueGate.readRange();
  while(distance < BLUE_DISTANCE)                // Wait for the gate to open
  {
    distance = blueGate.readRange();
  }
  while(distance > BLUE_DISTANCE)                // While gate is open
  {
    blueTime = millis();
    distance = blueGate.readRange();
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


void pushRamps() {
  int pos;
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    RAMP_SERVO.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    RAMP_SERVO.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void sendGoToGetters()                      //activate side robots
{
  pushRamps();
  // activate robots
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
      SERVO_1.write(150) // servos placed opposite directions, release at a delay of 10 ms
      delay(10);
      SERVO_2.write(30);
      delay(10);
      SERVO_CENTER.write(150)
    }
  }
}

void launchBluePuck()
{
  while (puckSensor1 == 1 || puckSensor2 == 1) {
    if (blueSafeToLaunch()) {
      SERVO_1.write(30)
      delay(10);
      SERVO_2.write(150);
      delay(10);

    }
  }
}

void launchProperPucks()
{ // calls functions to launch pucks
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
