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

// state LEDs identify if gate is open for user testing
void updateStateLED()
{
  digitalWrite(redStatusLED, redIsOpen());
  digitalWrite(blueStatusLED, blueIsOpen());
}

//Main functions-----------------------------------------------------------------------------------------------------------------------------------------------------
void initializePins()                           //Setup all pins
{
    if (fullTestMode) {
      redGateSensor.begin();                          //Initialize red gate sensor
      redGateSensor.startRanging();
    }
    blueGateSensor.begin();                         //Initialize blue gate sensor
    // drive low others
    blueGateSensor.setI2CAddress(0x60);
    blueGateSensor.startRanging();

    // servo pins attach
    servo1.attach(9);
    if (fullTestMode) {
      centerServo.attach(10);
      servo2.attach(11);
      rampServo.attach(12);
    }

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
   }
   displayAvailable = true;
   testdrawcircle();// tests display
//   display.clearDisplay();
   
}

// Get the timing and state of the red gate
void setupStateRed()
{  
  int distance = redGate.getDistance();
  while(distance < GATE_DISTANCE)     //closed
  {
    //Wait for gate to close
    distance = blueGate.getDistance();
    Serial.println(distance);
  }
  while(distance > GATE_DISTANCE)         //reopen
  {
    distance = blueGate.getDistance();
    Serial.println(distance);
  }
  redTime = millis();
  updateStateLED();
  write("Red state updated");
}

// Get the timing and state of the blue gate
void setupStateBlue()
{
  int distance = blueGate.getDistance();
  while(distance < GATE_DISTANCE)     //closed
  {
    //Wait for gate to close
    distance = blueGate.getDistance();
    Serial.println("Blue gate closed: " + distance);
  }
  while(distance > GATE_DISTANCE)         //open, wait for close
  {
    distance = blueGate.getDistance();
    Serial.println("Blue gate open: " + distance);
  }
  blueTime = millis();
  updateStateLED();
  write("Blue state updated");
}

void pushRamps() {
  int pos;
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    rampServo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  for (pos = 100; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    rampServo.write(pos);              // tell servo to go to position in variable 'pos'
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
  while(digitalRead(puckSensor1) || digitalRead(puckSensor2) || digitalRead(puckSensor3) || digitalRead(puckSensor4))// low active ?
  {
    // Do nothing until all of the pucks have returned
  }
  delay(100);
}

void launchRedPuck()
{
  write("Launching red");
  while (puckSensor1 == 1 || puckSensor2 == 1) {
    updateStateLED();
    if (!fullTestMode || redSafeToLaunch()) {
      servo1.write(120); // servos placed opposite directions, release at a delay of 10 ms
      delay(100);
      servo2.write(60);
      delay(100);
      centerServo.write(120);
    }
    if (!fullTestMode) {
      puckSensor1 = 0;
      puckSensor2 = 0;
    }
  }
}

void launchBluePuck()
{
  write("Launching blue");
  updateStateLED();
  while (puckSensor3 == 1 || puckSensor4 == 1) {
    updateStateLED();

    if (!fullTestMode || blueSafeToLaunch()) {
      servo1.write(60);
      delay(100);
      servo2.write(120);
      delay(100);
    }
    if (fullTestMode) {
      puckSensor3 = 0;
      puckSensor4 = 0;
    }
  }
}

void launchProperPucks()
{ // calls functions to launch pucks
  if (fullTestMode) {
    waitForReturn();
  }
  write("Ready to launch");
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

void write(char msg[]) {
  if (displayAvailable) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F(msg));
  }
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}
