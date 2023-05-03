#pragma once
#include <stdint.h>

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


Hex bin2hex(Binary const& bin);

}
