#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>
#include "args.hh"
#include "detect-and-draw.hh"
#include <iostream>
#include <vector>

using namespace cv;

int main(int argc, char **argv)
{
	args::ArgumentParser parser("Elcano Vision Program", "Detect objects from the RPI camera.");
	args::HelpFlag help(parser, "help", "Show this help menu", {'h', "help"});
	args::ValueFlag<double> scale(parser, "scale", "The scale factor of the program (>=1)", {'s', "scale"}, 1.0);
	args::ValueFlag<std::string> cascade_name(parser, "cascade", "The location of the cascade file", {'c', "cascade"}, "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
	
	try { parser.ParseCLI(argc, argv); }
	catch (args::Help) { std::cout << parser; return 0; }
	catch (args::ParseError e) { std::cerr << e.what() << std::endl << parser; return 1; }
	
	CascadeClassifier cascade;
	if (!cascade.load(args::get(cascade_name)))
	{
		std::cerr << "Unable to load cascade classifier file!" << std::endl << parser;
		return -1;
	}
	
	raspicam::RaspiCam_Cv camera;
	camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
	if (!camera.open())
	{
		std::cerr << "Couldn't open the camera!" << std::endl;
		return -1;
	}
	
	Mat img;
	int c;
	for(;;)
	{
		camera.grab();
		camera.retrieve(img);
		elcano::detect_and_draw(img, cascade, args::get(scale));
		c = waitKey(10);
		if (c == 'q' || c == 'Q' || c == 27) break;
	}
}
