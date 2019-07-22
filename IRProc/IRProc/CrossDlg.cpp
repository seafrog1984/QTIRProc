#include "CrossDlg.h"
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMouseEvent>
#include <QPixmap>

extern 	cv::Mat imageRow;
extern 	cv::Mat imageCol;

extern int g_color;

extern QPoint p[4];

using namespace cv;

CrossDlg::CrossDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.label, SIGNAL(calHist()), this, SLOT(updateHist()));


	connect(ui.btn_red, SIGNAL(clicked()), this, SLOT(btn_red()));
	connect(ui.btn_blue, SIGNAL(clicked()), this, SLOT(btn_blue()));
	connect(ui.btn_green, SIGNAL(clicked()), this, SLOT(btn_green()));
	connect(ui.btn_yellow, SIGNAL(clicked()), this, SLOT(btn_yellow()));

	connect(ui.btn_clear, SIGNAL(clicked()), this, SLOT(btn_clear()));




	ui.label_2->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");
	ui.label_3->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");

}

CrossDlg::~CrossDlg()
{
}

void CrossDlg::exec()
{
	// 设置为模态;
	this->setWindowModality(Qt::ApplicationModal);
	show();
	// 使用事件循环QEventLoop ,不让exec()方法结束，在用户选择确定或者取消后，关闭窗口结束事件循环，并返回最后用户选择的结果;
	// 根据返回结果得到用户按下了确定还是取消，采取相应的操作。从而模拟出QDialog类的exec()方法;
	m_eventLoop = new QEventLoop(this);
	m_eventLoop->exec();

}

void CrossDlg::btn_red()
{
	g_color = 0;
}

void CrossDlg::btn_blue()
{
	g_color = 1;
}

void CrossDlg::btn_green()
{
	g_color = 2;
}

void CrossDlg::btn_yellow()
{
	g_color = 3;
}

void CrossDlg::btn_clear()
{
	for (int i = 0; i < 4; i++)
		p[i] = QPoint(0, 0);


	imageRow.setTo(cv::Scalar(0, 0, 0));
	imageCol.setTo(cv::Scalar(0, 0, 0));
	updateHist();
}

void CrossDlg::updateHist()
{
	QImage image_row = QImage((const unsigned char*)(imageRow.data), imageRow.cols, imageRow.rows, QImage::Format_RGB888);

	QPixmap pixmap_row = QPixmap::fromImage(image_row);
	//QPixmap fitpixmap = pixmap.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui.label_3->setPixmap(pixmap_row);

	QImage image_col = QImage((const unsigned char*)(imageCol.data), imageCol.cols, imageCol.rows, QImage::Format_RGB888);

	QPixmap pixmap_col = QPixmap::fromImage(image_col);
	//QPixmap fitpixmap_col = pixmap_col.scaled(240, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	ui.label_2->setPixmap(pixmap_col);
}