#include <Arduino.h>
#include <Wire.h>
void receiveEvent(int);
void requestEvent();
template <typename... Args>
inline void debugprint(Args... args);
template <typename... Args>
inline void debugprintln(Args... args);

#define _DEBUG 1

constexpr std::int8_t IN1 = 32;
constexpr std::int8_t IN2 = 33;
constexpr std::int8_t TL = 4;
constexpr std::int8_t TR = 2;
int motor_speed[2] = {-1, -1};
bool recieved = false;
int cmd = 0x10;

void setup()
{
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop()
{
  if (motor_speed[0] == -1 || motor_speed[1] == -1)
  {
    return;
  }
  analogWrite(IN1, motor_speed[0]);
  analogWrite(IN2, motor_speed[1]);
  debugprintln("RECIEVED!", motor_speed[0], " ", motor_speed[1]);
  recieved = false;
}

void receiveEvent(int byteNum)
{
  debugprintln(byteNum);
  if (byteNum < 3)
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
      motor_speed[0] = Wire.read();
      motor_speed[1] = Wire.read();
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
  Wire.write(touchRead(TL));
  Wire.write(touchRead(TR));
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
