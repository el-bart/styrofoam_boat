#pragma once
#include "detail/protocol.hpp"
#include "Io.hpp"

namespace LoRa::Ebyte
{

struct Protocol final
{
  explicit Protocol(Io& io):
    io_{&io}
  {
    mode_transmission();
  }

  void reconfigure(Settings s)
  {
    auto const bin = detail::reconfigure(s);
    reconfigure_send(s, bin);
  }

  void reconfigure_persist(Settings s)
  {
    auto const bin = detail::reconfigure_persist(s);
    reconfigure_send(s, bin);
  }

  template<uint8_t N>
  void send_package(uint8_t const (&data)[N])
  {
    static_assert(N <= 54u, "package is larger than module's MTU");
    send(data, N);
    wait_for_package_complete();
  }

private:
  void reconfigure_send(Settings const& s, detail::Binary const& bin)
  {
    auto const hex = detail::bin2hex(bin);
    mode_settings();
    send(hex.byte_);
    wait_for_reconfiguration();
    mode_transmission();
    io_->speed(s.serial_link_.speed_);
  }

  template<uint8_t N>
  void send(uint8_t const (&data)[N])
  {
    send(data, N);
  }

  void send(uint8_t const *data, uint8_t size)
  {
    for(auto i=0u; i<size; ++i)
      io_->tx(data[i]);
  }

  void mode_settings()
  {
    wait_for_mode_switch_ready();
    io_->m0(false);
    io_->m1(true);
    wait_for_mode_switch_completed();
    io_->speed(Settings::Serial_Link::Speed::b9600);
  }

  void mode_transmission()
  {
    wait_for_mode_switch_ready();
    io_->m0(false);
    io_->m1(false);
    wait_for_mode_switch_completed();
  }

  void wait_for_mode_switch_ready()
  {
    while(not io_->aux())
      io_->delay(1);   // prevent busy loop
    io_->delay(2);     // extra delay required by datasheet
  }
  void wait_for_mode_switch_completed() { io_->delay(1); }
  void wait_for_reconfiguration() { io_->delay(1); }
  void wait_for_package_complete() { io_->delay(20); }

  Io* io_{nullptr};
};

}
