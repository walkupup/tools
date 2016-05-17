#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	int width = 720;
	int height = 576;
	int frameNum = 0;
	VideoCapture cap(0); // open the default camera
	Mat frame;
	std::string videoName = "d:\\1.avi";
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 720);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 576);
	printf("width = %d, height = %d\n", (int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("framerate = %f, format = %f\n", cap.get(CV_CAP_PROP_FPS), cap.get(CV_CAP_PROP_FORMAT));

	cap >> frame; // get a new frame from camera
	VideoWriter save(videoName, -1, 15, Size(frame.cols, frame.rows));
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	namedWindow("capture", 1);
	printf("start recording to %s, press c to stop\n", videoName.c_str());

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		save << frame;
		imshow("capture", frame);
		printf("frame %d\n", frameNum++);
		if (waitKey(1) == 'c')
			break;
	}

	save.release();
	return 0;
}