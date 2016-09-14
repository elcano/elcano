#pragma once

/* Helper function for processing camera input */

using namespace cv;

namespace elcano
{
	Mat convertToEdgeMap(Mat input, int lowThresh, int highThresh, int apertureSize);
	Mat filterByColor(Mat input, int thresholds[], int colorType);
	Point templateMatch(Mat image, Mat target, int matchMedthod, double &value);
	Point templateMatchEdges(Mat image, Mat target, int lowThresh, int highThresh, int apertureSize, int matchMethod);
	Point templateMatchBlobs(Mat image, Mat target, int thresholds[], int matchMethod);
}
