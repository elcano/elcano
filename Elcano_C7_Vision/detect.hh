#pragma once

/* detect.hh
 * 
 * Author: Aaron Conrad
 * Last modified: Sept. 16, 2016
 * 
 * Image processing helper functions for detecting objects.
 * 
 * Includes high level function calls for image processing
 * methods in OpenCV.
 */

namespace elcano
{
	cv::Mat convertToEdgeMap(cv::Mat input, int lowThresh, int highThresh, int apertureSize);
	cv::Mat filterByColor(cv::Mat input, int thresholds[], int colorType);
	cv::Point templateMatch(cv::Mat image, cv::Mat target, int matchMedthod, double &value);
}
