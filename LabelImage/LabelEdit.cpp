
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

extern vector<Rect> posBuf;
extern cv::Mat gImg;

// 鼠标回调函数，矩形框
void onMouse(int event, int x, int y, int, void*);

// 在图像img上画rois矩形，显示信息info
void showRois(cv::Mat img, string info, vector<cv::Rect> rois, cv::Scalar color, cv::Scalar color1)
{
	for (unsigned int i = 0; i < rois.size(); i++)
		rectangle(img, rois[i], color, 1, 8, 0);
	if (rois.size() > 0)// 最后一个框颜色区别
		rectangle(img, rois[rois.size() - 1], color1, 1, 8, 0);
	putText(img, info, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
	return;
}

// 每行多个标记.
vector<ImageLabel> readRectLable(std::string fileName)
{
	// 读取所有记录.
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

// 每行一个标记.
vector<ImageLabel> readRectLableOneSampleOneLine(std::string fileName)
{
	// 读取所有记录
	vector<ImageLabel> labels;
	std::ifstream infile(fileName);
	ImageLabel item;
	string imageName;
	int x, y, w, h;
	while (infile >> imageName >> x >> y >> w >> h)
	{
		if (item.rois.size() == 0)
		{
			item.name = imageName;
			item.rois.push_back(Rect(x, y, w, h));
		}
		else if (item.rois.size() > 0)
		{
			if (item.name.compare(imageName) == 0) // 如果是同一张图
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
	if (item.rois.size() > 0) // 最后一个记录
		labels.push_back(item);
	return labels;
}

void labelEidt(string fileName)
{
	vector<ImageLabel> labels, outLabels;
	string imageName;
	cv::Scalar color(255, 0, 0);
	cv::Scalar color1(0, 0, 255);
	string outFileName = fileName;
	outFileName.insert(outFileName.length() - 4, "-out");

	labels = readRectLable(fileName);
	namedWindow("image");// 0);
	Mat img;
	printf("\n查看/编辑模式。\n请确保当前目录下存在pos.txt，并删除pos-out.txt，编辑后结果输出在pos-out.txt中\n");

	// 显示每一条记录
	for (unsigned int j = 0; j < labels.size(); j++)
	{
		ImageLabel curLabel = labels[j];
		cout << curLabel.name << endl;
		img = imread(curLabel.name, 1);
		if (img.data == NULL)
			continue;
		Mat orgImg = img.clone();
		img.copyTo(gImg);

		showRois(img, curLabel.name, curLabel.rois, color, color1);
		imshow("image", img);
		posBuf.clear();
		for (size_t i = 0; i < curLabel.rois.size(); i++)
		{
			posBuf.push_back(curLabel.rois[i]);
		}
		setMouseCallback("image", onMouse, (void*)&curLabel.rois);

		// 处理按键
		while (1)
		{
			char c = waitKey(0);
			if (c == ' ')
			{
				break;
			}
			else if (c == 8)	// backspace
			{
				if (curLabel.rois.size() == 0)
					break;
				curLabel.rois.pop_back();
				orgImg.copyTo(img);
				showRois(img, curLabel.name, curLabel.rois, color, color1);
				imshow("image", img);
			}
			else if (c == 9) // Tab
			{
				// 开头插入，末尾删除
				curLabel.rois.insert(curLabel.rois.begin(), curLabel.rois[curLabel.rois.size() - 1]);
				curLabel.rois.pop_back();
				showRois(img, curLabel.name, curLabel.rois, color, color1);
				imshow("image", img);
			}
		}
		if (curLabel.rois.size() > 0)
			outLabels.push_back(curLabel);
		// 输出
		size_t i = outLabels.size() - 1;
		std::ofstream outfile(outFileName, ios::app);
		
		outfile << outLabels[i].name << " " << outLabels[i].rois.size();
		for (unsigned int j = 0; j < outLabels[i].rois.size(); j++)
			outfile << " " << outLabels[i].rois[j].x 
			<< " " << outLabels[i].rois[j].y
			<< " " << outLabels[i].rois[j].width
			<< " " << outLabels[i].rois[j].height;
		outfile << endl;

		//for (unsigned int j = 0; j < outLabels[i].rois.size(); j++)
		//	outfile << outLabels[i].name << " " << 1
		//	<< " " << outLabels[i].rois[j].x
		//	<< " " << outLabels[i].rois[j].y
		//	<< " " << outLabels[i].rois[j].width
		//	<< " " << outLabels[i].rois[j].height << endl;
	}

}
