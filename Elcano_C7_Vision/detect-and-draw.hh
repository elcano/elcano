#pragma once

#include <opencv2/opencv.hpp>

namespace elcano
{
	void detect_and_draw(cv::Mat&, cv::CascadeClassifier&, double);
}
