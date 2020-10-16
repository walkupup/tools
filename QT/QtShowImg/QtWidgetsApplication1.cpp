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
			rgb.cols, rgb.rows, rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()�����滻Ϊimage.step
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
	// 1a.��ȡ��Ӧ��
	IKinectSensor* pSensor = nullptr;
	GetDefaultKinectSensor(&pSensor);

	// 1b. �򿪸�Ӧ��
	pSensor->Open();

	// 2a. ȡ���������
	IDepthFrameSource* pFrameSource = nullptr;
	pSensor->get_DepthFrameSource(&pFrameSource);

	// 2b. ȡ��������ݵ�������Ϣ�����ߣ�
	int        iWidth = 0;
	int        iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	pFrameSource->get_FrameDescription(&pFrameDescription);
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;

	// 2c. ȡ�ø�Ӧ���ġ��ɿ���ȡ��������С����
	UINT16 uDepthMin = 0, uDepthMax = 0;
	pFrameSource->get_DepthMinReliableDistance(&uDepthMin);
	pFrameSource->get_DepthMaxReliableDistance(&uDepthMax);
	//cout << "Reliable Distance: "
	//	<< uDepthMin << " �C " << uDepthMax << endl;

	// ����ͼ�����mDepthImg�����洢16λ��ͼ�����ݣ�ֱ��������ʾ��ӽ�ȫ��
	//������۲죬��mImg8bitת����8λ����ʾ
	cv::Mat mDepthImg(iHeight, iWidth, CV_16UC1);
	cv::Mat mImg8bit(iHeight, iWidth, CV_8UC1);
	//cv::namedWindow("DepthImage");

	// 3a. ����������Ķ���
	IDepthFrameReader* pFrameReader = nullptr;
	pFrameSource->OpenReader(&pFrameReader);
	IDepthFrame* pFrame = nullptr;

	// ��ѭ��
	while (1)
	{
		// 4a. ȡ����������
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{
			// 4c. �����ݴ���16λͼ�������
			pFrame->CopyFrameDataToArray(iWidth * iHeight,
				reinterpret_cast<UINT16*>(mDepthImg.data));//ǿ��ת����������

														   // 4d. ��16λת����8λ
			mDepthImg.convertTo(mImg8bit, CV_8U, 255.0f / uDepthMax);//converto()��һ��������������󣬵ڶ�����ת�����ͣ����������������ӣ�����4500��������ݵ�������
			//cv::imshow("DepthImage", mImg8bit);
			mImg8bit.copyTo(img);
			//Ҫ�ı���ʾ����ɫ��Ч�����͸ı��mDepthImg��mImg8bit��ת��  
			// 4e. �ͷű���pFrame
			pFrame->Release();
			break;
		}
	}

	// 3b. �ͷű���pFrameReader
	pFrameReader->Release();
	pFrameReader = nullptr;

	// 2d.�ͷű���pFramesSource
	pFrameSource->Release();
	pFrameSource = nullptr;

	// 1c.�رո�Ӧ��
	pSensor->Close();

	// 1d.�ͷŸ�Ӧ��
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
