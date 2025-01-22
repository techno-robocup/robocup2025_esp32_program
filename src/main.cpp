#include <Arduino.h>
#include <Wire.h>
void receiveEvent(int);
void requestEvent();
template<typename... Args>
inline void debugprint(Args... args);
template<typename... Args>
inline void debugprintln(Args... args);

#define _DEBUG 1

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop() {
  debugprintln("LOOP");
}

void receiveEvent(int byteNum) {
  debugprint("Received: ", byteNum, "\n");

  for (int i = 0; i < byteNum; i++) {
    char c = Wire.read();
    debugprint(c);
  }
  debugprintln("");
}

void requestEvent() {
  return;
}

#ifdef _DEBUG
template<typename... Args>
inline void debugprint(Args... args) {
  (Serial.print(args), ...);
}

template<typename... Args>
inline void debugprintln(Args... args) {
  (Serial.print(args), ...);
  Serial.println();
}
#else
template<typename... Args>
inline void debugprint(Args...) {}
template<typename... Args>
inline void debugprintln(Args...) {}
#endif
