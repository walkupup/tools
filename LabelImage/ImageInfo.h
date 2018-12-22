#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define MAX_OBJ_NUM 3000 // 单张图的最大目标个数

enum ObjectName
{
	RECT_OBJ = 0,
	VEHICLE_OBJ = 1,	// vehicle，上下左右远顶.
	PLANE_OBJ = 2		// plane
};

class ImageInfo
{
public:
	ImageInfo::ImageInfo(int pointNum, int objectName, std::string resultName);
	~ImageInfo();

public:
	int objName;
	Mat img, backupImg;
	Point startPt, endPt;
	int labeling = 0;
	int num = 0;
	int count_ = 0;
	vector<Point2d> curPoint;
	vector<vector<Point2d>> posBuf;
	vector<Rect> negBuf;
	int lastFlag;	// 1为positive, 0为negtive
	int numPoints;
	std::string imageFileName;
	FILE *fpos, *fneg;
	vector<cv::Scalar> colors;
	int radius;

	// 新一帧图像，返回<0表示失败.
	int newImage(std::string name);

	// 保存一帧标注结果.
	void savePoints();

	// 设置一个点
	void setOnePoint(Point2d pt);

	// 删除一个点
	void removeOnePoint();


	// 显示画面图像
	Mat getShowImage();

	// 显示点的大小，支持1和2
	void setShowPointSize(int r);

	void setShowScale();

	// 车辆点转换为矩形
	vector<Rect> vehiclePoint2Rect(vector<Point2d> pts);

};
