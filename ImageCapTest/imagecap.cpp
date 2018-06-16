#include <iostream>
#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
	int width = 640;
	int height = 480;
	int frameNum = 0;
	VideoCapture cap(0); // open the default camera
	Mat frame;
	//cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	//cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	printf("width = %d, height = %d\n", (int)cap.get(CV_CAP_PROP_FRAME_WIDTH), (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	printf("framerate = %f, format = %f\n", cap.get(CV_CAP_PROP_FPS), cap.get(CV_CAP_PROP_FORMAT));

	cap >> frame; // get a new frame from camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	namedWindow("capture", 1);

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		imshow("capture", frame);
		char name[100];
		sprintf(name, "frame%d.jpg", frameNum++);
		if (waitKey(20) != ' ')
		{
			imwrite(name, frame);
		}
	}

	return 0;
}
