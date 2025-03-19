#include <Arduino.h>
#include <Wire.h>
#include <motorio.hpp>
void receiveEvent(int);
void requestEvent();
template <typename... Args>
inline void debugprint(Args... args);
template <typename... Args>
inline void debugprintln(Args... args);

#define _DEBUG 1

constexpr std::int8_t LIN1 = 32;
constexpr std::int8_t LIN2 = 33;
constexpr std::int8_t RIN1 = 26;
constexpr std::int8_t RIN2 = 27;
int motor_speed[2] = {-1, -1};
bool recieved = false;
int cmd = 0x10;
int left_speed_array[41] = {
    200, 190, 180, 170, 160, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10,
    0,
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200
  },
  right_speed_array[41] = {
    200, 190, 180, 170, 160, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60, 50, 40, 30, 20, 10,
    0,
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200
  };
robot::motorio left_motor(LIN1, LIN2, left_speed_array);
robot::motorio right_motor(RIN1, RIN2, right_speed_array);
void setup()
{
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  // pinMode(IN1, OUTPUT);
  // pinMode(IN2, OUTPUT);
}

void loop()
{
  if (recieved) {
    debugprintln("RECIEVED!",motor_speed[0], motor_speed[1]);
    recieved = false;
    left_motor.set_speed(motor_speed[0]);
    right_motor.set_speed(motor_speed[1]);
  }
  left_motor.run_speed();
  right_motor.run_speed();
}

void receiveEvent(int byteNum)
{
  debugprintln(byteNum);
  if (byteNum < 5)
  {
    while (Wire.available())
    {
      Wire.read();
      // debugprint(Wire.read(), " ");
      // debugprintln();
    }
    return;
  }
  else
  {
    cmd = Wire.read();
    if (cmd == 0x00)
    {
      int posneg = Wire.read();
      int recievednum = Wire.read();
      if (posneg == 0x01)
        motor_speed[0] = -recievednum;
      else
        motor_speed[0] = recievednum;
      posneg = Wire.read();
      recievednum = Wire.read();
      if (posneg == 0x01)
        motor_speed[1] = -recievednum;
      else
        motor_speed[1] = recievednum;
    }
    else
    {
      return;
    }
  }
  while (Wire.available())
  {
    Wire.read();
  }
  recieved = true;
  cmd = 0x10;
  return;
}

void requestEvent()
{
  if (recieved)
    Wire.write(0x00); // request raspberrypi to stop sending
  if (!recieved)
    Wire.write(0x01); // Allow raspberrypi to send
  return;
}

#ifdef _DEBUG
template <typename... Args>
inline void debugprint(Args... args)
{
  (Serial.print(args), ...);
}

template <typename... Args>
inline void debugprintln(Args... args)
{
  (Serial.print(args), ...);
  Serial.println();
}
#else
template <typename... Args>
inline void debugprint(Args...) {}
template <typename... Args>
inline void debugprintln(Args...) {}
#endif
