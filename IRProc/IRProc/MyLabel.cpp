#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QLine>
#include"MYLABEL.h"

#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <imgProcDll.h>
#include <CvxText.h>
#include "IRProc.h"

using namespace cv;

#define IMAGE_WIDTH  384//原始图像宽
#define IMAGE_HEIGHT 288//原始图像高

#define IMAGE_PER_ROW 5  //每行显示图像数
#define IMGE_TOTAL_NUM 12 //显示的图像总数
#define COMMENT_PER_IMAGE 30 //每张图像的标注数



extern int g_mouse_mode;//鼠标事件模式： 0-放大缩小，按住移动， 默认；1-加点；2-矩形；3-圆角矩形；4-椭圆，
extern int g_flag_showTemper;//显示温度标志： 0-不显示；1-显示
extern int g_flagShowBigImg;
extern int g_cur_img;
extern float g_ratio[IMGE_TOTAL_NUM];//图像放大倍数
extern Mat g_img[IMGE_TOTAL_NUM];//opencv图像-彩色
extern Mat g_img_gray[IMGE_TOTAL_NUM];//opencv图像-灰度
extern Mat g_temper[IMGE_TOTAL_NUM];//温度矩阵
extern int g_color_type;
extern Shape allshape[IMGE_TOTAL_NUM][COMMENT_PER_IMAGE];
extern int g_shape_no[IMGE_TOTAL_NUM];//图像上标注的数量
extern QPoint g_offset[IMGE_TOTAL_NUM];//显示图像的偏移量

MyLabel::MyLabel( QWidget* parent)
:QLabel(parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
	//connect(this, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
	p1 = QPoint(0, 0);
	p2 = QPoint(0, 0);//QPoint构造函数
	offset = QPoint(0, 0);
	Alloffset = QPoint(0, 0);
	m_shapeType = 1;
	m_flag_press=0;
	this->setAlignment(Qt::AlignCenter);

}

void MyLabel::setOffset(QPoint offset)
{
	this->getCurImgIndex();
	Alloffset = g_offset[g_cur_img];
}

int MyLabel::getCurImgIndex()
{
	QString lname = this->objectName();
	if (g_flagShowBigImg)
		g_cur_img = lname.toInt() - 20;
	else
		g_cur_img = lname.toInt();

	return g_cur_img;
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	m_flag_press = 0;
	this->getCurImgIndex();
	if (g_mouse_mode==2)
	{
		int ltx, lty, rbx, rby,t;

		this->calRealCor(p1, ltx, lty);
		this->calRealCor(p2, rbx, rby);
		if (ltx > rbx)
		{
			t = ltx; ltx = rbx; rbx = t;
		}
		if (lty > rby)
		{
			t = lty; lty = rby; rby = t;
		}
		allshape[g_cur_img][g_shape_no[g_cur_img]].shape_type = g_mouse_mode;
		allshape[g_cur_img][g_shape_no[g_cur_img]].lt_x = ltx;
		allshape[g_cur_img][g_shape_no[g_cur_img]].lt_y = lty;
		allshape[g_cur_img][g_shape_no[g_cur_img]].rb_x = rbx;
		allshape[g_cur_img][g_shape_no[g_cur_img]].rb_y = rby;
		allshape[g_cur_img][g_shape_no[g_cur_img]].comment = "None";
		allshape[g_cur_img][g_shape_no[g_cur_img]].del_flag = false;

		this->calPar(g_shape_no[g_cur_img]);
		g_shape_no[g_cur_img]++;
		draw_shape(g_shape_no[g_cur_img]);

	}
	update();
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
	p1 = event->pos();
	m_flag_press = 1;
	this->getCurImgIndex();
	switch (g_mouse_mode)
	{

	case 0:
	{// 如果是鼠标左键按下
			  if (event->button() == Qt::LeftButton)
			  {
				  g_ratio[g_cur_img] += 0.1;
				  if (g_ratio[g_cur_img] > 3) g_ratio[g_cur_img] = 3;
			  }
			  // 如果是鼠标右键按下
			  else if (event->button() == Qt::RightButton)
			  {
				  g_ratio[g_cur_img] -= 0.1;
				  if (g_ratio[g_cur_img] < 1) g_ratio[g_cur_img] = 1;
			  }
			  update();
			  break;
	}
	case 1:
	{
			  if (event->button() == Qt::LeftButton)
			  {
				  int rx, ry;

				  this->calRealCor(p1, rx, ry);

				  float temper = g_temper[g_cur_img].at<float>(ry, rx);

				  allshape[g_cur_img][g_shape_no[g_cur_img]].shape_type = g_mouse_mode;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].lt_x = rx;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].lt_y = ry;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].rb_x = rx;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].rb_y = ry;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].t_max = temper;;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].t_min = temper;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].t_aver = temper;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].t_msd = 0;
				  allshape[g_cur_img][g_shape_no[g_cur_img]].comment = "None";
				  allshape[g_cur_img][g_shape_no[g_cur_img]].del_flag = false;

				  g_shape_no[g_cur_img]++;	
				  draw_shape(g_shape_no[g_cur_img]);
			  }
		update();
	}
	}

}

void MyLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//emit doubled(ev);
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
	p2 = ev->pos();
	if(g_mouse_mode==0&&m_flag_press)
	{
			offset.setX(p2.x() - p1.x());
			offset.setY(p2.y() - p1.y());
			p1 = p2;
	}
	update();
}

void MyLabel::keyPressEvent(QKeyEvent *ev)
{

}

void MyLabel::paintEvent(QPaintEvent *ev)
{
	QLabel::paintEvent(ev);
	QPainter painter(this);
	Mat timg;

	if (this->pixmap() == NULL) return;
	int imgw = this->pixmap()->width();
	int imgh = this->pixmap()->height();
	int lw = this->width();
	int lh = this->height();

	QRect Paint((lw - imgw) / 2, (lh - imgh) / 2, imgw, imgh);

	int offsetx = Alloffset.x() + offset.x();
	Alloffset.setX(offsetx);

	int offsety = Alloffset.y() + offset.y();
	Alloffset.setY(offsety);


	int NowW = imgw*g_ratio[g_cur_img];
	int NowH = imgh*g_ratio[g_cur_img];

	if (abs(Alloffset.x()) >= abs(Paint.width() / 2 - NowW / 2))    //限制X偏移值
	{
		if (Alloffset.x() > 0)
			Alloffset.setX(-Paint.width() / 2 + NowW / 2);
		else
			Alloffset.setX(Paint.width() / 2 - NowW / 2);
	}
	if (abs(Alloffset.y()) >= abs(Paint.height() / 2 - NowH / 2))    //限制Y偏移值
	{
		if (Alloffset.y() > 0)
			Alloffset.setY(-Paint.height() / 2 + NowH / 2);
		else
			Alloffset.setY(Paint.height() / 2 - NowH / 2);

	}

	g_offset[g_cur_img]=Alloffset  ;

	int x = Paint.width() / 2 + Alloffset.x() - NowW / 2;
	if (x < 0)
		x = 0;

	int y = Paint.height() / 2 + Alloffset.y() - NowH / 2;
	if (y < 0)
		y = 0;

	int  sx = NowW / 2 - Paint.width() / 2 - Alloffset.x();
	if (sx<0)
		sx = 0;

	int  sy = NowH / 2 - Paint.height() / 2 - Alloffset.y();
	if (sy<0)
		sy = 0;

	int w = (NowW - sx)>Paint.width() ? Paint.width() : (NowW - sx);
	if (w>(Paint.width() - x))
		w = Paint.width() - x;

	int h = (NowH - sy) > Paint.height() ? Paint.height() : (NowH - sy);
	if (h > (Paint.height() - y))
		h = Paint.height() - y;
	if (g_color_type)
	{
		cv::resize(g_img[g_cur_img], timg, Size(NowW, NowH), 0, 0);
		QImage image = QImage((const unsigned char*)(timg.data), timg.cols, timg.rows, QImage::Format_RGB888);
		painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
		painter.drawTiledPixmap(Paint.x() - 1, Paint.y() - 1, imgw, imgh, QPixmap::fromImage(image), sx, sy);
	}
	else
	{
		cv::resize(g_img_gray[g_cur_img], timg, Size(NowW, NowH), 0, 0);
		QImage image = QImage((const unsigned char*)(timg.data), timg.cols, timg.rows, QImage::Format_Grayscale8);
		painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
		painter.drawTiledPixmap(Paint.x(), Paint.y(), Paint.width(), Paint.height(), QPixmap::fromImage(image), sx, sy);
	}

	if (g_mouse_mode == 2 && m_flag_press)
		painter.drawRect(QRect(p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y()));


}

void MyLabel::setShapeType(int shapeType)
{
	m_shapeType = shapeType;
}


void MyLabel::draw_shape(int shape_no)
{
	this->getCurImgIndex();
	char label[1000];
	CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
	cv::Scalar size{ 12, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
	text.setFont(nullptr, &size, nullptr, 0);

	for (int i = 0; i < shape_no; i++)
	{
		int x = allshape[g_cur_img][i].rb_x;
		int y = allshape[g_cur_img][i].rb_y;

		switch (allshape[g_cur_img][i].shape_type)
		{
		case 1:
			{
				  rectangle(g_img[g_cur_img], Point(x - 1, y - 1), Point(x + 1, y + 1), CV_RGB(255, 255, 255), CV_FILLED, 8, 0);
				  sprintf(label, "[%02d]- %.2lf", i+1, allshape[g_cur_img][i].t_max);
				  text.putText(g_img[g_cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}
			break;
		case 2:
			{
				  rectangle(g_img[g_cur_img], Point(allshape[g_cur_img][i].lt_x, allshape[g_cur_img][i].lt_y), Point(allshape[g_cur_img][i].rb_x, allshape[g_cur_img][i].rb_y), Scalar(255, 255, 255), 1, 8, 0);
				  sprintf(label, "[%02d]-%.2lf,%.2lf,%.2lf,%.2f", i + 1, allshape[g_cur_img][i].t_max, allshape[g_cur_img][i].t_min, allshape[g_cur_img][i].t_aver, allshape[g_cur_img][i].t_msd);
				  text.putText(g_img[g_cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}

		}

	}
}

void MyLabel::calRealCor(QPoint pt, int &rx, int &ry)//计算鼠标位置在原始图像中的实际坐标
{
	if (this->pixmap() == NULL) return;
	int w = this->pixmap()->width();
	int h = this->pixmap()->height();

	int lw = this->width();
	int lh = this->height();

	int x = pt.x() - (lw - w) / 2;
	int y = pt.y() - (lh - h) / 2;

	if (x < 0) x = 0;
	if (x >= w) x = w - 1;
	if (y < 0) y = 0;
	if (y >= h) y = h - 1;
	int NowW = w*g_ratio[g_cur_img];
	int NowH = h*g_ratio[g_cur_img];
	int  sx = NowW / 2 - w / 2 - Alloffset.x();
	if (sx<0)
		sx = 0;

	int  sy = NowH / 2 - h / 2 - Alloffset.y();
	if (sy<0)
		sy = 0;

	rx = (x + sx)*IMAGE_HEIGHT / NowW;
	ry = (y + sy)*IMAGE_WIDTH / NowH;
}

void MyLabel::calPar(int cur_shape_no)
{
	this->getCurImgIndex();

	float topvalue = -999999, bottomvalue = 9999, aver = 0, sum = 0, sd = 0;
	int num = (allshape[g_cur_img][cur_shape_no].rb_x - allshape[g_cur_img][cur_shape_no].lt_x)*(allshape[g_cur_img][cur_shape_no].rb_y - allshape[g_cur_img][cur_shape_no].lt_y);

	if (allshape[g_cur_img][cur_shape_no].shape_type != 1)
	{
		for (int i = allshape[g_cur_img][cur_shape_no].lt_y; i < allshape[g_cur_img][cur_shape_no].rb_y; i++)
		{
			float *p_tData = g_temper[g_cur_img].ptr<float>(i);
			for (int j = allshape[g_cur_img][cur_shape_no].lt_x; j < allshape[g_cur_img][cur_shape_no].rb_x; j++)
			{
				float value = *(p_tData + j);
				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
				bottomvalue = bottomvalue<value ? bottomvalue : value;
				topvalue = topvalue>value ? topvalue : value;
				sum += value;
			}
		}
		aver = sum / num;
		for (int i = allshape[g_cur_img][cur_shape_no].lt_y; i < allshape[g_cur_img][cur_shape_no].rb_y; i++)
		{
			float *p_tData = g_temper[g_cur_img].ptr<float>(i);
			for (int j = allshape[g_cur_img][cur_shape_no].lt_x; j < allshape[g_cur_img][cur_shape_no].rb_x; j++)
			{
				float value = *(p_tData + j);
				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
				sd += (value - aver)*(value - aver);
			}
		}
		allshape[g_cur_img][cur_shape_no].t_max = topvalue;;
		allshape[g_cur_img][cur_shape_no].t_min = bottomvalue;
		allshape[g_cur_img][cur_shape_no].t_aver = aver;
		allshape[g_cur_img][cur_shape_no].t_msd = sqrt(sd) / num;
	}

}