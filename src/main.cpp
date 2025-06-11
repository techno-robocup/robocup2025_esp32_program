#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  long time = millis();
  while (true) {
    if (millis() - time > 1000) {
      Serial.println("Test message");
      time = millis();
    }
    while (!Serial.available());
    String str = Serial.readStringUntil('\n');
    if (str == "[RASPI] READY?") {
      Serial.println("[ESP32] READY");
    }
    str = Serial.readStringUntil('\n');
    if (str == "[RASPI] READY CONFIRMED") {
      break;
    }
  }
}

void loop() {}
