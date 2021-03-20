#ifndef TIME
#define TIME
void SetDelta(){
  CurrentTime = millis();
}

float DeltaTime(){
  float temp = millis() - CurrentTime;
  return temp;
}

#endif
