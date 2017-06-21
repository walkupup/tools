
#include "opencv2/opencv.hpp"
#include "Windows.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

class ImageLabel
{
public:
	std::string name;
	std::vector<cv::Rect> rois;
};

// ��ͼ��img�ϻ�rois���Σ���ʾ��Ϣinfo
void showRois(cv::Mat img, string info, vector<cv::Rect> rois, cv::Scalar color, cv::Scalar color1)
{
	for (unsigned int i = 0; i < rois.size(); i++)
		rectangle(img, rois[i], color, 1, 8, 0);
	if (rois.size() > 0)// ���һ������ɫ����
		rectangle(img, rois[rois.size() - 1], color1, 1, 8, 0);
	putText(img, info, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
	return;
}

vector<ImageLabel> readRectLable(std::string fileName)
{
	// ��ȡ���м�¼.
	vector<ImageLabel> labels;
	std::ifstream infile(fileName);
	std::string buf;
	int n;
	while (getline(infile, buf))
	{
		ImageLabel item;
		Rect rec;
		istringstream ss;
		ss.str(buf);
		ss >> item.name;
		ss >> n;
		for (int i = 0; i < n; i++)
		{
			ss >> rec.x >> rec.y >> rec.width >> rec.height;
			item.rois.push_back(rec);
		}
		labels.push_back(item);
	}
	return labels;
}

void labelEidt(string fileName)
{
	vector<ImageLabel> labels, outLabels;
	string imageName;
	int count;
	int x, y, w, h;
	cv::Scalar color(255, 0, 0);
	cv::Scalar color1(0, 0, 255);

	// ��ȡ���м�¼
	std::ifstream infile(fileName);
	ImageLabel item;
	while (infile >> imageName >> count >> x >> y >> w >> h) 
	{
		if (item.rois.size() == 0)
		{
			item.name = imageName;
			item.rois.push_back(Rect(x, y, w, h));
		}
		else if (item.rois.size() > 0)
		{
			if (item.name.compare(imageName) == 0) // �����ͬһ��ͼ
			{
				item.rois.push_back(Rect(x, y, w, h));
			}
			else
			{
				labels.push_back(item);
				item.rois.clear();
				item.name = imageName;
				item.rois.push_back(Rect(x, y, w, h));
			}
		}
	}
	if (item.rois.size() > 0) // ���һ����¼
		labels.push_back(item);

	// ��ʾÿһ����¼
	for (unsigned int j = 0; j < labels.size(); j++)
	{
		ImageLabel curLabel = labels[j];
		Mat img = imread(curLabel.name, 1);
		if (img.data == NULL)
			continue;
		Mat orgImg = img.clone();

		showRois(img, curLabel.name, curLabel.rois, color, color1);
		imshow("pos", img);

		// ������
		while (1)
		{
			char c = waitKey(0);
			if (c == ' ')
				break;
			else if (c == 8)	// backspace
			{
				if (curLabel.rois.size() == 0)
					break;
				curLabel.rois.pop_back();
				orgImg.copyTo(img);
				showRois(img, curLabel.name, curLabel.rois, color, color1);
				imshow("pos", img);
			}
			else if (c == 9) // Tab
			{
				// ��ͷ���룬ĩβɾ��
				curLabel.rois.insert(curLabel.rois.begin(), curLabel.rois[curLabel.rois.size() - 1]);
				curLabel.rois.pop_back();
				showRois(img, curLabel.name, curLabel.rois, color, color1);
				imshow("pos", img);
			}
		}
		if (curLabel.rois.size() > 0)
			outLabels.push_back(curLabel);
	}

	// ���
	string outFileName = fileName;
	outFileName.insert(outFileName.length() - 4, "-out");
	std::ofstream outfile(outFileName);
	for (unsigned int i = 0; i < outLabels.size(); i++)
	{
		for (unsigned int j = 0; j < outLabels[i].rois.size(); j++)
			outfile << outLabels[i].name << " " << 1 
			<< " " << outLabels[i].rois[j].x
			<< " " << outLabels[i].rois[j].y
			<< " " << outLabels[i].rois[j].width
			<< " " << outLabels[i].rois[j].height << endl;
	}
}
