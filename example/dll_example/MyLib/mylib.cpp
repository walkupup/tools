#include "opencv2/opencv.hpp"

__declspec(dllexport) int mymul(int a, int b);
__declspec(dllexport) int myadd(int a, int b);
using namespace cv;
int mymul(int a, int b)
{
	Mat img = imread("f:\\011.jpg");
	imshow("img", img);
	waitKey();
	return a * b;
}

int myadd(int a, int b)
{
	return a + b;
}

