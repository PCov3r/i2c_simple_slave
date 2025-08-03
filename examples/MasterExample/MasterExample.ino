#include <TinySensor.h>

TinySensor sensor;

void setup() {
  Serial.begin(115200);
  bool status = sensor.begin();
  delay(500);

  if (!status) {
    Serial.println(status);
    Serial.println("i2c sensor not found... Double check wiring & address.");
    while (1);  // Stop execution
  }

  Serial.println("Sensor successfully initialized.");
  status = sensor.setAccumulatedRain(0.5);
  if(!status) {
    Serial.println("Could not write value.");
    while (1);
  }
  delay(500);
}

void loop() {
  float value = sensor.getInstantRain();
  Serial.print("Got instant value: "); 
  Serial.println(value);
  bool status = sensor.requestAverageRain(1);
  Serial.print("Got status: ");
  Serial.println(status);
  float avg = sensor.getAverageRainResult();
  Serial.print("Got average value: "); 
  Serial.println(avg);
  delay(5000);
}