#include <Arduino.h>
#include "serial_io.hpp"

SerialIO serial;

void setup() { serial.init(); }

void loop() {
  if (serial.isMessageAvailable()) {
    Message message = serial.receiveMessage();
    serial.sendMessage(Message(message.getId(), "Hello from ESP32"));
  }
}
