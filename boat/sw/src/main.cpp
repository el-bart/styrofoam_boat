#include <Arduino.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "ProtoFsm.hpp"

Servo servo;
auto constexpr pin_servo = 2;
auto constexpr pin_engine = 5;
auto constexpr pin_engine_enable = 10;  // L298: enable pin  -> pin 6
auto constexpr pin_engine_input_a = 11; // L298: input pin C -> pin 5
auto constexpr pin_engine_input_b = 12; // L298: input pin D -> pin 7
auto constexpr use_debug_writes = false;
ProtoFsm fsm;

namespace
{
template<typename T>
void debug_write(T const& t)
{
  if(use_debug_writes)
    Serial.write(t);
}

void apply_engine(Frame const& frame)
{
  analogWrite( pin_engine, frame.engine_speed() );
  if( frame.engine_speed() == 0u )
  {
    digitalWrite(pin_engine_enable, 0);
    return;
  }
  switch( frame.engine_direction() )
  {
    case EngineDir::Forward:
          digitalWrite(pin_engine_input_a, 1);
          digitalWrite(pin_engine_input_b, 0);
          break;
    case EngineDir::Reverse:
          digitalWrite(pin_engine_input_a, 0);
          digitalWrite(pin_engine_input_b, 1);
          break;
  }
  digitalWrite(pin_engine_enable, 1);
}

void apply_servo(Frame const& frame)
{
  servo.write( frame.rudder_angle() );
}

bool apply_outputs(uint8_t byte)
{
  if( not fsm.add_byte(byte) )
    return false;
  auto const frame = fsm.pop();
  apply_engine(frame);
  apply_servo(frame);
  return true;
}

void start_position()
{
  auto constexpr start_servo = 127/2;
  auto constexpr start_engine = 0;
  apply_outputs(start_servo);
  apply_outputs(start_engine);
  apply_outputs((start_servo ^ start_engine) | 0x80);
}
}

void setup()
{
  Serial.begin(1200);   // RX == 0, TX == 1
  servo.attach(pin_servo);
  {
    pinMode(pin_engine, OUTPUT);
    pinMode(pin_engine_enable, OUTPUT);
    pinMode(pin_engine_input_a, OUTPUT);
    pinMode(pin_engine_input_b, OUTPUT);
  }
  start_position();
  {
    wdt_disable();
    wdt_enable(WDTO_1S);
  }
  debug_write("booted\n");
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
  debug_write('.');
  return SerialByte{ static_cast<uint8_t>(byte) };
}
}

void loop()
{
  auto const serial_byte = read_serial();
  if(not serial_byte.is_set_)
    return;
  if( apply_outputs(serial_byte.byte_) )
    debug_write('#');
  wdt_reset();
}
