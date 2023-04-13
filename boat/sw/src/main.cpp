#include <Arduino.h>
#include <Servo.h>
#include <avr/wdt.h>

enum class EngineDir
{
  Forward,
  Reverse
};

Servo servo;
auto constexpr pin_servo = 2;
auto constexpr pin_engine = 5;

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
uint8_t g_data[3];
uint8_t g_bytes = 0;

void read_serial()
{
  while(Serial.available() < 1)
  { }
  auto byte = Serial.read();
  if(byte < 0)
    return;
  Serial.write('x');
  // TODO: do real adding byte and parsing...
  g_engine_power = byte;

}

void apply_outputs()
{
  analogWrite(pin_engine, g_engine_power);
  servo.write(g_servo_pos);
}
}

void loop()
{
  read_serial();
  apply_outputs();
  wdt_reset();
}
