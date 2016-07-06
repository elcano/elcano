#pragma once

#include <string>

/* Low level helper class for RPi GPIO */

namespace elcano
{
	class rpi_gpio
	{
		std::string gpio_num;
	public:
		rpi_gpio() : gpio_num("4") { }
		rpi_gpio(std::string n) : gpio_num(n) { }
		inline std::string number() { return gpio_num; }
		int claim();
		int unclaim();
		int set_dir(std::string);
		int set_val(std::string);
		int get_val(std::string&);
	};
}
