// LabelImage.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _CRT_SECURE_NO_WARNINGS
#include "tchar.h"
#include "opencv2/opencv.hpp"
#include "Windows.h"

using namespace std;
using namespace cv;

#define Clip3(min,max,val) (((val)<(min))?(min):(((val)>(max))?(max):(val)))

Mat gImg;// , backupImg;
Point startPt, endPt;
int labeling = 0;
vector<Rect> posBuf;
vector<Rect> negBuf;
int lastFlag = 1;	// 1Ϊpositive, 0λnegtive
static char square = 'n';

void labelEidt(string fileName);

// �ָ�ԭͼ�����еľ��ο�
void restortImage(Mat& img, vector<Rect>& rec1, vector<Rect>& rec2)
{
	int i;
	//backupImg.copyTo(img);
	for (i = 0; i < rec1.size(); i++)
		rectangle(img, rec1[i], CV_RGB(255, 0, 0), 1, 8, 0);
	for (i = 0; i < rec2.size(); i++)
		rectangle(img, rec2[i], CV_RGB(0, 255, 0), 1, 8, 0);

	return;
}

// ��¼һ��Label
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

// ��¼���Label
void recordLabelsOne(FILE *fp, char *name, vector<Rect> rec)
{
	int i;
	for (i = 0; i < rec.size(); i++)
		fprintf(fp, "%s %d %d %d %d %d\n", name, 1, rec[i].x, rec[i].y, rec[i].width, rec[i].height);
	return;
}

// ��¼���Label
void recordLabels(FILE *fp, char *name, vector<Rect> rec, int scale)
{
	int i;
	fprintf(fp, "%s %d", name, rec.size());
	for (i = 0; i < rec.size(); i++)
		fprintf(fp, " %d %d %d %d", rec[i].x * scale, rec[i].y * scale, rec[i].width * scale, rec[i].height * scale);
	fprintf(fp, "\n");
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
		sprintf(negName, "neg/neg%08d.jpg", num);
		imwrite(negName, img(rec[i]));
		num++;
	}
	return;
}

void makeSquareBox(cv::Point startPt, cv::Point &endPt)
{
	int len1 = endPt.x - startPt.x;
	int len2 = endPt.y - startPt.y;
	if (abs(len1) > abs(len2))
	{
		endPt.x = startPt.x + len2;
		endPt.y = startPt.y + len2;
	}
	else
	{
		endPt.x = startPt.x + len1;
		endPt.y = startPt.y + len1;
	}
	return;
}

// ���ص�����
void onMouse(int event, int x, int y, int, void* __posBuf)
{
	cv::Mat *_img = &gImg;
	cv::Mat showImg;
	std::vector<cv::Rect> *_posBuf = (std::vector<cv::Rect> *)__posBuf;
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
		endPt.x = Clip3(0, _img->cols - 1, x);
		endPt.y = Clip3(0, _img->rows - 1, y);
		if (square == 'y')
			makeSquareBox(startPt, endPt);
		if (abs(startPt.x - endPt.x) < 5 || abs(startPt.y - endPt.y) < 5)
			break;
		{
			Rect rec(startPt, endPt);
			_posBuf->push_back(rec);
			lastFlag = 1;
		}
		_img->copyTo(showImg);
		restortImage(showImg, *_posBuf, negBuf);
		break;
	case CV_EVENT_RBUTTONUP:
		labeling = 0;
		endPt.x = Clip3(0, _img->cols - 1, x);
		endPt.y = Clip3(0, _img->rows - 1, y);
		if (square == 'y')
			makeSquareBox(startPt, endPt);
		if (abs(startPt.x - endPt.x) < 30 || abs(startPt.y - endPt.y) < 30)
			break;
		{
			Rect rec(startPt, endPt);
			negBuf.push_back(rec);
			lastFlag = 0;
		}
		_img->copyTo(showImg);
		restortImage(showImg, *_posBuf, negBuf);
		break;
	case CV_EVENT_MOUSEMOVE:
		if (labeling)
		{
			_img->copyTo(showImg);
			restortImage(showImg, *_posBuf, negBuf);
			endPt.x = Clip3(0, _img->cols - 1, x);
			endPt.y = Clip3(0, _img->rows - 1, y);
			if (square == 'y')
				makeSquareBox(startPt, endPt);
			if (labeling == 1)
				rectangle(showImg, startPt, endPt, CV_RGB(255, 0, 0), 1, 8, 0);
			else
				rectangle(showImg, startPt, endPt, CV_RGB(0, 255, 0), 1, 8, 0);
			imshow("image", showImg);
			//waitKey(1);
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

int main1(int argc, TCHAR* argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = NULL;
	FILE *fpos, *fneg;
	TCHAR name[100];
	char a, b;
	int startNum = 0, curNum;
	char fileName[200], buf[10];
	char preName[20], midName[20], extName[10]; // ǰ׺�����м�������ţ�����չ��
	int padZero = 0;	// �ļ�������貹�㣬��չΪ���ٸ��ַ�
	int i;

	printf("ͼ����ο��ע���ߣ�������ѡ�\n\
��֡�ű�ע s:��ʼ֡��, ����˳���עx, �鿴/�༭������p, �鿴������n ");
	scanf("%s", buf);
	fflush(stdin);
	printf("��ע������(y/n): ");
	scanf("%c", &square);

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
		labelEidt("pos.txt");
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

	printf("\n��ע���ߡ��������϶���ע���������Ҽ��϶���ע��������һ��ͼ�ɱ�ע���Ŀ�ꡣ\n \
��ݼ�����Enter��ע����ͼΪ�����������ո��ע��һ��ͼ����Backspaceɾ����һ����ע��\n");

	if (buf[0] == 's') // ��֡�ű�ע
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
			printf("�� %s ��ʼ? y / n: ", fileName);
			scanf("%s", &aa);
			if (aa == 'y' || aa == 'Y')
				break;
		} while (FindNextFile(hFind, &FindFileData));

		WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);

		printf("��ʼ��ע %s\n", fileName);

		int len = strlen(fileName);

		// �ж��Ƿ�Ϊ��ȷͼ����չ��
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
	else if (buf[0] == 'x') // ����˳���ע
	{
		// ����png �� jpg �ļ�
		hFind = FindFirstFile(_T("*.png"), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			hFind = FindFirstFile(_T("*.jpg"), &FindFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				printf("û���ҵ�png��jpg�ļ� (%d)\n", GetLastError());
				return -2;
			}
			else
				printf("�ҵ� jpg �ļ�! \n");
		}
		else
			printf("�ҵ� png �ļ�! \n");
	}

	namedWindow("image");// 0);
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
		else if (buf[0] == 'x')
		{
			WideCharToMultiByte(CP_OEMCP, NULL, FindFileData.cFileName, -1, fileName, 200, NULL, FALSE);
			if (!FindNextFile(hFind, &FindFileData))
				break;
		}

		printf("%s\n", fileName);
		cv::Mat gImg_ = imread(fileName, CV_LOAD_IMAGE_COLOR);
		if (gImg_.data == NULL)
		{
			printf("�ļ� %s ��ʧ, ���ע���\n", fileName);
			continue;
		}
		int scale = 1;
		if (gImg_.rows > 960 || gImg_.cols > 1280)
		{
			scale = 2;
			cv::resize(gImg_, gImg, cv::Size(gImg_.cols / scale, gImg_.rows / scale));
		}
		else
			gImg = gImg_;
		putText(gImg, fileName, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
		//img.copyTo(backupImg);
		posBuf.clear();
		negBuf.clear();
		imshow("image", gImg);
		setMouseCallback("image", onMouse, (void *)&posBuf);
		while (1)
		{
			char c = cvWaitKey(0);
			if (c == ' ')	// �ո�
			{
				recordLabels(fpos, fileName, posBuf, scale);
				recordLabels(fneg, fileName, negBuf, scale);
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
				restortImage(gImg, posBuf, negBuf);
				imshow("image", gImg);
			}
			else if (c == 13) // press enter
			{
				{
					Rect rec(0, 0, gImg.cols, gImg.rows);
					negBuf.clear();
					negBuf.push_back(rec);
				}
				recordLabels(fneg, fileName, negBuf, scale);
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

