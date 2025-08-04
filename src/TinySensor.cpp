#include "TinySensor.h"

TinySensor::TinySensor(uint8_t address) {
  _address = address;
}

bool TinySensor::begin() {
  Wire.begin();
  Wire.beginTransmission(_address);
  uint8_t error = Wire.endTransmission();

  return (error == 0);  // 0 = success (ACK), others = error
}

float TinySensor::read(uint8_t reg) {
  // Write the register address to the slave first
  Wire.beginTransmission(_address);
  Wire.write(reg);
  delayMicroseconds(20);
  Wire.endTransmission();

  // Read 4 bytes value from register
  Wire.requestFrom(_address, NUM_BYTES);
  if (Wire.available() < NUM_BYTES) {
    return NAN;
  }

  uint8_t data[NUM_BYTES];
  for (uint8_t i = 0; i < NUM_BYTES; i++) {
    data[i] = Wire.read();
  }

  float value = 0.0f;
  memcpy(&value, data, sizeof(value));
  return value;
}

