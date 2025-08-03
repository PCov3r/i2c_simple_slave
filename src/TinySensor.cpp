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

float TinySensor::getInstantRain() {
  return read(RAIN_COUNT);
}

float TinySensor::getAverageWind() {
  return read(WIND_AVG);
}




bool TinySensor::requestInstantWind() {
  bool return_value = false;

  FloatByteUnion control;
  control.asBinary = 0;
  control.asBinary |= (0x01 << CTRL_SHIFT_WIND);
  write(CONTROL_REG, control.asBinary);

  return_value = true;
  uint32_t timeout_start = millis();
  while (read_status(STAT_SHIFT_WIND) != READY) {
    // In case of a timeout, stop the while loop
    if ((millis() - timeout_start) > 2000) {
      return_value = false;
      break;
    }
  delay(1);
  }
  return return_value;
}

float TinySensor::getInstantWindResult() {
  return read(WIND_INSTANT);
}




bool TinySensor::requestInstantDirection() {
  bool return_value = false;

  FloatByteUnion control;
  control.asBinary = 0;
  control.asBinary |= (0x01 << CTRL_SHIFT_DIR);
  write(CONTROL_REG, control.asBinary);

  return_value = true;
  uint32_t timeout_start = millis();
  while (read_status(STAT_SHIFT_DIR) != READY) {
    // In case of a timeout, stop the while loop
    if ((millis() - timeout_start) > 2000) {
      return_value = false;
      break;
    }
  delay(1);
  }
  return return_value;
}

float TinySensor::getInstantDirResult() {
  return read(WIND_DIR);
}




bool TinySensor::requestAverageRain(int hours) {
  bool return_value = false;
  if (hours < 1 || hours > 24) return return_value;
  FloatByteUnion control;
  control.asBinary = 0;
  control.asBinary |= (0x01 << CTRL_SHIFT_RAIN);
  control.asBinary |= (uint32_t(hours) << ARG_SHIFT_RAIN);
  write(CONTROL_REG, control.asBinary);

  return_value = true;
  uint32_t timeout_start = millis();
  while (read_status(STAT_SHIFT_RAIN) != READY) {
    // In case of a timeout, stop the while loop
    if ((millis() - timeout_start) > 2000) {
      return_value = false;
      break;
    }
  delay(1);
  }
  return return_value;
}

float TinySensor::getAverageRainResult() {
  return read(RAIN_HOUR);
}




bool TinySensor::requestAll(int rain_hours){
  bool return_value = false;
  if (rain_hours < 1 || rain_hours > 24) return return_value;

  FloatByteUnion control;
  control.asBinary = 0;
  control.asBinary |= (0x01 << CTRL_SHIFT_ALL);
  control.asBinary |= (uint32_t(rain_hours) << ARG_SHIFT_RAIN);
  write(CONTROL_REG, control.asBinary);

  return_value = true;
  uint32_t timeout_start = millis();
  while ((read_status(STAT_SHIFT_WIND) != READY) && (read_status(STAT_SHIFT_DIR) != READY) && (read_status(STAT_SHIFT_RAIN) != READY)) {
    // In case of a timeout, stop the while loop
    if ((millis() - timeout_start) > 2000) {
      return_value = false;
      break;
    }
  delay(1);
  }
  return return_value;
}


bool TinySensor::setAccumulatedRain(float value) {
  return setParameter(RAIN_INCR, value);
}

bool TinySensor::setWindZero(float value) {
  return setParameter(WIND_ZERO, value);
}

bool TinySensor::setWindCoeff(float value) {
  return setParameter(WIND_COEFF, value);
}


bool TinySensor::setParameter(uint8_t reg, float value) {
  float return_value = false;
  FloatByteUnion data;
  data.asFloat = value;
  write(reg, data.asBinary);

  return_value = true;
  uint32_t timeout_start = millis();
  while (read_status(STAT_SHIFT_WRITE) != READY) {
    // In case of a timeout, stop the while loop
    if ((millis() - timeout_start) > 2000) {
      return_value = false;
      break;
    }
  delay(1);
  }
  return return_value;
}

uint8_t TinySensor::read_status(int shift) {
  FloatByteUnion status;
  status.asFloat = read(CONTROL_REG);
  return ((status.asBinary & STATUS_MASK) >> shift) & 0b11;
}

float TinySensor::read(uint8_t reg) {
  // Write the register address to the slave
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission();
  // delayMicroseconds(20);  // Optional, may not be needed on all platforms

  // Request 4 bytes from the slave
  Wire.requestFrom(_address, (uint8_t)4);
  if (Wire.available() < 4) {
    return NAN;
  }

  FloatByteUnion data;
  for (uint8_t i = 0; i < 4; i++) {
    data.bytes[i] = Wire.read();
  }

  return data.asFloat;
}

bool TinySensor::write(uint8_t reg, uint32_t val) {
  FloatByteUnion data;
  data.asBinary = val;

  Wire.beginTransmission(_address);
  Wire.write(reg);            // Send the register address
  Wire.write(data.bytes, 4);  // Send the 4 bytes of the value
  return (Wire.endTransmission() == 0);  // Return true on success
}

