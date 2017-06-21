
#include "opencv2/opencv.hpp"
#include "Windows.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class ImageLabelVessel
{
public:
	std::string name;
	std::vector<cv::Point2d> points;
};


vector<ImageLabelVessel> readVesselLable(std::string fileName)
{
	// ��ȡ���м�¼.
	vector<ImageLabelVessel> labels;
	std::ifstream infile(fileName);
	std::string buf;
	std::string pos, comma;
	while (getline(infile, buf))
	{
		ImageLabelVessel item;
		cv::Point2d range;
		istringstream ss;
		ss.str(buf);
		ss >> item.name;
		ss >> pos;
		item.name.erase(item.name.length() - 1, item.name.length());
		while (ss >> range.x >> range.y)
		{
			item.points.push_back(range);
			if (!(ss >> comma))
				break;
		}
		labels.push_back(item);
	}
	return labels;
}

// ��ͼ��img�ϻ��㣬��ʾ��Ϣinfo
void showVessel(cv::Mat img, string info, vector<cv::Point2d> points, cv::Scalar color, cv::Scalar color1)
{
	for (int i = 0; i < points.size(); i++)
	{
		Point pt1, pt2;
		pt1.x = points[i].x;
		pt1.y = 0;
		pt2.x = points[i].y;
		pt2.y = img.rows - 1;
		rectangle(img, Rect(pt1, pt2), Scalar(0, 0, 255), 2);
	}
	putText(img, info, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
	return;
}

// �༭��ע��. 
void labelEditVessel(string fileName)
{
	vector<ImageLabelVessel> labels, outLabels;
	string imageName;
	int count;
	int x, y, w, h;
	cv::Scalar color(255, 0, 0);
	cv::Scalar color1(0, 0, 255);

	// ��ȡ���м�¼.
	labels = readVesselLable(fileName);

	// ��ʾÿһ����¼
	for (unsigned int j = 0; j < labels.size(); j++)
	{
		ImageLabelVessel curLabel = labels[j];
		Mat img = imread(curLabel.name, CV_LOAD_IMAGE_COLOR);
		if (img.data == NULL)
			continue;
		Mat orgImg = img.clone();

		showVessel(img, curLabel.name, curLabel.points, color, color1);
		imshow("pos", img);

		// ������
		while (1)
		{
			char c = waitKey(0);
			if (c == ' ')
				break;
/*			else if (c == 8)	// backspace
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
			*/
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
		//for (unsigned int j = 0; j < outLabels[i].points.size(); j++)
		//	outfile << outLabels[i].name << " " << 1
		//	<< " " << outLabels[i].points[j][0].x
		//	<< " " << outLabels[i].points[j][0].y << endl;
	}
}
