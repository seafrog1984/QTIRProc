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


#include "IRProc.h"


extern int flagShowBigImg;//显示大图还是小图的标志： 1-大图；0-小图

using namespace cv;
using namespace std;


#define IMAGE_WIDTH  384//原始图像宽
#define IMAGE_HEIGHT 288//原始图像高

#define IMAGE_PER_ROW 5  //每行显示图像数
#define IMGE_TOTAL_NUM 10 //显示的图像总数

int g_picNum = 0;//读取的图像总数
int g_currentBigNum = 0;//当前大图下标


unsigned short *g_pData[12];//原始数据
Mat g_img[12];//opencv原始图像-彩色
Mat g_img_gray[12];//opencv原始图像-灰度
Mat g_temper[12];//温度矩阵
QImage g_qImgShow[12];//Qt原始图像-彩色
QImage g_qImgShow_gray[12];//Qt原始图像-灰度

float g_referTemper = 25;//参考温度
int g_win_width = 16;//窗宽
int g_color_type = 2;// 颜色模式 0-灰度；1-传统伪彩色；2-TTM
int g_filter_type = 2;//滤波模式 0-中值；1-直方图均衡；2-不滤波
float g_bot = 22;//断层参考值，初始22

float g_step = 0.01;



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



	if (flagShowBigImg == 0)
	{
		QString num = name.mid(2);
		int r = num.toInt();
		g_currentBigNum = r;
		QLabel *lb = new QLabel;
		lb->setMaximumSize(480, 640);
		lb->setMinimumSize(480, 640);
		lb->setText(QString::number(r));
		lb->setObjectName("Big"+QString::number(r));
		lb->setFrameShape(QFrame::Box);
		lb->setStyleSheet("border: 1px solid  #000000");
		ui.gridLayout_6->addWidget(lb, 0, 0);

		QPushButton *bt = new QPushButton;
		
		bt->setText(QString::number(r));
		ui.gridLayout_6->addWidget(bt, 0, 0, Qt::AlignRight | Qt::AlignTop);
		bt->setMinimumSize(32, 32);
		bt->setMaximumSize(32, 32);
		bt->setObjectName(QString::number(r));
		connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

		ui.stackedWidget_21->setCurrentWidget(ui.pageBigImg);

		QPixmap pixmap = QPixmap::fromImage(g_qImgShow[r]);
		int with = lb->width();
		int height = lb->height();
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

		lb->setPixmap(fitpixmap);

		flagShowBigImg = 1;
	}
	else
	{
		int num = ui.gridLayout_6->count();
		if (num)
		{
			for (int i = num-1; i >=0; i--)
			{
				QWidget *p = ui.gridLayout_6->itemAt(i)->widget();
				ui.gridLayout_6->removeWidget(p);
				delete p;

			}

		}

		ui.stackedWidget_21->setCurrentWidget(ui.widget2);
		flagShowBigImg = 0;
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

	g_temper[g_picNum].create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_32FC1);
	data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);

	if (g_color_type)
	{
		data2Img(g_pData[g_picNum], g_img[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH,  g_win_width, g_color_type, g_filter_type, g_bot);
		QImage image = QImage((const unsigned char*)(g_img[g_picNum].data), g_img[g_picNum].cols, g_img[g_picNum].rows, QImage::Format_RGB888);
		g_qImgShow[g_picNum] = image.copy();
	}
	else
	{
		data2Img(g_pData[g_picNum], g_img_gray[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, 0, g_filter_type, g_bot);
		QImage image_gray = QImage((const unsigned char*)(g_img_gray[g_picNum].data), g_img_gray[g_picNum].cols, g_img_gray[g_picNum].rows, QImage::Format_Grayscale8);
		g_qImgShow_gray[g_picNum] = image_gray.copy();
	}

	g_picNum = g_picNum  % IMGE_TOTAL_NUM +1;

	changeLabel(g_picNum, IMAGE_PER_ROW);
	showImage(g_picNum);
}


void IRProc::changeLabel(int totalNum, int imagePerRow)//调整显示窗口数
{
	for (int i = 0; i < totalNum; i++)
	{
		QLabel *p = ui.widget2->findChild<QLabel*>(QString::number(i));
		if (p != NULL) delete p;
	}

	int rows = (totalNum-1) / imagePerRow +1;
	int count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imagePerRow; y++)
		{
			QLabel *lb = new QLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			//lb->setFixedSize(240, 320);
			lb->setText(QString::number(x * imagePerRow + y));
			lb->setObjectName(QString::number(x * imagePerRow + y));
			lb->setFrameShape(QFrame::Box);
			ui.gridLayout_2->addWidget(lb, x, y);
			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
			lb->setAlignment(Qt::AlignCenter);
			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

	count = 0;
	for (int x = 0; x <rows; x++)
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
			bt->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255)"));
			connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

}


void IRProc::showImage(int pic_num)
{

	for (int i = 0; i < g_picNum; i++)
	{
		QLabel *p = ui.widget2->findChild<QLabel*>(QString::number(i));
		int with = p->width();
		int height = p->height();

		if (g_color_type)
		{	
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow[i]);
			//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
			p->setPixmap(fitpixmap);
		}
		else
		{
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow_gray[i]);
			//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
			p->setPixmap(fitpixmap);
		}

	
	}
}

void IRProc::updateImage()
{
	for (int i = 0; i < g_picNum; i++)
	{
		if (g_color_type)
		{
			data2Img(g_pData[i], g_img[i], IMAGE_HEIGHT, IMAGE_WIDTH,  g_win_width, g_color_type, g_filter_type, g_bot);
			QImage image = QImage((const unsigned char*)(g_img[i].data), g_img[i].cols, g_img[i].rows, QImage::Format_RGB888);
			g_qImgShow[i] = image.copy();
		}
		else
		{
			data2Img(g_pData[i], g_img_gray[i], IMAGE_HEIGHT, IMAGE_WIDTH,  g_win_width, 0, g_filter_type, g_bot);
			QImage image_gray = QImage((const unsigned char*)(g_img_gray[i].data), g_img_gray[i].cols, g_img_gray[i].rows, QImage::Format_Grayscale8);
			g_qImgShow_gray[i] = image_gray.copy();

		}

	}

	if (flagShowBigImg)
	{
		QLabel *p = ui.pageBigImg->findChild<QLabel*>("Big" + QString::number(g_currentBigNum));

		int with = p->width();
		int height = p->height();
		if (g_color_type)
		{
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow[g_currentBigNum]);
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放

			p->setPixmap(fitpixmap);
		}
		else
		{
			QPixmap pixmap = QPixmap::fromImage(g_qImgShow_gray[g_currentBigNum]);
			QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
			p->setPixmap(fitpixmap);
		}

	}
	else
	{
		showImage(g_picNum);
	}

}

void IRProc::wheelEvent(QWheelEvent*event)
{
	g_bot += 1.0*event->delta() / 120 *g_step;
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
	g_filter_type=ui.cbox_smooth->currentIndex();
	updateImage();
}