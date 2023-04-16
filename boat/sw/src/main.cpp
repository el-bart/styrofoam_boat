#include <Arduino.h>
#include <Servo.h>
#include <avr/wdt.h>
#include "ProtoFsm.hpp"

enum class EngineDir
{
  Forward,
  Reverse
};

Servo servo;
auto constexpr pin_servo = 2;
auto constexpr pin_engine = 5;
ProtoFsm fsm;

auto g_engine_dir = EngineDir::Forward;
uint8_t g_engine_power = 0;  // 1kHz PWM: 0-255
uint8_t g_servo_pos = 0;     // angle in degrees: 0-180

void setup()
{
  g_engine_dir = EngineDir::Forward;
  g_engine_power = 0;
  g_servo_pos = 0;

  Serial.begin(9600);   // RX == 0, TX == 1
  servo.attach(pin_servo);
  pinMode(pin_engine, OUTPUT);

  {
    wdt_disable();
    // wdt_enable(WDTO_2S)
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

void apply_outputs(uint8_t byte)
{
  if( not fsm.add_byte(byte) )
    return;
  analogWrite(pin_engine, g_engine_power);
  servo.write(g_servo_pos);
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
