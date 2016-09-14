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
	
	int64_t CAM_OFFSET_POS_X = 0;		//Camera's position relative to
	int64_t CAM_OFFSET_POS_Y = 0;		//trike's point of reference, in cm
	int64_t CAM_OFFSET_POS_Z = 0;
	double CAM_OFFSET_ANGLE_X = 0;	//in degrees
	double CAM_OFFSET_ANGLE_Y = 0;
	double CAM_OFFSET_ANGLE_Z = 0;
	tuple<int64_t, int64_t, int64_t> cam_pos;
	typedef tuple<int64_t, int64_t, int64_t> target_pos;
	vector<target_pos> allCones;
	tuple<double, double, double> cam_angle;
	tuple<int64_t, int64_t, int64_t> relative_pos;

	//Camera specifics
	int MAX_CAMERA_RANGE = 1000;		//in cm
	double CAM_FOCAL_LENGTH = 0.36;		//in cm
	double CAM_SENSOR_WIDTH = 0.3629;	//in cm
	double CAM_SENSOR_HEIGHT = 0.2722;	//in cm
	//int CAM_PIXEL_WIDTH = 2592;			//num of pixels horizontal
	//int CAM_PIXEL_HEIGHT = 1944;		//num of pixels vertical
	int CAM_PIXEL_WIDTH = 640;			//num of pixels horizontal
	int CAM_PIXEL_HEIGHT = 480;		//num of pixels vertical
	tuple<uint64_t, uint64_t> IMG_SIZE(CAM_PIXEL_HEIGHT, CAM_PIXEL_WIDTH);
	tuple<double, double> SENSOR_SIZE(CAM_SENSOR_HEIGHT, CAM_SENSOR_WIDTH);

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
	//Actual values
	int FRAME_THRESHOLDS[] = {
		0,		//low hue
		10,		//high hue
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
	tuple<double, double> expectedPixel;
	Point expectedPoint;
	Point searchPoint;
	Point foundPoint;
	Point actualPoint;
	double matchValue;
	int TEMPLATE_SEARCH_BOUNDARY_X = 20;	//how many pixels outside expected location box to search
	int TEMPLATE_SEARCH_BOUNDARY_Y = 20;
	int expectedHeight;					//in pixels
	double templResizeRatio;
	double MIN_TEMPL_RESIZE_RATIO;
	double MAX_TEMPL_RESIZE_RATIO;


	/* MAIN DRIVER USING EXPECTED LOCATIONS --------------------------------------------- */
	//Recieve Arduino information, with cone positions
	//TODO

	//For now, use pixels
	/*target_pos cone1(320, 240, 160);
	target_pos cone2(200, 400, 60);
	allCones.push_back(cone1);
	allCones.push_back(cone2);

	//Open template image file, and save appropriate variables
	targetTemplate = imread(TEMPLATE_IMG_FILE);
	imshow("Template", targetTemplate);
	TEMPLATE_PIXEL_HEIGHT = targetTemplate.rows;
	TEMPLATE_PIXEL_WIDTH = targetTemplate.cols;

	//Convert template image to binary for faster processing
	templBinary = elcano::filterByColor(targetTemplate, TEMPLATE_THRESHOLDS, 1);

	//Find largest allowable resize ratio
	MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_HEIGHT / (TEMPLATE_PIXEL_HEIGHT + 2 * TEMPLATE_SEARCH_BOUNDARY_Y);
	if ((double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X) < MAX_TEMPL_RESIZE_RATIO) {
		MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X);
	}

	//Find smallest allowable resize ratio based on max distance
	//TODO
	//int minHeight = calculation(MAX_CAMERA_RANGE, cameraspecs);
	//MIN_TEMPL_RESIZE_RATIO = (double)minHeight / TEMPLATE_PIXEL_HEIGHT;

	while (true) {
		//Recieve current trike position and bearing
		//TODO - replace 0s with recieved data
		get<0>(cam_pos) = 0 + CAM_OFFSET_POS_X;
		get<1>(cam_pos) = 0 + CAM_OFFSET_POS_Y;
		get<2>(cam_pos) = 0 + CAM_OFFSET_POS_Z;

		//TODO - replace one of these 0s with bearing data,
		//or multiple angles if that data is available
		get<0>(cam_angle) = 0.0 + CAM_OFFSET_ANGLE_X;
		get<1>(cam_angle) = 0.0 + CAM_OFFSET_ANGLE_Y;
		get<2>(cam_angle) = 0.0 + CAM_OFFSET_ANGLE_Z;

		//Capture new image
		camera.grab();
		camera.retrieve(frame);

		//Once per target, analyze the image
		for (int i = 0; i < allCones.size(); i++) {
			//Compute relative coordinates between trike and cone
			//get<0>(relative_pos) = get<0>(allCones[i]) - get<0>(cam_pos);
			//get<1>(relative_pos) = get<1>(allCones[i]) - get<1>(cam_pos);
			//get<2>(relative_pos) = get<2>(allCones[i]) - get<2>(cam_pos);

			//Find expected target height and compute resize ratio
			//TODO
			//expectedHeight = calculation(distance between allCones[i] and cam_pos, camspecs)
			expectedHeight = get<2>(allCones[i]);
			templResizeRatio = (double)expectedHeight / TEMPLATE_PIXEL_HEIGHT;

			//If target is out of range, skip
			if (templResizeRatio < MIN_TEMPL_RESIZE_RATIO) {
				continue;
			}
			//If target is too close, skip
			else if (templResizeRatio > MAX_TEMPL_RESIZE_RATIO) {
				continue;
			}

			//Scale template image
			resize(targetTemplate, templResize, Size(), templResizeRatio, templResizeRatio, INTER_LINEAR);

			//Find expected pixel location of target
			expectedPixel = elcano::global_to_relative(cam_angle, cam_pos, allCones[i], IMG_SIZE, SENSOR_SIZE, CAM_FOCAL_LENGTH);
			expectedPoint = Point((int)get<0>(expectedPixel), (int)get<1>(expectedPixel));
			//TODO
			//If point is out of frame, or close to it, skip
			//Implement as, if Point - templateSize < 0 AND Point + templateSize > frameSize

			//Capture sub-image of main image at expected location
			searchPoint.x = expectedPoint.x - (0.5 * templResize.cols) - TEMPLATE_SEARCH_BOUNDARY_X;
			searchPoint.y = expectedPoint.y - templResize.rows - TEMPLATE_SEARCH_BOUNDARY_Y;
			subFrameRect = Rect(searchPoint.x, searchPoint.y,
				templResize.cols + 2 * TEMPLATE_SEARCH_BOUNDARY_X, templResize.rows + 2 * TEMPLATE_SEARCH_BOUNDARY_Y);
			subFrameTemp = Mat(frame, subFrameRect);
			subFrameTemp.copyTo(subFrame);

			//Perform image detection
			subFrameBinary = elcano::filterByColor(subFrame, FRAME_THRESHOLDS, 1);
			foundPoint = elcano::templateMatch(subFrameBinary, templResize, TM_CCORR, matchValue);

			//Find bottom middle of result
			actualPoint.x = foundPoint.x + searchPoint.x + (0.5 * templBinary.cols);
			actualPoint.y = foundPoint.y + searchPoint.y + templBinary.rows;

			//Find real location

			//GET CONFIDENCE
			//Save data for sending to ardunio

			//Debug displaying
			imshow("SubFrame", subFrame);
			imshow("TemplateResize", templResize);
			rectangle(frame, subFrameRect, Scalar::all(255), 2, 8, 0);
			line(frame, actualPoint, actualPoint, Scalar(0, 0, 255), 4, 8, 0);
		}

		//send data
		imshow("Frame", frame);
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}*/


	/* MAKERFARE DEMO -------------------------------------------------------- */
	//Open template image file, and save appropriate variables
	targetTemplate = imread(TEMPLATE_IMG_FILE);
	TEMPLATE_PIXEL_HEIGHT = targetTemplate.rows;
	TEMPLATE_PIXEL_WIDTH = targetTemplate.cols;

	//Convert template image to binary for faster processing
	templBinary = elcano::filterByColor(targetTemplate, TEMPLATE_THRESHOLDS, 1);

	//Find largest allowable resize ratio for the template
	MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_HEIGHT / (TEMPLATE_PIXEL_HEIGHT + 2 * TEMPLATE_SEARCH_BOUNDARY_Y);
	if ((double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X) < MAX_TEMPL_RESIZE_RATIO) {
		MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X);
	}

	//Find smallest allowable resize ratio based on max distance
	//TODO
	//int minHeight = calculation(MAX_CAMERA_RANGE, cameraspecs);
	//MIN_TEMPL_RESIZE_RATIO = (double)minHeight / TEMPLATE_PIXEL_HEIGHT;

	while (true) {
		//Get frame from video
		bool bSuccess = cap.read(frame);
		if (!bSuccess) {
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		//Convert video frame to binary, and do a first pass to find number of positive pixels
		subFrameBinary = elcano::filterByColor(frame, FRAME_THRESHOLDS, 1);
		int whitePixelCount = countNonZero(subFrameBinary);
		if (whitePixelCount < 10) { continue; }

		//Determine expected height of target based on number of positive pixels
		expectedHeight = 2 * sqrt(whitePixelCount);
		templResizeRatio = (double)expectedHeight / TEMPLATE_PIXEL_HEIGHT;
		//If target is out of range, skip
		if (templResizeRatio < MIN_TEMPL_RESIZE_RATIO) {
			continue;
		}
		//If target is too close, skip
		else if (templResizeRatio > MAX_TEMPL_RESIZE_RATIO) {
			continue;
		}

		//Resize template image
		resize(templBinary, templResize, Size(), templResizeRatio, templResizeRatio, INTER_LINEAR);
		foundPoint = elcano::templateMatch(subFrameBinary, templResize, TM_CCORR_NORMED, matchValue);

		//Debug displaying
		imshow("FrameBinary", subFrameBinary);
		imshow("TemplateResize", templResize);
		std::cout << "Found point: " << foundPoint << "   Value: " << matchValue << std::endl;
		rectangle(frame, foundPoint, Point(foundPoint.x + templResize.cols, foundPoint.y + templResize.rows), Scalar::all(255), 2, 8, 0);
		line(frame, foundPoint, foundPoint, Scalar(0, 0, 255), 4, 8, 0);
		imshow("Frame", frame);

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}
