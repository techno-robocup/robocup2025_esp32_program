#include <Arduino.h>
#include <cassert>
#include "armio.hpp"
#include "motorio.hpp"
#include "mutex_guard.hpp"
#include "serial_io.hpp"
#include "ultrasonic_io.hpp"
SerialIO serial;

constexpr int tyre_1 = 13, tyre_2 = 14, tyre_3 = 15, tyre_4 = 16;
constexpr int button_pin = 21;
constexpr int arm_feedback = 34, arm_pulse = 17;
constexpr int wire_SIG = 32;
constexpr int tyre_interval = 40;
constexpr int ultrasonic_trig1 = 18, ultrasonic_echo1 = 19, ultrasonic_trig2 = 22,
              ultrasonic_echo2 = 23, ultrasonic_trig3 = 26, ultrasonic_echo3 = 27;

// Fixed array size to match actual usage (4 motors, not 2)
static int tyre_values[4] = {1500, 1500, 1500, 1500};
static int arm_value = 0;
static bool wire = false;

static long ultrasonic_values[3] = {0, 0, 0};
UltrasonicIO ultrasonic_1(ultrasonic_trig1, ultrasonic_echo1),
    ultrasonic_2(ultrasonic_trig2, ultrasonic_echo2),
    ultrasonic_3(ultrasonic_trig3, ultrasonic_echo3);

MOTORIO tyre_1_motor(tyre_1, tyre_interval), tyre_2_motor(tyre_2, tyre_interval),
    tyre_3_motor(tyre_3, tyre_interval), tyre_4_motor(tyre_4, tyre_interval), wire_motor(wire_SIG, 15);

ARMIO arm(arm_pulse, arm_feedback, wire_SIG);

inline int readbutton() { return digitalRead(button_pin); }

TaskHandle_t motor_task;
SemaphoreHandle_t motor_sem = xSemaphoreCreateMutex();

void motor_task_func(void* arg) {
  while (true) {
    MutexGuard guard(motor_sem);
    tyre_1_motor.run_msec(tyre_values[0]);
    tyre_2_motor.run_msec(tyre_values[1]);
    tyre_3_motor.run_msec(tyre_values[2]);
    tyre_4_motor.run_msec(tyre_values[3]);
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

// Optimized string parsing without String operations
bool parseMotorCommand(const char* message, int* values, int max_values) {
  assert(max_values == 2);
  int idx = 0;
  const char* ptr = message;

  while (idx < max_values && *ptr) {
    // Skip leading spaces
    while (*ptr == ' ') ptr++;
    if (!*ptr) break;

    // Parse number
    int val = 0;
    bool negative = false;

    if (*ptr == '-') {
      negative = true;
      ptr++;
    }

    while (*ptr >= '0' && *ptr <= '9') {
      val = val * 10 + (*ptr - '0');
      ptr++;
    }

    if (negative) val = -val;
    values[idx++] = val;

    // Skip to next space or end
    while (*ptr == ' ') ptr++;
  }

  return idx == max_values;
}

void setup() {
  serial.init();
  pinMode(button_pin, INPUT);
  /*
  ARGS for xTaskCreatePinnedToCore:
  - Task function
  - Task name
  - Stack size (reduced from 10000 to 2048)
  - Task parameter
  - Task priority
  - Task handle
  - Core ID
  */
  xTaskCreatePinnedToCore(motor_task_func, "MotorTask", 2048, nullptr, 1, &motor_task, 0);
  arm.arm_set_position(2000);
  arm.wire_tension_function(true);
}

int ultrasonic_clock = 0;

void loop() {
  // Always read ultrasonic sensors regardless of serial communication
  ++ultrasonic_clock;
  if (ultrasonic_clock >= 3) {
    ultrasonic_clock = 0;
  }

  if (ultrasonic_clock == 0) {
    ultrasonic_1.read(&ultrasonic_values[0]);
  } else if (ultrasonic_clock == 1) {
    ultrasonic_2.read(&ultrasonic_values[1]);
  } else if (ultrasonic_clock == 2) {
    ultrasonic_3.read(&ultrasonic_values[2]);
  }

  // Check for serial messages
  if (!serial.isMessageAvailable()) return;

  Message msg = serial.receiveMessage();
  const String& message = msg.getMessage();

  if (message.startsWith("MOTOR ")) {
    const char* motor_data = message.c_str() + 6;  // Skip "MOTOR "
    if (parseMotorCommand(motor_data, tyre_values, 2)) {
      // TODO: Fix legacy code for assuming 2 motor values
      tyre_values[2] = tyre_values[0];
      tyre_values[3] = tyre_values[1];
      // Optimized: Use char buffer instead of String concatenation
      char response[64];
      snprintf(response, sizeof(response), "OK %d %d %d %d", tyre_values[0], tyre_values[1],
               tyre_values[2], tyre_values[3]);
      serial.sendMessage(Message(msg.getId(), String(response)));
    }
  } else if (message.startsWith("Rescue ")) {
    const char* rescue_data = message.c_str() + 7;  // Skip "Rescue "
    if (strlen(rescue_data) >= 5) {
      // Parse arm_angle (4 digits) and wire (1 digit)
      char angle_str[5] = {0, 0, 0, 0, 0};
      strncpy(angle_str, rescue_data, 4);
      arm_value = atoi(angle_str);
      wire = (rescue_data[4] == '1');
    }
  } else if (message.startsWith("GET button")) {
    const char* status = readbutton() ? "ON" : "OFF";
    serial.sendMessage(Message(msg.getId(), status));
  } else if (message.startsWith("GET ultrasonic")) {
    // Optimized: Use char buffer instead of String concatenation
    char response[64];
    snprintf(response, sizeof(response), "%ld %ld %ld", ultrasonic_values[0], ultrasonic_values[1],
             ultrasonic_values[2]);
    serial.sendMessage(Message(msg.getId(), String(response)));
  } else if (message.startsWith("Wire")) {
    int val = message[5] - '0';
    if (val == 0) {
      wire_motor.run_msec(500)
    }
    else {
      wire_motor.run_msec(2400);
    }
    snprintf(response, sizeof(response), "Wire %d OK", val);
    serial.sendMessage(Message(msg.getId(), String(response)));
  }
}
