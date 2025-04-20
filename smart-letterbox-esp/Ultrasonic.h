
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

class Ultrasonic {
  public:
    Ultrasonic(int trigPin, int echoPin, int powerPin);
    void begin();
    float readDistanceCm();

  private:
    int _trigPin;
    int _echoPin;
    int _powerPin;
};

#endif