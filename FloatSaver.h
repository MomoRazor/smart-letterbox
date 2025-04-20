
#ifndef FLOATSAVER_H
#define FLOATSAVER_H

#include <Arduino.h>   // <-- brings in the 'byte' typedef

class FloatSaver {
  public:
    FloatSaver(byte overrideSavedByte = 0x42);
    //TODO this should be generate to save any value type
    float saveFloat(int index, float value);
    //TODO this should be generate to get any value type
    float readFloat(int index);
    void deleteFloat(int index);

  private:
    byte _savedByte;
};

#endif