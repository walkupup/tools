#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"
#include "opencv2/opencv.hpp"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);

private:
    Ui::QtWidgetsApplication1Class ui;

	cv::Mat image;

private slots:
	void DisplayMat(cv::Mat image);
	void on_pushButton_clicked();
	void on_depthButton_clicked();

};
