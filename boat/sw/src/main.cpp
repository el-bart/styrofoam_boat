#include <Arduino.h>
#include <Servo.h>

Servo servo;
auto const pin_servo = 2;
auto const pin_engine = 5;

void setup()
{
  servo.attach(pin_servo);
  pinMode(pin_engine, OUTPUT);
}

void loop()
{
  for(auto i=0; i<180; ++i)
  {
    analogWrite(pin_engine, i); // 1kHz PWM: 0-255
    servo.write(i);             // angle in degrees: 0-180
    delay(150);
  }
}
