#pragma once

/* Helper function for processing camera input */

namespace elcano
{
	void detect_and_draw(cv::Mat&, cv::CascadeClassifier&, double);
}
