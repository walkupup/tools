#include "QtWidgetsApplication1.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QTextCodec>
#include <QMessageBox>
#include <Kinect.h>

int getDepthImg(cv::Mat &img);

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_clicked()));
	connect(ui.depthButton, SIGNAL(clicked(bool)), this, SLOT(on_depthButton_clicked()));

}

void QtWidgetsApplication1::DisplayMat(cv::Mat image)
{
	cv::Mat rgb;
	QImage img;
	if (image.channels() == 3)
	{
		cvtColor(image, rgb, cv::COLOR_BGR2RGB);
		img = QImage((const unsigned char*)(rgb.data),
			rgb.cols, rgb.rows, rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
			QImage::Format_RGB888);
	}
	else
	{
		img = QImage((const unsigned char*)(image.data),
			image.cols, image.rows, rgb.cols*image.channels(),
			QImage::Format_Grayscale8);
	}
	ui.image_label->setPixmap(QPixmap::fromImage(img).scaled(ui.image_label->size()));//setPixelmap(QPixmap::fromImage(img));
	ui.image_label->resize(ui.image_label->pixmap()->size());//resize(ui->label->pixmap()->size());
}

int getDepthImg(cv::Mat &img)
{
	// 1a.获取感应器
	IKinectSensor* pSensor = nullptr;
	GetDefaultKinectSensor(&pSensor);

	// 1b. 打开感应器
	pSensor->Open();

	// 2a. 取得深度数据
	IDepthFrameSource* pFrameSource = nullptr;
	pSensor->get_DepthFrameSource(&pFrameSource);

	// 2b. 取得深度数据的描述信息（宽、高）
	int        iWidth = 0;
	int        iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;

	// 2c. 取得感应器的“可靠深度”的最大、最小距离
	UINT16 uDepthMin = 0, uDepthMax = 0;
	pFrameSource->get_DepthMinReliableDistance(&uDepthMin);
	pFrameSource->get_DepthMaxReliableDistance(&uDepthMax);
	//cout << "Reliable Distance: "
	//	<< uDepthMin << " – " << uDepthMax << endl;

	// 建立图像矩阵，mDepthImg用来存储16位的图像数据，直接用来显示会接近全黑
	//不方便观察，用mImg8bit转换成8位再显示
	cv::Mat mDepthImg(iHeight, iWidth, CV_16UC1);
	cv::Mat mImg8bit(iHeight, iWidth, CV_8UC1);
	//cv::namedWindow("DepthImage");

	// 3a. 打开深度数据阅读器
	IDepthFrameReader* pFrameReader = nullptr;
	pFrameSource->OpenReader(&pFrameReader);
	IDepthFrame* pFrame = nullptr;

	// 主循环
	while (1)
	{
		// 4a. 取得最新数据
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{
			// 4c. 把数据存入16位图像矩阵中
			pFrame->CopyFrameDataToArray(iWidth * iHeight,
				reinterpret_cast<UINT16*>(mDepthImg.data));//强制转换数据类型

														   // 4d. 把16位转换成8位
			mDepthImg.convertTo(mImg8bit, CV_8U, 255.0f / uDepthMax);//converto()第一个参数是输出矩阵，第二个是转换类型，第三个是缩放因子，其中4500是深度数据的最大距离
			//cv::imshow("DepthImage", mImg8bit);
			mImg8bit.copyTo(img);
			//要改变显示的颜色和效果，就改变从mDepthImg到mImg8bit的转换  
			// 4e. 释放变量pFrame
			pFrame->Release();
			break;
		}
	}

	// 3b. 释放变量pFrameReader
	pFrameReader->Release();
	pFrameReader = nullptr;

	// 2d.释放变量pFramesSource
	pFrameSource->Release();
	pFrameSource = nullptr;

	// 1c.关闭感应器
	pSensor->Close();

	// 1d.释放感应器
	pSensor->Release();
	pSensor = nullptr;

	return 1;
}



void QtWidgetsApplication1::on_pushButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), ".", tr("Image File (*.jpg *.png *.bmp)"));
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	std::string name = code->fromUnicode(filename).data();//filename.toAscii().data()
	image = cv::imread(name);
	if (!image.data)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Image Data Is Null"));
		msgBox.exec();
	}
	else {
		DisplayMat(image);
	}
}

void QtWidgetsApplication1::on_depthButton_clicked()
{
	getDepthImg(image);
	if (!image.data)
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Image Data Is Null"));
		msgBox.exec();
	}
	else {
		DisplayMat(image);
	}
}
