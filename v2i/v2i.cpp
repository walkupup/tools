#include <iostream>
#include <opencv2/opencv.hpp>

//#define DIR_SEQ "E:\\Data\\bsd\\20140826\\s4\\"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat img;
	int i;
	int frameCount;
	char name[200], path[200], *preName;

	printf("�÷��������ļ��� ���ǰ׺\n");

	if (argc == 1)
	{
		printf("���������\n");
		return 0;
	}
	if (argc == 2)
	{
		preName = "image";
	}
	else
		preName = argv[2];

	{
		//sprintf_s(name, 200, "%s%d.avi", DIR_SEQ, j);
		VideoCapture v1(argv[1]);
		frameCount = v1.get(CV_CAP_PROP_FRAME_COUNT);

		sprintf_s(path, 200, "md img");
		system(path);

		for (i = 0; i < frameCount; i++)
		{
			v1.read(img);
			sprintf_s(name, 200, "img/%s%08d.png", preName, i);
			printf("writing frame %d / %d\n", i, frameCount);
			imshow("video", img);
			waitKey(10);
			imwrite(name, img);
		}
	}

	printf("finished\n");


	return 0;

}
