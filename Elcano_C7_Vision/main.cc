/* main.cc
 * 
 * Authors: Aaron Conrad and Dylan Katz
 * Last updated: Sept. 16, 2016
 * 
 * Main driver for Elcano module C7, the Vision module.
 * 
 * Currently runs a demonstration for Seattle Maker Faire that
 * looks for one target object in view of the camera, and draws
 * a box around it.
 */

#include <serial/serial.h>
#include "detect.hh"
#include "args.hh"

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
	
	/* Connect to Output */
	//serial::Serial output(args::get(output_name), args::get(baudrate), serial::Timeout::simpleTimeout(100));
	//if (!output.isOpen()) { std::cerr << "Unable to open the output device!" << std::endl << parser; return -1; }
	
	/* Connect to Camera */
	raspicam::RaspiCam_Cv camera;
	camera.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	camera.set(CV_CAP_PROP_BRIGHTNESS, 50);
	if (!camera.open()) { std::cerr << "Unable to open the camera!" << std::endl; return -1; }	
	
	int64_t CAM_OFFSET_POS_X = 0;		//Camera's position relative to
	int64_t CAM_OFFSET_POS_Y = 0;		//trike's point of reference, in cm
	int64_t CAM_OFFSET_POS_Z = 0;
	double CAM_OFFSET_ANGLE_X = 0;	//in degrees
	double CAM_OFFSET_ANGLE_Y = 0;
	double CAM_OFFSET_ANGLE_Z = 0;
	std::tuple<int64_t, int64_t, int64_t> cam_pos;
	typedef std::tuple<int64_t, int64_t, int64_t> target_pos;
	std::vector<target_pos> allCones;
	std::tuple<double, double, double> cam_angle;
	std::tuple<int64_t, int64_t, int64_t> relative_pos;

	//Camera specifics
	int MAX_CAMERA_RANGE = 1000;		//in cm
	double CAM_FOCAL_LENGTH = 0.36;		//in cm
	double CAM_SENSOR_WIDTH = 0.3629;	//in cm
	double CAM_SENSOR_HEIGHT = 0.2722;	//in cm
	//int CAM_PIXEL_WIDTH = 2592;			//num of pixels horizontal
	//int CAM_PIXEL_HEIGHT = 1944;		//num of pixels vertical
	int CAM_PIXEL_WIDTH;
	int CAM_PIXEL_HEIGHT;
	std::tuple<uint64_t, uint64_t> IMG_SIZE(CAM_PIXEL_HEIGHT, CAM_PIXEL_WIDTH);
	std::tuple<double, double> SENSOR_SIZE(CAM_SENSOR_HEIGHT, CAM_SENSOR_WIDTH);

	//Target object specifics
	cv::Mat targetTemplate;							//Template image
	std::string TEMPLATE_IMG_FILE = "/home/pi/Documents/aaron-elcano/Elcano_C7_Vision/Cone.jpg";
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
	cv::Mat frame;
	//Actual values
	int FRAME_THRESHOLDS[] = {
		160,		//low hue
		180,		//high hue
		90,		//low saturation
		255,	//high saturation
		60,		//low value
		255		//high value
	};

	//Image processing variables (for template matching)
	cv::Rect subFrameRect;
	cv::Mat subFrameTemp;
	cv::Mat subFrame;
	cv::Mat subFrameBinary;
	cv::Mat templResize;
	cv::Mat templBinary;
	std::tuple<double, double> expectedPixel;
	cv::Point expectedPoint;
	cv::Point searchPoint;
	cv::Point foundPoint;
	cv::Point actualPoint;
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
	targetTemplate = cv::imread(TEMPLATE_IMG_FILE);
	TEMPLATE_PIXEL_HEIGHT = targetTemplate.rows;
	TEMPLATE_PIXEL_WIDTH = targetTemplate.cols;
	
	//Grab a single frame to get image size
	camera.grab();
	camera.retrieve(frame);
	cv::resize(frame, frame, cv::Size(), args::get(scale), args::get(scale));
	CAM_PIXEL_HEIGHT = frame.rows;
	CAM_PIXEL_WIDTH = frame.cols;
	
	//Convert template image to binary for faster processing
	templBinary = elcano::filterByColor(targetTemplate, TEMPLATE_THRESHOLDS, 1);

	//Find largest allowable resize ratio for the template
	MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_HEIGHT / (TEMPLATE_PIXEL_HEIGHT + 2 * TEMPLATE_SEARCH_BOUNDARY_Y);
	if ((double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X) < MAX_TEMPL_RESIZE_RATIO) {
		MAX_TEMPL_RESIZE_RATIO = (double)CAM_PIXEL_WIDTH / (TEMPLATE_PIXEL_WIDTH + 2 * TEMPLATE_SEARCH_BOUNDARY_X);
	}
	
	//Find smallest allowable resize ratio based on allowable pixel height
	int minHeight = 20;
	MIN_TEMPL_RESIZE_RATIO = (double)minHeight / TEMPLATE_PIXEL_HEIGHT;

	while (true) {
		//Get frame from video
		camera.grab();
		camera.retrieve(frame);
		cv::resize(frame, frame, cv::Size(), args::get(scale), args::get(scale));
		cv::cvtColor(frame, frame, CV_RGB2BGR);

		//Convert video frame to binary, and do a first pass to find number of positive pixels
		subFrameBinary = elcano::filterByColor(frame, FRAME_THRESHOLDS, 1);
		int whitePixelCount = cv::countNonZero(subFrameBinary);

		//Determine expected height of target based on number of positive pixels
		expectedHeight = 0.025 * whitePixelCount;
		templResizeRatio = (double)expectedHeight / TEMPLATE_PIXEL_HEIGHT;
		
		//If target is out of range, skip frame
		if (templResizeRatio < MIN_TEMPL_RESIZE_RATIO) {
			std::cout << "No visible target" << std::endl;
		}
		//If target is too close, skip frame
		else if (templResizeRatio > MAX_TEMPL_RESIZE_RATIO) {
			std::cout << "Target is too close" << std::endl;
		}
		//Otherwise, perform computations
		else {
			//Resize template image, and template match
			cv::resize(templBinary, templResize, cv::Size(), templResizeRatio, templResizeRatio);
			foundPoint = elcano::templateMatch(subFrameBinary, templResize, cv::TM_CCORR_NORMED, matchValue);
			
			//Determine where the target's reference point is located
			actualPoint.x = foundPoint.x + (0.5 * templResize.cols);
			actualPoint.y = foundPoint.y + templResize.rows;

			//Debug displaying
			//imshow("TemplateResize", templResize);
			std::cout << "Actual point: " << actualPoint << "   Correlation: " << matchValue << std::endl;
			if (matchValue > 0.8) {
				cv::rectangle(frame, foundPoint, cv::Point(foundPoint.x + templResize.cols, foundPoint.y + templResize.rows), cv::Scalar(0, 255, 0), 2, 8, 0);
			}
			else if (matchValue > 0.6) {
				cv::rectangle(frame, foundPoint, cv::Point(foundPoint.x + templResize.cols, foundPoint.y + templResize.rows), cv::Scalar(0, 255, 255), 2, 8, 0);
			}
			else if (matchValue > 0.4) {
				cv::rectangle(frame, foundPoint, cv::Point(foundPoint.x + templResize.cols, foundPoint.y + templResize.rows), cv::Scalar(0, 0, 255), 2, 8, 0);
			}
		}

		//Display video frame
		//cv::resize(subFrameBinary, subFrameBinary, Size(), 0.25, 0.25);
		cv::resize(frame, frame, cv::Size(), 1.25, 1.25);
		cv::imshow("FrameBinary", subFrameBinary);
		cv::imshow("Frame", frame);

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (cv::waitKey(30) == 27) {
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
}
