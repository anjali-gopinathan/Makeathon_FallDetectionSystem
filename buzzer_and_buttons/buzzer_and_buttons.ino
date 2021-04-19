#include <Adafruit_Arcada.h>

#define buzzerPin 16
#define heartbeatSensorPin 1

bool disableBuzzer;
int touchSensor;

// constants for calculating finger pulse
double alpha = 0.75;
double change = 0.0;

Adafruit_Arcada emergencySystem;  //initialize object

void setup() {
  Serial.begin(115200);
  emergencySystem.arcadaBegin();
  emergencySystem.displayBegin();
  

  //set buzzerPin
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);


  // arcada display
  
  for (int i=0; i<250; i+=10) {
    emergencySystem.setBacklight(i);
    delay(1);
  }
  emergencySystem.display->setCursor(0, 0);
  emergencySystem.display->setTextWrap(true);
  emergencySystem.display->setTextSize(8);
  emergencySystem.display->setTextColor(ARCADA_RED, ARCADA_WHITE);

}

void loop() {

  //finger pulse sensor
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead (heartbeatSensorPin);
  double pulseValue = alpha * oldValue + (1 - alpha) * rawValue;
  Serial.print (rawValue);
  Serial.print (",");
  Serial.println (pulseValue);
  oldValue = pulseValue;
  emergencySystem.display->setCursor(0,0);
  emergencySystem.display->print("ExistenceAlarm");
  emergencySystem.display->setTextSize(4);
  emergencySystem.display->print(pulseValue);


  //read button values
  emergencySystem.readButtons();
  uint8_t buttonVals = emergencySystem.justPressedButtons();
  
  //if A is pressed
  if (buttonVals == 65 || pulseValue > 850){
    flashEmergencyScreen();
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

void flashEmergencyScreen(){
  
}
