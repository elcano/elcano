/* detect.cc
 * 
 * Author: Aaron Conrad
 * Last modified: Sept. 16, 2016
 */

#include "detect.hh"

namespace elcano
{
	cv::Mat convertToEdgeMap(cv::Mat input, int lowThresh, int highThresh, int apertureSize) {
		cv::Mat result;
		cv::cvtColor(input, result, CV_BGR2GRAY);
		//Reduce noise with a 3x3 kernel
		cv::blur(result, result, cv::Size(3, 3));
		//Detect edges
		cv::Canny(result, result, lowThresh, highThresh, apertureSize);
		return result;
	}
	
	/* colorType 0 is RGB; colorType 1 is BGR */
	cv::Mat filterByColor(cv::Mat input, int thresholds[], int colorType) {
		cv::Mat result;
		
		if (colorType == 0) {
			cv::cvtColor(input, result, cv::COLOR_RGB2HSV);
		}
		else if (colorType == 1) {
			cv::cvtColor(input, result, cv::COLOR_BGR2HSV);
		}
		cv::inRange(result, cv::Scalar(thresholds[0], thresholds[2], thresholds[4]), cv::Scalar(thresholds[1], thresholds[3], thresholds[5]), result);
	
		//morphological closing (fill small holes in the foreground)
		cv::erode(result, result, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
		cv::dilate(result, result, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	
		return result;
	}


	cv::Point templateMatch(cv::Mat image, cv::Mat target, int matchMethod, double &value) {
		double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
		cv::Point matchLoc;
		cv::Mat result;
		int resultCols = image.cols - target.cols + 1;
		int resultRows = image.rows - target.rows + 1;
	
		result.create(resultRows, resultCols, CV_32FC1);
	
		cv::matchTemplate(image, target, result, matchMethod);
		//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	
		//cv::imshow("TemplateMatch", result);

		/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (matchMethod == cv::TM_SQDIFF || matchMethod == cv::TM_SQDIFF_NORMED) {
			matchLoc = minLoc;
			value = minVal;
		}
		else {
			matchLoc = maxLoc;
			value = maxVal;
		}

		return matchLoc;
	}
}
