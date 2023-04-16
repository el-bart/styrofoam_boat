#pragma once
#include "Frame.hpp"

struct ProtoFsm
{
  bool add_byte(uint8_t b)
  {
    frame_.data_[size_] = b;
    ++size_;
    if(size_<3u)
      return false;
    if( not frame_.correct() )
    {
      frame_.data_[0] = frame_.data_[1];
      frame_.data_[1] = frame_.data_[2];
      --size_;
      return false;
    }
    return true;
  }

private:
  unsigned size_{0};
  Frame frame_;
};
