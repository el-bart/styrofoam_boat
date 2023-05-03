#pragma once
#include "Settings.hpp"
#include "Data.hpp"

namespace LoRa::Ebyte::detail
{

// apply configuration; do not persist when power-cycling
Binary reconfigure(Settings const& settings);

// apply configuration; persist between power-cycles
Binary reconfigure_persist(Settings const& settings);

}
