#include <Arduino.h>
#include <utility>
#include "serial_io.hpp"

SerialIO serial;

void setup() { serial.init(); }

void loop() {
  if (!serial.isMessageAvailable()) return;

  Message msg = serial.receiveMessage();
  String message = msg.getMessage();

  if (message.startsWith("Motor ")) {
    message = message.substring(6);
    if (message.length() == 19) {
      int tyre_values[4];
      int idx = 0;

      while (message.length() > 0 && idx < 4) {
        int spaceIndex = message.indexOf(' ');
        if (spaceIndex == -1) {
          tyre_values[idx++] = message.toInt();
          break;
        } else {
          tyre_values[idx++] = message.substring(0, spaceIndex).toInt();
          message = message.substring(spaceIndex + 1);
        }
      }

      if (idx == 4) {
        Serial.printf("motor values = %d %d %d %d\n", tyre_values[0], tyre_values[1],
                      tyre_values[2], tyre_values[3]);  // TODO: clate motor fn
      } else {
        Serial.println("Error: less than 4 values");
      }
    } else {
      Serial.println("Error: motor message must be 4-digit");
    }
  }
  if (message.startsWith("Rescue ")) {
    message = message.substring(7);
    if (message.length() == 5) {  // NOTE: ID Rescue arm_angle wire(0,1)
      int arm_values = message.substring(0, 4).toInt();
      bool wire = message[5];
    }
  }
}
