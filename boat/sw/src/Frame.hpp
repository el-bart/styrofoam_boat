#pragma once
#include <inttypes.h>

struct Frame
{
  bool checksum_correct() const
  {
    auto const x = data_[0] ^ data_[1];
    return ( x & 0x7f ) == data_[2];
  }
  bool correct() const
  {
    if( ( data_[0] & 0x80 ) != 0 ) return false;
    if( ( data_[1] & 0x80 ) != 0 ) return false;
    if( ( data_[2] & 0x80 ) != 1 ) return false;
    return checksum_correct();
  }

  uint8_t data_[3];
};
