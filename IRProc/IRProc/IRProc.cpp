#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QPushButton>
#include <QScrollArea>


#include "IRProc.h"


extern int g_picTotalNum;
extern int flagShowBigImg;



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
	connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_2, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_3, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_4, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_5, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_6, SIGNAL(clicked()), this, SLOT(customize()));
	connect(ui.checkBox_7, SIGNAL(clicked()), this, SLOT(customize()));


	g_picTotalNum = 8;
	int count = 0;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < (g_picTotalNum - 1) / 2 + 1; y++)
		{
			QLabel *lb = new QLabel;
			lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
			//lb->setFixedSize(240, 320);
			lb->setText(QString::number(x * ((g_picTotalNum - 1) / 2 + 1) + y));
			lb->setFrameShape(QFrame::Box);
			ui.gridLayout_2->addWidget(lb, x, y);
			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255)"));
			count++;
			if (count >= g_picTotalNum) break;
		}
		if (count >= g_picTotalNum) break;
	}

	count = 0;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < (g_picTotalNum - 1) / 2 + 1; y++)
		{
			QPushButton *bt = new QPushButton;
			//lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

			bt->setText(QString::number(x *((g_picTotalNum - 1) / 2 + 1) + y));
			ui.gridLayout_2->addWidget(bt, x, y, Qt::AlignRight | Qt::AlignTop);
			bt->setMinimumSize(32, 32);
			bt->setMaximumSize(32, 32);
			bt->setObjectName("bt" + QString::number(x * ((g_picTotalNum - 1) / 2 + 1) + y));
			bt->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255)"));
			connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));

			count++;
			if (count >= g_picTotalNum) break;
		}
		if (count >= g_picTotalNum) break;
	}

	ui.toolBar->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
		"background-color: rgb(19, 35, 67);"));



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

void IRProc::imgChange()
{

	QPushButton *optBtn = qobject_cast<QPushButton *>(sender());		
	QString name = sender()->objectName();//发送信号者的对象名



	if (flagShowBigImg == 0)
	{
		QString num = name.mid(2);
		int r = num.toInt();

	/*	QList<QLabel*> labelList = ui.widget2->findChildren<QLabel*>();

		QLabel* label = labelList.at(r);
		label->setMaximumSize(640, 480);
		label->setMinimumSize(640, 480);

		

		ui.gridLayout_6->addWidget(label,0,0);

		QList<QPushButton*> btList = ui.widget2->findChildren<QPushButton*>();

		QPushButton *button = btList.at(r);

		ui.gridLayout_6->addWidget(button, 0, 0, Qt::AlignRight | Qt::AlignTop);*/

		QLabel *lb = new QLabel;
		lb->setMaximumSize(480, 640);
		lb->setMinimumSize(480, 640);
		lb->setText(QString::number(r));
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