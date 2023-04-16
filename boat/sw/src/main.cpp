#include <Arduino.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "ProtoFsm.hpp"

Servo servo;
auto constexpr pin_servo = 2;
auto constexpr pin_engine = 5;
ProtoFsm fsm;

namespace
{
void apply_outputs(uint8_t byte)
{
  if( not fsm.add_byte(byte) )
    return;
  auto const frame = fsm.pop();
  // TODO: apply frame.engine_direction()
  analogWrite( pin_engine, frame.engine_speed() );
  servo.write( frame.rudder_angle() );
}

void start_position()
{
  apply_outputs(60);
  apply_outputs(0);
  apply_outputs((60^0) | 0x80);
}
}

void setup()
{
  Serial.begin(9600);   // RX == 0, TX == 1
  servo.attach(pin_servo);
  pinMode(pin_engine, OUTPUT);
  start_position();
  {
    wdt_disable();
    wdt_enable(WDTO_1S);
  }
}


namespace
{
struct SerialByte
{
  SerialByte() = default;
  explicit SerialByte(uint8_t byte):
    is_set_{true},
    byte_{byte}
  { }

  bool is_set_{false};
  uint8_t byte_{0};
};

SerialByte read_serial()
{
  while(Serial.available() < 1)
  { }
  auto byte = Serial.read();
  if(byte < 0)
    return {};
  Serial.write('x');
  return SerialByte{ static_cast<uint8_t>(byte) };
}
}

void loop()
{
  auto const serial_byte = read_serial();
  if(not serial_byte.is_set_)
    return;
  apply_outputs(serial_byte.byte_);
  wdt_reset();
}
