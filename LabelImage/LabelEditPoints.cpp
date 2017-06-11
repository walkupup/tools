
#include "opencv2/opencv.hpp"
#include "Windows.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class ImageLabelPoints
{
public:
	std::string name;
	std::vector<std::vector<cv::Point2d>> points;
};

// ��ͼ��img�ϻ��㣬��ʾ��Ϣinfo
void showPoints(cv::Mat img, string info, vector<vector<cv::Point2d>> points, cv::Scalar color, cv::Scalar color1)
{
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = 0; j < points[i].size(); j++)
		{
			circle(img, points[i][j], 2, color, -1);
		}
	}
	putText(img, info, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
	return;
}

// �༭��ע��. 
void labelEditPoints(string fileName, int numPoints)
{
	vector<ImageLabelPoints> labels, outLabels;
	string imageName;
	int count;
	int x, y, w, h;
	cv::Scalar color(255, 0, 0);
	cv::Scalar color1(0, 0, 255);

	// ��ȡ���м�¼.
	std::ifstream infile(fileName);
	std::string buf;
	int n;
	while (getline(infile, buf))
	{
		ImageLabelPoints item;
		istringstream ss;
		std::vector<Point2d> pt(numPoints);
		ss.str(buf);
		ss >> item.name;
		ss >> n;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < numPoints; j++)
			{
				ss >> pt[j].x >> pt[j].y;
			}
			item.points.push_back(pt);
		}
		labels.push_back(item);
	}

	// ��ʾÿһ����¼
	for (unsigned int j = 0; j < labels.size(); j++)
	{
		ImageLabelPoints curLabel = labels[j];
		Mat img = imread(curLabel.name, 1);
		if (img.data == NULL)
			continue;
		Mat orgImg = img.clone();

		showPoints(img, curLabel.name, curLabel.points, color, color1);
		imshow("pos", img);

		// ������
		while (1)
		{
			char c = waitKey(0);
			if (c == ' ')
				break;
			else if (c == 8)	// backspace
			{
				if (curLabel.points.size() == 0)
					break;
				curLabel.points.pop_back();
				orgImg.copyTo(img);
				showPoints(img, curLabel.name, curLabel.points, color, color1);
				imshow("pos", img);
			}
			else if (c == 9) // Tab
			{
				// ��ͷ���룬ĩβɾ��
				curLabel.points.insert(curLabel.points.begin(), curLabel.points[curLabel.points.size() - 1]);
				curLabel.points.pop_back();
				showPoints(img, curLabel.name, curLabel.points, color, color1);
				imshow("pos", img);
			}
		}
		if (curLabel.points.size() > 0)
			outLabels.push_back(curLabel);
	}

	// ���
	string outFileName = fileName;
	outFileName.insert(outFileName.length() - 4, "-out");
	std::ofstream outfile(outFileName);
	for (unsigned int i = 0; i < outLabels.size(); i++)
	{
		for (unsigned int j = 0; j < outLabels[i].points.size(); j++)
			outfile << outLabels[i].name << " " << 1 
			<< " " << outLabels[i].points[j][0].x
			<< " " << outLabels[i].points[j][0].y << endl;
	}
}
