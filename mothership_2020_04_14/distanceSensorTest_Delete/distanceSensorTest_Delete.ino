void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A3, INPUT);
  pinMode(A2, INPUT);
  digitalWrite(A0, HIGH);
  Serial.begin(115200);

}

void loop() {
  //Serial.println(analogRead(A0));
  Serial.println(digitalRead(A2));

}
