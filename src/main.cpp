#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  while(true){
    while(!Serial.available());
    String str = Serial.readStringUntil('\n');
    if (str == "[RASPI] READY?"){
      Serial.println("[ESP32] READY");
    }
    str = Serial.readStringUntil('\n');
    if (str == "[RASPI] READY CONFIRMED"){
      break;
    }
  }
}

void loop() {

}