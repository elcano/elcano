#include "arduino.hh"
#include "args.hh"

/* Test suite for communicating over serial sockets.
   Requires two active serial connections to run */

int
main(
	int argc,
	char **argv
) {
	args::ArgumentParser parser("Test serial connection", "Connect to sockets");
	args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
	args::ValueFlag<std::string> name(parser, "dev", "The input device", {'D', "device"}, "/dev/ttyACM0");
	args::ValueFlag<unsigned long> rate(parser, "rate", "The input baudrate", {'b', "baudrate"}, 9600ul);
	args::ValueFlag<unsigned long> count(parser, "count", "The number of iterations", {'c', "count"}, 10ul);
	args::ValueFlag<unsigned long> delay(parser, "delay", "The delay between serial calls (ms)", {'d', "delay"}, 100ul);
	
	try { parser.ParseCLI(argc, argv); }
	catch (args::Help) { std::cout << parser; return 0; }
	catch (args::ParseError e) { std::cerr << e.what() << std::endl << parser; return 1; }
	
	serial::Serial device(args::get(name), args::get(rate), serial::Timeout::simpleTimeout(args::get(delay)));
	std::chrono::duration<unsigned long, std::milli> d(args::get(delay));
	
	elcano::SerialData data;
	data.kind = elcano::MsgType::drive;
	data.speed = 20;
	data.angle = 60;
	
	for (unsigned long i = 1; i < args::get(count); ++i)
	{
		elcano::read(device, data);
		
		std::cout << "parsed output: ";
		elcano::write(std::cout, data);
		std::cout << std::endl;
		
		elcano::write(device, data);
		std::this_thread::sleep_for(d);
	}
}
