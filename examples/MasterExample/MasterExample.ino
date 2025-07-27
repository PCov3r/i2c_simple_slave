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
}

void loop() {
  float value = sensor.read();
  Serial.print("Got value: ");
  Serial.print(value);
  delay(1000);
}