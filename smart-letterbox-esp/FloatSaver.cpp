#include "FloatSaver.h"
#include <EEPROM.h>
#include <Arduino.h>

const float indexError = -1;
const float valueNotFoundError = -2;

FloatSaver::FloatSaver(byte overrideSavedByte) {
  //TODO this should be optional that would change the private saveChar if provided
  _savedByte = overrideSavedByte;
}

float FloatSaver::saveFloat(int index, float value) {
  if(index <= 0){
    return indexError;
  }

  EEPROM.update(index-1, _savedByte);  // Magic number
  EEPROM.put(index, value); // Store float after magic byte

  return value;
}

float FloatSaver::readFloat(int index){
  if(index <= 0){
    return indexError;
  }

  byte magic = EEPROM.read(index-1);

  if (magic == _savedByte) {
    float distance;
    EEPROM.get(index, distance);
    return distance;
  } else {
    return valueNotFoundError; // Indicates not calibrated
  }
}

void FloatSaver::deleteFloat(int index){
    if(index <= 0){
    return indexError;
  }

  byte magic = EEPROM.read(index-1);

  if (magic == _savedByte) {
    EEPROM.update(index, 0x00);
    EEPROM.update(index-1, 0x00);
  } else {
    return valueNotFoundError; // Indicates not calibrated
  }

}