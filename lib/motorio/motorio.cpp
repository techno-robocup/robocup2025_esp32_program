#include <motorio.hpp>
MOTORIO::MOTORIO(const std::int8_t &_PIN) : PIN(_PIN) { pinMode(PIN, OUTPUT); }

MOTORIO::MOTORIO() {}
void MOTORIO::run_msec(const int &msec) {
  digitalWrite(PIN, HIGH);
  delayMicroseconds(msec);
  digitalWrite(PIN, LOW);
  return;
}