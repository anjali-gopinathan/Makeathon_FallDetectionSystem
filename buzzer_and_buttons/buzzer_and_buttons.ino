#include <Adafruit_Arcada.h>

#define buzzerPin 16
#define buttonPin 15

bool disableBuzzer;

Adafruit_Arcada emergencySystem;  //initialize object

void setup() {
  Serial.begin(115200);
  emergencySystem.arcadaBegin();

  //set buzzerPin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {

  //read button values
  emergencySystem.readButtons();
  uint8_t buttonVals = emergencySystem.justPressedButtons();

  //if A is pressed
  if (buttonVals == 65){
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
