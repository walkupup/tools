#include "opencv2/opencv.hpp"
using namespace cv;

int main(int, char**)
{
	VideoCapture cap(0); // open the default camera
	Mat frame;
	cap >> frame; // get a new frame from camera
	VideoWriter save("2.avi", -1, 5, Size(frame.cols, frame.rows));
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	namedWindow("capture", 1);
	for (;;)
	{
		cap >> frame; // get a new frame from camera
		save << frame;
		imshow("capture", frame);
		waitKey(1);
	}
	return 0;
}