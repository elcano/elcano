// LineDetectionMain.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <math.h>

//extern "C" {
//#include "rs232.h"
//}


#define DEG_PER_PIXEL		0.036
#define BUF_SIZE 		128

using namespace cv;
using namespace std;

const Scalar light_green = Scalar(100, 255, 0);
const Scalar yellow = Scalar(0, 255, 255);
const Scalar red = Scalar(0, 0, 255);
int cport_nr = 24; /* /dev/ttyACM0 */


void detectLines(Mat& imgOriginal, int processCount);
double getLength(Point p1, Point p2);
int getShiftAmount(int x);
//void sendToArduino(float dist, float deg);
//void receiveFromArduino();


Mat rotate(Mat src, double angle)
{
	if (src.empty()) {
		cout << "Error loading the image" << endl;
		exit(1);
	}
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, Size(src.cols, src.rows));
	return dst;
}

int main(int argc, char* argv[]) {
	int bdrate = 57600; /* 9600 baud */

	char mode[] = { '8','N','1',0 }; // 8 data bits, no parity, 1 stop bit

									 //if (RS232_OpenComport(cport_nr, bdrate, mode)) {
									 //	cout << "Can not open comport\n";
									 //    return 0;
									 //}
									 //Sleep(2000000);
									 //usleep(2000000);  /* waits 2000ms for stable condition */

									 //VideoCapture capture(0);
	VideoCapture capture("outside6.mp4");
	Mat frame;
	
	
	// Some of the test videos are upside down so we use this when testing on those videos
	Mat flipped;


	int processCount = 0;
	while (true) {
		capture >> frame;

		// Rotate image 180 degrees so it is rightside up
		flipped = rotate(frame, 180);
		// Draws a black rectangle on top half of image. Lines found on the ground are what we are interested in
		rectangle(flipped, Point(0, 0),
			Point(640, 240), Scalar(0, 0, 0), CV_FILLED, 8);

		//imshow("flipped", flipped);
		
		Mat dst;
		frame = flipped;
		/// Convert to grayscale
		cvtColor(frame, frame, CV_BGR2GRAY);

		/// Apply Histogram Equalization
		equalizeHist(frame, dst);

		//imshow("original", frame);
		//imshow("equalized", dst);
		//detectLines(frame, processCount);
		detectLines(dst, processCount);
		
		
		//detectLines(flipped, processCount);
		processCount++;
		if (waitKey(20) == 27)
			break;
	}

	capture.release();
	return 0;
}



void detectLines(Mat& input, int processCount) {
	cout << processCount << endl;
	if (input.empty()) {
		cout << "Error loading the image" << endl;
		exit(1);
	}


	int numAvgLines = 0;
	Point topAvg(0, 0);
	Point botAvg(0, 0);
	Point topTemp(0, 0);
	Point botTemp(0, 0);


	// Masks image for yellow color
	//Mat yellowOnly = yellowFilter(input);

	//// Draws a line straight down middle of screen as a reference line
	//line(frame, Point(320, 0),
	//	Point(320, 480), Scalar(0, 0, 0), 3, 8);
	//// Draws two lines in prospective straight area
	//line(frame, Point(220, 0),
	//	Point(220, 480), Scalar(0, 0, 0), 3, 8);
	//line(frame, Point(420, 0),
	//	Point(420, 480), Scalar(0, 0, 0), 3, 8);


	//////////////////////////////////////////////
	int thresh = 40;
	Mat input_blur;
	GaussianBlur(input, input_blur, Size(15, 15), 0, 0);

	Mat canny_output;
	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;


	/// Detect edges using canny
	Canny(input_blur, canny_output, 40, 120, 3);
	//imshow("input_blur", input_blur);
	imshow("canny output", canny_output);
	// Create a vector which contains 4 integers in each element (coordinates of the line)
	// This vector will be used the output of the Hough Lines transform
	vector<Vec4i> lines;

	// Set limits on hough trasnform line detection
	//Good value here
	//double minLineLength = 80;
	double minLineLength = 120;
	double maxLineGap = 5;

	HoughLinesP(canny_output, lines, 1, CV_PI / 180, 80, minLineLength, maxLineGap);

	int numLines = lines.size();
	//cout << "numLines = " << numLines << endl;
	double longestLine = 0;

	Point highestPoint(0, 0);
	Point lowestPoint(0, 0);

	// Draws the lines found from the houghline transform
	for (size_t i = 0; i < numLines; i++) {
		//cout << lines[i] << endl;
		float x1 = lines[i][0];
		float y1 = lines[i][1];
		float x2 = lines[i][2];
		float y2 = lines[i][3];

		float slope = (y2 - y1) / (x2 - x1);
		int length = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
		
		if (slope != 0) {
			line(input, Point(x1, y1),
				Point(x2, y2), Scalar(156, 25, 0), 3, 8);
		}



		cout << "slope = " << slope << " length = " << length << endl;
		//circle(input, Point(220, 230), 5, Scalar(0, 255, 255), -1);
		//circle(input, Point(420, 230), 5, Scalar(0, 255, 255), -1);
		//circle(input, Point(10, 470), 5, Scalar(0, 255, 255), -1);
		//circle(input, Point(630, 470), 5, Scalar(0, 255, 255), -1);

	}
	imshow("input", input);
}


//imshow("canny_output", canny_output);
//imshow("yellowOnly", yellowOnly);
//imshow("input", input);







int getShiftAmount(int x) {
	// We want to shift the x into the middle to figure out the angle that would be made between these two lines
	int shiftTarget = 320;
	return shiftTarget - x;
}

double getLength(Point p1, Point p2) {
	int xVal = pow(p2.x - p1.x, 2);
	int yVal = pow(p2.y - p1.y, 2);
	int distance = sqrt(xVal + yVal);
	return distance;
}


//void sendToArduino(float dist, float deg) {
//	int i_deg = (int)deg + 10;
//	if (i_deg < 0) i_deg = 0;
//	string send_str = to_string((int)dist) + "," + to_string(i_deg) + "\n";
//	RS232_cputs(cport_nr, send_str.c_str());
//	cout << "Sent to Arduino: " << send_str;
//	//usleep(8000);
//	Sleep(8000);
//}
//
//void receiveFromArduino() {
//	unsigned char str_recv[BUF_SIZE]; // recv data buffer
//	int n = RS232_PollComport(cport_nr, str_recv, (int)BUF_SIZE);
//	if (n > 0) {
//		str_recv[n] = 0;   // put null at end
//		cout << "Received " << n << " bytes: " << (char *)str_recv << "\n";
//	}
//}