#include "arduino-usb.hh"

/* Send information to an output device */

namespace elcano
{
	void send_to_arduino(std::vector<Vec3> data, serial::Serial device)
	{
		std::vector<uint8_t> output(data.size() * 24);

		for (Vec3 vec : data)
			for (uint8_t *it = vec.as_bytes; it < vec.as_bytes + 24; ++it)
				output.push_back(*it);

		device.write(output);
	}
}
