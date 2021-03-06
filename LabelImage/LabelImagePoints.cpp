// LabelImage.cpp : 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS
#include "tchar.h"
#include "opencv2/opencv.hpp"
#include "Windows.h"
#include "ImageInfo.h"

using namespace std;
using namespace cv;

static ImageInfo *ii;

void labelEditPoints(string fileName, int numPoints);
void labelEditVessel(string fileName);


// 记录多个Label
void savePoints(FILE *fp, char *name, vector<vector<Point2d>> rec)
{
	int n = 0;
	for (int i = 0; i < rec.size(); i++)
	{
		if (rec[i][0].x != 0 && rec[i][0].y != 0)
			n++;
		else
			break;
	}
	fprintf(fp, "%s %d", name, n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < rec[i].size(); j++)
		{
			fprintf(fp, " %d %d", (int)rec[i][j].x, (int)rec[i][j].y);
		}
	}
	fprintf(fp, "\n");
	return;
}

static void saveLabelImage(char *name, vector<Rect>& rec)
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
		sprintf(negName, "neg/neg%08d.jpg", num);
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
		break;
	case CV_EVENT_RBUTTONDOWN:
		break;
	case CV_EVENT_LBUTTONUP:
		ii->setOnePoint(Point2d(x, y));
		imshow("image", ii->getShowImage());
		break;
	case CV_EVENT_RBUTTONUP:
		break;
	case CV_EVENT_MOUSEMOVE:
		break;
	}

}

static void showLabels(FILE *fp, CvScalar& color, int save, int ms)
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
	HANDLE hFind = NULL;
	FILE *fpos, *fneg;
	TCHAR name[100];
	char a, b;
	int startNum = 0, curNum;
	char fileName[200], buf[10];
	char preName[20], midName[20], extName[10]; // 前缀名，中间名（编号），扩展名
	int padZero = 0;	// 文件名如果需补零，扩展为多少个字符
	int i;
	int numPoints;

	printf("\n标注工具。一幅图可标注多个目标。\n\
说明：按Enter跳过当前图像，按空格保存当前结果并标注下一张图，按Backspace删除上一个标注。\
按1显示小点，按2显示大点\n\n");

	printf("请输入选项：\n\
按帧号标注 s:起始帧号, 任意顺序标注x, 查看正样本p, 查看负样本n ");
	scanf("%s", buf);
	fflush(stdin);
	printf("每个目标几个点：");
	scanf("%d", &numPoints);

	if (buf[0] == 's')
	{
		sscanf(buf, "%c%c%d", &a, &b, &startNum);
	}
	else if (buf[0] == 'x')
	{

	}
	else if (buf[0] == 'p')
	{
		//FILE *fp = fopen("pos.txt", "rt");
		//showLabels(fp, CV_RGB(255, 0, 0), 0, 0);
		//fclose(fp);
		labelEditPoints("pos.txt", 5);
		//labelEditVessel("label.txt");
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
		hFind = FindFirstFile(_T("*.*"), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			//hFind = FindFirstFile(_T("*.jpg"), &FindFileData);
			//if (hFind == INVALID_HANDLE_VALUE)
			//{
			//	printf("没有找到png或jpg文件 (%d)\n", GetLastError());
			//	return -2;
			//}
			//else
			//	printf("找到 jpg 文件! \n");
		}
		//else
		//	printf("找到 png 文件! \n");
	}

	namedWindow("image");// 0);
	setMouseCallback("image", onMouse, 0);
	curNum = startNum;

	ii = new ImageInfo(numPoints, RECT_OBJ, "pos.txt");

	while (1)
	{
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
		else if (buf[0] == 'x')
		{
			WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);
			if ((strlen(fileName) < 5) ||
				(strcmp(&fileName[strlen(fileName) - 3], "jpg") != 0 &&
				strcmp(&fileName[strlen(fileName) - 3], "JPG") != 0 &&
				strcmp(&fileName[strlen(fileName) - 3], "png") != 0 &&
				strcmp(&fileName[strlen(fileName) - 3], "PNG") != 0 && 
				strcmp(&fileName[strlen(fileName) - 3], "bmp") != 0 && 
				strcmp(&fileName[strlen(fileName) - 3], "BMP") != 0 ))
			{
				printf("%s skipped\n", fileName);
				if (!FindNextFile(hFind, &FindFileData))
					break;
				continue;
			}
		}

		printf("%s\n", fileName);
		if (ii->newImage(fileName) < 0)
		{
			printf("文件 %s 错误, 或标注已完成\n", fileName);
			continue;
		}
		cv::Mat showImg = ii->getShowImage();
		//cv::resizeWindow("image", showImg.cols, showImg.rows);
		//cv::setWindowProperty("image", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
		imshow("image", showImg);
		int curWinX = 0;
		int curWinY = 0;
		while (1)
		{
			char c = cvWaitKey(0);
			if (c == ' ')	// 空格
			{
				ii->savePoints();
				break;
			}
			else if (c == 8)	// press backspace
			{
				ii->removeOnePoint();
				imshow("image", ii->getShowImage());
			}
			else if (c == 13) // press enter
			{
				//{
				//	Rect rec(0, 0, img.cols, img.rows);
				//	negBuf.clear();
				//	negBuf.push_back(rec);
				//}
				//savePoints(fneg, fileName, negBuf);
				break;
			}
			else if (c == 'a')
			{
				curWinX -= 20;
				cv::moveWindow("image", curWinX, curWinY);
			}
			else if (c == 's')
			{
				curWinY += 20;
				cv::moveWindow("image", curWinX, curWinY);
			}
			else if (c == 'd')
			{
				curWinX += 20;
				cv::moveWindow("image", curWinX, curWinY);
			}
			else if (c == 'w')
			{
				curWinY -= 20;
				cv::moveWindow("image", curWinX, curWinY);
			}

			else if (c == '1')
			{
				ii->setShowPointSize(1);
			}
			else if (c == '2')
			{
				ii->setShowPointSize(2);
			}
		}
		curNum++;
		if (buf[0] == 'x')
		{
			if (!FindNextFile(hFind, &FindFileData))
				break;
		}
	}

	delete ii;
	return 0;
}

