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
	elcano::read("GOAL {Num 3} {Pos 5,9} {Br 87} {Prob 3468}", s);
	std::cout << s << std::endl;
}
