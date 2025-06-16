#include <Arduino.h>
#include <utility>
#include "serial_io.hpp"

SerialIO serial;

void setup() { serial.init(); }

void loop() {
  if (!serial.isMessageAvailable()) continue;
}
