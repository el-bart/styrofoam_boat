#pragma once
#include <stdint.h>

namespace LoRa::Ebyte
{

struct Settings final
{
  using Module_Address = uint16_t;

  struct Serial_Link
  {
    enum class Speed
    {
      b1200,
      b2400,
      b4800,
      b9600,
      b12200,
      b38400,
      b57600,
      b115200
    };

    Speed speed_{Speed::b9600};
  };

  struct Wireless_Link
  {
    using Channel = uint8_t;    // valid range: 0x00..0xC8 ; f = 410M + chan*0.5M
    enum class Speed
    {
      b1200,
      b2400,
      b4800,
      b9600,
      b19200,   // 19.2k
      b50000,   // 50k
      b100000,  // 100k
      b200000   // 200k
    };
    enum class Power
    {
      dBm20,
      dBm17,
      dBm14,
      dBm10
    };

    Channel channel_{0x2E};         //  default: 433M
    Speed speed_{Speed::b2400};
    Power power_{Power::dBm20};
  };

  // defaults are as per datasheet
  Module_Address module_address_{0x0000};
  Serial_Link serial_link_;
  Wireless_Link wireless_link_;
};

}
