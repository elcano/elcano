#include "arduino.hh"

/* Test suite for arduino.cc */

int
main(
	int argc,
	char **argv
) {
	std::cout << "Test elcano::clear" << std::endl;
	elcano::SerialData s;
	std::cout << s << std::endl;
	elcano::clear(s);
	std::cout << s << std::endl;
	
	std::cout << "Test elcano::write for MsgType::drive" << std::endl;
	s.kind = elcano::MsgType::drive;
	s.speed = 50;
	s.angle = 80;
	std::cout << s << std::endl;
	elcano::write(std::cout, s);
	std::cout << std::endl << std::endl;
	
	std::cout << "Test elcano::write for MsgType::sensor" << std::endl;
	s.kind = elcano::MsgType::sensor;
	s.posE = 69;
	s.posN = 420;
	std::cout << s << std::endl;
	elcano::write(std::cout, s);
	std::cout << std::endl << std::endl;
	
	std::cout << "Test elcano::read" << std::endl;
	std::cout << "D {s 5} {a -3}" << std::endl;
	elcano::read("D {s 5} {a -3}", s);
	std::cout << s << std::endl;
	
	std::cout << "G {n 3} {p 5,9} {b 87} {r 3468}" << std::endl;
	elcano::read("G {n 3} {p 5,9} {b 87} {r 3468}", s);
	std::cout << s << std::endl;
	
	std::cout << "X {n 8} {p 42,67} {b 1} {s 5}" << std::endl;
	elcano::read("X {n 8} {p 42,67} {b 1} {s 5}", s);
	std::cout << s << std::endl;
}
