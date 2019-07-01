#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QPushButton>
#include <QScrollArea>
#include <QFileDialog>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <imgProcDll.h>
#include <CvxText.h>

#include "IRProc.h"
#include "MyLabel.h"


using namespace cv;
using namespace std;


#define IMAGE_WIDTH  384//原始图像宽
#define IMAGE_HEIGHT 288//原始图像高

#define IMAGE_PER_ROW 5  //每行显示图像数
#define IMGE_TOTAL_NUM 12 //显示的图像总数
#define COMMENT_PER_IMAGE 30 //每张图像的标注数

int g_flagShowBigImg = 0;//显示大图还是小图的标志： 1-大图；0-小图
int g_picNum = 0;//读取的图像总数
int g_currentBigNum = 0;//当前大图下标
int g_cur_img = 0;//当前操作的图像下标

double g_ratio[IMGE_TOTAL_NUM];//图像放大倍数

unsigned short *g_pData[IMGE_TOTAL_NUM];//原始数据
Mat g_src[IMGE_TOTAL_NUM]; // opencv图像原始- 彩色
Mat g_src_gray[IMGE_TOTAL_NUM];//opencv图像原始- 灰度
Mat g_img[IMGE_TOTAL_NUM];//opencv图像-彩色
Mat g_img_gray[IMGE_TOTAL_NUM];//opencv图像-灰度
Mat g_temper[IMGE_TOTAL_NUM];//温度矩阵
QImage g_qImgShow[IMGE_TOTAL_NUM];//Qt原始图像-彩色
QImage g_qImgShow_gray[IMGE_TOTAL_NUM];//Qt原始图像-灰度

QPoint g_offset[IMGE_TOTAL_NUM] = {};//显示图像的偏移量

float g_referTemper = 25;//参考温度
int g_win_width = 16;//窗宽
int g_color_type = 2;// 颜色模式 0-灰度；1-传统伪彩色；2-TTM
int g_filter_type = 2;//滤波模式 0-中值；1-直方图均衡；2-不滤波
float g_bot = 22;//断层参考值，初始22

float g_step = 0.01;

Shape allshape[IMGE_TOTAL_NUM][COMMENT_PER_IMAGE] = { { false, 0, 0, 0, 0, 0, 0, 0, 0, 0, "None" } };
int g_shape_no[IMGE_TOTAL_NUM] = { 0 };//图像上标注的数量

int g_mouse_mode;//鼠标事件模式： 0-放大缩小，按住移动， 默认；1-加点；2-矩形；3-圆角矩形；4-椭圆，
int g_flag_showTemper;//显示温度标志： 0-不显示；1-显示

QString g_merge_path;
QString g_gender;



IRProc::IRProc(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);
	//QScrollArea *s = new QScrollArea(this);
	//s->setGeometry(20, 90, 1605,787);
	////垂直滚动条不可见，只能通过鼠标滑动
	////s->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//s->setWidget(ui.centralWidget);


	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(21,85,141);color:rgb(255,255,255);}"); //设置表头背景色
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setVisible(true);

	//setWindowTitle(tr("Main Window"));

	connect(ui.imgProcAct, &QAction::triggered, this, &IRProc::imgProc);
	connect(ui.imgMergeAct, &QAction::triggered, this, &IRProc::imgMerge);
	connect(ui.dataManageAct, &QAction::triggered, this, &IRProc::dataManage);
	connect(ui.sysSettingAct, &QAction::triggered, this, &IRProc::sysSetting);

	connect(ui.userAreaBt, SIGNAL(clicked()), this, SLOT(userAreaFull()));
	connect(ui.toolBarExpandBt, SIGNAL(clicked()), this, SLOT(toolBarExpand()));
	connect(ui.sysSettingBt, SIGNAL(clicked()), this, SLOT(sysSettingOp()));
	connect(ui.btn_analyze, SIGNAL(clicked()), this, SLOT(btnAnalyze()));
	connect(ui.btn_colorType_change, SIGNAL(clicked()), this, SLOT(colorTypeChange()));
	connect(ui.btn_set_step, SIGNAL(clicked()), this, SLOT(setStep()));
	connect(ui.btn_add_point, SIGNAL(clicked()), this, SLOT(addPoint()));
	connect(ui.btn_add_rect, SIGNAL(clicked()), this, SLOT(addRect()));
	connect(ui.btn_add_rect2, SIGNAL(clicked()), this, SLOT(addCircle()));
	connect(ui.btn_add_ellipse, SIGNAL(clicked()), this, SLOT(addEllipse()));

	connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_2, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_3, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_4, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_5, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_6, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_7, SIGNAL(clicked()), this, SLOT(customize()));

	connect(ui.btn_win_w4, SIGNAL(clicked()), this, SLOT(changeWinWidth()));
	connect(ui.btn_win_w5, SIGNAL(clicked()), this, SLOT(changeWinWidth()));
	connect(ui.btn_win_w6, SIGNAL(clicked()), this, SLOT(changeWinWidth()));
	connect(ui.btn_win_w8, SIGNAL(clicked()), this, SLOT(changeWinWidth()));
	connect(ui.btn_win_w10, SIGNAL(clicked()), this, SLOT(changeWinWidth()));
	connect(ui.btn_win_w12, SIGNAL(clicked()), this, SLOT(changeWinWidth()));

	connect(ui.cbox_smooth, SIGNAL(currentIndexChanged(int)), this, SLOT(setFilter(int)));



	//	changeLabel(g_picNum, IMAGE_PER_ROW);
	ui.cbox_smooth->addItem(QStringLiteral("中值"), 0);
	ui.cbox_smooth->addItem(QStringLiteral("直方图"), 1);
	ui.cbox_smooth->addItem(QStringLiteral("原始"), 2);


	ui.toolBar->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
		"background-color: rgb(19, 35, 67);"));

	//	changeLabel(10, IMAGE_PER_ROW);

	g_mouse_mode = 0;
	g_flag_showTemper = 1;
	this->setMouseTracking(true);
	ui.widget2->setMouseTracking(true);

	QDir dir;
	g_merge_path = dir.currentPath() + "//Refer_image//";
	g_gender = "Male";

	statusBar();

}


void IRProc::imgProc()
{
	//QMessageBox::information(this, tr("Information"), tr("imgProc"));

	ui.stackedWidget->setCurrentWidget(ui.pageProc);
	ui.stackedWidget_2->setCurrentWidget(ui.showpageImg);
	QIcon icon1;
	icon1.addFile(QStringLiteral(":/IRProc/ImgProc-Sel"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgProcAct->setIcon(icon1);

	QIcon icon2;
	icon2.addFile(QStringLiteral(":/IRProc/ImgMerg"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgMergeAct->setIcon(icon2);

	QIcon icon3;
	icon3.addFile(QStringLiteral(":/IRProc/Data"), QSize(), QIcon::Selected, QIcon::Off);
	ui.dataManageAct->setIcon(icon3);

	QIcon icon4;
	icon4.addFile(QStringLiteral(":/IRProc/Sys"), QSize(), QIcon::Selected, QIcon::Off);
	ui.sysSettingAct->setIcon(icon4);


}

void IRProc::imgMerge()
{
	//QMessageBox::information(this, tr("Information"), tr("imgMerge"));
	ui.stackedWidget->setCurrentWidget(ui.pageMerge);
	ui.stackedWidget_2->setCurrentWidget(ui.showpageImg);

	QIcon icon1;
	icon1.addFile(QStringLiteral(":/IRProc/ImgProc"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgProcAct->setIcon(icon1);

	QIcon icon2;
	icon2.addFile(QStringLiteral(":/IRProc/ImgMerge-Sel"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgMergeAct->setIcon(icon2);

	QIcon icon3;
	icon3.addFile(QStringLiteral(":/IRProc/Data"), QSize(), QIcon::Selected, QIcon::Off);
	ui.dataManageAct->setIcon(icon3);

	QIcon icon4;
	icon4.addFile(QStringLiteral(":/IRProc/Sys"), QSize(), QIcon::Selected, QIcon::Off);
	ui.sysSettingAct->setIcon(icon4);


}

void IRProc::dataManage()
{
	//QMessageBox::information(this, tr("Information"), tr("dataManage"));
	ui.stackedWidget->setCurrentWidget(ui.pageData);
	ui.stackedWidget_2->setCurrentWidget(ui.showpageData);

	QIcon icon1;
	icon1.addFile(QStringLiteral(":/IRProc/ImgProc"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgProcAct->setIcon(icon1);

	QIcon icon2;
	icon2.addFile(QStringLiteral(":/IRProc/ImgMerg"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgMergeAct->setIcon(icon2);

	QIcon icon3;
	icon3.addFile(QStringLiteral(":/IRProc/Data-Sel"), QSize(), QIcon::Selected, QIcon::Off);
	ui.dataManageAct->setIcon(icon3);

	QIcon icon4;
	icon4.addFile(QStringLiteral(":/IRProc/Sys"), QSize(), QIcon::Selected, QIcon::Off);
	ui.sysSettingAct->setIcon(icon4);
}

void IRProc::sysSetting()
{
	//QMessageBox::information(this, tr("Information"), tr("sysSetting"));
	ui.stackedWidget->setCurrentWidget(ui.pageSetting);
	ui.stackedWidget_2->setCurrentWidget(ui.showpageImg);

	QIcon icon1;
	icon1.addFile(QStringLiteral(":/IRProc/ImgProc"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgProcAct->setIcon(icon1);

	QIcon icon2;
	icon2.addFile(QStringLiteral(":/IRProc/ImgMerg"), QSize(), QIcon::Selected, QIcon::Off);
	ui.imgMergeAct->setIcon(icon2);

	QIcon icon3;
	icon3.addFile(QStringLiteral(":/IRProc/Data"), QSize(), QIcon::Selected, QIcon::Off);
	ui.dataManageAct->setIcon(icon3);

	QIcon icon4;
	icon4.addFile(QStringLiteral(":/IRProc/Sys-Sel"), QSize(), QIcon::Selected, QIcon::Off);
	ui.sysSettingAct->setIcon(icon4);
}

void IRProc::userAreaFull()
{
	QMessageBox::information(this, tr("Information"), QStringLiteral("<font size='26' color='white'>用户区最大化</font>"));

}

void IRProc::toolBarExpand()
{
	QMessageBox::information(this, tr("Information"), QStringLiteral("展开所有工具栏"));
}

void IRProc::sysSettingOp()
{
	QMessageBox::information(this, tr("Information"), QStringLiteral("系统设置"));
}

void IRProc::customize()
{

	QCheckBox *chbox = (QCheckBox*)this->sender();
	QString text = chbox->text();

	QMessageBox::information(this, tr("Information"), text);
}

void IRProc::changeWinWidth()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();

	g_win_width = text.toInt();

	updateImage();

}



void IRProc::imgChange()
{

	QPushButton *optBtn = qobject_cast<QPushButton *>(sender());
	QString name = sender()->objectName();//发送信号者的对象名



	if (g_flagShowBigImg == 0)
	{

		g_flagShowBigImg = 1;
		QString num = name.mid(2);
		int r = num.toInt();
		g_cur_img = g_currentBigNum = r;
		MyLabel *lb = new MyLabel;
		lb->setMaximumSize(480, 640);
		lb->setMinimumSize(480, 640);
		lb->setText(QString::number(r));
		lb->setObjectName(QString::number(r + 20));
		lb->setFrameShape(QFrame::Box);
		lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
		lb->getCurImgIndex();
		//lb->setMouseTracking(true);
		ui.gridLayout_6->addWidget(lb, 0, 0);

		QPushButton *bt = new QPushButton;

		bt->setText(QString::number(r));
		ui.gridLayout_6->addWidget(bt, 0, 0, Qt::AlignRight | Qt::AlignTop);
		bt->setMinimumSize(32, 32);
		bt->setMaximumSize(32, 32);
		bt->setObjectName(QString::number(r));
		bt->setStyleSheet(QLatin1String("color:rgb(255,255,255)"));
		connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

		ui.stackedWidget_21->setCurrentWidget(ui.pageBigImg);

		QPixmap pixmap = QPixmap::fromImage(g_qImgShow[r]);
		int with = lb->width();
		int height = lb->height();
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

		lb->setPixmap(fitpixmap);

	}
	else
	{
		int num = ui.gridLayout_6->count();
		if (num)
		{
			for (int i = num - 1; i >= 0; i--)
			{
				QWidget *p = ui.gridLayout_6->itemAt(i)->widget();
				ui.gridLayout_6->removeWidget(p);
				delete p;

			}

		}

		ui.stackedWidget_21->setCurrentWidget(ui.widget2);
		g_flagShowBigImg = 0;
	}


}

void IRProc::btnAnalyze()
{

	imgProc();

	QString filename;
	filename = QFileDialog::getOpenFileName(this, tr("Select Data"), "", tr("Data (*.dat)"));
	if (filename.isEmpty())
	{
		return;
	}
	char*  path;
	QByteArray t = filename.toLatin1(); // must
	path = t.data();

	Mat img;
	img.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	unsigned short *tmp = (unsigned short*)malloc(IMAGE_HEIGHT*IMAGE_WIDTH * sizeof(short));

	dataRead(tmp, path, IMAGE_HEIGHT* IMAGE_WIDTH);
	g_pData[g_picNum] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
	memcpy(g_pData[g_picNum], tmp, IMAGE_HEIGHT*IMAGE_WIDTH*sizeof(short));

	g_temper[g_picNum].create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_32FC1);

	data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);
	data2Img(g_pData[g_picNum], g_img_gray[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, 0, g_filter_type, g_bot);
	data2Img(g_pData[g_picNum], g_img[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, g_color_type, g_filter_type, g_bot);
	g_img[g_picNum].copyTo(g_src[g_picNum]);
	g_img_gray[g_picNum].copyTo(g_src_gray[g_picNum]);


	if (g_color_type)
	{
		QImage image = QImage((const unsigned char*)(g_img[g_picNum].data), g_img[g_picNum].cols, g_img[g_picNum].rows, QImage::Format_RGB888);
		g_qImgShow[g_picNum] = image.copy();
	}
	else
	{
		QImage image_gray = QImage((const unsigned char*)(g_img_gray[g_picNum].data), g_img_gray[g_picNum].cols, g_img_gray[g_picNum].rows, QImage::Format_Grayscale8);
		g_qImgShow_gray[g_picNum] = image_gray.copy();
	}

	g_ratio[g_picNum] = 1;
	g_picNum = g_picNum  % IMGE_TOTAL_NUM + 1;

	changeLabel(g_picNum, IMAGE_PER_ROW);

	showImage(g_picNum);
	//updateImage();
}


void IRProc::changeLabel(int totalNum, int imagePerRow)//调整显示窗口数
{
	for (int i = 0; i < totalNum; i++)
	{
		QLabel *p = ui.widget2->findChild<QLabel*>(QString::number(i));
		if (p != NULL) delete p;
	}

	int rows = (totalNum - 1) / imagePerRow + 1;
	int count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imagePerRow; y++)
		{
			MyLabel *lb = new MyLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			//lb->setFixedSize(240, 320);
			lb->setText(QString::number(x * imagePerRow + y));
			lb->setObjectName(QString::number(x * imagePerRow + y));
			lb->setFrameShape(QFrame::Box);
			ui.gridLayout_2->addWidget(lb, x, y);
			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
			lb->setAlignment(Qt::AlignCenter);
			lb->setMaximumSize(480, 640);
			lb->getCurImgIndex();
			//lb->setMouseTracking(true);
			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

	count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imagePerRow; y++)
		{
			QPushButton *bt = new QPushButton;
			//lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

			bt->setText(QString::number(x *imagePerRow + y));
			ui.gridLayout_2->addWidget(bt, x, y, Qt::AlignRight | Qt::AlignTop);
			bt->setMinimumSize(32, 32);
			bt->setMaximumSize(32, 32);
			bt->setObjectName("bt" + QString::number(x * imagePerRow + y));
			bt->setStyleSheet(QLatin1String("color:rgb(255,255,255)"));
			connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

}


void IRProc::showImage(int pic_num)
{

	if (g_flagShowBigImg)
	{

		MyLabel *p = ui.pageBigImg->findChild<MyLabel*>(QString::number(g_cur_img + 20));

		p->getCurImgIndex();
		p->draw_shape(g_shape_no[g_cur_img]);
		p->setOffset(g_offset[g_cur_img]);

		int with = p->width();
		int height = p->height();
		int i = g_cur_img;
		if (g_color_type)
		{
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow[i]);
			//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
			p->setPixmap(fitpixmap);

			int w = p->pixmap()->width();
			int h = p->pixmap()->height();
			Mat timg;

			cv::resize(g_img[i], timg, Size(w * g_ratio[i], h * g_ratio[i]), 0, 0);

			int m_mx = w * g_ratio[i] / 2 - w / 2;
			int m_my = h * g_ratio[i] / 2 - h / 2;
			Rect rect1(m_mx, m_my, w, h);
			//	Rect rect2(224, 224, 128, 128);
			Mat roi1;
			timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

			QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
			//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

			p->setPixmap(QPixmap::fromImage(image));

			p->update();

			timg.release();
			roi1.release();

		}
		else
		{
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow_gray[i]);
			//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
			p->setPixmap(fitpixmap);

			int w = p->pixmap()->width();
			int h = p->pixmap()->height();
			Mat timg;

			cv::resize(g_img_gray[i], timg, Size(w * g_ratio[i], h * g_ratio[i]), 0, 0);

			int m_mx = w * g_ratio[i] / 2 - w / 2;
			int m_my = h * g_ratio[i] / 2 - h / 2;
			Rect rect1(m_mx, m_my, w, h);
			//	Rect rect2(224, 224, 128, 128);
			Mat roi1;
			timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

			//	QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
			QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_Grayscale8);

			p->setPixmap(QPixmap::fromImage(image));
			p->update();
			timg.release();
			roi1.release();
		}
	}
	else
	{
		changeLabel(g_picNum, IMAGE_PER_ROW);
		for (int i = 0; i < g_picNum; i++)
		{
			MyLabel *p = ui.widget2->findChild<MyLabel*>(QString::number(i));

			//p->getCurImgIndex();
			p->draw_shape(g_shape_no[i]);
			p->setOffset(g_offset[i]);

			int with = p->width();
			int height = p->height();

			if (g_color_type)
			{
				QPixmap pixmap = QPixmap::fromImage(g_qImgShow[i]);
				//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
				QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
				p->setPixmap(fitpixmap);

				int w = p->pixmap()->width();
				int h = p->pixmap()->height();
				Mat timg;

				cv::resize(g_img[i], timg, Size(w * g_ratio[i], h * g_ratio[i]), 0, 0);
			
				int m_mx = w * g_ratio[i] / 2 - w / 2;
				int m_my = h * g_ratio[i] / 2 - h / 2;
				Rect rect1(m_mx, m_my, w, h);
				//	Rect rect2(224, 224, 128, 128);
				Mat roi1;
				timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

				QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
				//QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, QImage::Format_Grayscale8);

				p->setPixmap(QPixmap::fromImage(image));
				p->update();
				timg.release();
				roi1.release();

			}
			else
			{
				QPixmap pixmap = QPixmap::fromImage(g_qImgShow_gray[i]);
				//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
				QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
				p->setPixmap(fitpixmap);

				int w = p->pixmap()->width();
				int h = p->pixmap()->height();
				Mat timg;

				cv::resize(g_img_gray[i], timg, Size(w * g_ratio[i], h * g_ratio[i]), 0, 0);

				int m_mx = w * g_ratio[i] / 2 - w / 2;
				int m_my = h * g_ratio[i] / 2 - h / 2;
				Rect rect1(m_mx, m_my, w, h);
				//	Rect rect2(224, 224, 128, 128);
				Mat roi1;
				timg(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

				//	QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_RGB888);
				QImage image = QImage((const unsigned char*)(roi1.data), roi1.cols, roi1.rows, QImage::Format_Grayscale8);

				p->setPixmap(QPixmap::fromImage(image));
				p->update();
				timg.release();
				roi1.release();
			}


		}
	}

}

void IRProc::updateImage()
{
	for (int i = 0; i < g_picNum; i++)
	{
		if (g_color_type)
		{
			data2Img(g_pData[i], g_img[i], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, g_color_type, g_filter_type, g_bot);
			g_img[i].copyTo(g_src[i]);

			//draw_shape(g_img[i], allshape[i], g_shape_no[i]);
			QImage image = QImage((const unsigned char*)(g_img[i].data), g_img[i].cols, g_img[i].rows, QImage::Format_RGB888);
			g_qImgShow[i] = image.copy();
		}
		else
		{
			data2Img(g_pData[i], g_img_gray[i], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, 0, g_filter_type, g_bot);
			g_img_gray[i].copyTo(g_src_gray[i]);
			//draw_shape(g_img_gray[i], allshape[i], g_shape_no[i]);
			QImage image_gray = QImage((const unsigned char*)(g_img_gray[i].data), g_img_gray[i].cols, g_img_gray[i].rows, QImage::Format_Grayscale8);
			g_qImgShow_gray[i] = image_gray.copy();
		}
	}

	showImage(g_picNum);
}

void IRProc::wheelEvent(QWheelEvent*event)
{
	g_bot += 1.0*event->delta() / 120 * g_step;
	updateImage();

}

void IRProc::colorTypeChange()
{
	g_color_type = (g_color_type + 1) % 3;
	updateImage();
}

void IRProc::setStep()
{
	g_step = ui.lineEdit_step->text().toFloat();

}

void IRProc::setFilter(int)
{
	g_filter_type = ui.cbox_smooth->currentIndex();
	updateImage();
}


//void IRProc::mousePressEvent(QMouseEvent *event)
//{   //单击
//	switch (g_mouse_mode)
//	{
//
//	case 0:
//	{// 如果是鼠标左键按下
//			  if (event->button() == Qt::LeftButton)
//			  {
//
//				  int mouse_x = QCursor::pos().x();//鼠标点击处横坐标
//				  int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标
//				  QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//获取鼠标点击处的控件
//				  if (action == NULL)
//				  {
//					  return;
//				  }
//				  QString lname = action->objectName();
//				  if (g_flagShowBigImg)
//					  g_cur_img = lname.toInt() - 20;
//				  else
//					  g_cur_img = lname.toInt();
//
//				  g_ratio[g_cur_img] += 0.1;
//				  if (g_ratio[g_cur_img] > 3) g_ratio[g_cur_img] = 3;
//				  showImage(g_picNum);
//			  }
//			  // 如果是鼠标右键按下
//			  else if (event->button() == Qt::RightButton)
//			  {
//				  //	QMessageBox::information(NULL, "Title", "r click"); 
//				  //setMouseState(MouseState::R_C, 0);
//				  int mouse_x = QCursor::pos().x();//鼠标点击处横坐标
//				  int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标
//				  QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//获取鼠标点击处的控件
//				  if (action == NULL)
//				  {
//					  return;
//				  }
//				  //QMessageBox::information(NULL, "Title", action->objectName());
//
//				  QString lname = action->objectName();
//
//				  if (g_flagShowBigImg)
//					  g_cur_img = lname.toInt() - 20;
//				  else
//					  g_cur_img = lname.toInt();
//
//				  g_ratio[g_cur_img] -= 0.1;
//				  if (g_ratio[g_cur_img] < 1) g_ratio[g_cur_img] = 1;
//
//
//				  showImage(g_picNum);
//			  }
//			  break;
//	}
//		case 1:
//		{ 
//				 if (event->button() == Qt::LeftButton)
//				{
//					  Mat imgt;
//					  g_img[0].copyTo(imgt);
//					  int i = 0;
//					  char label[1000];
//					  char label2[1000];
//					  int mouse_x = QCursor::pos().x();//鼠标点击处横坐标
//					  int mouse_y = QCursor::pos().y();//鼠标点击处纵坐标
//
//					  QWidget *action = QApplication::widgetAt(mouse_x, mouse_y);//获取鼠标点击处的控件
//					  if (action == NULL)
//					  {
//						  return;
//					  }
//					  QString lname = action->objectName();
//			
//					  for ( i= 0; i < g_picNum; i++)
//					  {
//						  if (QString::number(i) == lname)
//							  break;
//					  }
//					  if (i >= g_picNum) return;
//					  
//					  QLabel *p;
//					  if (g_flagShowBigImg)
//					  {
//						  g_cur_img = lname.toInt() - 20;
//						  p = ui.pageBigImg->findChild<QLabel*>(lname);
//					  }
//					  else
//					  {
//						  g_cur_img = lname.toInt();
//						  p = ui.widget2->findChild<QLabel*>(lname);
//					  }
//
//					  QPoint curPoint = event->globalPos();
//					  curPoint = p->mapFromGlobal(curPoint);
//
//					  int w = p->pixmap()->width();
//					  int h = p->pixmap()->height();
//
//					  int lw = p->width();
//					  int lh = p->height();
//
//					  int x = curPoint.x() - (lw - w) / 2;
//					  int y = curPoint.y() - (lh - h) / 2;
//
//					  //int x = 50;
//					  //int y = 60;
//					  if (x < 0) x = 0;
//					  if (x >= w) x = w- 1;
//					  if (y < 0) y = 0;
//					  if (y >= h) y = h - 1;
//
//					  //		int tmp = imgt.at<uchar>(x, y);
//
//					  Mat T;
//					  T.create(Size(w, h), CV_32FC1);
//					  cv::resize(g_temper[0], T, Size(w, h));
//					  cv::resize(imgt, imgt, Size(w, h));
//
//					  float temper = T.at<float>(y, x);
//					  sprintf(label, "温度:%.2f)", temper);
//					  sprintf(label2, "坐标:(%d, %d)", x, y);	//获取坐标点位置
//					 rectangle(imgt, Point(x-1, y-1), Point(x + 1, y + 1),CV_RGB(255, 255, 255), CV_FILLED, 8, 0);
//					  /*	putText(imgt, label, Point(x, y), FONT_HERSHEY_SIMPLEX, 0.2, Scalar(255, 23, 0), 1, 8);
//					  putText(imgt, label2, Point(x, y + 20), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(255, 23, 0), 1, 8);*/
//
//					  CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
//					  cv::Scalar size{ 12, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
//					  text.setFont(nullptr, &size, nullptr, 0);
//					  text.putText(imgt, label, Point(x+2, y+2), Scalar(255, 23, 0));
//					  text.putText(imgt, label2, Point(x+50, y + 2), Scalar(255, 23, 0));
//
//					  QImage image = QImage((const unsigned char*)(imgt.data), imgt.cols, imgt.rows, QImage::Format_RGB888);
//					  p->setPixmap(QPixmap::fromImage(image));
//
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].shape_type = g_mouse_mode;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].lt_x = x*IMAGE_HEIGHT/w;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].lt_y = y*IMAGE_WIDTH/h;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].rb_x = x*IMAGE_HEIGHT / w;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].rb_y = y*IMAGE_WIDTH / h;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].t_max = temper;;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].t_min = temper;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].t_aver = temper;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].t_msd = 0;
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].comment = "None";
//					  allshape[g_cur_img][g_shape_no[g_cur_img]].del_flag = false;
//					  g_shape_no[g_cur_img]++;
//
//					  draw_shape(g_img[g_cur_img], allshape[g_cur_img], g_shape_no[g_cur_img]);
//					  showImage(g_cur_img);
//				}
//				 
//		}
//	}
//}


void IRProc::addPoint()
{
	if (g_mouse_mode != 1)
	{
		g_mouse_mode = 1;
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));

	}
	else
	{
		g_mouse_mode = 0;
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}

void IRProc::addRect()
{
	if (g_mouse_mode != 2)
	{
		g_mouse_mode = 2;
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 0;
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}

void IRProc::addCircle()
{
	if (g_mouse_mode != 3)
	{
		g_mouse_mode = 3;
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 0;
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}

void IRProc::addEllipse()
{
	if (g_mouse_mode != 4)
	{
		g_mouse_mode = 4;
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 0;
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}





void IRProc::calPar(Mat &T, Shape &s)
{
	float topvalue = -999999, bottomvalue = 9999, aver = 0, sum = 0, sd = 0;
	int num = (s.rb_x - s.lt_x)*(s.rb_y - s.lt_y);

	if (s.shape_type != 1)
	{
		for (int i = s.lt_y; i < s.rb_y; i++)
		{
				float *p_tData = T.ptr<float>(i);
				for (int j = s.lt_x; j < s.rb_x; j++)
				{
					float value = *(p_tData + j);
					//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
					bottomvalue = bottomvalue<value ? bottomvalue : value;
					topvalue = topvalue>value ? topvalue : value;
					sum += value;
				}
		}
		aver = sum / num;
		for (int i = s.lt_y; i < s.rb_y; i++)
		{
			float *p_tData = T.ptr<float>(i);
			for (int j = s.lt_x; j < s.rb_x; j++)
			{
				float value = *(p_tData + j);
				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
				sd += (value - aver)*(value - aver);
			}
		}
		s.t_max = topvalue;;
		s.t_min = bottomvalue;
		s.t_aver = aver;
		s.t_msd = sqrt(sd) / num;
	}
	
}

//void IRProc::draw_shape(Mat& img, Shape allshape[], int shape_no)
//{
//	CvxText text("C:\\Windows\\Fonts\\simhei.ttf"); //指定字体
//	cv::Scalar size{ 12, 0, 0.1, 0 }; // (字体大小, 无效的, 字符间距, 无效的 }
//	text.setFont(nullptr, &size, nullptr, 0);
//	char label[1000];
//	for (int i = 0; i < shape_no;i++)
//	{
//		int x = allshape[i].lt_x;
//		int y = allshape[i].lt_y;
//
//		switch (allshape[i].shape_type)
//		{
//			case 1:
//			{
//					  sprintf(label, "[%d]- %.2f)", i, allshape[i].t_max);
//					  rectangle(img, Point(x - 1, y - 1), Point(x + 1, y + 1), CV_RGB(255, 255, 255), CV_FILLED, 8, 0);
//					  text.putText(img, label, Point(x + 4, y + 4), Scalar(0, 0, 0));
//			}
//			case 2:
//			{
//					  rectangle(g_img[g_cur_img], Point(allshape[i].lt_x, allshape[i].lt_y), Point(allshape[i].rb_x, allshape[i].rb_y), Scalar(255, 255, 255), 1, 8, 0);
//					  sprintf(label, "([%02d]-%.2lf,%.2lf,%.2lf,%.2f)", i + 1, allshape[i].t_max, allshape[i].t_min, allshape[i].t_aver, allshape[i].t_msd);
//					  text.putText(g_img[g_cur_img], label, Point(x + 4, y + 4), Scalar(0, 0, 0));
//			}
//		}
//		
//	}
//}

