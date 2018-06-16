#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include "opencv2/opencv.hpp"

using namespace cv;

std::mutex mu;

void capFunc(VideoCapture cap, std::queue<cv::Mat> &frames)
{
	cv::Mat frame;
	while (1)
	{
		cap >> frame;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (frame.empty())
			continue;
		mu.lock();
		frames.push(frame);
		//std::cout << "push " << frames.size() << std::endl;
		mu.unlock();
	}
}
void saveFunc(std::queue<cv::Mat> &frames)
{
	char name[100];
	int frameNum = 0;
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (frames.empty())
			continue;
		mu.lock();
		cv::Mat frame = frames.front();
		frames.pop();
		std::cout << "frame " << frameNum << ", pop " << frames.size() << std::endl;
		mu.unlock();
		sprintf(name, "frame%d.jpg", frameNum++);
		imwrite(name, frame);
	}
}
int main(int, char**)
{
	int width = 720;
	int height = 576;
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
	std::queue<cv::Mat> frames;

	std::thread t1(capFunc, cap, std::ref(frames));
	std::thread t2(saveFunc, std::ref(frames));
	//for (;;)
	//{
	//	cap >> frame; // get a new frame from camera
	//	imshow("capture", frame);
	//	char name[100];
	//	sprintf(name, "frame%d.jpg", frameNum++);
	//	if (waitKey(1) == ' ')
	//	{
	//		imwrite(name, frame);
	//	}
	//}

	t1.join();
	t2.join();

	return 0;
}
