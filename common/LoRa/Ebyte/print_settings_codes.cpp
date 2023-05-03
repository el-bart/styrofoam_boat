#include "Protocol.hpp"
#include "Io.hpp"
#include <iostream>

using namespace LoRa::Ebyte;

struct PrintIo: Io
{
  // these are I/O with LoRa module:
  void tx(uint8_t byte) override { std::cerr << byte; }
  uint8_t rx() override { throw std::logic_error{"N/A"}; }
  void m0(bool) override { }
  void m1(bool) override { }
  bool aux() const override { return true; }

  void speed(Settings::Serial_Link::Speed) override { }
  void delay(uint8_t) const override { }
};


int main()
{
  PrintIo pio;
  Protocol proto{pio};
  Settings s;
  proto.reconfigure(s);
  std::cerr << std::endl;
}
