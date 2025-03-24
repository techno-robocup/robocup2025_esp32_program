#include <motorio.hpp>
robot::motorio::motorio(const std::int8_t &_IN1_PIN, const std::int8_t &_IN2_PIN) : IN1_PIN(_IN1_PIN), IN2_PIN(_IN2_PIN) {
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  analogWrite(IN1_PIN, 255);
  analogWrite(IN2_PIN, 255);
}


void robot::motorio::set_speed(int _speed) {
  speed = _speed;
  Serial.println("set ");
  Serial.println(speed);
  return;
}

void robot::motorio::run_speed() {
  if (speed == 0) {
    motor_brake();
    return;
  }
  if (speed < 0) {
    analogWrite(IN1_PIN, 0);
    analogWrite(IN2_PIN, -speed);
    Serial.print(0, -speed);
    Serial.println();
  }
  else {
    analogWrite(IN1_PIN, speed);
    analogWrite(IN2_PIN, 0);
    Serial.print(speed, 0);
    Serial.println();
  }
}