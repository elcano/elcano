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
	args::ValueFlag<std::string> in_name(parser, "dev", "The input device", {'i', "input"}, "/dev/ttyACM0");
	args::ValueFlag<unsigned long> in_rate(parser, "rate", "The input baudrate", {'j', "in-baudrate"}, 9600ul);
	args::ValueFlag<std::string> out_name(parser, "dev", "The output device", {'o', "output"}, "/dev/ttyACM0");
	args::ValueFlag<unsigned long> out_rate(parser, "rate", "The output baudrate", {'p', "out-baudrate"}, 9600ul);
	
	try { parser.ParseCLI(argc, argv); }
	catch (args::Help) { std::cout << parser; return 0; }
	catch (args::ParseError e) { std::cerr << e.what() << std::endl << parser; return 1; }
	
	std::cout << "Test reading from an external device:" << std::endl;
	{
		serial::Serial input(args::get(in_name), args::get(in_rate), serial::Timeout::simpleTimeout(100));
		if (!input.isOpen()) { std::cerr << "Unable to open input!" << std::endl; return 2; }
		elcano::SerialData data;

		elcano::read(input, data);
		std::cout << data << std::endl;
	}
	
	std::cout << "Test writing to an external device:" << std::endl;
	{
		serial::Serial output(args::get(out_name), args::get(out_rate), serial::Timeout::simpleTimeout(100));
		if (!output.isOpen()) { std::cerr << "Unable to open output!" << std::endl; return 3; }
		
		elcano::SerialData data;
		data.kind = elcano::MsgType::sensor;
		data.speed = 20;
		data.angle = 58;
		data.posE = 1234;
		data.posN = 4321;
		
		elcano::write(output, data);
		elcano::read(output, data);
		std::cout << data << std::endl;
	}
}
