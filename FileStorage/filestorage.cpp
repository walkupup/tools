#include "stdio.h"

#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

int main()
{
	FileStorage fs("000001_pose.xml", FileStorage::READ);

	if (!fs.isOpened())
	{
		return 1;
	}

	Mat data;
	fs["pose_0"] >> data;

	ofstream outfile("landmark.txt");
	Mat img = imread("000001.jpg");
	std::cout << data.step[0] << " " << data.step[1] << " " << data.step[2] << std::endl;
	std::cout << data.size[0] << " " << data.size[1] << " " << data.size[2] << std::endl;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				outfile << data.at<float>(i, j, k) << " ";
			}
			circle(img, cv::Point(data.at<float>(i, j, 0), data.at<float>(i, j, 1)), 2, Scalar(255, 0, 0));
		}
	}
	imshow("image", img);
	waitKey();


	return 1;
}