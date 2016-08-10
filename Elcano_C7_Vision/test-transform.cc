#include "transform.hh"
#include <iostream>

/* Test suite for transform.cc
   Compare with [project path]/Vision/spacial_prototype.py */

const double M_PI = 3.141592654;

int
main(
	int argc,
	char **argv
) {
	std::cout << "Testing the Transform Library" << std::endl;

	std::cout << std::endl << "Testing 2D->3D Z-dist Generation:" << std::endl;
	std::cout << "(h=height,f=field of view (y-axis))" << std::endl;
	std::cout << "h=480,f=pi/2: " << elcano::get_z(480, M_PI / 2.0) << std::endl;
	std::cout << "h=480,f=8pi/9: " << elcano::get_z(480, 8.0 * M_PI / 9.0) << std::endl;
	
	std::cout << std::endl << "Testing Global->Relative Transformation" << std::endl;
	std::cout << "(a=camera angle, c=camera position, p=cone position, i=image size," << std::endl;
	std::cout << " s=sensor size, f=focal length)" << std::endl;

	std::cout << "a=(0,0,0),c=(0,0,0),p=(10,2,0),i=(320,240),s=(0.05,0.05),f=0.01: ";
	std::cout << elcano::global_to_relative(
		std::tuple<double, double, double>(0, 0, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(10, 2, 0),
		std::tuple<uint64_t, uint64_t>(320, 240),
		std::tuple<double, double>(0.05, 0.05),
		0.01) << std::endl;

	std::cout << "a=(0,0,0),c=(0,0,1),p=(10,2,0),i=(320,240),s=(0.05,0.05),f=0.01: ";
	std::cout << elcano::global_to_relative(
		std::tuple<double, double, double>(0, 0, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 1),
		std::tuple<uint64_t, uint64_t, uint64_t>(10, 2, 0),
		std::tuple<uint64_t, uint64_t>(320, 240),
		std::tuple<double, double>(0.05, 0.05),
		0.01) << std::endl;

	std::cout << "a=(0,0.3,0),c=(0,0,0),p=(10,2,0),i=(320,240),s=(0.05,0.05),f=0.01: ";
	std::cout << elcano::global_to_relative(
		std::tuple<double, double, double>(0, 0.3, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(10, 2, 0),
		std::tuple<uint64_t, uint64_t>(320, 240),
		std::tuple<double, double>(0.05, 0.05),
		0.01) << std::endl;

	std::cout << "a=(0,0,0.3),c=(0,0,0),p=(10,2,0),i=(320,240),s=(0.05,0.05),f=0.01: ";
	std::cout << elcano::global_to_relative(
		std::tuple<double, double, double>(0, 0, 0.3),
		std::tuple<uint64_t, uint64_t, uint64_t>(0, 0, 0),
		std::tuple<uint64_t, uint64_t, uint64_t>(10, 2, 0),
		std::tuple<uint64_t, uint64_t>(320, 240),
		std::tuple<double, double>(0.05, 0.05),
		0.01) << std::endl;

	return 0;
}
