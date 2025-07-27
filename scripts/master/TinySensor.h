#ifndef TINYSENSOR_H
#define TINYSENSOR_H

#include <Arduino.h>
#include <Wire.h>

#define NUM_BYTES 4       // 4 bytes registers
#define DEFAULT_REG 0x01  

class TinySensor {
public:
  TinySensor(uint8_t address=0x08);
  bool begin();
  float read(uint8_t reg = DEFAULT_REG); 
private:
  uint8_t _address;
};

#endif
