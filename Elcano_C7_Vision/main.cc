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
	
	/* Basic test for finding one and only one cone */
	/*Mat original; 
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
	}*/


	vector<Point3d> allCones;
	Point3d trikeLocation;
	int trikeBearing;
	int CAMERA_OFFSET_X = 0;		//Camera's position relative to
	int CAMERA_OFFSET_Y = 30;		//trike's point of reference, in cm
	int CAMERA_OFFSET_Z = 50;
	int CAMERA_OFFSET_BEARING = 0;	//in degrees

	//Camera specifics
	int MAX_CAMERA_RANGE = 1000;		//in cm
	double CAM_FOCAL_LENGTH = 0.36;		//in cm
	double CAM_SENSOR_WIDTH = 0.3629;	//in cm
	double CAM_SENSOR_HEIGHT = 0.2722;	//in cm
	int CAM_PIXEL_WIDTH = 2592;			//num of pixels horizontal
	int CAM_PIXEL_HEIGHT = 1944;		//num of pixels vertical

	//Target object specifics
	Mat targetTemplate;							//Template image
	string TEMPLATE_IMG_FILE = "/home/pi/Desktop/aaron-elcano/Elcano_C7_Vision/Cone.jpg";
	int TEMPLATE_PIXEL_WIDTH;
	int TEMPLATE_PIXEL_HEIGHT;
	double TEMPLATE_REAL_WIDTH = 20;		//real object width in cm
	double TEMPLATE_REAL_HEIGHT = 60;		//real object height in cm
	int TEMPLATE_THRESHOLDS[] = {
		0,		//low hue
		10,		//high hue
		90,		//low saturation
		255,	//high saturation
		60,		//low value
		255		//high value
	};

	//Video image specifics
	Mat frame;
	int FRAME_THRESHOLDS[] = {
		0,		//low hue
		20,		//high hue
		90,		//low saturation
		255,	//high saturation
		60,		//low value
		255		//high value
	};

	//Image processing variables (for template matching)
	Rect subFrameRect;
	Mat subFrameTemp;
	Mat subFrame;
	Mat subFrameBinary;
	Mat templResize;
	Mat templBinary;
	Point expectedPoint;
	Point searchPoint;
	Point foundPoint;
	Point actualPoint;
	int TEMPLATE_SEARCH_BOUNDARY = 20;	//how many pixels outside expected location box to search
	int expectedHeight;					//in pixels
	double templResizeRatio;

	//Recieve Arduino information, with cone positions
	//TODO

	//For now, use pixels
	Point3d cone1;
	cone1.x = 320;
	cone1.y = 240;
	cone1.z = 160;
	Point3d cone2;
	cone2.x = 200;
	cone2.y = 400;
	cone2.z = 60;
	allCones.push_back(cone1);
	allCones.push_back(cone2);

	//Open template image file
	targetTemplate = imread(TEMPLATE_IMG_FILE);
	imshow("Template", targetTemplate);
	TEMPLATE_PIXEL_HEIGHT = targetTemplate.rows;
	TEMPLATE_PIXEL_WIDTH = targetTemplate.cols;

	while (true) {
		//Recieve current trike position
		//TODO
		trikeLocation.x = 0;
		trikeLocation.y = 0;
		trikeLocation.z = 0;


		//Capture new image
		camera.grab();
		camera.retrieve(frame);

		//For each cone
		for (int i = 0; i < allCones.size(); i++) {
			//Compute relative coordinates between trike and cone
			Point3d relativeToTrike;
			relativeToTrike.x = allCones[i].x - trikeLocation.x;
			relativeToTrike.y = allCones[i].y - trikeLocation.y;
			relativeToTrike.z = allCones[i].z - trikeLocation.z;

			//If distance is out of range, skip this cone
			/*if (distance_2d() > MAX_CAMERA_RANGE) {
			continue;
			}*/

			//Find expected location
			//expectedPoint = calculation()
			expectedPoint = Point(allCones[i].x, allCones[i].y);
			/*if (computedXY outside (image range - width of cone)) {
			continue;
			}*/

			//Find expected cone height/width and scale template image
			//expectedHeight = calculation()
			expectedHeight = allCones[i].z;
			templResizeRatio = (double)expectedHeight / TEMPLATE_PIXEL_HEIGHT;
			//If the ratio is too small, ignore this cone
			if (templResizeRatio > 0) {
				resize(targetTemplate, templResize, Size(), templResizeRatio, templResizeRatio, INTER_LINEAR);
			}
			else {
				continue;
			}
			imshow("TemplateResize", templResize);

			//Take sub-image of main image
			searchPoint.x = expectedPoint.x - (0.5 * templResize.cols) - TEMPLATE_SEARCH_BOUNDARY;
			searchPoint.y = expectedPoint.y - templResize.rows - TEMPLATE_SEARCH_BOUNDARY;
			subFrameRect = Rect(searchPoint.x, searchPoint.y,
				templResize.cols + 2 * TEMPLATE_SEARCH_BOUNDARY, templResize.rows + 2 * TEMPLATE_SEARCH_BOUNDARY);
			subFrameTemp = Mat(frame, subFrameRect);
			subFrameTemp.copyTo(subFrame);

			imshow("SubFrame", subFrame);
			rectangle(frame, subFrameRect, Scalar::all(255), 2, 8, 0);

			//Perform image detection
			templBinary = ImageProcessing::filterByColor(templResize, TEMPLATE_THRESHOLDS, 1);
			subFrameBinary = ImageProcessing::filterByColor(subFrame, FRAME_THRESHOLDS, 0);
			foundPoint = ImageProcessing::templateMatch(subFrameBinary, templBinary, TM_CCORR);

			//Find bottom middle of result
			actualPoint.x = foundPoint.x + searchPoint.x + (0.5 * templBinary.cols);
			actualPoint.y = foundPoint.y + searchPoint.y + templBinary.rows;
			cout << actualPoint << endl;

			//Find real location

			//GET CONFIDENCE
			//Save data for sending to ardunio
		}

		//send data
		imshow("Frame", frame);
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}
