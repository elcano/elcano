#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

bool isConvexHullPointingUp(vector<Point>& convexHull);

Scalar light_green = Scalar(255, 255, 100);
Scalar yellow = Scalar(255, 255, 0);
Scalar red = Scalar(255, 0, 0);

int main(int argc, char* argv[]) {
	Mat imgOriginal = imread(argv[1], 1);
	Mat imgHSV, imgThreshLow, imgThreshHigh, imgThreshSmooth, imgCanny, 
		imgContours, imgAllConvexHulls, imgConvexHulls3to10, imgTrafficCones;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);
	inRange(imgHSV, Scalar(0, 135, 135), Scalar(15, 255, 255), imgThreshLow);
	inRange(imgHSV, Scalar(159, 135, 135), Scalar(179, 255, 255), imgThreshHigh);

	Mat imgThresh = imgThreshLow | imgThreshHigh;

	erode(imgThresh, imgThreshSmooth, getStructuringElement(MORPH_RECT, Size(3, 3)));
	dilate(imgThreshSmooth, imgThreshSmooth, getStructuringElement(MORPH_RECT, Size(3, 3)));

	GaussianBlur(imgThreshSmooth, imgThreshSmooth, Size(3, 3), 0, 0);
	Canny(imgThreshSmooth, imgCanny, 160.0, 80.0);
	findContours(imgCanny.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<vector<Point> > hull(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(contours[i], contours[i], 6.0, true);
		convexHull(Mat(contours[i]), hull[i], CV_CLOCKWISE);
	}
	imgContours = Mat::zeros(imgThresh.size(), CV_8UC3);
	imgAllConvexHulls = Mat::zeros(imgThresh.size(), CV_8UC3);
	imgConvexHulls3to10 = Mat::zeros(imgThresh.size(), CV_8UC3);
	imgTrafficCones = Mat::zeros(imgThresh.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		drawContours(imgContours, contours, i, light_green, 1, 8, hierarchy, 0, Point());
		drawContours(imgAllConvexHulls, hull, i, yellow, 1, 8, hierarchy, 0, Point());
		if (hull[i].size() >= 3 || hull.size() <= 10) {
			drawContours(imgConvexHulls3to10, hull, i, red, 1, 8, hierarchy, 0, Point());
			if (isConvexHullPointingUp(hull[i])) {
				drawContours(imgTrafficCones, hull, i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
			}
		}
	}
	//imshow("contours", imgContours);
	imshow("hulls", imgAllConvexHulls);
	//imshow("hulls 3 to 10", imgConvexHulls3to10);
	imshow("cones", imgTrafficCones);
	//imshow("hsv colorspace", imgHSV);
	//imshow("low threshold", imgThreshLow);
	//imshow("high threshold", imgThreshHigh);
	//imshow("threshold", imgThresh);
	//imshow("threshold smoothed", imgThreshSmooth);
	waitKey(0);
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