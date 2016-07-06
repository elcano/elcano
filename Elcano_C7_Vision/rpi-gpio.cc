#include "rpi-gpio.hh"
#include <fstream>
#include <sstream>

/* Low level helper class for RPi GPIO */

namespace elcano
{
	int rpi_gpio::claim()
	{
		const std::string path = "/sys/class/gpio/export";
		std::ofstream exporter(path.c_str());
		if (exporter < 0) return -1;
		exporter << gpio_num;
		exporter.close();
		return 0;
	}
	
	int rpi_gpio::unclaim()
	{
		const std::string path = "/sys/class/gpio/unexport";
		std::ofstream unexporter(path.c_str());
		if (unexporter < 0) return -1;
		unexporter << gpio_num;
		unexporter.close();
		return 0;
	}
	
	int rpi_gpio::set_dir(std::string dir)
	{
		const std::string path = "/sys/class/gpio/gpio" + gpio_num + "/direction";
		std::ofstream direction(path.c_str());
		if (direction < 0) return -1;
		direction << dir;
		direction.close();
		return 0;
	}
	
	int rpi_gpio::set_val(std::string val)
	{
		const std::string path = "/sys/class/gpio/gpio" + gpio_num + "/value";
		std::ofstream value(path.c_str());
		if (value < 0) return -1;
		value << val;
		value.close();
		return 0;
	}
	
	int rpi_gpio::get_val(std::string &val)
	{
		const std::string path = "/sys/class/gpio/gpio" + gpio_num + "/value";
		std::ifstream value(path.c_str());
		if (value < 0) return -1;
		value >> val;
		val = val != "0" ? "1" : "0";
		value.close();
		return 0;
	}
}
