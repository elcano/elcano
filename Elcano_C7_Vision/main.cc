#include <serial/serial.h>
#include "detect.hh"
#include "args.hh"

using namespace cv;

/* Signal Handling Boilerplate */
bool ctrl_c_pressed = false;
static void sig_capture(int s) { ctrl_c_pressed = true; }

int main(int argc, char **argv)
{
	/* Parse Command-Line Arguments */
	args::ArgumentParser parser("Elcano Vision Program", "Detect objects from the RPI camera.");
	args::HelpFlag help(parser, "help", "Show this help menu", {'h', "help"});
	args::ValueFlag<double> scale(parser, "scale", "The scale factor of the program (>=1)", {'s', "scale"}, 1.0);
	args::ValueFlag<std::string> cascade_name(parser, "cascade", "The location of the cascade file", {'c', "cascade"}, "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml");
	args::ValueFlag<std::string> output_name(parser, "device", "The output device", {'d', "device"}, "/dev/ttyACM0");
	args::ValueFlag<unsigned long> baudrate(parser, "rate", "The output baudrate", {'b', "baudrate"}, 122500ul);
	
	try { parser.ParseCLI(argc, argv); }
	catch (args::Help) { std::cout << parser; return 0; }
	catch (args::ParseError e) { std::cerr << e.what() << std::endl << parser; return 1; }
	
	/* Signal Handling */
	struct sigaction sig_struct;
	sig_struct.sa_flags = 0;
	sig_struct.sa_handler = sig_capture;
	sigemptyset(&sig_struct.sa_mask);
	if (sigaction(SIGINT, &sig_struct, nullptr) == -1) { std::cerr << "Unable to establish signal action!" << std::endl; return -1; }
	
	/* Load Resources */
	CascadeClassifier cascade;
	if (!cascade.load(args::get(cascade_name))) { std::cerr << "Unable to load cascade classifier file!" << std::endl << parser; return -1; }
	
	/* Connect to Output */
	//serial::Serial output(args::get(output_name), args::get(baudrate), serial::Timeout::simpleTimeout(100));
	//if (!output.isOpen()) { std::cerr << "Unable to open the output device!" << std::endl << parser; return -1; }
	
	/* Connect to Camera */
	raspicam::RaspiCam_Cv camera;
	camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	if (!camera.open()) { std::cerr << "Unable to open the camera!" << std::endl; return -1; }	
	
	
	
	
	
	/* Program Mainloop */
	/*Mat img;
	int c;
	for(;;)
	{
		camera.grab();
		camera.retrieve(img);
		elcano::detect_and_draw(img, cascade, args::get(scale));
		c = waitKey(10);
		if (c == 'q' || c == 'Q' || c == 27 || ctrl_c_pressed) break;
	}*/
	
	Mat original; 
	while (true) {	
		camera.grab();
		camera.retrieve(original); // read a new frame from video

		Mat templ = imread("/home/pi/Desktop/aaron-elcano/Elcano_C7_Vision/Cone.jpg");
		int thresholds[] = { 0, 20, 90, 255, 60, 255 };
		//imshow("TemplateFilter", elcano::filterByColor(templ, thresholds, 1));
		//resize(templ, templ, Size(), 0.5, 0.5, INTER_LINEAR);

		Point matchingLoc = elcano::templateMatchBlobs(original, templ, thresholds, TM_CCORR);
		//cout << matchingLoc << endl;

		rectangle(original, matchingLoc, Point(matchingLoc.x + templ.cols, matchingLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
		//resize(original, original, Size(), 0.5, 0.5, INTER_LINEAR);
		imshow("Original", original);
		imshow("Template", templ);

		if (waitKey(30) == 27) {
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
}
