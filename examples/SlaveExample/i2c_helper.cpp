#include "i2c_helper.h"

FloatByteUnion regbuff[REGISTERS_NB];
volatile byte regAddr = 0;
volatile uint32_t tipCounter = 0;

/*******
 Check for i2c address shift by reading the input pins
*******/
uint8_t getDynamicI2CAddress() {
  pinMode(ADDR_SEL0_PIN, INPUT_PULLUP);
  pinMode(ADDR_SEL1_PIN, INPUT_PULLUP);

  delay(10); 

  uint8_t sel0 = (digitalRead(ADDR_SEL0_PIN) == LOW) ? 1 : 0;
  uint8_t sel1 = (digitalRead(ADDR_SEL1_PIN) == LOW) ? 1 : 0;

  uint8_t offset = (sel1 << 1) | sel0;
  return BASE_I2C_ADDRESS + offset;
}

/*******
 Initialize registers default content
*******/
void initRegistersAt(const uint8_t* addresses, const float* values, size_t count) {
  for (size_t i = 0; i < count; ++i) {
    uint8_t addr = addresses[i];
    if (addr < REGISTERS_NB) {
      regbuff[addr].asFloat = values[i];
    }
  }
}

/*******
 On receive from master
 if 1 byte, then read from register
 if > 1 byte, write to register
*******/
void onReceive(int numBytes) {
  if(numBytes == 1) {
    regAddr = Wire.read();
  } else if (numBytes > 1) {
    byte addr = Wire.read();
    FloatByteUnion val;

    for (int i = 0; i < 4 && Wire.available(); i++) {
      val.bytes[i] = Wire.read();
    }

    if (addr == CONTROL_REG) {
      // Only bits 8–31 (control and arguments) can be written
      uint32_t current = regbuff[CONTROL_REG].asBinary;
      uint32_t incoming = val.asBinary;

      // Preserve status bits (0–7), allow writing only to bits 8–31
      current &= 0x000000FF;
      incoming &= 0xFFFFFF00;

      regbuff[CONTROL_REG].asBinary = current | incoming;

    } else if (addr == RAIN_INCR || addr == WIND_ZERO || addr == WIND_COEFF || addr == AVG_DURATION) {
      regbuff[addr] = val;
      setStatus(STAT_SHIFT_WRITE, READY); // Set 'write' status flag to true after writing
    } else {
      // Invalid write attempt: ignore or optionally set error flag
    }

  }
}

/*******
 Read from register at address addr
*******/
float readReg(int addr, int bytecount){
  float regval;
  if (addr >= REGISTERS_NB || bytecount != 4) return 0.0f;

  regval = regbuff[addr].asFloat;
  return regval;
}

/*******
 Handle master's request by sending register's content
 If we read from rain register, reset rain tips count
*******/
void onRequest() {
  FloatByteUnion val;

  val.asFloat = readReg(regAddr, 4);
  Wire.write(val.bytes, 4);

  if (regAddr == CONTROL_REG) {
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << STAT_SHIFT_WRITE);
  }
  if (regAddr == RAIN_HOUR) {
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << STAT_SHIFT_RAIN);
  }
    if (regAddr == WIND_INSTANT) {
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << STAT_SHIFT_WIND);
  }
    if (regAddr == WIND_DIR) {
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << STAT_SHIFT_DIR);
  }

  if (regAddr == RAIN_COUNT) {
    tipCounter = 0;
    regbuff[regAddr].asFloat = 0.0;
  }
}


uint8_t getControl(uint8_t shift) {
  return (regbuff[CONTROL_REG].asBinary >> shift) & 0b11;
}

uint8_t getArg(uint8_t shift) {
  return (regbuff[CONTROL_REG].asBinary >> shift) & 0b1111;
}

void setStatus(uint8_t shift, uint8_t value) {
  regbuff[CONTROL_REG].asBinary &= ~(0b11 << shift);
  regbuff[CONTROL_REG].asBinary |= ((value & 0b11) << shift);
}