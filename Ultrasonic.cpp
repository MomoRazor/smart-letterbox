#include "Ultrasonic.h"
#include <Arduino.h>

Ultrasonic::Ultrasonic(int trigPin, int echoPin, int powerPin) {
  _trigPin = trigPin;
  _echoPin = echoPin;
  _powerPin = powerPin;
}

void Ultrasonic::begin() {
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
  pinMode(_powerPin, OUTPUT);

  digitalWrite(_powerPin, LOW); // Sensor off by default
}

float Ultrasonic::readDistanceCm() {
  digitalWrite(_powerPin, HIGH);
  delay(100); // Allow time for sensor to stabilize

  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  long duration = pulseIn(_echoPin, HIGH);
  digitalWrite(_powerPin, LOW); // Turn sensor off

  float distanceCm = duration * 0.034 / 2.0;

  if (distanceCm > 780) {
    return -1;
  } else {
    return distanceCm;
  }
}