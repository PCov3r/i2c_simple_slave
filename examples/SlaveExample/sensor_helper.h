#include "i2c_helper.h"

// Define pinout
// #define ANEMOMETER_PIN 2
// #define VANE_PIN 7
// #define RAIN_PIN 13 
// #define LED_PIN 5 
#define ANEMOMETER_PIN A1
#define VANE_PIN A0
#define RAIN_PIN 2 
#define LED_PIN 13

// Default register values
#define rain_tips 0.28f
#define wind_zero 0.0f
#define wind_slope 1.0f
#define avg_delay 15

#define ROLL_WINDOW  30
#define MAX_HOURS 24
extern const unsigned long HOUR;
extern const unsigned long MINUTE;

// Wind directions
extern const int directions[8];

// Variables for wind speed rolling average
extern unsigned long last_wind_update;
extern int roll_arr[ROLL_WINDOW];
extern uint8_t roll_pointer;
extern float sum;

// Variables for rain gauge history
extern bool rain_flag;
extern unsigned long last_rain_update;
extern float rain_history[MAX_HOURS];
extern uint8_t hour_pointer;
extern volatile uint32_t hourlyTipCounter; 

// Analog value buffer
extern int analog_val;

void handleWindAverage();
void handleControlRequests();
void handleRain();
void handleRainHourly();
float rainOverLastHours(uint8_t hours);
void blink(int, int);
