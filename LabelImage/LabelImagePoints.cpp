// LabelImage.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _CRT_SECURE_NO_WARNINGS
#include "tchar.h"
#include "opencv2/opencv.hpp"
#include "Windows.h"

using namespace std;
using namespace cv;

static Mat img, backupImg;
static Point startPt, endPt;
static int labeling = 0;
static int num = 0;
static int count_ = 0; // 
static vector<Point2d> curPoint;
static vector<vector<Point2d>> posBuf;
static vector<Rect> negBuf;
static int lastFlag = 1;	// 1Ϊpositive, 0λnegtive
static int numPoints = 4;

void labelEditPoints(string fileName, int numPoints);

// �ָ�ԭͼ�����еľ��ο�
void restortImage(Mat& img, vector<vector<Point2d>>& rec1, vector<Rect>& rec2)
{
	int i;
	backupImg.copyTo(img);
	for (int i = 0; i < rec1.size(); i++)
	{
		for (int j = 0; j < rec1[i].size(); j++)
			circle(img, rec1[i][j], 2, Scalar(0, 0, 255), -1);
	}
	for (i = 0; i < rec2.size(); i++)
		rectangle(img, rec2[i], CV_RGB(0, 255, 0), 1, 8, 0);

	return;
}


// ��¼���Label
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

// ���ص�����
static void onMouse(int event, int x, int y, int, void*)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		break;
	case CV_EVENT_RBUTTONDOWN:
		break;
	case CV_EVENT_LBUTTONUP:
		labeling = 1;
		posBuf[count_][num] = Point2d(x, y);
		num++;
		if (num > numPoints - 1)
		{
			num = 0;
			count_++;
		}
		restortImage(img, posBuf, negBuf);
		imshow("image", img);
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
	char preName[20], midName[20], extName[10]; // ǰ׺�����м�������ţ�����չ��
	int padZero = 0;	// �ļ�������貹�㣬��չΪ���ٸ��ַ�
	int i;

	printf("�ؼ����ע���ߣ�������ѡ�\n\
��֡�ű�ע s:��ʼ֡��, ����˳���עx, �鿴������p, �鿴������n ");
	scanf("%s", buf);
	printf("ÿ��Ŀ�꼸���㣺");
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
		labelEditPoints("pos.txt", numPoints);
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

//	printf("\n��ע���ߡ��������϶���ע���������Ҽ��϶���ע��������һ��ͼ�ɱ�ע���Ŀ�ꡣ\n \
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
	setMouseCallback("image", onMouse, 0);
	fopen_s(&fpos, "pos.txt", "wt");
	fclose(fpos);
	fopen_s(&fneg, "neg.txt", "wt");
	fclose(fneg);
	curNum = startNum;

	posBuf.resize(100);
	for (int i = 0; i < posBuf.size(); i++)
	{
		posBuf[i].resize(numPoints);
	}
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

		count_ = 0;
		printf("%s\n", fileName);
		img = imread(fileName, CV_LOAD_IMAGE_COLOR);
		if (img.data == NULL)
		{
			printf("�ļ� %s ��ʧ, ���ע���\n", fileName);
			continue;
		}
		putText(img, fileName, Point(1, 10), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
		img.copyTo(backupImg);
		for (int i = 0; i < posBuf.size(); i++)
		{
			for (int j = 0; j < posBuf[i].size(); j++)
			{
				posBuf[i][j].x = 0;
				posBuf[i][j].y = 0;
			}
		}
		negBuf.clear();
		imshow("image", img);
		while (1)
		{
			char c = cvWaitKey(0);
			if (c == ' ')	// �ո�
			{
				savePoints(fpos, fileName, posBuf);
				break;
			}
			else if (c == 8)	// press backspace
			{
				if (lastFlag)
				{
					for (int i = 0; i < posBuf[count_].size(); i++)
					{
						posBuf[count_][i].x = 0;
						posBuf[count_][i].y = 0;
					}
					if (count_ > 0)
						count_--;
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
				//savePoints(fneg, fileName, negBuf);
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

