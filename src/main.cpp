#include <Arduino.h>
#include "serial_io.hpp"

SerialIO serial;

constexpr int button_pin = 21;
int tyre_values[4];
int arm_value;
bool wire;

int readbutton() { return digitalRead(button_pin); }

void setup() {
  serial.init();
  pinMode(button_pin, INPUT);
}

void loop() {
  if (!serial.isMessageAvailable()) return;

  Message msg = serial.receiveMessage();

  if (msg.getMessage().startsWith("Motor ")) {
    String message = msg.getMessage().substring(6);
    if (message.length() == 19) {
      int idx = 0;
      // TODO: Optimize the coe by not copying the String
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
    } else {
      return;
    }
  }
  if (msg.getMessage().startsWith("Rescue ")) {
    String message = msg.getMessage().substring(7);
    if (message.length() == 5) {  // NOTE: ID Rescue arm_angle wire(0,1)
      arm_value = message.substring(0, 4).toInt();
      wire = message[5];
    } else {
      return;
    }
  }
  // if (message.startsWith("Sonic")) {
  //   serial.sendMessage();
  // }
  if (msg.getMessage().startsWith("Get button")) {
    serial.sendMessage(readbutton());
  }
}
