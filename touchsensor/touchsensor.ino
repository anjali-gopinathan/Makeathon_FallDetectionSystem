int touchSensorPin = 16;
int buzzerPin = 15;
int touched;

void setup() {
  // put your setup code here, to run once:
  pinMode(touchSensorPin, INPUT);  
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  touched = digitalRead(touchSensorPin);
  digitalWrite(buzzerPin, touched);

}
