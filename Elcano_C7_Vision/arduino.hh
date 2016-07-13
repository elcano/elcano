#pragma once

#include "types.hh"
#include <serial/serial.h>

/* Send information to an output device */

namespace elcano
{
	void send_to_arduino(Vectors3, serial::Serial);
}
