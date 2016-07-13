#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "detect.hh"

/* Helper function for processing camera input */

using namespace cv;

namespace elcano
{
	void detect_and_draw(Mat &img, CascadeClassifier &cascade, double scale)
	{
		double fx = 1 / scale;
		std::vector<Rect> detection;
		Mat smallImg;
		
		resize(img, smallImg, Size(), fx, fx, INTER_LINEAR);
		equalizeHist(smallImg, smallImg);
		cascade.detectMultiScale(smallImg, detection, 1.1, 2, CASCADE_SCALE_IMAGE, Size(30, 30));
		
		for (Rect r : detection)
			rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
				cvPoint(cvRound((r.x + r.width - 1) * scale), cvRound((r.y + r.height - 1) * scale)),
				Scalar(255, 255, 0), 3, 8, 0);
		
		imshow("result", img);
	}
}
