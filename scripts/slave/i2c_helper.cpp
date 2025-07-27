#include "i2c_helper.h"

union FloatByteUnion {
    float f;
    byte b[4];
};

float regbuff[REGISTERS_NB];
volatile byte regAddr = 0; // Address to read from, set by master write

uint8_t getDynamicI2CAddress() {
  pinMode(ADDR_SEL0_PIN, INPUT_PULLUP);
  pinMode(ADDR_SEL1_PIN, INPUT_PULLUP);

  delay(10); 

  uint8_t sel0 = (digitalRead(ADDR_SEL0_PIN) == LOW) ? 1 : 0;
  uint8_t sel1 = (digitalRead(ADDR_SEL1_PIN) == LOW) ? 1 : 0;

  uint8_t offset = (sel1 << 1) | sel0;
  return BASE_I2C_ADDRESS + offset;
}

void onReceive(int numBytes) {
  if(numBytes == 1) {
    regAddr = Wire.read();
  } else if (numBytes >= 5) {
    byte addr = Wire.read();
    FloatByteUnion val;

    for (int i = 0; i < 4 && Wire.available(); i++) {
      val.b[i] = Wire.read();
    }

    if (addr < REGISTERS_NB) {
      regbuff[addr] = val.f;
    }
  }
}

float readReg(int addr, int bytecount){
  float regval;
  if (addr >= REGISTERS_NB || bytecount != 4) return 0.0f;

  regval = regbuff[addr];
  return regval;
}

void onRequest() {
  FloatByteUnion val;

  val.f = readReg(regAddr, 4);
  Wire.write(val.b, 4);
}