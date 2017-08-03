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
	char name[200], path[200], *preName, *extName;

	printf("用法：输入文件名 输出格式 输出前缀\n");

	if (argc == 1)
	{
		printf("请输入参数\n");
		return 0;
	}
	if (argc == 2)
	{
		extName = "jpg";
		preName = "image";
	}
	else if (argc == 3)
	{
		extName = argv[2];
		preName = "image";
	}
	else
	{
		extName = argv[2];
		preName = argv[3];
	}

	{
		//sprintf_s(name, 200, "%s%d.avi", DIR_SEQ, j);
		VideoCapture v1(argv[1]);
		frameCount = v1.get(CV_CAP_PROP_FRAME_COUNT);

		sprintf_s(path, 200, "md img");
		system(path);

		for (i = 0; i < frameCount; i++)
		{
			v1.read(img);
			if (i % 20 == 0)
			{
				sprintf_s(name, 200, "img/%s%08d.%s", preName, i, extName);
				printf("writing frame %d / %d\n", i, frameCount);
				imshow("video", img);
				waitKey(1);
				imwrite(name, img);
			}
		}
	}

	printf("finished\n");


	return 0;

}
