#include <Arduino.h>
#include <Servo.h>

Servo servo;

void setup()
{
  servo.attach(2);    // D2
}

void loop()
{
  for(auto i=0; i<180; ++i)
  {
    servo.write(i);
    delay(15);
  }
}
