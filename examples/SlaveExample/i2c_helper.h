#include <Wire.h>
#include "Arduino.h"

#define BASE_I2C_ADDRESS 0x08 //Slave Address 
#define ADDR_SEL0_PIN 2     // GPIO for address bit 0
#define ADDR_SEL1_PIN 3     // GPIO for address bit 1

#define REGISTERS_NB 256


extern float regbuff[REGISTERS_NB];
const byte EXAMPLE_REG = 0x01;

uint8_t getDynamicI2CAddress();
void onReceive(int);
float readReg(int, int);
void onRequest();