#pragma once
#include <inttypes.h>

enum class EngineDir
{
  Forward,
  Reverse
};


struct Frame
{
  bool checksum_correct() const
  {
    auto const x = data_[0] ^ data_[1];
    return ( x | 0x80 ) == data_[2];
  }
  bool correct() const
  {
    if( ( data_[0] & 0x80 ) != 0x00 ) return false;
    if( ( data_[1] & 0x80 ) != 0x00 ) return false;
    if( ( data_[2] & 0x80 ) != 0x80 ) return false;
    return checksum_correct();
    return true;
  }

  EngineDir engine_direction() const
  {
    return ( data_[1] & 0x40 ) ? EngineDir::Forward : EngineDir::Reverse;
  }

  uint8_t engine_speed() const
  {
    return ( data_[1] & 0x3fu ) << 2u;
  }

  uint8_t rudder_angle() const
  {
    auto constexpr dead_angle = (180u-127u)/2u;
    auto const value = data_[0] & 0x7fu;    // 0..127
    return dead_angle + value;
  }

  uint8_t data_[3];
};
