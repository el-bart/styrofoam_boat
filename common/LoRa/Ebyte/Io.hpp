#pragma once
#include "Settings.hpp"

namespace LoRa::Ebyte
{

struct Io
{
  virtual ~Io() = default;

  Io(Io const&) = delete;
  Io& operator=(Io const&) = delete;
  Io(Io &&) = delete;
  Io& operator=(Io &&) = delete;

  // these are I/O with LoRa module:
  virtual void tx(uint8_t byte) = 0;
  virtual uint8_t rx() = 0;
  virtual void m0(bool state) = 0;
  virtual void m1(bool state) = 0;
  virtual bool aux() const = 0;

  // these shall affect local MCU settings:
  virtual void speed(Settings::Serial_Link::Speed s) = 0;
  virtual void delay(uint8_t ms) const = 0;
};

}
