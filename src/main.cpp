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
int motor_speed[2] = {0, 0};

int motor_pin = 32;

bool recieved = false;
int cmd = 0x10;
MOTORIO motor;
void setup(){
  Serial.begin(115200);
  Wire.begin(0x08);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  motor = MOTORIO(motor_pin);
}


void loop()
{
  long lastmil=millis();
  while(millis()-lastmil<1000){
    motor.run_msec(1000);
    delay(20);
    lastmil=millis();
    Serial.println("1000");
  }
  while(millis()-lastmil<1000){
    motor.run_msec(1500);
    delay(20);
    lastmil=millis();
    Serial.println("1500");
  }
  while(millis()-lastmil<1000){
    motor.run_msec(2000);
    delay(20);
    lastmil=millis();
    Serial.println("2000");
  }
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
