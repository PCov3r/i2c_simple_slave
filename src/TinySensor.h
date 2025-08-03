#ifndef TINYSENSOR_H
#define TINYSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include "control_register.h"

union FloatByteUnion {
    uint32_t asBinary;
    float asFloat;
    byte bytes[4];
};

#define NUM_BYTES 4       // 4 bytes registers
const byte CONTROL_REG      = 0x00; // Rain increment in mm
const byte RAIN_INCR        = 0x01; // Rain increment in mm
const byte WIND_ZERO        = 0x02; // Analog value at 0 m/s
const byte WIND_COEFF       = 0x03; // Wind speed voltage slope
const byte AVG_DURATION     = 0x04; // Time interval for average calculation (in minutes) 
const byte RAIN_COUNT       = 0x05; // Rain gauge value
const byte RAIN_HOUR        = 0x06; // Hourly rain value
const byte WIND_AVG         = 0x07; // Average wind speed over duration
const byte WIND_INSTANT     = 0x08; // Instantaneous wind speed
const byte WIND_DIR         = 0x09; // Wind direction

class TinySensor {
public:
  TinySensor(uint8_t address=0x08);
  bool begin();
  float getInstantRain();
  float getAverageWind();
  bool requestInstantWind();
  float getInstantWindResult();
  bool requestInstantDirection();
  float getInstantDirResult();
  bool requestAverageRain(int);
  float getAverageRainResult();
  bool requestAll(int);
  bool setAccumulatedRain(float value);  
  bool setWindZero(float value);  
  bool setWindCoeff(float value);  
  
private:
  uint8_t _address;
  float read(uint8_t reg); 
  bool write(uint8_t reg, uint32_t val);
  uint8_t read_status(int shift);
  bool setParameter(uint8_t, float);
};

#endif
