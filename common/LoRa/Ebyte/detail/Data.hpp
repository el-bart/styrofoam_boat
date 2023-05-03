#pragma once

namespace LoRa::Ebyte::detail
{

struct Binary
{
  uint8_t byte_[6];
};

struct Hex
{
  char byte_[2*sizeof(Binary::byte_)];
};


Hex bin2hex(Binary const& in)

}
