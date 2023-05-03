#pragma once
#include "detail/protocol.hpp"
#include "detail/bin2hex.hpp"

namespace LoRa::Ebyte
{

/**
 * example interface integration should look like this:
struct ExampleIo
{
  // these are I/O with LoRa module:
  static void tx(uint8_t byte) { ... }
  static uint8_t rx() { ... }
  static void m0(bool state) { ... }
  static void m1(bool state) { ... }
  static bool aux() { ... }

  // these shall affect local MCU settings:
  static void speed(Settings::Serial_Link::Speed s) { ... }
  static void detail(uint8_t ms) { ... }
};
*/

template<typename Io>
struct Protocol final
{
  void reconfigure(Settings s)
  {
    auto const bin = detail::reconfigure(s);
    auto const hex = detail::bin2hex(bin);
    mode_settings();
    send(hex.bytes_);
    wait_for_reconfiguration();
    mode_transmission(s.serial_link_.speed_);
  }

  template<uint8_t N>
  void send_package(uint8_t const (&data)[N])
  {
    static_assert(N <= 54u, "package is larger than module's MTU");
    send(data, N);
    wait_for_package_complete();
  }

private:
  template<uint8_t N>
  void send(uint8_t const (&data)[N])
  {
    send(data, N);
  }

  void send(uint8_t const *data, uint8_t size)
  {
    for(auto i=0u; i<size; ++i)
      Io::tx(data[i]);
  }

  void mode_settings()
  {
    wait_for_mode_switch_ready();
    Io::m0(false);
    Io::m1(true);
    wait_for_mode_switch_completed();
    Io::speed(Settings::Serial_Link::Speed::b9600);
  }

  void mode_transmission(Settings::Serial_Link::Speed speed)
  {
    wait_for_mode_switch_ready();
    Io::m0(false);
    Io::m1(false);
    wait_for_mode_switch_completed();
    Io::speed(speed);
  }

  void wait_for_mode_switch_ready()
  {
    while(not Io::aux())
      Io::delay(1);   // prevent busy loop
    Io::delay(2);     // extra delay required by datasheet
  }
  void wait_for_mode_switch_completed() { Io::delay(1); }
  void wait_for_reconfiguration() { Io::delay(1); }
  void wait_for_package_complete() { Io::delay(20); }
};

}
