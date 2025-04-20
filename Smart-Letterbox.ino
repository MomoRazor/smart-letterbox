#include <LowPower.h>
#include "Ultrasonic.h"
#include "FloatSaver.h"

//Ultrasonic Pin Numbers
const int ultraSonicPoweringPin = 8;
const int ultraSonicSendingPin = 9;
const int ultraSonicReceivingPin = 10;

Ultrasonic sensor(ultraSonicSendingPin, ultraSonicReceivingPin, ultraSonicPoweringPin); // Trig, Echo, Power
FloatSaver store;

//turning values
const int sleepSecondsWhileEmpty = 8;
const int sleepSecondsWhileFilled = 16;
const int readingsForStateChange = 5;
const int marginOfErrorCm = 1.5;

//internal contants
const int emptyDistanceStoreIndex = 1;

int filledInSequence = 0;
int notFilledInSequence = 0;
bool currentFillState = false;
float emptyDistance = -1;

void setupEmptySpace () {

  //TODO to remove this and use it else where once controls are introduced
  store.deleteFloat(emptyDistanceStoreIndex);
  
  emptyDistance = store.readFloat(emptyDistanceStoreIndex);

  if(emptyDistance == -2){
    float distance = sensor.readDistanceCm();

    if(distance == -1){
      Serial.println("Total Distance out of Range!");
      //TODO we should make this state visible to the user to show that there is an issue
    }else{
      Serial.print("Setting Empty Distance to ");
      Serial.print(distance);
      Serial.println(" cm");

      store.saveFloat(emptyDistanceStoreIndex, distance);

      emptyDistance = distance;

    }
  }else{
    Serial.println("Calibration Loaded");
  }
}

void sleepAccordingToState () {
  int sleeps = 0;

  if (currentFillState) {
    sleeps = (int)ceil(sleepSecondsWhileFilled / 8);
  } else {
    sleeps = (int)ceil(sleepSecondsWhileEmpty / 8);
  }

  Serial.flush();
  for (int i = 0; i < sleeps; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds
  }
}

void activeMode () {
  float distance = sensor.readDistanceCm();
  if(distance == -1){
    Serial.println("Total Distance out of Range!");
    //TODO we should make this state visible to the user to show that there is an issue
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds
  }else{
    Serial.print(distance);
    Serial.print(" cm / ");
    Serial.print(emptyDistance);
    Serial.println(" cm");
    
    if (distance < (emptyDistance - marginOfErrorCm)) {
      filledInSequence++;
      notFilledInSequence = 0;
    } else {
      notFilledInSequence++;
      filledInSequence = 0;
    }

    if(filledInSequence == readingsForStateChange){
      Serial.println("Mailbox Full!");
      currentFillState = true;
      filledInSequence = 0;

      sleepAccordingToState();
    }else if(notFilledInSequence == readingsForStateChange){
      Serial.println("Mailbox Empty!");
      currentFillState = false;
      notFilledInSequence = 0;
  
      sleepAccordingToState();
    } else {
      Serial.flush();
      LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
    }
  }
}

void setup() {
  Serial.begin(9600);

  sensor.begin();

  delay(1000);

  setupEmptySpace();

}

void loop() {
  if(emptyDistance == -1){
      Serial.println("Not yet calibrated, powering down until calibration");
      for (int i = 0; i < 8; i++) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // Sleep for 8 seconds
      }
  } else { 
    activeMode();
  }
}







