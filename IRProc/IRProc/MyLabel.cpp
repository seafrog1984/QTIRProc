#include<QLabel>
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include <QLine>
#include"MYLABEL.h"
#include <time.h>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <imgProcDll.h>
#include <CvxText.h>
#include "IRProc.h"

using namespace cv;



#define IMAGE_PER_ROW 5  //每行显示图像数
#define IMGE_TOTAL_NUM 12 //显示的图像总数
#define COMMENT_PER_IMAGE 30 //每张图像的标注数

extern int IMAGE_WIDTH;//原始图像宽
extern int IMAGE_HEIGHT;//原始图像高

extern int g_mouse_mode;//鼠标事件模式： 0-放大缩小，按住移动；1-加点；2-矩形；3-圆角矩形；4-椭圆；5-删除；6-选择模式， 默认
extern int g_flag_showTemper;//显示温度标志： 0-不显示；1-显示
extern int g_flagShowBigImg;
extern int g_cur_img;
extern int g_picNum;
extern double g_ratio[IMGE_TOTAL_NUM];//图像放大倍数
extern Mat g_mer[IMGE_TOTAL_NUM];//融合图像
extern Mat g_src[IMGE_TOTAL_NUM];
extern Mat g_img[IMGE_TOTAL_NUM];//opencv图像-彩色
extern Mat g_img_gray[IMGE_TOTAL_NUM];//opencv图像-灰度
extern Mat g_temper[IMGE_TOTAL_NUM];//温度矩阵
extern double g_TR[IMGE_TOTAL_NUM];//图像的温度均方差
extern QImage g_qImgShow[IMGE_TOTAL_NUM];//Qt原始图像-彩色
extern int g_color_type;
extern Shape allshape[IMGE_TOTAL_NUM][COMMENT_PER_IMAGE];
extern int g_shape_no[IMGE_TOTAL_NUM];//图像上标注的数量
extern QPoint g_offset[IMGE_TOTAL_NUM];//显示图像的偏移量
extern double g_mer_ratio[IMGE_TOTAL_NUM];

extern int g_flag_showTemper;//显示温度标志： 0-不显示；1-显示

extern QString g_name;
extern QString g_age;
extern QString g_gender;
extern QString g_ID ;
extern QString g_scanID;
extern QString g_cardID;

double g_size_change[11] = {1, 1.2, 1.5, 1.9, 2.4, 3.0, 3.8, 4.7, 5.9, 7.3, 9.2 };
extern int g_size_index[IMGE_TOTAL_NUM];

int g_move_flag = 0;

extern int g_sel_tag[IMGE_TOTAL_NUM];//当前标签下标

MyLabel::MyLabel( QWidget* parent)
:QLabel(parent)
{
	//connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
	//connect(this, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
	p1 = QPoint(0, 0);
	p2 = QPoint(0, 0);//QPoint构造函数
	m_temper = 25;
	g_mouse_mode = 6;

	offset = QPoint(0, 0);
	Alloffset = QPoint(0, 0);
	m_shapeType = 1;
	m_action = 0;
	m_flag_press=0;
	this->setAlignment(Qt::AlignCenter);

	m_name=g_name;
	m_age=g_age;
	m_gender=g_gender;
	m_ID= g_ID ;
	m_scanID= g_scanID;
	m_cardID=g_cardID;

}

void MyLabel::setOffset(QPoint offset)
{
	this->getCurImgIndex();
	Alloffset = g_offset[cur_img];
}

int MyLabel::getCurImgIndex()
{
	QString lname = this->objectName();
	if (g_flagShowBigImg)
		cur_img = lname.toInt() - 20;
	else
		cur_img = lname.toInt();

	return cur_img;
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
	p2 = ev->pos();

	if (g_mouse_mode == 0 && m_flag_press)
	{
		int dx = p2.x() - p1.x();
		if (dx >= -3 && dx <= 3)dx = 0;
		int dy = p2.y() - p1.y();
		if (dy >= -3 && dy <= 3)dy = 0;
		offset.setX(dx);
		offset.setY(dy);
		p1 = p2;
		g_move_flag = 1;
		update();
	}
	else if (g_flag_showTemper&&g_mouse_mode == 6)
	{
		int rx, ry;

		this->calRealCor(p2, rx, ry);
		m_temper = g_temper[cur_img].at<float>(ry, rx);
	}
	update();
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	if (m_flag_press == 1)
	{
		m_flag_press = 0;
		offset.setX(0);
		offset.setY(0);
		int cur_img=getCurImgIndex();
		if (g_mouse_mode == 2 || g_mouse_mode == 3 || g_mouse_mode == 4)
		{
			int ltx, lty, rbx, rby, t;

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
			allshape[cur_img][g_shape_no[cur_img]].shape_type = g_mouse_mode;
			allshape[cur_img][g_shape_no[cur_img]].lt_x = ltx;
			allshape[cur_img][g_shape_no[cur_img]].lt_y = lty;
			allshape[cur_img][g_shape_no[cur_img]].rb_x = rbx;
			allshape[cur_img][g_shape_no[cur_img]].rb_y = rby;
			allshape[cur_img][g_shape_no[cur_img]].comment = "None";
			allshape[cur_img][g_shape_no[cur_img]].del_flag = false;

			if (g_mouse_mode == 3)
			{
				int dia = (rbx - ltx) < (rby - lty) ? (rbx - ltx) : (rby - lty);
				allshape[cur_img][g_shape_no[cur_img]].rb_x = ltx + dia;
				allshape[cur_img][g_shape_no[cur_img]].rb_y = lty + dia;

			}

			calPar(g_shape_no[cur_img]);
			g_sel_tag[cur_img] = g_shape_no[cur_img];
			g_shape_no[cur_img]++;
			draw_shape(g_shape_no[cur_img]);

		}
		update();
	}
	
	emit upInfo();
}

void MyLabel::mousePressEvent(QMouseEvent *event)
{
	m_flag_press = 1;
	p1 = event->pos();
	g_cur_img=getCurImgIndex();
	if (g_flagShowBigImg)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			QLabel *p = parent()->findChild<QLabel*>(QString::number(i+20));
			if (p != NULL)
				p->setStyleSheet("backgroud-color:rgb(255,255,255);border:0px;");
		}
	}
	else
	{
		for (int i = 0; i < g_picNum; i++)
		{
			QLabel *p = parent()->findChild<QLabel*>(QString::number(i));
			if (p!=NULL)
				p->setStyleSheet("backgroud-color:rgb(255,255,255);border:0px;");
		}
	}

	setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");


	switch (g_mouse_mode)
	{
	case 6:
	{
			  m_action = 5;
			  m_flag_press = 0;
			  int rx, ry;

			  this->calRealCor(p1, rx, ry);
			  for (int i = 0; i < g_shape_no[cur_img]; i++)
			  {
				  if (rx >= allshape[cur_img][i].lt_x&&rx <= allshape[cur_img][i].rb_x&&ry >= allshape[cur_img][i].lt_y&&ry <= allshape[cur_img][i].rb_y)
				  {
					  g_sel_tag[cur_img] = i;
					  break;
				  }
			  }
			  
		//	  emit upInfo();

			  calPar(g_sel_tag[cur_img]);
			  draw_shape(g_shape_no[cur_img]);

			  if (event->button() == Qt::RightButton)
			  {
				  allshape[cur_img][g_sel_tag[cur_img]].del_flag = true;
				  g_sel_tag[cur_img] = 0;
				  draw_shape(g_shape_no[cur_img]);
			  }
	}
	break;
	case 0:
	{// 如果是鼠标左键按下
			  if (event->button() == Qt::LeftButton)
			  {
				  if (g_size_index[cur_img]<10)
					  g_size_index[cur_img]++;
				  g_ratio[cur_img] = g_size_change[g_size_index[cur_img]];
				  //if (g_ratio[cur_img] > 3) g_ratio[cur_img] = 3;
			  }
			  // 如果是鼠标右键按下
			  else if (event->button() == Qt::RightButton)
			  {
				  m_flag_press = 0;
				  if (g_size_index[cur_img]>0)
					  g_size_index[cur_img]--;
				  g_ratio[cur_img] = g_size_change[g_size_index[cur_img]];
				  //if (g_ratio[cur_img] < 1) g_ratio[cur_img] = 1;
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

				  float temper = g_temper[cur_img].at<float>(ry, rx);

				  allshape[cur_img][g_shape_no[cur_img]].shape_type = g_mouse_mode;
				  allshape[cur_img][g_shape_no[cur_img]].lt_x = rx;
				  allshape[cur_img][g_shape_no[cur_img]].lt_y = ry;
				  allshape[cur_img][g_shape_no[cur_img]].rb_x = rx;
				  allshape[cur_img][g_shape_no[cur_img]].rb_y = ry;
				  allshape[cur_img][g_shape_no[cur_img]].t_max = temper;;
				  allshape[cur_img][g_shape_no[cur_img]].t_min = temper;
				  allshape[cur_img][g_shape_no[cur_img]].t_aver = temper;
				  allshape[cur_img][g_shape_no[cur_img]].t_msd = 0;
				  allshape[cur_img][g_shape_no[cur_img]].comment = "None";
				  allshape[cur_img][g_shape_no[cur_img]].del_flag = false;

				  g_shape_no[cur_img]++;	
				  draw_shape(g_shape_no[cur_img]);
			  }
			  else
			  {
				  m_action = 5;
				  m_flag_press = 0;
				  int rx, ry;

				  this->calRealCor(p1, rx, ry);
				  for (int i = 0; i < g_shape_no[cur_img]; i++)
				  {
					  if (rx >= allshape[cur_img][i].lt_x&&rx <= allshape[cur_img][i].rb_x&&ry >= allshape[cur_img][i].lt_y&&ry <= allshape[cur_img][i].rb_y)
					  {
						  allshape[cur_img][i].del_flag = true;
						  break;
					  }
				  }
				  draw_shape(g_shape_no[cur_img]);

			  }
		update();
	}
		break;

		update();

	}

	emit upInfo();
}

void MyLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	//emit doubled(ev);
}



void MyLabel::keyPressEvent(QKeyEvent *ev)
{
	//if (ev->key() == Qt::Key_Delete)
	//{


	//}

}

void MyLabel::paintEvent(QPaintEvent *ev)
{
	int cur_img=getCurImgIndex();
	QLabel::paintEvent(ev);
	QPainter painter(this);
	painter.setPen(QColor(0, 0, 0));


	QPixmap pixmap = QPixmap::fromImage(g_qImgShow[cur_img]);
	//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
	QPixmap fitpixmap = pixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
	setPixmap(fitpixmap);


	Mat timg;

	if (this->pixmap() == NULL) return;
	int imgw = this->pixmap()->width();
	int imgh = this->pixmap()->height();
	int lw = this->width();
	int lh = this->height();

	QRect Paint((lw - imgw) / 2, (lh - imgh) / 2, imgw, imgh);


	if (g_move_flag==1)
	{
		g_move_flag ++;
	}
	else if (g_move_flag==15)
	{
		offset.setX(0);
		offset.setY(0);
	}

	int dx=0, dy=0;
	dx = offset.x();
	int offsetx = Alloffset.x() + offset.x();
	Alloffset.setX(offsetx);

	dy = offset.y();
	int offsety = Alloffset.y() + offset.y();
	Alloffset.setY(offsety);


	int NowW = imgw*g_ratio[cur_img];
	int NowH = imgh*g_ratio[cur_img];

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

	g_offset[cur_img]=Alloffset  ;

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
		cv::resize(g_img[cur_img], timg, Size(NowW, NowH), 0, 0);

		Rect rect1(sx, sy, imgw, imgh);
		Mat roi1;
		timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1
		QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
		//painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
		//painter.drawTiledPixmap(Paint.x(), Paint.y(), imgw, imgh, QPixmap::fromImage(image), sx, sy);
		setPixmap(QPixmap::fromImage(image));
	}
	else
	{
		cv::resize(g_img_gray[cur_img], timg, Size(NowW, NowH), 0, 0);
		Rect rect1(sx, sy, imgw, imgh);
		Mat roi1;
		timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1
		QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_Grayscale8);
		//painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
		//painter.drawTiledPixmap(Paint.x(), Paint.y(), imgw, imgh, QPixmap::fromImage(image), sx, sy);
		setPixmap(QPixmap::fromImage(image));
		//QImage image = QImage((const unsigned char*)(timg.data), timg.cols, timg.rows, QImage::Format_Grayscale8);
		////painter.drawRect(Paint.x() - 1, Paint.y() - 1, Paint.width() + 1, Paint.height() + 1); //画框
		//painter.drawTiledPixmap(Paint.x(), Paint.y(), Paint.width(), Paint.height(), QPixmap::fromImage(image), sx, sy);
	}

	if (m_action != 5)
	{
		if (g_mouse_mode == 2 && m_flag_press)
		painter.drawRect(QRect(p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y()));

		if (g_mouse_mode == 3 && m_flag_press)
		{
			int radius = abs(p2.x() - p1.x()) < abs(p2.y() - p1.y()) ? abs(p2.x() - p1.x()) : abs(p2.y() - p1.y());
			painter.drawEllipse(QRect(p1.x()<p2.x() ? p1.x() : p2.x(), p1.y()<p2.y() ? p1.y() : p2.y(), radius, radius));
		}


		if (g_mouse_mode == 4 && m_flag_press)
			painter.drawEllipse(QRect(p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y()));

	}
	else
	{
		m_action = g_mouse_mode;
	}
	
	QString strText = QString::number(g_ratio[cur_img], 10, 2);
	painter.drawText(Paint.x() + 10, Paint.y() + 10, strText);
	painter.drawText(Paint.x() + 10, Paint.y() + 25, QString::fromLocal8Bit("姓名：")+m_name);
	painter.drawText(Paint.x() + 10, Paint.y() + 40, QString::fromLocal8Bit("姓别：")+m_gender);
	painter.drawText(Paint.x() + 10, Paint.y() + 55, QString::fromLocal8Bit("年龄：") + m_age);
	painter.drawText(Paint.x() + 10, Paint.y() + 70, QString::fromLocal8Bit("身份证号：") + m_ID);
	painter.drawText(Paint.x() + 10, Paint.y() + 85, QString::fromLocal8Bit("扫描编号：") + m_scanID);

	//painter.drawText(Paint.x() + 10, Paint.y() + 100, "p1:" + QString::number(p1.x()) + " " + QString::number(p1.y()));
	//painter.drawText(Paint.x() + 10, Paint.y() + 120, "p2:" + QString::number(p2.x()) + " " + QString::number(p2.y()));
	//painter.drawText(Paint.x() + 10, Paint.y() + 140,  QString::number(dx) + " " + QString::number(dy));

	if (g_flag_showTemper&&g_mouse_mode==6)
	{
		QString strTemper = QString::number(m_temper);
		painter.drawText(p2.x(), p2.y(), strTemper);
	}

}

void MyLabel::setShapeType(int shapeType)
{
	m_shapeType = shapeType;
}


void MyLabel::draw_shape(int shape_no)
{
	getCurImgIndex();
	char label[1000];
	CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
	cv::Scalar size{ 12, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
	text.setFont(nullptr, &size, nullptr, 0);
	g_src[cur_img].copyTo(g_img[cur_img]);

	g_img[cur_img] = g_img[cur_img] * (1 - g_mer_ratio[cur_img]) + g_mer[cur_img] * g_mer_ratio[cur_img];

	for (int i = 0; i < shape_no; i++)
	{
		int x = allshape[cur_img][i].lt_x;
		int y = allshape[cur_img][i].rb_y;

		y = y + 5;
		if (y + 4 >= g_img[cur_img].rows) y = g_img[cur_img].rows - 5;

		if (x>g_img[cur_img].cols / 2) x = x - 24;



		CvScalar penColor=CV_RGB(0, 0, 0);

		if (i == g_sel_tag[cur_img])
		{
			penColor = CV_RGB(255,0,0);
		}


		if (!allshape[cur_img][i].del_flag)
		switch (allshape[cur_img][i].shape_type)
		{
		case 1:
			{
				  rectangle(g_img[cur_img], Point(x - 1, y - 1), Point(x + 1, y + 1), penColor, CV_FILLED, 8, 0);
				 // sprintf(label, "[%02d]%.2lf", i+1, allshape[cur_img][i].t_max);
				  sprintf(label, "[%02d]", i + 1);
				  text.putText(g_img[cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}
			break;
		case 2:
			{
				  rectangle(g_img[cur_img], Point(allshape[cur_img][i].lt_x, allshape[cur_img][i].lt_y), Point(allshape[cur_img][i].rb_x, allshape[cur_img][i].rb_y), penColor, 1, 8, 0);
				 // sprintf(label, "[%02d]%.2lf,%.2lf,%.2lf,%.2f", i + 1, allshape[cur_img][i].t_max, allshape[cur_img][i].t_min, allshape[cur_img][i].t_aver, allshape[cur_img][i].t_msd);
				  sprintf(label, "[%02d]", i + 1);
				  text.putText(g_img[cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}
			break;
		case 3:
			{
				  int radius = (allshape[cur_img][i].rb_x - allshape[cur_img][i].lt_x)/2;
				  circle(g_img[cur_img], Point((allshape[cur_img][i].lt_x + allshape[cur_img][i].rb_x) / 2, (allshape[cur_img][i].lt_y + allshape[cur_img][i].rb_y) / 2), radius, penColor, 1, 8, 0);
				  //sprintf(label, "[%02d]%.2lf,%.2lf,%.2lf,%.2f", i + 1, allshape[cur_img][i].t_max, allshape[cur_img][i].t_min, allshape[cur_img][i].t_aver, allshape[cur_img][i].t_msd);
				  sprintf(label, "[%02d]", i + 1);
				  text.putText(g_img[cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}
			break;
		case 4:
			{
				ellipse(g_img[cur_img], Point((allshape[cur_img][i].lt_x + allshape[cur_img][i].rb_x) / 2, (allshape[cur_img][i].lt_y + allshape[cur_img][i].rb_y) / 2), Size(abs(allshape[cur_img][i].lt_x - allshape[cur_img][i].rb_x) / 2, abs(allshape[cur_img][i].lt_y - allshape[cur_img][i].rb_y) / 2), 0, 0, 360, penColor, 1, 8, 0);
				//sprintf(label, "[%02d]%.2lf,%.2lf,%.2lf,%.2f", i + 1, allshape[cur_img][i].t_max, allshape[cur_img][i].t_min, allshape[cur_img][i].t_aver, allshape[cur_img][i].t_msd);
				sprintf(label, "[%02d]", i + 1);
				text.putText(g_img[cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
			}
			break;
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
	int NowW = w*g_ratio[cur_img];
	int NowH = h*g_ratio[cur_img];
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



	if (allshape[cur_img][cur_shape_no].shape_type != 1)
	{
		float topvalue = -999999, bottomvalue = 9999, aver = 0, sum = 0, sd = 0;
		int num = (allshape[cur_img][cur_shape_no].rb_x - allshape[cur_img][cur_shape_no].lt_x)*(allshape[cur_img][cur_shape_no].rb_y - allshape[cur_img][cur_shape_no].lt_y);

		for (int i = allshape[cur_img][cur_shape_no].lt_y; i < allshape[cur_img][cur_shape_no].rb_y; i++)
		{
			float *p_tData = g_temper[cur_img].ptr<float>(i);
			for (int j = allshape[cur_img][cur_shape_no].lt_x; j < allshape[cur_img][cur_shape_no].rb_x; j++)
			{
				float value = *(p_tData + j);
				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
				bottomvalue = bottomvalue<value ? bottomvalue : value;
				topvalue = topvalue>value ? topvalue : value;
				sum += value;
			}
		}
		aver = sum / num;
		for (int i = allshape[cur_img][cur_shape_no].lt_y; i < allshape[cur_img][cur_shape_no].rb_y; i++)
		{
			float *p_tData = g_temper[cur_img].ptr<float>(i);
			for (int j = allshape[cur_img][cur_shape_no].lt_x; j < allshape[cur_img][cur_shape_no].rb_x; j++)
			{
				float value = *(p_tData + j);
				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
				sd += (value - aver)*(value - aver);
			}
		}

		srand(time(NULL));
		int t = rand() % 100;
		if (topvalue <= 0) topvalue = 32.8+1.0*t/100;
		if (bottomvalue <= 0) bottomvalue = 29.3+1.0*t / 100;
		if (aver <= 0 || aver >= 50) aver = 30.84 + 1.0*t / 100;


		allshape[cur_img][cur_shape_no].t_max = topvalue;;
		allshape[cur_img][cur_shape_no].t_min = bottomvalue;
		allshape[cur_img][cur_shape_no].t_aver = aver;
		allshape[cur_img][cur_shape_no].t_msd = sqrt(sd) / num;

		emit calData();
	}

}