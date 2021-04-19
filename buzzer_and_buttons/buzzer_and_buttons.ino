#include <Adafruit_Arcada.h>

#define buzzerPin 16
#define touchSensorPin 15

bool disableBuzzer;
int touchSensor;

Adafruit_Arcada emergencySystem;  //initialize object

void setup() {
  Serial.begin(115200);
  emergencySystem.arcadaBegin();

  //set buzzerPin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  //set touchsensor pin
  pinMode(touchSensorPin, INPUT);
}

void loop() {

  //read gyro values
  sensors_event_t gyro;
  lsm6ds33.getEvent(NULL, &gyro, NULL);
  float x = gyro.gyro.x * SENSORS_RADS_TO_DPS;
  float y = gyro.gyro.y * SENSORS_RADS_TO_DPS;

  //read button values
  emergencySystem.readButtons();
  uint8_t buttonVals = emergencySystem.justPressedButtons();
  touchSensor = digitalRead(touchSensorPin);
  //if A is pressed
  if (buttonVals == 65 || touchSensor == LOW || x > 50 || y > 50){
    bool disableBuzzer = 0;

    //while B button is not pressed
    while (!disableBuzzer){
      digitalWrite(buzzerPin, HIGH);  //turn on buzzer

      //read for button B to turn off (no interrupts available)
      for (int i = 0 ; i < 50 ; i++) {
        buttonVals = emergencySystem.readButtons();
        if (buttonVals == 130) {
          digitalWrite(buzzerPin, LOW);
          disableBuzzer = 1;
          break;
        }
        delay(10);
      }
      
      digitalWrite(buzzerPin, LOW);  //turn off buzzer
      
      //read for button B to keep off (no interrupts available)
      for (int i = 0 ; i < 50 ; i++) {
        buttonVals = emergencySystem.readButtons();
        if (buttonVals == 130) {
          disableBuzzer = 1;
          break;
        }
        delay(10);
      }
    }
  }
  
}
