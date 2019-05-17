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

#define CONE_HEIGHT			0.4953
#define CAM_CONSTANT		1583.5
#define DIST_CONSTANT		CONE_HEIGHT * CAM_CONSTANT
#define DEG_PER_PIXEL		0.036
#define BUF_SIZE 			128

using namespace cv;
using namespace std;

const Scalar light_green = Scalar(100, 255, 0);
const Scalar yellow = Scalar(0, 255, 255);
const Scalar red = Scalar(0, 0 , 255);
int cport_nr=24; /* /dev/ttyACM0 */
int imgXCenter = 0;

bool isConvexHullPointingUp(vector<Point>& convexHull);
vector<pair<Rect,Point>> detectCones(Mat& imgOriginal);
void sendToArduino(float dist, float deg);
void receiveFromArduino();
vector<pair<Rect,Point>> removeInnerOverlappingCones(vector<pair<Rect,Point>>& cones);
void findClosestCone(vector<pair<Rect,Point>>& cones, KalmanFilter& kf, Mat& state, Mat& meas, bool& f);

int main(int argc, char* argv[]) {
	int bdrate=57600; /* 9600 baud */

	char mode[]={'8','N','1',0}; // 8 data bits, no parity, 1 stop bit

//	if(RS232_OpenComport(cport_nr, bdrate, mode))
//	{
//		cout << "Can not open comport\n";
//		return 0;
//	}

	usleep(2000000);  /* waits 2000ms for stable condition */

	int stateSize = 6;
	int measSize = 4;

	cv::KalmanFilter kf(stateSize, measSize);

	cv::Mat state(stateSize, 1, CV_32F);  // [x,y,v_x,v_y,w,h]
	cv::Mat meas(measSize, 1, CV_32F);    // [z_x,z_y,z_w,z_h]
	// [E_x,E_y,E_v_x,E_v_y,E_w,E_h]

	// Transition State Matrix A
	// Note: set dT at each processing step!
	// [ 1 0 dT 0  0 0 ]
	// [ 0 1 0  dT 0 0 ]
	// [ 0 0 1  0  0 0 ]
	// [ 0 0 0  1  0 0 ]
	// [ 0 0 0  0  1 0 ]
	// [ 0 0 0  0  0 1 ]
	cv::setIdentity(kf.transitionMatrix);

	// Measure Matrix H
	// [ 1 0 0 0 0 0 ]
	// [ 0 1 0 0 0 0 ]
	// [ 0 0 0 0 1 0 ]
	// [ 0 0 0 0 0 1 ]
	kf.measurementMatrix = cv::Mat::zeros(measSize, stateSize, CV_32F);
	kf.measurementMatrix.at<float>(0) = 1.0f;
	kf.measurementMatrix.at<float>(7) = 1.0f;
	kf.measurementMatrix.at<float>(16) = 1.0f;
	kf.measurementMatrix.at<float>(23) = 1.0f;

	// Process Noise Covariance Matrix Q
	// [ Ex   0   0     0     0    0  ]
	// [ 0    Ey  0     0     0    0  ]
	// [ 0    0   Ev_x  0     0    0  ]
	// [ 0    0   0     Ev_y  0    0  ]	cout << "Img center = " << imgXCenter << endl;
	// [ 0    0   0     0     Ew   0  ]
	// [ 0    0   0     0     0    Eh ]
	kf.processNoiseCov.at<float>(0) = 1e-2;
	kf.processNoiseCov.at<float>(7) = 1e-2;
	kf.processNoiseCov.at<float>(14) = 5.0f;
	kf.processNoiseCov.at<float>(21) = 5.0f;
	kf.processNoiseCov.at<float>(28) = 1e-2;
	kf.processNoiseCov.at<float>(35) = 1e-2;

	// Measures Noise Covariance Matrix R
	cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));

	VideoCapture capture(0);
	Mat frame;
	vector<pair<Rect, Point>> cones; // first - boudning rect of cone, second - center point
	double ticks = 0;
	bool found = false;

	int notFoundCount = 0;
	imgXCenter = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH) / 2;

	while (true){
		double precTicks = ticks;
		ticks = (double) getTickCount();
		double dT = (ticks - precTicks) / getTickFrequency();

		capture >> frame;
		if (found){
			kf.transitionMatrix.at<float>(2) = dT;
			kf.transitionMatrix.at<float>(9) = dT;
			state = kf.predict();

			cv::Rect predRect;
			predRect.width = state.at<float>(4);
			predRect.height = state.at<float>(5);
			predRect.x = state.at<float>(0) - predRect.width / 2;
			predRect.y = state.at<float>(1) - predRect.height / 2;

			cv::Point center;
			center.x = state.at<float>(0);
			center.y = state.at<float>(1);
			float distance = DIST_CONSTANT / predRect.height;
			float degrees = DEG_PER_PIXEL * (center.x - imgXCenter);
			cout << "Predicted distance, degrees = " << distance << ", " << degrees << endl;
		}

		cones = detectCones(frame);

		if (cones.size() == 0){
			notFoundCount++;
			if (notFoundCount >= 20){
				found = false;
			}
		}
		else {
			notFoundCount = 0;
			findClosestCone(cones, kf, state, meas, found);
		}

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

vector<pair<Rect, Point>> detectCones(Mat& imgOriginal){
	Mat imgHSV, imgThreshLow, imgThreshHigh, imgThreshSmooth, imgCanny,
		imgContours, imgAllConvexHulls, imgConvexHulls3to10, imgTrafficCones;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<pair<Rect, Point>> cones; // bounding rectangle of cone, and its center point

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
				// get rectangle and center
				Rect rectangle = boundingRect(hulls[i]);
				Point center = Point(cX, cY);
				//cout << "(" << center.x << ", " << center.y << ")";
				//cout << "\tDistance, degrees: " << distance << ", " << degrees << endl;
				cones.push_back(make_pair(rectangle,center));
			}
		}
	}

	//imshow("imgContours", imgContours);
	imshow("imgThreshSmooth", imgThreshSmooth);
	//imshow("imgTrafficCones", imgTrafficCones);
	imshow("imgOriginal", imgOriginal);

	return cones;
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

// logic is not correct for this function yet, do not use
vector<pair<Rect,Point>> removeInnerOverlappingCones(vector<pair<Rect,Point>>& cones){
	vector<pair<Rect,Point>> conesWithOverlapsRemoved;
	for (size_t i = 0; i < cones.size(); i++){
		// if a point is inside another rectangle
		for (size_t j = i+1; j < cones.size(); j++){
			// if the point is between the x & y coordinates of another rectangle
			if ((cones[i].second.x >= cones[j].first.x) &&
					(cones[i].second.x <= (cones[j].first.x + cones[j].first.width)) &&
					(cones[i].second.y >= cones[j].first.y) &&
					(cones[i].second.y <= cones[j].first.y + cones[j].first.height)){
				// return the cone that has larger area
				if (cones[i].first.area() > cones[j].first.area()){
					conesWithOverlapsRemoved.push_back(cones[i]);
				}
				else{
					conesWithOverlapsRemoved.push_back(cones[j]);
				}
			}
		}
	}
	return conesWithOverlapsRemoved;
}

void findClosestCone(vector<pair<Rect,Point>>& cones, KalmanFilter &kf, Mat& state, Mat& meas, bool &found){
	pair<Rect,Point> cone;
	float minDistance = INT_MAX;
	float minDegrees = 0;
	float maxArea = INT_MIN;

	for (size_t i = 0; i < cones.size(); i++){
		float distance = DIST_CONSTANT / cones[i].first.height;
		float degrees = DEG_PER_PIXEL * (cones[i].second.x - imgXCenter);
		if(cones[i].first.area() > maxArea){
			maxArea = cones[i].first.area();
			minDistance = distance;
			minDegrees = degrees;
			cone = cones[i];
		}
	}

	// update measurement matrix
	meas.at<float>(0) = cone.second.x;
	meas.at<float>(1) = cone.second.y;
	meas.at<float>(2) = (float)cone.first.width;
	meas.at<float>(3) = (float)cone.first.height;

	if (!found) // First detection!
	{
		// >>>> Initialization
		kf.errorCovPre.at<float>(0) = 1; // px
		kf.errorCovPre.at<float>(7) = 1; // px
		kf.errorCovPre.at<float>(14) = 1;
		kf.errorCovPre.at<float>(21) = 1;
		kf.errorCovPre.at<float>(28) = 1; // px
		kf.errorCovPre.at<float>(35) = 1; // px

		state.at<float>(0) = meas.at<float>(0);
		state.at<float>(1) = meas.at<float>(1);
		state.at<float>(2) = 0;
		state.at<float>(3) = 0;
		state.at<float>(4) = meas.at<float>(2);
		state.at<float>(5) = meas.at<float>(3);
		// <<<< Initialization

		kf.statePost = state;

		found = true;
	}
	else { // Kalman Correction
		kf.correct(meas);
	}

	cout << "Calculated distance, degrees = " << minDistance << ", " << minDegrees << endl;
	//sendToArduino(minDistance, minDegrees);
	//receiveFromArduino();
}
