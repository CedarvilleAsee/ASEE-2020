// Retrieval bot 4/4/2020

int sensors[8] = {0};
int LINE_SENSOR[] = {PA4, PA5, PA6, PA7, PB3, PA15, PB10, PB11};
int linelocation;
int ts = 50; //percentage
int strictness = 10; //percent reduction per sensor off

int motora = PB0;     // pwm pins for motors
int motorb = PB1;

int adir1 = PB13;      //direction pins for motors
int adir2 = PB12;
int bdir1 = PB14;
int bdir2 = PB15;

void setup() {
  // put your setup code here, to run once:
ts = 255*(ts/100);
}

void loop() {
  // put your main code here, to run repeatedly:
readLine();
}


void readLine() {
  for (int i = 7; i >= 0; --i) {                    //populate sensor array
    sensors[i] = digitalRead(LINE_SENSOR[i]);
  }
  bool sensorsCentered = (sensors[3] == HIGH && sensors[4] == HIGH);
  if (!sensorsCentered){
    linelocation = offcenter();                      //decide locale if not centered
  }
  else {
    linelocation = 0;
  }
  int slowspeed = linelocation * strictness;
  slowspeed = ts - abs(slowspeed);                  // determine speed based on deviance
  if (linelocation <=0){
    motorwrite(false,slowspeed);                     //turn left
  }
  else{
    motorwrite(true,slowspeed);                      //turn right
  }
}

int offcenter(){
  bool onleft = false;
  bool onright = false;
  int location = 0;                                 //what to assume if line is lost
  if (sensors[0] + sensors[1] + sensors[2] + sensors[3] > 1){
    onright = !onright; 
    if(sensors[2] == true){location = 1;}
    if(sensors[1] == true){location = 2;}
    if(sensors[0] == true){location = 3;}
    if(location == 3 && !sensors[1]){location = 4;}
  }
  else{
    onleft = !onleft;
    if(sensors[5] == true){location = 1;}
    if(sensors[6] == true){location = 2;}
    if(sensors[7] == true){location = 3;}
    if(location == 3 && !sensors[7]){location = 4;}
    location *= -1;
  }
  return location;
}

void motorwrite(bool dir, int slow){
  digitalWrite(adir1, HIGH);
  digitalWrite(adir2, LOW);
  digitalWrite(bdir1, HIGH);
  digitalWrite(bdir2, LOW);               //set the direction as forward because we don't quit, we don't look back, #yolofam

  if (dir){
    analogWrite(motora,ts);
    analogWrite(motorb,ts-slow);
  }
  else{
    analogWrite(motora,ts-slow);
    analogWrite(motorb,ts);
  }
}


