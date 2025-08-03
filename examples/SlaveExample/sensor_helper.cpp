#include "sensor_helper.h"

const unsigned long HOUR = 10000;//3600000UL;
const unsigned long MINUTE = 10000;//60000UL;

unsigned long last_wind_update = 0;
int roll_arr[ROLL_WINDOW] = {0};
uint8_t roll_pointer = 0;
float sum = 0;

bool rain_flag = false;
unsigned long last_rain_update = 0;
float rain_history[MAX_HOURS] = {0.0f};
uint8_t hour_pointer = 0;
volatile uint32_t hourlyTipCounter = 0; 

const int directions[8] = {0,45,90,135,180,225,270,315};

int analog_val = 0;


void handleControlRequests() {
  // Wind Instant
  if (getControl(CTRL_SHIFT_WIND) == REQUEST) {
    analog_val = analogRead(ANEMOMETER_PIN);
    regbuff[WIND_INSTANT].asFloat = analog_val * regbuff[WIND_COEFF].asFloat + regbuff[WIND_ZERO].asFloat;
    setStatus(STAT_SHIFT_WIND, READY);
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << CTRL_SHIFT_WIND);
  }

  // Wind Dir
  if (getControl(CTRL_SHIFT_DIR) == REQUEST) {
    analog_val = analogRead(VANE_PIN);
    regbuff[WIND_DIR].asFloat = directions[analog_val / 8];
    setStatus(STAT_SHIFT_DIR, READY);
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << CTRL_SHIFT_DIR);
  }

  // Rain Hour
  if (getControl(CTRL_SHIFT_RAIN) == REQUEST) {
    uint8_t hours = getArg(ARG_SHIFT_RAIN);
    regbuff[RAIN_HOUR].asFloat = rainOverLastHours(hours);
    setStatus(STAT_SHIFT_RAIN, READY);
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << CTRL_SHIFT_RAIN);
  }

  // All sensors
  if (getControl(CTRL_SHIFT_ALL) == REQUEST) {
    analog_val = analogRead(ANEMOMETER_PIN);
    regbuff[WIND_INSTANT].asFloat = analog_val * regbuff[WIND_COEFF].asFloat + regbuff[WIND_ZERO].asFloat;
    analog_val = analogRead(VANE_PIN);
    regbuff[WIND_DIR].asFloat = directions[analog_val / 8];
    uint8_t hours = getArg(ARG_SHIFT_RAIN);
    regbuff[RAIN_HOUR].asFloat = rainOverLastHours(hours);
    setStatus(STAT_SHIFT_WIND, READY);
    setStatus(STAT_SHIFT_DIR, READY);
    setStatus(STAT_SHIFT_RAIN, READY);
    regbuff[CONTROL_REG].asBinary &= ~(0b11 << CTRL_SHIFT_ALL);
  }
}

/*******
 Read wind speed every minute and calculate new average
*******/
void handleWindAverage(){
  unsigned long now = millis();

  if (now - last_wind_update >= MINUTE) {
    // save the last time you blinked the LED
    last_wind_update = now;
    roll_arr[roll_pointer] = analogRead(ANEMOMETER_PIN);
    roll_pointer = (roll_pointer + 1) % ROLL_WINDOW ;
    sum = 0;
    for(int i =0; i < ROLL_WINDOW; i++){
      sum += roll_arr[i];
    }
    float avg_ADC = (sum/ROLL_WINDOW);
    regbuff[WIND_AVG].asFloat = avg_ADC*regbuff[WIND_COEFF].asFloat+regbuff[WIND_ZERO].asFloat;
  }
}

/*******
 Update rain level on interrupt
*******/
void handleRain(){
  if (rain_flag==true){
    blink(2,50);
    tipCounter++;
    hourlyTipCounter++;
    float rain_increment = regbuff[RAIN_INCR].asFloat;  // Use the current register value
    float rainfall = tipCounter * rain_increment;

    regbuff[RAIN_COUNT].asFloat = rainfall;
    rain_flag = false;
  }
}

void handleRainHourly() {
  unsigned long now = millis();

  if (now - last_rain_update >= HOUR) {
    last_rain_update = now;
    float rain_increment = regbuff[RAIN_INCR].asFloat;
    rain_history[hour_pointer] = hourlyTipCounter * rain_increment;

    hour_pointer = (hour_pointer + 1) % MAX_HOURS;
    hourlyTipCounter = 0;
  }
}

float rainOverLastHours(uint8_t hours) {
  if (hours > MAX_HOURS) hours = MAX_HOURS;

  float total = 0.0f;
  for (uint8_t i = 0; i < hours; i++) {
    int index = (hour_pointer + MAX_HOURS - i - 1) % MAX_HOURS;
    total += rain_history[index];
  }
  return total;
}

/*******
 Blink onboard led
*******/
void blink(int n, int time){
  for (int i = 0; i < n; i++){
  digitalWrite(LED_PIN, HIGH); // sets the digital pin 13 on
  delay(time);            // waits for a second
  digitalWrite(LED_PIN, LOW);
  delay(time);
  }
}