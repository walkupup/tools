// LabelImage.cpp : 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS
#include "tchar.h"
#include "opencv2/opencv.hpp"
#include "Windows.h"

using namespace std;
using namespace cv;

Mat img, backupImg;
Point startPt, endPt;
static int labeling = 0;
vector<Rect> posBuf;
vector<Rect> negBuf;
int lastFlag = 1;	// 1为positive, 0位negtive

// 恢复原图和现有的矩形框
void restortImage(Mat& img, vector<Rect>& rec1, vector<Rect>& rec2)
{
	int i;
	backupImg.copyTo(img);
	for (i = 0; i < rec1.size(); i++)
		rectangle(img, rec1[i], CV_RGB(255, 0, 0), 1, 8, 0);
	for (i = 0; i < rec2.size(); i++)
		rectangle(img, rec2[i], CV_RGB(0, 255, 0), 1, 8, 0);

	return;
}

// 记录一个Label
void recordOneLabel(FILE *fp, char *name, Point pt1, Point pt2)
{
	int left, top, width, height;
	left = (pt1.x < pt2.x ? pt1.x : pt2.x);
	top = (pt1.y < pt2.y ? pt1.y : pt2.y);
	width = abs(pt1.x - pt2.x) + 1;
	height = abs(pt1.y - pt2.y) + 1;
	fprintf(fp, "%s %d %d %d %d %d\n", name, 1, left, top, width, height);
	return;
}

// 记录多个Label
void recordLabels(FILE *fp, char *name, vector<Rect> rec)
{
	int i;
	for (i = 0; i < rec.size(); i++)
		fprintf(fp, "%s %d %d %d %d %d\n", name, 1, rec[i].x, rec[i].y, rec[i].width, rec[i].height);
	return;
}

void saveLabelImage(char *name, vector<Rect>& rec)
{
	int i;
	static int num = 0;
	Mat img = imread(name, 1);
	char negName[100];

	if (img.data == NULL)
		return;

	for (i = 0; i < rec.size(); i++)
	{
		if (rec[i].x + rec[i].width > img.cols || rec[i].x < 0 ||
			rec[i].y + rec[i].height > img.rows || rec[i].y < 0)
			continue;
		sprintf(negName, "neg/neg%08d.png", num);
		imwrite(negName, img(rec[i]));
		num++;
	}
	return;
}

// 鼠标回调函数
static void onMouse(int event, int x, int y, int, void*)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		labeling = 1;
		startPt.x = x;
		startPt.y = y;
		break;
	case CV_EVENT_RBUTTONDOWN:
		labeling = 2;
		startPt.x = x;
		startPt.y = y;
		break;
	case CV_EVENT_LBUTTONUP:
		labeling = 0;
		endPt.x = x;
		endPt.y = y;
		if (abs(startPt.x - endPt.x) < 5 || abs(startPt.y - endPt.y) < 5)
			break;
		{
			Rect rec(startPt, endPt);
			posBuf.push_back(rec);
			lastFlag = 1;
		}
		restortImage(img, posBuf, negBuf);
		break;
	case CV_EVENT_RBUTTONUP:
		labeling = 0;
		endPt.x = x;
		endPt.y = y;
		if (abs(startPt.x - endPt.x) < 30 || abs(startPt.y - endPt.y) < 30)
			break;
		{
			Rect rec(startPt, endPt);
			negBuf.push_back(rec);
			lastFlag = 0;
		}
		restortImage(img, posBuf, negBuf);
		break;
	case CV_EVENT_MOUSEMOVE:
		if (labeling)
		{
			restortImage(img, posBuf, negBuf);
			endPt.x = x;
			endPt.y = y;
			if (labeling == 1)
				rectangle(img, startPt, endPt, CV_RGB(255, 0, 0), 1, 8, 0);
			else
				rectangle(img, startPt, endPt, CV_RGB(0, 255, 0), 1, 8, 0);
			imshow("image", img);
		}
		break;
	}

}

void showLabels(FILE *fp, CvScalar& color, int save, int ms)
{
	char buf[100], name[100], name1[100];
	int a;
	vector<Rect> recBuf;
	Rect rec;
	if (!fp)
		return;

	char *ready = fgets(buf, 100, fp);
	while (ready)
	{
		sscanf(buf, "%s%d%d%d%d%d", &name, &a, &rec.x, &rec.y, &rec.width, &rec.height);
		recBuf.clear();
		recBuf.push_back(rec);
		while ((ready = fgets(buf, 100, fp)) != NULL)
		{
			sscanf(buf, "%s%d%d%d%d%d", &name1, &a, &rec.x, &rec.y, &rec.width, &rec.height);
			if (strcmp(name, name1) == 0)
			{
				recBuf.push_back(rec);
			}
			else
				break;
		}

		if (save)
			saveLabelImage(name, recBuf);

		Mat img = imread(name, 1);
		if (img.data == NULL)
			continue;
		for (int i = 0; i < recBuf.size(); i++)
			rectangle(img, recBuf[i], color, 1, 8, 0);
		putText(img, name, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
		imshow("pos", img);
		char c = waitKey(ms);
		if (c == ' ')
			continue;	
	}
}

int main(int argc, TCHAR* argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	FILE *fpos, *fneg;
	TCHAR name[100];
	char a, b;
	int startNum = 0, curNum;
	char fileName[200], buf[10];
	char preName[20], midName[20], extName[10]; // 前缀名，中间名（编号），扩展名
	int padZero = 0;	// 文件名如果需补零，扩展为多少个字符
	int i;

	printf("图像标注工具，请输入选项：\n\
按帧号标注 s:起始帧号, 任意顺序标注x, 查看正样本p, 查看负样本n ");
	scanf("%s", buf);

	if (buf[0] == 's')
	{
		sscanf(buf, "%c%c%d", &a, &b, &startNum);
	}
	else if (buf[0] == 'x')
	{

	}
	else if (buf[0] == 'p')
	{
		FILE *fp = fopen("pos.txt", "rt");
		showLabels(fp, CV_RGB(255, 0, 0), 0, 0);
		fclose(fp);
		return 0;
	}
	else if (buf[0] == 'n')
	{
		system("md neg");
		FILE *fp = fopen("neg.txt", "rt");
		showLabels(fp, CV_RGB(0, 255, 0), 1, 20);
		fclose(fp);
		return 0;
	}
	else
		return 0;

	printf("\n标注工具。鼠标左键拖动标注正样本，右键拖动标注负样本，一幅图可标注多个目标。\n \
快捷键：按Enter标注整张图为负样本，按空格标注下一张图，按Backspace删除上一个标注。\n");

	if (buf[0] == 's') // 按帧号标注
	{
	swprintf(name, 100, _T("*%d.*"), startNum);
	hFind = FindFirstFile(name, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("can't find frame %d\n", startNum);
		return 0;
	}
	do {
		char aa;
		WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);
		printf("从 %s 开始? y / n: ", fileName);
		scanf("%s", &aa);
		if (aa == 'y' || aa == 'Y')
			break;
	} while (FindNextFile(hFind, &FindFileData));

	WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);

	printf("开始标注 %s\n", fileName);

	int len = strlen(fileName);

	// 判断是否为正确图像扩展名
	if (strcmp(&fileName[len - 3], "png") == 0)
		strcpy(extName, ".png");
	else if (strcmp(&fileName[len - 3], "jpg") == 0)
		strcpy(extName, ".jpg");
	else
		return 0;

	char startNumStr[10];
	sprintf(startNumStr, "%d", startNum);
	char *p = strstr(fileName, startNumStr); 
	while (p > fileName)
	{
		if (p[-1] == '0')
		{
			padZero++;
			p--;
		}
		else
			break;
	}
	memcpy(preName, fileName, p - fileName);
	preName[p - fileName] = '\0';
	padZero += strlen(startNumStr);
	}
	else if (buf[0] == 'x') // 任意顺序标注
	{
		// 查找png 或 jpg 文件
		hFind = FindFirstFile(_T("*.png"), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			hFind = FindFirstFile(_T("*.jpg"), &FindFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				printf("没有找到png或jpg文件 (%d)\n", GetLastError());
				return -2;
			}
			else
				printf("找到 jpg 文件! \n");
		}
		else
			printf("找到 png 文件! \n");
	}

	namedWindow("image", 0);
	setMouseCallback("image", onMouse, 0);
	fopen_s(&fpos, "pos.txt", "wt");
	fclose(fpos);
	fopen_s(&fneg, "neg.txt", "wt");
	fclose(fneg);
	curNum = startNum;

	while (1)
	{
		fopen_s(&fpos, "pos.txt", "a+");
		fopen_s(&fneg, "neg.txt", "a+");

		if (buf[0] == 's')
		{
			sprintf(midName, "%d", curNum);
			if (padZero)
			{
				sprintf(midName, "%d", curNum);
				int pad = padZero - strlen(midName);
				for (i = 0; i < pad; i++)
					midName[i] = '0';
				sprintf(&midName[pad], "%d", curNum);
			}
			strcpy(fileName, preName);
			strcat(fileName, midName);
			strcat(fileName, extName);
		}
		else
		{
			WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);
			if (!FindNextFile(hFind, &FindFileData))
				break;
		}

		printf("%s\n", fileName);
		img = imread(fileName, CV_LOAD_IMAGE_COLOR);
		if (img.data == NULL)
		{
			printf("文件 %s 丢失, 或标注完成\n", fileName);
			break;
		}
		putText(img, fileName, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
		img.copyTo(backupImg);
		posBuf.clear();
		negBuf.clear();
		imshow("image", img);
		while (1)
		{
			char c = cvWaitKey(0);
			if (c == ' ')	// 空格
			{
				recordLabels(fpos, fileName, posBuf);
				recordLabels(fneg, fileName, negBuf);
				break;
			}
			else if (c == 8)	// press backspace
			{
				if (lastFlag)
				{
					if (posBuf.size() > 0)
						posBuf.pop_back();
				}
				else
				{
					if (negBuf.size() > 0)
						negBuf.pop_back();
				}
				restortImage(img, posBuf, negBuf);
				imshow("image", img);
			}
			else if (c == 13) // press enter
			{
				{
					Rect rec(0, 0, img.cols, img.rows);
					negBuf.clear();
					negBuf.push_back(rec);
				}
				recordLabels(fneg, fileName, negBuf);
				break;
			}
			//else
			//	printf("%d", c);
		}
		fclose(fpos);
		fclose(fneg);
		curNum++;

	} //while (FindNextFile(hFind, &FindFileData));

	return 0;
}

