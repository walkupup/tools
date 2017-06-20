#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

enum ObjectName
{
	RECT_OBJ = 0,
	VEHICLE_OBJ = 1,	// vehicle����������Զ.
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
	int lastFlag;	// 1Ϊpositive, 0Ϊnegtive
	int numPoints;
	std::string imageFileName;
	FILE *fpos, *fneg;

	// ��һ֡ͼ�񣬷���<0��ʾʧ��.
	int newImage(std::string name);

	// ����һ֡��ע���.
	void savePoints();

	// ����һ����
	void setOnePoint(Point2d pt);

	// ɾ��һ����
	void removeOnePoint();


	// ��ʾ����ͼ��
	Mat getShowImage();

	// ������ת��Ϊ����
	vector<Rect> vehiclePoint2Rect(vector<Point2d> pts);

};
