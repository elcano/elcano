#include "detect.hh"

/* Helper function for processing camera input */

using namespace cv;

namespace elcano
{
	Mat convertToEdgeMap(Mat input, int lowThresh, int highThresh, int apertureSize) {
		Mat result;
		cvtColor(input, result, CV_BGR2GRAY);
		//Reduce noise with a 3x3 kernel
		blur(result, result, Size(3, 3));
		//Detect edges
		Canny(result, result, lowThresh, highThresh, apertureSize);
		return result;
	}
	
	/* colorType 0 is RGB; colorType 1 is BGR */
	Mat filterByColor(Mat input, int thresholds[], int colorType) {
		Mat result;
		
		if (colorType == 0) {
			cvtColor(input, result, COLOR_RGB2HSV);
		}
		else if (colorType == 1) {
			cvtColor(input, result, COLOR_BGR2HSV);
		}
		inRange(result, Scalar(thresholds[0], thresholds[2], thresholds[4]), Scalar(thresholds[1], thresholds[3], thresholds[5]), result);
	
		//morphological closing (fill small holes in the foreground)
		erode(result, result, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(result, result, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	
		return result;
	}


	Point templateMatch(Mat image, Mat target, int matchMethod, double &value) {
		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;
		Mat result;
		int resultCols = image.cols - target.cols + 1;
		int resultRows = image.rows - target.rows + 1;
	
		result.create(resultRows, resultCols, CV_32FC1);
	
		matchTemplate(image, target, result, matchMethod);
		normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	
		//resize(result, result, Size(), 0.5, 0.5, INTER_LINEAR);
		imshow("TemplateMatch", result);

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED)
		{
			matchLoc = minLoc;
			value = minVal;
		}
		else
		{
			matchLoc = maxLoc;
			value = maxVal;
		}

		return matchLoc;
	}


	Point templateMatchBlobs(Mat image, Mat target, int thresholds[], int matchMethod) {
		/*Mat origBlob = filterByColor(image, thresholds, 0);
		//resize(origBlob, origBlob, Size(), 0.5, 0.5, INTER_LINEAR);
		imshow("origBlob", origBlob);
		Mat targetBlob = filterByColor(target, thresholds, 1);
		imshow("targetBlob", targetBlob);*/
		
		int origThresholds[] = { 160, 180, 90, 255, 60, 255 };
		Mat origBlob = filterByColor(image, origThresholds, 0);
		imshow("origBlob", origBlob);
		int templThresholds[] = { 0, 20, 90, 255, 60, 255 };
		Mat targetBlob = filterByColor(target, templThresholds, 1);
		imshow("targetBlob", targetBlob);
		
		return templateMatch(origBlob, targetBlob, matchMethod);
	}


	Point templateMatchEdges(Mat image, Mat target, int lowThresh, int highThresh, int apertureSize, int matchMethod) {
		Mat origEdge = convertToEdgeMap(image, lowThresh, highThresh, apertureSize);
		Mat targetEdge = convertToEdgeMap(target, lowThresh, highThresh, apertureSize);
		return templateMatch(origEdge, targetEdge, matchMethod);
	}
	
}
