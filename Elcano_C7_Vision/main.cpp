#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
extern "C" {
	#include "rs232.h"
}

#define CONE_HEIGHT		0.4953
#define CAM_CONSTANT		1583.5
#define DIST_CONSTANT		CONE_HEIGHT * CAM_CONSTANT
#define DEG_PER_PIXEL		0.036
#define BUF_SIZE 		128

using namespace cv;
using namespace std;

const Scalar light_green = Scalar(100, 255, 0);
const Scalar yellow = Scalar(0, 255, 255);
const Scalar red = Scalar(0, 0 , 255);
int cport_nr=24; /* /dev/ttyACM0 */


bool isConvexHullPointingUp(vector<Point>& convexHull);
void detectCones(Mat& imgOriginal);
void sendToArduino(float dist, float deg);
void receiveFromArduino();

int main(int argc, char* argv[]) {
	int bdrate=57600; /* 9600 baud */

	char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit

	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
		cout << "Can not open comport\n";
		return 0;
	}

	usleep(2000000);  /* waits 2000ms for stable condition */

	VideoCapture capture(0);
	Mat frame;
	while (true){
		capture >> frame;
		detectCones(frame);
		if (waitKey(20) == 27)
			break;
	}
	capture.release();
	return 0;
}

bool isConvexHullPointingUp(vector<Point>& convexHull) {
	Rect rectangle = boundingRect(convexHull);
	double aspectRatio = rectangle.width / rectangle.height;
	if (aspectRatio > 0.8)
		return false;
	vector<Point> pointsAboveCenter, pointsBelowCenter;

	int verticalCenter = rectangle.y + (int)((double)rectangle.height / 2.0);
	int leftMostPointBelowCenter = convexHull[0].x;
	int rightMostPointBelowCenter = convexHull[0].x;
	for (size_t i = 0; i < convexHull.size(); i++) {
		if (convexHull[i].y < verticalCenter)
			pointsAboveCenter.push_back(convexHull[i]);
		else
			pointsBelowCenter.push_back(convexHull[i]);
	}

	for (size_t i = 0; i < pointsBelowCenter.size(); i++) {
		if (convexHull[i].x < leftMostPointBelowCenter) {
			leftMostPointBelowCenter = convexHull[i].x;
		}
		if (convexHull[i].x > rightMostPointBelowCenter) {
			rightMostPointBelowCenter = convexHull[i].x;
		}
	}

	for (size_t i = 0; i < pointsAboveCenter.size(); i++) {
		if (pointsAboveCenter[i].x < leftMostPointBelowCenter ||
			pointsAboveCenter[i].x > rightMostPointBelowCenter)
			return false;
	}

	return true;
}

void detectCones(Mat& imgOriginal){
	Mat imgHSV, imgThreshLow, imgThreshHigh, imgThreshSmooth, imgCanny,
		imgContours, imgAllConvexHulls, imgConvexHulls3to10, imgTrafficCones;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<pair<Rect, Point>> cones; // bounding rectangle of cone, and its center point
	int imgXCenter = imgOriginal.cols / 2;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
	inRange(imgHSV, Scalar(0, 145, 96), Scalar(12, 255, 255), imgThreshLow);
	inRange(imgHSV, Scalar(174, 135, 117), Scalar(179, 255, 255), imgThreshHigh);

	Mat imgThresh = imgThreshLow | imgThreshHigh;

	erode(imgThresh, imgThreshSmooth, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dilate(imgThreshSmooth, imgThreshSmooth, getStructuringElement(MORPH_RECT, Size(3, 3)));

	GaussianBlur(imgThreshSmooth, imgThreshSmooth, Size(3, 3), 0, 0);
	Canny(imgThreshSmooth, imgCanny, 160.0, 80.0);
	findContours(imgCanny.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > hulls(contours.size());

	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(contours[i], contours[i], 9.0, true);
		convexHull(Mat(contours[i]), hulls[i], CV_CLOCKWISE);
	}
	//imgContours = Mat::zeros(imgThresh.size(), CV_8UC3);
	//imgAllConvexHulls = Mat::zeros(imgThresh.size(), CV_8UC3);
	//imgConvexHulls3to10 = Mat::zeros(imgThresh.size(), CV_8UC3);
	imgTrafficCones = Mat::zeros(imgThresh.size(), CV_8UC3);
	float maxArea = INT_MIN;
	float minDistance = INT_MAX;
	float minDegrees = 0;
	// drawing contours on the images is not necessary, but for visualization
	for (size_t i = 0; i < contours.size(); i++) {
		//drawContours(imgContours, contours, i, light_green, 1, 8, hierarchy, 0, Point());
		//drawContours(imgAllConvexHulls, hulls, i, yellow, 1, 8, hierarchy, 0, Point());
		if (hulls[i].size() >= 3 && hulls[i].size() <= 7) {
			//drawContours(imgConvexHulls3to10, hulls, i, red, 1, 8, hierarchy, 0, Point());
			if (isConvexHullPointingUp(hulls[i])) {
				drawContours(imgTrafficCones, hulls, i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
				drawContours(imgOriginal, hulls, i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());

				Moments m = moments(hulls[i], false);
				int cX = m.m10 / m.m00;
				int cY = m.m01 / m.m00;
				// get rectangle and draw center
				Rect rectangle = boundingRect(hulls[i]);
				Point center = Point(cX, cY);
				// determine distance
				float distance = DIST_CONSTANT / rectangle.height;
				float degrees = DEG_PER_PIXEL * (center.x - imgXCenter);
				if (rectangle.area() > maxArea){
					maxArea = rectangle.area();
					minDistance = distance;
					minDegrees = degrees;
				}
				//cout << "(" << center.x << ", " << center.y << ")";
				//cout << "\tDistance, degrees: " << distance << ", " << degrees << endl;
				cones.push_back(make_pair(rectangle,center));
			}
		}
	}
	
	// iterate through detected cones to find the closest one, and send that information to the arduino
	for (size_t i = 0; i < cones.size(); i++){
		if(cones[i].first.area() == maxArea){
			circle(imgOriginal, cones[i].second, 1, Scalar(0, 255, 0), 2);
			// get reading from Kalman filter on where cone should be
			sendToArduino(minDistance, minDegrees);
		}
		else {
			circle(imgOriginal, cones[i].second, 1, Scalar(255, 0, 0), 2);
		}
	}
	
	if (!cones.empty()){
		cones.clear();
	}
	else{ // tell Kalman filter that no cones were found in image
		
	}
	receiveFromArduino();

	//imshow("imgContours", imgContours);
	imshow("imgThreshSmooth", imgThreshSmooth);
	//imshow("imgTrafficCones", imgTrafficCones);
	imshow("imgOriginal", imgOriginal);
	//waitKey(0);
}

void sendToArduino(float dist, float deg){ 
	int i_deg = (int)deg+10;
	if (i_deg < 0) i_deg = 0;
	string send_str = to_string((int)dist) + "," + to_string(i_deg) + "\n";
	RS232_cputs(cport_nr, send_str.c_str());
	cout << "Sent to Arduino: " << send_str;
	usleep(8000);
}

void receiveFromArduino(){
	unsigned char str_recv[BUF_SIZE]; // recv data buffer
	int n = RS232_PollComport(cport_nr, str_recv, (int)BUF_SIZE);
	if(n > 0){
	  str_recv[n] = 0;   // put null at end
	  cout << "Received " << n << " bytes: "<< (char *)str_recv << "\n";
	}
}

