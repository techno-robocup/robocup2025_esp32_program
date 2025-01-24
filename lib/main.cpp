#include <Wire.h>
#include <iostream>
#define I2C_ADDRESS 0x04
#define LED_PIN 2

void setup()
{
    Serial.begin(115200);
    Wire.begin(0x08);
    pinMode(LED_PIN, OUTPUT);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
    wire.reqFrom(I2C_ADDRESS, 32);

    int bytecnt = 0;
    while (Wire.available())
    {
        byte data = Wire.read();
        bytecnt++;
    }

    for (int i = 0; i < bytecnt; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
}
