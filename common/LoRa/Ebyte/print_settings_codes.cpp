#include "Protocol.hpp"
#include "Io.hpp"
#include <iostream>

using namespace LoRa::Ebyte;

struct PrintIo: Io
{
  // these are I/O with LoRa module:
  void tx(uint8_t byte) override { std::cout << byte; }
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

  {
    std::cout << "default settings:\n";
    Settings s;
    proto.reconfigure(s);
    std::cout << "\n";
  }

  {
    std::cout << "W=19.2k S=115.2k settings:\n";
    Settings s;
    s.wireless_link_.speed_ = Settings::Wireless_Link::Speed::b19200;
    s.serial_link_.speed_ = Settings::Serial_Link::Speed::b115200;
    proto.reconfigure(s);
    std::cout << "\n";
  }
}
