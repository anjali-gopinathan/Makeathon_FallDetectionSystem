int heartbeatSensorPin = 1;
int buzzerPin = 16;
double alpha = 0.75;
int period = 20;
double change = 0.0;

void setup() {
  // put your setup code here, to run once:
//  pinMode(touchSensorPin, INPUT);  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
//  touched = digitalRead(touchSensorPin);
//  digitalWrite(buzzerPin, touched);
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead (heartbeatSensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  Serial.print (rawValue);
  Serial.print (",");
  Serial.println (value);
  oldValue = value;
  
  if (value > 850){
    digitalWrite(buzzerPin, HIGH);
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
  
  delay (period);
  

}
