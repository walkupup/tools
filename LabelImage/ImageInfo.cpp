
#include "opencv2/opencv.hpp"
#include "ImageInfo.h"
#include "Windows.h"
using namespace std;
using namespace cv;

ImageInfo::ImageInfo(int pointNum, int objectName, std::string resultName)
{
	numPoints = pointNum;
	objName = objectName;
	lastFlag = 1;

	img.copyTo(backupImg);
	posBuf.resize(MAX_OBJ_NUM);
	for (int i = 0; i < posBuf.size(); i++)
	{
		posBuf[i].resize(numPoints);
	}
	for (int i = 0; i < posBuf.size(); i++)
	{
		for (int j = 0; j < posBuf[i].size(); j++)
		{
			posBuf[i][j].x = 0;
			posBuf[i][j].y = 0;
		}
	}
	if ((fpos = fopen("pos.txt", "wt")) == NULL)
		printf("create txt failed\n");
	fclose(fpos);
	if ((fneg = fopen("neg.txt", "wt")) == NULL)
		printf("create txt failed\n");
	fclose(fneg);

	colors.resize(10);
	colors[0] = cv::Scalar(255, 0, 0);
	colors[1] = cv::Scalar(0, 255, 0);
	colors[2] = cv::Scalar(0, 0, 255);
	colors[3] = cv::Scalar(255, 255, 0);
	colors[4] = cv::Scalar(0, 255, 255);
	colors[5] = cv::Scalar(255, 0, 255);
	colors[6] = cv::Scalar(128, 255, 0);
	colors[7] = cv::Scalar(0, 128, 255);
	colors[8] = cv::Scalar(255, 0, 128);
	colors[9] = cv::Scalar(0, 255, 128);
	radius = 2;
}
ImageInfo::~ImageInfo()
{
}

int ImageInfo::newImage(std::string name)
{
	img = imread(name, CV_LOAD_IMAGE_COLOR);
	if (img.empty())
		return -1;
	imageFileName = name;
	img.copyTo(backupImg);
	return 1;
}

vector<Rect> ImageInfo::vehiclePoint2Rect(vector<Point2d> pts)
{
	// 上下左右远顶.
	vector<Rect> vr;
	Point pt1, pt2;
	pt1.x = pts[4].x;
	pt1.y = pts[5].y;
	pt2.y = pts[1].y;
	if (pts[4].x < (pts[2].x + pts[3].x) / 2) // 偏左
		pt2.x = pts[3].x;
	else
		pt2.x = pts[2].x;
	vr.push_back(Rect(pt1, pt2));

	pt1.x = pts[2].x;
	pt1.y = pts[0].y;
	pt2.x = pts[3].x;
	pt2.y = pts[1].y;
	vr.push_back(Rect(pt1, pt2));
	return vr;
}


// 用于外部显示的图像.
Mat ImageInfo::getShowImage()
{
	backupImg.copyTo(img);
	for (int i = 0; i < posBuf.size(); i++)
	{
		if (i > count_)
			break;
		for (int j = 0; j < posBuf[i].size(); j++)
			if (posBuf[i][j].x > 0 || posBuf[i][j].y > 0)
				circle(img, posBuf[i][j], radius, colors[i % colors.size()], -1);
	}
	if (objName == VEHICLE_OBJ) 
	{
		for (int i = 0; i < count_; i++)
		{
			vector<Rect> vr = vehiclePoint2Rect(posBuf[i]);
			rectangle(img, vr[1], Scalar(255, 0, 0), 1);
			rectangle(img, vr[0], Scalar(0, 0, 255), 1);
		}
	}
	std::ostringstream info;
	info << num << "," << count_ << "," << imageFileName;
	putText(img, info.str(), Point(1, 8), FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(255, 0, 0));
	return img;
}

void ImageInfo::setOnePoint(Point2d pt)
{
	posBuf[count_][num] = pt;
	num++;
	if (num > numPoints - 1)
	{
		num = 0;
		count_++;
	}
}

void ImageInfo::removeOnePoint()
{
	if (num > 0) 
	{
		for (int i = 0; i < posBuf[count_].size(); i++)
		{
			posBuf[count_][i].x = 0;
			posBuf[count_][i].y = 0;
		}
		num = 0;
	}
	else if (count_ > 0)
	{
		for (int i = 0; i < posBuf[count_ - 1].size(); i++)
		{
			posBuf[count_ - 1][i].x = 0;
			posBuf[count_ - 1][i].y = 0;
		}
		count_--;
	}
	else
	{
		//if (negBuf.size() > 0)
		//	negBuf.pop_back();
	}
}

void ImageInfo::savePoints()
{
	if ((fpos = fopen("pos.txt", "at")) == NULL)
	{
		printf("create txt failed\n");
		return;
	}
	if ((fneg = fopen("neg.txt", "at")) == NULL)
	{
		printf("create txt failed\n");
		return;
	}
	int n = 0;
	//for (int i = 0; i < posBuf.size(); i++)
	//{
	//	if (posBuf[i][0].x != 0 && posBuf[i][0].y != 0)
	//		n++;
	//	else
	//		break;
	//}
	n = count_;
	if (objName == VEHICLE_OBJ)
	{
		if (n > 0)
			fprintf(fpos, "%s %d", imageFileName.c_str(), n * 2);
		for (int i = 0; i < n; i++)
		{
			vector<Rect> vr = vehiclePoint2Rect(posBuf[i]);
			fprintf(fpos, " %d %d %d %d", (int)vr[0].x, (int)vr[0].y, (int)vr[0].width, (int)vr[0].height);
			fprintf(fpos, " %d %d %d %d", (int)vr[1].x, (int)vr[1].y, (int)vr[1].width, (int)vr[1].height);
		}
	}
	else
	{
		if (n > 0)
			fprintf(fpos, "%s %d", imageFileName.c_str(), n);
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < numPoints; j++)
			{
				fprintf(fpos, " %d %d", (int)posBuf[i][j].x, (int)posBuf[i][j].y);
			}
		}
	}
	if (n > 0)
		fprintf(fpos, "\n");
	fclose(fpos);
	fclose(fneg);
	count_ = 0;
	num = 0;
	for (int i = 0; i < posBuf.size(); i++)
	{
		for (int j = 0; j < posBuf[i].size(); j++)
		{
			posBuf[i][j].x = 0;
			posBuf[i][j].y = 0;
		}
	}
	return;
}

void ImageInfo::setShowPointSize(int r)
{
	if (r < 0 || r > 2)
		r = 2;
	radius = r;
}

void ImageInfo::setShowScale()
{
}

