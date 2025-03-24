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
robot::motorio left_motor(LIN1, LIN2);
robot::motorio right_motor(RIN1, RIN2);
void setup()
{
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
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
    }
    return;
  }
  else
  {
    cmd = Wire.read();
    if (cmd == 0x00)
    {
      a = Wire.read();
      if(a == 0){
        motor_speed[0] = Wire.read();
      }if(a == 1){
        motor_speed[0] = -1 * Wire.read();
      }
      b = Wire.read();
      if(b == 0){
        motor_speed[1] = Wire.read();
      }if(b == 1){
        motor_speed[1] = -1 * Wire.read();
      }
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
