#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QLine>
#include"MyHistLabel.h"
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

#define IMGE_TOTAL_NUM 12 //显示的图像总数

using namespace cv;

Mat img;
Mat g_dst;
int g_row, g_col;
cv::Mat imageRow = cv::Mat::zeros(256, 240, CV_8UC3);
cv::Mat imageCol = cv::Mat::zeros(320, 256, CV_8UC3);
QPoint p[4] = { QPoint(0, 0) };

extern Mat g_src[IMGE_TOTAL_NUM]; // opencv图像原始- 彩色
extern int g_cur_img;
int g_color = 0;


MyHistLabel::MyHistLabel(QWidget* parent)
:QLabel(parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
	//connect(this, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
	p1 = QPoint(0, 0);

	for (int i = 0; i < 4; i++)
		p[i] = QPoint(0, 0);

	this->setAlignment(Qt::AlignCenter);
	if (!g_src[g_cur_img].empty())
		g_src[g_cur_img].copyTo(img);
	else
	{
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("没有图像"));
		return;
	}

	cv::resize(img, img, Size(240, 320));
	cvtColor(img, g_dst, CV_RGB2GRAY);
}

void MyHistLabel::mouseReleaseEvent(QMouseEvent *ev)
{

}

void MyHistLabel::mousePressEvent(QMouseEvent *ev)
{

	p1 = ev->pos();

}

void MyHistLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{

}

void MyHistLabel::mouseMoveEvent(QMouseEvent *ev)
{
	//p2 = ev->pos();

	p1 = ev->pos();
	if (p1.x() < 0) p1.setX(0);
	if (p1.x()>239) p1.setX(239);
	if (p1.y() < 0) p1.setY(0);
	if (p1.y()>319) p1.setY(319);

	p[g_color] = p1;



	cv::Scalar pen_color;
	imageRow.setTo(cv::Scalar(0, 0, 0));
	imageCol.setTo(cv::Scalar(0, 0, 0));

	for (int i = 3; i >= 0; i--)
	{
		g_row = p[i].y();
		g_col = p[i].x();
		if (g_row != 0 || g_col != 0)
		{
			if (i == 0)
				pen_color = cv::Scalar(255, 0, 0);
			else if (i == 1)
				pen_color = cv::Scalar(0, 0, 255);
			else if (i == 2)
				pen_color = cv::Scalar(0, 255, 0);
			else if (i == 3)
				pen_color = cv::Scalar(255, 255, 0);

			std::vector<cv::Point> points_row;
			int nRowLength = g_dst.cols;

			for (int x = 0; x < nRowLength; x++)
			{
				int y = g_dst.at<uchar>(g_row, x);
				points_row.push_back(cv::Point(x, y));
			}
			cv::polylines(imageRow, points_row, false, pen_color, 1, 8, 0);


			std::vector<cv::Point> points_col;
			int nColLength = g_dst.rows;
			for (int y = 0; y < nColLength; y++)
			{
				int x = g_dst.at<uchar>(y, g_col);
				points_col.push_back(cv::Point(x, y));
			}
			cv::polylines(imageCol, points_col, false, pen_color, 1, 8, 0);
		}
	}

	emit calHist();

}

void MyHistLabel::keyPressEvent(QKeyEvent *ev)
{

}

void MyHistLabel::paintEvent(QPaintEvent *ev)
{
	QLabel::paintEvent(ev);
	
	QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_RGB888);
	//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

	this->setPixmap(QPixmap::fromImage(image));

	QPainter paint(this);
	QPoint pt;

	for (int i = 0; i<4; i++)
	{
		pt = p[i];
		if (i == 0)
			paint.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 1)
			paint.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 2)
			paint.setPen(QPen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap));
		if (i == 3)
			paint.setPen(QPen(Qt::yellow, 1, Qt::SolidLine, Qt::RoundCap));

		paint.drawLine(0, pt.y(), 240, pt.y());
		paint.drawLine(pt.x(), 0, pt.x(), 320);
	}



}
