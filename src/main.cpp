#include <Arduino.h>

bool isReady = false;
unsigned long lastMessageTime = 0;

void setup() {
  Serial.begin(9600);
  while(true) {
    if (Serial.available()) {
      String str = Serial.readStringUntil('\n');
      if (str == "[RASPI] READY?") {
        Serial.println("[ESP32] READY");
      } else if (str == "[RASPI] READY CONFIRMED") {
        isReady = true;
      }
    }
  }
}

void loop() {
  
}
