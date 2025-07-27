#include "i2c_helper.h"

#define SENSOR_PIN A0
int raw = 0;

void setup() {
  uint8_t i2c_slave_addr = getDynamicI2CAddress();
  Wire.begin(i2c_slave_addr);
  // define callbacks for i2c communication
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);

}

void loop() {
  raw = analogRead(SENSOR_PIN);
  float value = raw * (5.0 / 1023);

  regbuff[EXAMPLE_REG] = value;

}
