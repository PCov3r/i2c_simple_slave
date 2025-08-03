#include <Wire.h>
#include "Arduino.h"
#include <control_register.h>

union FloatByteUnion {
    uint32_t asBinary;
    float asFloat;
    byte bytes[4];
};

#define BASE_I2C_ADDRESS 0x08 // Slave Address 
#define ADDR_SEL0_PIN 11     // GPIO for address bit 0
#define ADDR_SEL1_PIN 10     // GPIO for address bit 1

#define REGISTERS_NB 10      // Number of registers

extern FloatByteUnion regbuff[REGISTERS_NB];
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

extern volatile byte regAddr; // Buffer holding the register's address
extern volatile uint32_t tipCounter; // Number of tips from rain gauge

uint8_t getDynamicI2CAddress();
void initRegistersAt(const uint8_t*, const float*, size_t);
void onReceive(int);
float readReg(int, int);
void onRequest();

uint8_t getControl(uint8_t);
uint8_t getArg(uint8_t);
void setStatus(uint8_t, uint8_t);