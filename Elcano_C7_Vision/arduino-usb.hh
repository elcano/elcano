#pragma once

#include <utility>
#include <vector>
#include <serial/serial.h>

/* Send information to an output device */

namespace elcano
{
	union Vec3
	{
		struct { int64_t x, y, z; };
		uint8_t as_bytes[24];
	};
	
	void send_to_arduino(std::vector<Vec3>, serial::Serial);
}
