#pragma once

#include <opencv2/objdetect/objdetect.hpp>

/* Helper function for processing camera input */

namespace elcano
{
	void detect_and_draw(cv::Mat&, cv::CascadeClassifier&, double);
}
