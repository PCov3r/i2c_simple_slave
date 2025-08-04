#include "sensor_helper.h"

uint8_t regAddrs[] = {CONTROL_REG, RAIN_INCR, WIND_ZERO, WIND_COEFF, AVG_DURATION};
float regVals[] = {0x0000, rain_tips, wind_zero, wind_slope, avg_delay};
unsigned long lastmillis = 0;

void setup() {
  // Serial.begin(115200);
  // start i2c slave
  uint8_t i2c_slave_addr = getDynamicI2CAddress();
  initRegistersAt(regAddrs, regVals, sizeof(regAddrs) / sizeof(uint8_t));
  Wire.begin(i2c_slave_addr);
  // define callbacks for i2c communication
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  
  // init interrupt;
  pinMode(RAIN_PIN, INPUT_PULLUP);  // GPIO 1 = PA2, enable internal pull-up
  attachInterrupt(digitalPinToInterrupt(RAIN_PIN), tipISR, FALLING);
  // init other pins
  pinMode(LED_PIN, OUTPUT);
  // blink to signal sensor start
  blink(5, 50);
}

void loop() {

  handleControlRequests();
  handleWindAverage();
  handleRain();
  handleRainHourly();
  // printRegisterContent();
}

// set rain flag, on interrupt
void tipISR()
{
  rain_flag = true;
}

void printRegisterContent(){
  unsigned long now = millis();
  if (now - lastmillis >= 200) {
    Serial.println("###########");
    for(int i = 1; i < 10; i++){
      Serial.println(regbuff[i].asFloat);
    }
    Serial.println("\n\n");
    lastmillis = now;
  }
  
  
}
