#include "Data.hpp"

namespace LoRa::Ebyte::detail
{

struct Binary
{
  uint8_t byte_[6];
};

struct Hex
{
  uint8_t byte_[2*sizeof(Binary::byte_)];
};


namespace
{
uint8_t halfbyte2hex(const uint8_t halfbyte)
{
  constexpr uint8_t lut[] = "0123456789ABCDEF";
  return lut[halfbyte];
}

void byte2hex(const uint8_t byte, uint8_t* out)
{
  out[0] = halfbyte2hex(byte >> 4);
  out[1] = halfbyte2hex(byte & 0x0F);
}
}

Hex bin2hex(Binary const& in)
{
  Hex out;
  auto tmp = out.bytes_;
  for(auto b: in.bytes_)
  {
    byte2hex(b, tmp);
    tmp += 2;
  }
  return out;
}

}
