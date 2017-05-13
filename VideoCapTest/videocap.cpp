#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	int width = 640;
	int height = 480;
	int fps = 5;
	int frameNum = 0;
	VideoCapture cap(0); // open the default camera"E:\\data\\recored-hd\\32west1-1.mp4"
	Mat frame;
	std::string videoName = "d:\\1.avi";
	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	printf("width = %d, height = %d\n", (int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("framerate = %f, format = %f\n", cap.get(CV_CAP_PROP_FPS), cap.get(CV_CAP_PROP_FORMAT));

	cap >> frame; // get a new frame from camera
	VideoWriter save(videoName, -1, 15, Size(frame.cols, frame.rows));
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	//namedWindow("capture", 1);
	printf("start recording to %s, press c to stop\n", videoName.c_str());

	for (int i = 0; i < 10000; i++)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("error\n");
			continue;
		}
		save << frame;
		imshow("capture", frame);
		printf("frame %d\n", frameNum++);
		if (waitKey(1) == 'c')
			break;
	}

	save.release();
	return 0;
}