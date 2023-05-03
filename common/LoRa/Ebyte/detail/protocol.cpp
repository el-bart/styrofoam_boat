#include "protocol.hpp"

namespace LoRa::Ebyte::detail
{

namespace
{
uint8_t encode_parity() { return 0x00 << 6; }

uint8_t encode(Settings::Serial_Link::Speed s)
{
  uint8_t enc;
  switch(s)
  {
    case Settings::Serial_Link::Speed::b1200: enc = 0; break;
    case Settings::Serial_Link::Speed::b2400: enc = 1; break;
    case Settings::Serial_Link::Speed::b4800: enc = 2; break;
    case Settings::Serial_Link::Speed::b9600: enc = 3; break;
    case Settings::Serial_Link::Speed::b12200: enc = 4; break;
    case Settings::Serial_Link::Speed::b38400: enc = 5; break;
    case Settings::Serial_Link::Speed::b57600: enc = 6; break;
    case Settings::Serial_Link::Speed::b115200: enc = 7; break;
  }
  return enc << 3u;
}

uint8_t encode(Settings::Wireless_Link::Speed s)
{
  switch(s)
  {
    case Settings::Wireless_Link::Speed::b1200: return 0;
    case Settings::Wireless_Link::Speed::b2400: return 1;
    case Settings::Wireless_Link::Speed::b4800: return 2;
    case Settings::Wireless_Link::Speed::b9600: return 3;
    case Settings::Wireless_Link::Speed::b19200: return 4;
    case Settings::Wireless_Link::Speed::b50000: return 5;
    case Settings::Wireless_Link::Speed::b100000: return 6;
    case Settings::Wireless_Link::Speed::b200000: return 7;
  }
  return 0x00;
}

uint8_t encode_speed(Settings const& s)
{
  uint8_t out = 0x00;
  out |= encode_parity();
  out |= encode(s.serial_link_.speed_);
  out |= encode(s.wireless_link_.speed_);
  return out;
}

uint8_t encode(Settings::Wireless_Link::Power p)
{
  switch(p)
  {
    case Settings::Wireless_Link::Power::dBm20: return 0;
    case Settings::Wireless_Link::Power::dBm17: return 1;
    case Settings::Wireless_Link::Power::dBm14: return 2;
    case Settings::Wireless_Link::Power::dBm10: return 3;
  }
  return 0x00;
}

Binary reconfigure(Settings const& s, uint8_t op)
{
  Binary bin;
  bin.byte_[0] = 0xC2;  // write settings; do not persist when power-cycled
  bin.byte_[1] = s.module_address_ >> 8;
  bin.byte_[2] = s.module_address_ & 0xFF;
  bin.byte_[3] = encode_speed(s);
  bin.byte_[4] = s.wireless_link_.channel_;
  bin.byte_[5] = encode(s.wireless_link_.power_);
  return bin;
}
}

Binary reconfigure(Settings const& s)
{
  return reconfigure(s, 0xC2);
}

Binary reconfigure_persist(Settings const& s)
{
  return reconfigure(s, 0xC0);
}

}
