#include <Arduino.h>
#include <Wire.h>
#include <motorio.hpp>
#include <mutex>

void receiveEvent(int);
void requestEvent();
template <typename... Args> inline void debugprint(Args... args);
template <typename... Args> inline void debugprintln(Args... args);

#define _DEBUG 1

int motor_speed[2] = {1500, 1500};

int motor_pin = 32;
int fb_pin = 33;
int motor2_pin = 26;
int fb2_pin = 27;
int speed = 1500;
int incrementer = 1;
TaskHandle_t motor_Taskhandler;

bool recieved = false;
int cmd = 0x10;

MOTORIO motor;
MOTORIO motor2;
std::mutex motormutex;

void MotorControlTask(void *pvParameters) {
  while (true) {
    {
      debugprintln("[0] locking mutex");
      std::lock_guard<std::mutex> _(motormutex);
      motor.run_msec(motor_speed[0]);
      motor2.run_msec(motor_speed[1]);
      vTaskDelay(15 / portTICK_PERIOD_MS);
      debugprintln("[0] unlocking mutex");
    }
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  motor = MOTORIO(motor_pin);
  motor2 = MOTORIO(motor2_pin);
  pinMode(fb_pin, INPUT);
  pinMode(fb2_pin, INPUT);

  xTaskCreatePinnedToCore(MotorControlTask, "MotorTask", 2048, nullptr, 1,
                          &motor_Taskhandler, 0);
}

void receiveEvent(int byteNum) {
  std::int8_t a, b;
  debugprintln(byteNum);
  if (byteNum < 5) {
    while (Wire.available()) {
      Wire.read();
    }
    return;
  } else {
    cmd = Wire.read();
    if (cmd == 0x00) {
      a = Wire.read();
      if (a == 0) {
        motor_speed[0] = Wire.read();
      }
      if (a == 1) {
        motor_speed[0] = -1 * Wire.read();
      }
      b = Wire.read();
      if (b == 0) {
        motor_speed[1] = Wire.read();
      }
      if (b == 1) {
        motor_speed[1] = -1 * Wire.read();
      }
    } else {
      return;
    }
  }
  while (Wire.available()) {
    Wire.read();
  }
  recieved = true;
  cmd = 0x10;
  return;
}

void requestEvent() {
  if (recieved)
    Wire.write(0x00);
  if (!recieved)
    Wire.write(0x01);
  return;
}
void loop() {
  speed += incrementer;
  if (speed >= 2000) incrementer = -1;
  if (speed <= 1000) incrementer = 1;
  {
    std::lock_guard<std::mutex> _(motormutex);
    motor_speed[0] = motor_speed[1] = speed;
  }
  delay(10);
}

#ifdef _DEBUG
template <typename... Args> inline void debugprint(Args... args) {
  (Serial.print(args), ...);
}

template <typename... Args> inline void debugprintln(Args... args) {
  (Serial.print(args), ...);
  Serial.println();
}
#else
template <typename... Args> inline void debugprint(Args...) {}
template <typename... Args> inline void debugprintln(Args...) {}
#endif
