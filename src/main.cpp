#include <Arduino.h>
#include "motorio.hpp"
#include "serial_io.hpp"
SerialIO serial;

constexpr int tyre_1 = 13, tyre_2 = 14, tyre_3 = 15, tyre_4 = 16;
constexpr int button_pin = 21;
constexpr int arm_feedback = 34, arm_pulse = 17;
constexpr int wire_SIG = 32;
constexpr int tyre_interval = 40;

int tyre_values[2];
int arm_value;
bool wire = false;

MOTORIO tyre_1_motor(tyre_1, tyre_interval), tyre_2_motor(tyre_2, tyre_interval),
    tyre_3_motor(tyre_3, tyre_interval), tyre_4_motor(tyre_4, tyre_interval);

int readbutton() { return digitalRead(button_pin); }

TaskHandle_t motor_task;
SemaphoreHandle_t motor_sem = xSemaphoreCreateMutex();

void motor_task_func(void* arg) {
  while (true) {
    if (xSemaphoreTake(motor_sem, portMAX_DELAY) == pdTRUE) {
      tyre_1_motor.run_msec(tyre_values[0]);
      tyre_2_motor.run_msec(tyre_values[1]);
      tyre_3_motor.run_msec(tyre_values[2]);
      tyre_4_motor.run_msec(tyre_values[3]);
      xSemaphoreGive(motor_sem);
    }
  }
}

void setup() {
  serial.init();
  pinMode(button_pin, INPUT);
  xTaskCreatePinnedToCore(motor_task_func, "MotorTask", 10000, NULL, 1, &motor_task, 1);
}

void loop() {
  if (!serial.isMessageAvailable()) return;

  Message msg = serial.receiveMessage();

  if (msg.getMessage().startsWith("Motor ")) {
    String message = msg.getMessage().substring(6);
    if (message.length() == 9) {
      int idx = 0;
      // TODO: Optimize the code by not copying the String
      while (message.length() > 0 && idx < 2) {
        if (xSemaphoreTake(motor_sem, portMAX_DELAY) == pdTRUE) {
          int spaceIndex = message.indexOf(' ');
          if (spaceIndex == -1) {
            tyre_values[idx++] = message.toInt();
            xSemaphoreGive(motor_sem);
            break;
          } else {
            tyre_values[idx++] = message.substring(0, spaceIndex).toInt();
            message = message.substring(spaceIndex + 1);
          }
        }
      }
    } else {
      return;
    }
  }
  if (msg.getMessage().startsWith("Rescue ")) {
    String message = msg.getMessage().substring(7);
    if (message.length() == 5) {  // NOTE: ID Rescue arm_angle(1000~2000) wire(0,1)
      arm_value = message.substring(0, 4).toInt();
      wire = message[5] - '0';
    } else {
      return;
    }
  }
  // if (message.startsWith("Sonic")) {
  //   serial.sendMessage();
  // }
  if (msg.getMessage().startsWith("GET button")) {
    serial.sendMessage(Message(msg.getId(), String(readbutton())));
  }
}
