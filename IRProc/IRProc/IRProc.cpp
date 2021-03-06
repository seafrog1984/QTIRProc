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
#include <QTimer>


#include "IRProc.h"
#include "MyLabel.h"


using namespace cv;
using namespace std;


int IMAGE_WIDTH = 640;//原始图像宽
int IMAGE_HEIGHT = 480;//原始图像高

#define IMAGE_PER_ROW 5  //每行显示图像数
#define IMGE_TOTAL_NUM 12 //显示的图像总数
#define COMMENT_PER_IMAGE 30 //每张图像的标注数
#define BIG_IMG_BASE 20 //显示大图的Label编号基数
#define THUM_IMG_BASE 30 //显示缩略图的Label编号基数
#define BIG_TOTAL_NUM 3
#define BIG_WIDTH 540
#define BIG_HEIGHT 720
#define MER_RATIO_STEP 0.01//需测试
#define MER_STEP 1


int g_show_progress = 1;//显示进度条标志，首次读取数据时显示

int g_flagShowBigImg = 0;//显示大图还是小图的标志： 1-大图；0-小图
int g_picNum = 0;//读取的图像总数
int g_cur_img = 0;//当前操作的图像下标

int g_bigNum = 0;//大图的数量，最大为3
int g_big_show[BIG_TOTAL_NUM] = { 0 };//显示的大图数量
int g_bigIndex[3] = { -1,-1,-1};//存储大图的原始下标
int g_curBig = 0;//当前大图的下标

double g_ratio[IMGE_TOTAL_NUM];//图像放大倍数
int g_size_index[IMGE_TOTAL_NUM] = { 0 };
int g_img_show_flag[IMGE_TOTAL_NUM] = { 0 };//图像显示标志

unsigned short *g_pData[IMGE_TOTAL_NUM];//原始数据

Mat g_mer_src[IMGE_TOTAL_NUM];//原始融合图像
Mat g_mer[IMGE_TOTAL_NUM];//融合图像
Mat g_src[IMGE_TOTAL_NUM]; // opencv图像原始- 彩色
Mat g_src_gray[IMGE_TOTAL_NUM];//opencv图像原始- 灰度
Mat g_img[IMGE_TOTAL_NUM];//opencv图像-彩色
Mat g_img_gray[IMGE_TOTAL_NUM];//opencv图像-灰度
Mat g_temper[IMGE_TOTAL_NUM];//温度矩阵
double g_TR[IMGE_TOTAL_NUM] = { 0 };//图像的温度均方差
QImage g_qImgShow[IMGE_TOTAL_NUM];//Qt原始图像-彩色
QImage g_qImgShow_gray[IMGE_TOTAL_NUM];//Qt原始图像-灰度

QPoint g_offset[IMGE_TOTAL_NUM] = {};//显示图像的偏移量

float g_referTemper = 25;//参考温度
int g_win_width[IMGE_TOTAL_NUM];//窗宽
int g_color_type = 2;// 颜色模式 0-灰度；1-传统伪彩色；2-TTM
int g_filter_type = 2;//滤波模式 0-中值；1-直方图均衡；2-不滤波
float g_bot[IMGE_TOTAL_NUM];//断层参考值，初始22

extern int g_sel_tag[IMGE_TOTAL_NUM] = { 0 };//当前标签下标

float g_step = 0.01;

Shape allshape[IMGE_TOTAL_NUM][COMMENT_PER_IMAGE] = { { false, 0, 0, 0, 0, 0, 0, 0, 0, 0, "None" } };
int g_shape_no[IMGE_TOTAL_NUM] = { 0 };//图像上标注的数量

int g_mouse_mode=6;//鼠标事件模式： 0-放大缩小，按住移动， 默认；1-加点；2-矩形；3-圆角矩形；4-椭圆；5-删除；6-选择模式
int g_flag_showTemper=0;//显示温度标志： 0-不显示；1-显示

int g_remember_flag;//记住密码标志
int g_upAll_flag=1;//同步断层标志

QString g_merge_path;
QString g_mer_gender;//融合图性别
QString g_mer_pose[IMGE_TOTAL_NUM];// 融合图姿势
QString g_mer_type[IMGE_TOTAL_NUM];//融合图类别
double g_mer_ratio[IMGE_TOTAL_NUM] = { 0 };//融合度
double g_mer_hratio[IMGE_TOTAL_NUM];//融合图水平放大倍数
int g_mer_flag[IMGE_TOTAL_NUM] = { 0 };//融合图打开标志

int g_offset_x[IMGE_TOTAL_NUM];//水平位移
double g_mer_vratio[IMGE_TOTAL_NUM];//融合图垂直放大倍数
int g_offset_y[IMGE_TOTAL_NUM];//垂直位移


extern QString g_ip;
extern QString g_port;
extern QString g_uport;

extern QString g_user;
extern QString g_passwd;

extern int iTestFlag;
extern client_t m_cli;
extern int g_log_flag;

std::vector<std::string> vecPngIDReq;
std::vector<std::string> vecPngIDResp;

QString g_name = "王五";
QString g_age = "20";
QString g_gender = "男";
QString g_ID = "id003";
QString g_scanID = "SCAN0016";
QString g_cardID = "CARD100000000003";
QString g_regTime = "2019-07-25 10:51";

QString g_dataFolder;//数据导出目录

int g_pageSize = 25;
int g_maxPage = 2;
int g_curPage = 1;

int pic_count = 0;

extern int g_reg_flag=0;

double g_cur_max=0;
double g_cur_min = 0;
double g_cur_mean = 0;

int g_sel_mode = 0;//筛选模式：0-初始化，1-按时间，2-按名字，3-全部显示


float inputEmiss=1;		//输入辐射率
float inputReflect=20;		//输入反射温度
float inputDis=2;			//输入距离


IRProc::IRProc(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);

	this->installEventFilter(this);

	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(21,85,141);color:rgb(255,255,255);}"); //设置表头背景色
	ui.tableWidget->horizontalHeader()->setSectionsClickable(false);
	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置单击选择一行
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//设置每行内容不可编辑
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);	//设置只能选择一行，不能选择多行

	ui.tagWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(21,85,141);color:rgb(255,255,255);}"); //设置表头背景色
	ui.tagWidget->horizontalHeader()->setSectionsClickable(false);
	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tagWidget->horizontalHeader()->setVisible(true);
	ui.tagWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tagWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置单击选择一行
//	ui.tagWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//设置每行内容不可编辑
	ui.tagWidget->setSelectionMode(QAbstractItemView::SingleSelection);	//设置只能选择一行，不能选择多行

	ui.dateEdit_start->setCalendarPopup(true);
	ui.dateEdit_end->setCalendarPopup(true);


	//setWindowTitle(tr("Main Window"));
	//主菜单
	connect(ui.imgProcAct, &QAction::triggered, this, &IRProc::imgProc);
	connect(ui.imgMergeAct, &QAction::triggered, this, &IRProc::imgMerge);
	connect(ui.dataManageAct, &QAction::triggered, this, &IRProc::dataManage);
	connect(ui.sysSettingAct, &QAction::triggered, this, &IRProc::sysSetting);
	connect(ui.btn_show_func, SIGNAL(clicked()), this, SLOT(btn_show_func()));

	//系统设置
	connect(ui.btn_sys_par, SIGNAL(clicked()), this, SLOT(btn_sysPar()));
	connect(ui.userAreaBt, SIGNAL(clicked()), this, SLOT(userAreaFull()));
	connect(ui.toolBarExpandBt, SIGNAL(clicked()), this, SLOT(toolBarExpand()));
	connect(ui.btn_set_auth, SIGNAL(clicked()), this, SLOT(sysSettingOp()));
	connect(ui.btn_cam_par, SIGNAL(clicked()), this, SLOT(btn_camPar()));


	//图像分析
	connect(ui.btn_colorType_change, SIGNAL(clicked()), this, SLOT(colorTypeChange()));
//	connect(ui.btn_set_step, SIGNAL(clicked()), this, SLOT(setStep()));
	connect(ui.btn_add_point, SIGNAL(clicked()), this, SLOT(addPoint()));
	connect(ui.btn_add_rect, SIGNAL(clicked()), this, SLOT(addRect()));
	connect(ui.btn_add_rect2, SIGNAL(clicked()), this, SLOT(addCircle()));
	connect(ui.btn_add_ellipse, SIGNAL(clicked()), this, SLOT(addEllipse()));
	connect(ui.btn_hist, SIGNAL(clicked()), this, SLOT(btn_hist()));
	connect(ui.btn_cross, SIGNAL(clicked()), this, SLOT(btn_cross()));
	connect(ui.btn_close_all, SIGNAL(clicked()), this, SLOT(btn_closeAll()));
	connect(ui.btn_close_other, SIGNAL(clicked()), this, SLOT(btn_closeOther()));
	connect(ui.btn_sel, SIGNAL(clicked()), this, SLOT(btn_sel()));
	connect(ui.btn_bigger, SIGNAL(clicked()), this, SLOT(btn_zoom()));
	connect(ui.btn_smaller, SIGNAL(clicked()), this, SLOT(btn_zoom()));
	connect(ui.show_temper, SIGNAL(clicked()), this, SLOT(showTemper()));
	connect(ui.btn_save_measure, SIGNAL(clicked()), this, SLOT(saveMeasure()));
	connect(ui.update_all, SIGNAL(clicked()), this, SLOT(setUpAll()));
	connect(ui.btn_sel_tag, SIGNAL(clicked()), this, SLOT(tagSel()));
	connect(ui.btn_del_tag, SIGNAL(clicked()), this, SLOT(tagDel()));
	connect(ui.toolButton_up, SIGNAL(clicked()), this, SLOT(keyUp()));
	connect(ui.toolButton_down, SIGNAL(clicked()), this, SLOT(keyDown()));
	connect(ui.toolButton_ctrl_up, SIGNAL(clicked()), this, SLOT(keyCtrlUp()));
	connect(ui.toolButton_ctrl_down, SIGNAL(clicked()), this, SLOT(keyCtrlDown()));
	connect(ui.toolButton_shift_up, SIGNAL(clicked()), this, SLOT(keyShiftUp()));
	connect(ui.toolButton_shift_down, SIGNAL(clicked()), this, SLOT(keyShiftDown()));
	connect(ui.comboBox_user, SIGNAL(currentIndexChanged(int)), this, SLOT(nameChanged(int)));

	connect(ui.listWidget_date, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(scanChanged(QListWidgetItem*)));

	ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));

	//图像融合
	connect(ui.btn_mer_def, SIGNAL(clicked()), this, SLOT(btnMerDef()));
	connect(ui.btn_mer_wid, SIGNAL(clicked()), this, SLOT(btnMerWid()));
	connect(ui.btn_mer_nar, SIGNAL(clicked()), this, SLOT(btnMerNar()));
	connect(ui.btn_mer_higher, SIGNAL(clicked()), this, SLOT(btnMerHigher()));
	connect(ui.btn_mer_lower, SIGNAL(clicked()), this, SLOT(btnMerLower()));
	connect(ui.btn_mer_left, SIGNAL(clicked()), this, SLOT(btnMerLeft()));
	connect(ui.btn_mer_right, SIGNAL(clicked()), this, SLOT(btnMerRight()));
	connect(ui.btn_mer_up, SIGNAL(clicked()), this, SLOT(btnMerUp()));
	connect(ui.btn_mer_down, SIGNAL(clicked()), this, SLOT(btnMerDown()));

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

	connect(ui.btn_male, SIGNAL(clicked()), this, SLOT(changeMerGender()));
	connect(ui.btn_female, SIGNAL(clicked()), this, SLOT(changeMerGender()));

	connect(ui.btn_pose_front, SIGNAL(clicked()), this, SLOT(changeMerPose()));
	connect(ui.btn_pose_back, SIGNAL(clicked()), this, SLOT(changeMerPose()));
	connect(ui.btn_pose_left, SIGNAL(clicked()), this, SLOT(changeMerPose()));
	connect(ui.btn_pose_right, SIGNAL(clicked()), this, SLOT(changeMerPose()));

	connect(ui.btn_mer_type1, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type2, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type3, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type4, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type5, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type6, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type7, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_type8, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_1, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_2, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_3, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_4, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_5, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_6, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_7, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_8, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_7, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_hb, SIGNAL(clicked()), this, SLOT(changeMerType()));
	connect(ui.btn_mer_meridian, SIGNAL(clicked()), this, SLOT(changeMerType()));

	

	//数据管理
	connect(ui.btn_analyze, SIGNAL(clicked()), this, SLOT(btnAnalyze()));
	connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(btn_del()));
	connect(ui.btn_change, SIGNAL(clicked()), this, SLOT(btn_change()));
	connect(ui.btn_pre, SIGNAL(clicked()), this, SLOT(btn_pre()));
	connect(ui.btn_next, SIGNAL(clicked()), this, SLOT(btn_next()));
	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(btn_start()));
	connect(ui.btn_end, SIGNAL(clicked()), this, SLOT(btn_end()));
	connect(ui.btn_date_sel, SIGNAL(clicked()), this, SLOT(btn_dateSel()));
	connect(ui.btn_name_sel, SIGNAL(clicked()), this, SLOT(btn_nameSel()));
	connect(ui.btn_show_all, SIGNAL(clicked()), this, SLOT(btn_showAll()));
	connect(ui.btn_data_out, SIGNAL(clicked()), this, SLOT(dataOut()));
	connect(ui.btn_data_in, SIGNAL(clicked()), this, SLOT(dataIn()));
	connect(ui.radioButton_week, SIGNAL(clicked()), this, SLOT(radio_week()));
	connect(ui.radioButton_month, SIGNAL(clicked()), this, SLOT(radio_month()));
	connect(ui.radioButton_year, SIGNAL(clicked()), this, SLOT(radio_year()));

	connect(ui.cbox_smooth, SIGNAL(currentIndexChanged(int)), this, SLOT(setFilter(int)));

	connect(ui.tableWidget, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(btnAnalyze()));



	ui.slider_mer_ratio->setMinimum(0);
	ui.slider_mer_ratio->setMaximum(100);
	ui.slider_mer_ratio->setSingleStep(1);
	ui.slider_mer_ratio->setValue(20);

	ui.slider_mer_ratio2->setMinimum(0);
	ui.slider_mer_ratio2->setMaximum(100);
	ui.slider_mer_ratio2->setSingleStep(1);
	ui.slider_mer_ratio2->setValue(20);

	ui.mer_cur_ratio->setText("20");
	ui.mer_cur_ratio2->setText("20");

	connect(ui.slider_mer_ratio, SIGNAL(valueChanged(int)), this, SLOT(sliderMerRatio()));
	connect(ui.slider_mer_ratio2, SIGNAL(valueChanged(int)), this, SLOT(sliderMerRatio2()));
	
	ui.slider_bot->setMinimum(0);
	ui.slider_bot->setMaximum(5000);
	ui.slider_bot->setSingleStep(1);
	ui.slider_bot->setValue(2200);
	ui.slider_bot->type = 1;

	ui.tmper_low->setText("22");
	ui.temper_high->setText("34");

	connect(ui.slider_bot, SIGNAL(valueChanged(int)), this, SLOT(sliderBot()));


	//	changeLabel(g_picNum, IMAGE_PER_ROW);
	ui.cbox_smooth->addItem(QStringLiteral("中值"), 0);
	ui.cbox_smooth->addItem(QStringLiteral("直方图"), 1);
	ui.cbox_smooth->addItem(QStringLiteral("原始"), 2);


	ui.toolBar->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
		"background-color: rgb(19, 35, 67);"));

	//	changeLabel(10, IMAGE_PER_ROW);

	g_mouse_mode = 6;
	g_flag_showTemper = 0;
	this->setMouseTracking(true);
	ui.widget2->setMouseTracking(true);

	QDir dir;
	g_merge_path = dir.currentPath() + "/Refer_image/";
	g_dataFolder = dir.currentPath() + "//Data//";

	m_mx = IMAGE_HEIGHT/ 2 - IMAGE_HEIGHT / 2;
	m_my = IMAGE_WIDTH / 2 - IMAGE_WIDTH / 2;

	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	ui.lineEdit_page_size->setText(QString::number(g_pageSize));

	ui.checkBox_5->hide();
	ui.checkBox->setChecked(true);
	ui.checkBox_2->setChecked(true);
	ui.checkBox_3->setChecked(true);
	if (g_remember_flag) ui.checkBox_6->setChecked(true);
	ui.update_all->setChecked(true);

	ui.btn_win_w12->setChecked(true);

	ui.btn_male->setChecked(true);
	ui.btn_pose_front->setChecked(true);
	ui.btn_mer_type1->setChecked(true);
	ui.btn_1->setChecked(true);



	updateData();


	statusBar();
	currentTimeLabel = new QLabel; // 创建QLabel控件
	currentTimeLabel->setStyleSheet("color:rgb(255,255,255);");
	//ui.statusBar->addWidget(currentTimeLabel); //在状态栏添加此控件
	ui.statusBar->addPermanentWidget(currentTimeLabel); //在状态栏添加此控件
	QTimer *timer = new QTimer(this);
	timer->start(1000); //每隔1000ms发送timeout的信号
	connect(timer, SIGNAL(timeout()), this, SLOT(time_update()));

//	this->grabKeyboard();//捕获键盘事件
	
	// 检查目录是否存在，若不存在则新建

	if (!dir.exists(g_dataFolder))
	{
		bool res = dir.mkpath(g_dataFolder);
		//		qDebug() << "新建目录是否成功" << res;
	}



	m_user_index = 0;
	m_user[0] = "";
	m_user[1] = "";
	m_scan[0] = "";
	m_scan[1] = "";


}


void IRProc::radio_week()
{
	QDateTime current_time = QDateTime::currentDateTime();

	ui.dateEdit_start->setDateTime(current_time.addDays(-6));
	ui.dateEdit_end->setDateTime(current_time.addDays(1));
}

void IRProc::radio_month()
{
	QDateTime current_time = QDateTime::currentDateTime();

	ui.dateEdit_start->setDateTime(current_time.addMonths(-1));
	ui.dateEdit_end->setDateTime(current_time.addDays(1));
}

void IRProc::radio_year()
{
	QDateTime current_time = QDateTime::currentDateTime();

	ui.dateEdit_start->setDateTime(current_time.addYears(-1));
	ui.dateEdit_end->setDateTime(current_time.addDays(1));
}


void IRProc::scanChanged(QListWidgetItem*)
{
	int index = ui.listWidget_date->currentRow();

	QString scan;
	if (index != -1)
	{
		scan = ui.listWidget_date->currentItem()->text();
	}

	conDataBase();

	map<string, string> mapUserInfoResp;

	int ret = m_cli.get_info(scan.toStdString(), mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		//	m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
		//mb_conn.EnableWindow(TRUE);
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息为空");
	}
	else
	{
		//std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();

		//it = mapUserInfoResp.begin();
		//QString rectmp;
		//it++;
		//rectmp = QString::fromLocal8Bit(it->second.c_str());
		//g_ID = rectmp;//证件号


		if (mapUserInfoResp.end() != mapUserInfoResp.find("pic"))
		{
			vecPngIDResp.clear();
			int size = split_vec(mapUserInfoResp["pic"].c_str(), vecPngIDResp, ',');
		}
		//return;
	}


	if (0 == vecPngIDResp.size())
	{
		m_msg = QString::fromLocal8Bit("图片index列表为空\n请先调用「获取信息」接口");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	else
	{

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;

		QProgressDialog *progressDialog = new QProgressDialog(this);
		progressDialog->setWindowFlags(flags);

		QLabel *lb = new QLabel;
		lb->setStyleSheet("color:rgb(255,255,255)");
		QPushButton *bt = new QPushButton;
		bt->setStyleSheet("color:rgb(255,255,255)");

		progressDialog->setLabel(lb);
		progressDialog->setLabelText(QString::fromLocal8Bit("图像载入中..."));
		progressDialog->setCancelButton(bt);
		progressDialog->setCancelButtonText(QString::fromLocal8Bit("取消"));     //设置进度对话框的取消按钮的显示文字

		progressDialog->setWindowModality(Qt::WindowModal);
		progressDialog->setMinimumDuration(5);
		progressDialog->setWindowTitle(QString::fromLocal8Bit("图像载入中..."));

		int pic_size = 640 * 480 + 2;

		unsigned short *sPicData = (unsigned short*)malloc(pic_size * sizeof(short));

		pic_count = (int)vecPngIDResp.size();

		if (pic_count > IMGE_TOTAL_NUM) pic_count = IMGE_TOTAL_NUM;

		int num = pic_count;

		progressDialog->setRange(0, num);                    //设置进度条的范围,从0到num


		for (int i = 0; i < pic_count; ++i)
		{
			//	g_pData[i] = (unsigned short*)malloc(pic_size * sizeof(short));
			m_msg = QString::fromLocal8Bit("获取图片 ");
			m_msg.append(vecPngIDResp[i].c_str());

			//memset(sPicData, 0, PIC_SIZE*sizeof(short));
			if (!m_cli.get_png(scan.toStdString(), vecPngIDResp[i], sPicData, pic_size))
			{
				m_msg.append(QString::fromLocal8Bit(" 图像为空\n"));
				//m_msg.append(m_cli.get_msg().c_str());
				QMessageBox::information(NULL, "Title", m_msg);
				m_cli.close();
				conDataBase();
				progressDialog->close();
				return;
				break;
			}
			else
			{
				m_msg.append(QString::fromLocal8Bit(" 成功\n"));

				IMAGE_WIDTH = sPicData[0];
				IMAGE_HEIGHT = sPicData[1];

				if (IMAGE_WIDTH != 384 && IMAGE_WIDTH != 640)
				{
					IMAGE_WIDTH = 384;
					IMAGE_HEIGHT = 288;

					g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];

					*(g_pData[i]) = sPicData[0];

					for (int j = 1; j < IMAGE_WIDTH*IMAGE_HEIGHT; ++j)
					{
						//outfile << " " << sPicData[j];
						*(g_pData[i] + j) = sPicData[j];
					}
				}
				else
				{
					g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];

					*(g_pData[i]) = sPicData[2];

					for (int j = 3; j < IMAGE_WIDTH*IMAGE_HEIGHT + 2; ++j)
					{
						//outfile << " " << sPicData[j];
						*(g_pData[i] + j - 2) = sPicData[j];
					}

				}

				//*(g_pData[i]) = sPicData[0];
				//
				//for (int j = 1; j < PIC_SIZE; ++j)
				//{
				//	//outfile << " " << sPicData[j];
				//	*(g_pData[i] + j) = sPicData[j];
				//}

				//	 QMessageBox::information(NULL, "Title", m_msg);

				progressDialog->setValue(i + 1);
				if (progressDialog->wasCanceled())               //检测取消按钮是否被触发,如果触发,则退出循环并关闭进度条
					return;
			}
		}
	}

	imgProc();

	for (g_picNum = 0; g_picNum < pic_count; g_picNum++)
	{
		g_bot[g_picNum] = 22;
		g_win_width[g_picNum] = 12;

		g_temper[g_picNum].create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_32FC1);

	//	data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);
		data2TemperCorrect(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100, inputEmiss, inputReflect, inputDis);

		g_TR[g_picNum] = calTR(g_temper[g_picNum]);

		data2Img(g_pData[g_picNum], g_img_gray[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], 0, g_filter_type, g_bot[g_picNum]);
		data2Img(g_pData[g_picNum], g_img[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], g_color_type, g_filter_type, g_bot[g_picNum]);
		g_img[g_picNum].copyTo(g_src[g_picNum]);
		g_img_gray[g_picNum].copyTo(g_src_gray[g_picNum]);

		g_mer[g_picNum] = Mat::zeros(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);

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

		g_img_show_flag[g_picNum] = 1;

		g_mer_hratio[g_picNum] = g_mer_vratio[g_picNum] = 1;
		g_mer_pose[g_picNum] = "Front";
		g_mer_type[g_picNum] = "1";

		resetShape(g_picNum);

	}

	changeLabel(g_picNum, IMAGE_PER_ROW);

	showImage(g_picNum);

	showThum();

}

void IRProc::nameChanged(int index)
{
	if (index != -1)
	{
		ui.listWidget_date->clear();
		ui.listWidget_date->addItem(m_scan[index]);
	}
}

void IRProc::keyUpdate()
{
	if (g_upAll_flag)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			g_bot[i] = g_bot[g_cur_img];
		}
	}

	updateImage();

	ui.slider_bot->setValue(g_bot[g_cur_img] * 100);
	/*ui.slider_bot->m_displayLabel->setText(QString::number(g_bot));

	ui.slider_bot->m_displayLabel->move((ui.slider_bot->width() - ui.slider_bot->m_displayLabel->width())*g_bot / 50, 0);*/
	ui.slider_bot->update();

	ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
	ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));
}

void IRProc::keyUp()
{

	double step = 0.01;
	g_bot[g_cur_img] += step;
	keyUpdate();
	
}

void IRProc::keyDown()
{
	double step = 0.01;
	g_bot[g_cur_img] -= step;
	keyUpdate();
}

void IRProc::keyCtrlUp()
{

	double step = 0.1;
	g_bot[g_cur_img] += step;
	keyUpdate();

}

void IRProc::keyCtrlDown()
{
	double step = 0.1;
	g_bot[g_cur_img] -= step;
	keyUpdate();
}

void IRProc::keyShiftUp()
{

	double step = 0.05;
	g_bot[g_cur_img] += step;
	keyUpdate();

}

void IRProc::keyShiftDown()
{
	double step = 0.05;
	g_bot[g_cur_img] -= step;
	keyUpdate();
}


void IRProc::updateTag()
{
	ui.tagWidget->setRowCount(0);

	int index = 0;

	for (int i = 0; i < g_shape_no[g_cur_img]; i++)
	{
		if (allshape[g_cur_img][i].del_flag == 0)
		{
			ui.tagWidget->insertRow(index);
			ui.tagWidget->setItem(index, 0, new QTableWidgetItem(QString::number(i+1)));
			ui.tagWidget->setItem(index, 1, new QTableWidgetItem(QString::number(allshape[g_cur_img][i].t_max)));
			ui.tagWidget->setItem(index, 2, new QTableWidgetItem(QString::number(allshape[g_cur_img][i].t_min)));
			ui.tagWidget->setItem(index, 3, new QTableWidgetItem(QString::number(allshape[g_cur_img][i].t_aver,'g',4)));
			ui.tagWidget->setItem(index, 4, new QTableWidgetItem(QString::number(allshape[g_cur_img][i].t_msd, 'g', 4)));
			index++;
		}

	}
}

//bool IRProc::eventFilter(QObject *object, QEvent *e)
//{
//	if (e->type() == QEvent::KeyPress)
//	{
//		QKeyEvent *event = static_cast<QKeyEvent *>(e);
//		double step = 0.01;
//		if (event->modifiers() == Qt::ControlModifier)
//		{
//			step = 0.1;
//		}
//		else if (event->modifiers() == Qt::ShiftModifier)
//		{
//			step = 0.05;
//		}
//
//		if (event->key() == Qt::Key_Up)
//		{
//			g_bot[g_cur_img] += step;
//		}
//		else if (event->key() == Qt::Key_Down)
//		{
//			g_bot[g_cur_img] -= step;
//		}
//
//		if (g_upAll_flag)
//		{
//			for (int i = 0; i < g_picNum; i++)
//			{
//				g_bot[i] = g_bot[g_cur_img];
//			}
//		}
//
//		updateImage();
//
//		ui.slider_bot->setValue(g_bot[g_cur_img] * 100);
//		/*ui.slider_bot->m_displayLabel->setText(QString::number(g_bot));
//
//		ui.slider_bot->m_displayLabel->move((ui.slider_bot->width() - ui.slider_bot->m_displayLabel->width())*g_bot / 50, 0);*/
//		ui.slider_bot->update();
//
//		ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
//		ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));
//
//	}
//	return QWidget::eventFilter(object, e);
//}

//void IRProc::keyPressEvent(QKeyEvent *event)
//{
//
//	if (event->key() == Qt::Key_Backspace)
//	{
//		QWidget *currentitem;
//		currentitem = QApplication::focusWidget();
//		if (currentitem->inherits("QLineEdit"))
//		{
//			QLineEdit *le = (QLineEdit *)currentitem;
//			le->setText("");
//		}
//
//	}
//
//
//	double step = 0.01;
//	if (event->modifiers() == Qt::ControlModifier)
//	{
//		step = 0.1;
//	}
//	else if (event->modifiers() == Qt::ShiftModifier)
//	{
//		step = 0.05;
//	}
//
//	if (event->key() == Qt::Key_Up)
//	{
//		g_bot[g_cur_img] += step;
//	}
//	else if (event->key() == Qt::Key_Down)
//	{
//		g_bot[g_cur_img] -= step;
//	}
//
//	if (g_upAll_flag)
//	{
//		for (int i = 0; i < g_picNum; i++)
//		{
//			g_bot[i] = g_bot[g_cur_img];
//		}
//	}
//
//	updateImage();
//
//	ui.slider_bot->setValue(g_bot[g_cur_img] * 100);
//	/*ui.slider_bot->m_displayLabel->setText(QString::number(g_bot));
//
//	ui.slider_bot->m_displayLabel->move((ui.slider_bot->width() - ui.slider_bot->m_displayLabel->width())*g_bot / 50, 0);*/
//	ui.slider_bot->update();
//
//	ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
//	ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));
//
//
//	QWidget::keyPressEvent(event);
//}

void IRProc::tagSel()
{
	int tagIndex = ui.tag_index->text().toInt()-1;

	if (tagIndex >= 0 && tagIndex < g_shape_no[g_cur_img]&&allshape[g_cur_img][tagIndex].del_flag==false)
	{
		g_sel_tag[g_cur_img] = tagIndex;
	}
	else if (tagIndex == -1)
	{
		m_msg = QString::fromLocal8Bit("请直接点击删除\n");
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else 
	{
		m_msg = QString::fromLocal8Bit("编号不正确\n");
		QMessageBox::information(NULL, "Title", m_msg);
	}
	showImage(g_picNum);
}

void IRProc::tagDel()
{
	int tagIndex = ui.tag_index->text().toInt() - 1;

	if (tagIndex >= 0 && tagIndex < g_shape_no[g_cur_img] && allshape[g_cur_img][tagIndex].del_flag == false)
	{
		allshape[g_cur_img][tagIndex].del_flag = true;
		g_sel_tag[g_cur_img] = 0;
		ui.tag_index->setText("1");
	}
	else if (tagIndex==-1)
	{
		g_shape_no[g_cur_img] = 0;
		g_sel_tag[g_cur_img] = 0;
		ui.tag_index->setText("0");
		//for (int i = 0; i < g_shape_no[g_cur_img]; i++)
		//{
		//	allshape[g_cur_img][i].del_flag = true;
		//}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("编号不正确\n");
		QMessageBox::information(NULL, "Title", m_msg);
	}
	showImage(g_picNum);
}


void IRProc::saveMeasure()
{
	QString filePath = g_dataFolder + "\\" + g_scanID +'_'+QString::number(g_cur_img)+ ".txt";
	char*  path;
	QByteArray t = filePath.toLatin1(); // must
	path = t.data();

	ofstream fout(path);
	fout << g_cardID.toStdString() << ' ' << g_scanID.toStdString() << ' ' << g_name.toStdString() << ' ' << g_gender.toStdString() << ' ' << g_age.toStdString() << ' '<<g_shape_no[g_cur_img]<<endl;

	for (int i = 0; i <g_shape_no[g_cur_img]; i++)
	{
		fout << i << ' ' << allshape[g_cur_img][i].shape_type << ' ' << allshape[g_cur_img][i].lt_x << ' ' << allshape[g_cur_img][i].lt_y << ' ' << allshape[g_cur_img][i].rb_x << ' ' << allshape[g_cur_img][i].rb_y << ' ' << allshape[g_cur_img][i].t_max << ' ' << allshape[g_cur_img][i].t_min << ' ' << allshape[g_cur_img][i].t_aver << ' ' << allshape[g_cur_img][i].t_msd << endl;
	}

	fout.close();

	m_msg = QString::fromLocal8Bit("保存成功\n");
	QMessageBox::information(NULL, "Title", m_msg);
}

void IRProc::dataOut()
{


	int row = ui.tableWidget->currentIndex().row();

	if (row == -1)
	{
		m_msg = QString::fromLocal8Bit("请选择用户\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}



	g_cardID = ui.tableWidget->item(row, 1)->text();
	g_scanID = ui.tableWidget->item(row, 2)->text();
	g_age = ui.tableWidget->item(row, 6)->text();
	g_name = ui.tableWidget->item(row, 3)->text();
	g_gender = ui.tableWidget->item(row, 4)->text();
	g_regTime = ui.tableWidget->item(row, 7)->text();
	g_ID = ui.tableWidget->item(row, 5)->text();


	QString filePath = g_dataFolder + "\\" + g_scanID + ".TSM";
	char*  path;
	QByteArray t = filePath.toLatin1(); // must
	path = t.data();

	ofstream fout(path);
	fout << g_cardID.toStdString() << ' ' << g_scanID.toStdString() << ' ' << g_name.toStdString() << ' ' << g_gender.toStdString() << ' ' << g_ID.toStdString() << ' ' << g_age.toStdString() << ' ' << g_regTime.toStdString()<<' ' << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << ' ';

	conDataBase();

	map<string, string> mapUserInfoResp;

	int ret = m_cli.get_png_id(g_scanID.toStdString(), mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		//	m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
		//mb_conn.EnableWindow(TRUE);
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息为空");
	}
	else
	{
		if (mapUserInfoResp.end() != mapUserInfoResp.find("pic"))
		{
			vecPngIDResp.clear();
			int size = split_vec(mapUserInfoResp["pic"].c_str(), vecPngIDResp, ',');
		}
		//return;
	}


	if (0 == vecPngIDResp.size())
	{
		m_msg = QString::fromLocal8Bit("图片index列表为空\n请先调用「获取信息」接口");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	else
	{
		int pic_size = IMAGE_WIDTH*IMAGE_HEIGHT + 2;

		//unsigned short sPicData[PIC_SIZE]; // = (unsigned short*)malloc(PIC_SIZE * sizeof(short));
		unsigned short *sPicData = (unsigned short*)malloc(pic_size * sizeof(short));

		pic_count = (int)vecPngIDResp.size();

		if (pic_count > IMGE_TOTAL_NUM) pic_count = IMGE_TOTAL_NUM;

		fout << pic_count << endl;
		for (int i = 0; i < pic_count; ++i)
		{
			if (!m_cli.get_png(g_scanID.toStdString(), vecPngIDResp[i], sPicData, pic_size))
			{
				m_msg.append(QString::fromLocal8Bit(" 图像为空,导出失败\n"));
				QMessageBox::information(NULL, "Title", m_msg);
				fout.close();
				QFile::remove(filePath);
				m_cli.close();
				conDataBase();
				return;
				break;
			}
			else
			{
				m_msg=QString::fromLocal8Bit(" 导出成功\n");

				g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
				*(g_pData[i]) = sPicData[0];
				for (int j = 1; j < pic_size; ++j)
				{
					//outfile << " " << sPicData[j];
					fout << sPicData[j] << ' '; 
				}

			}
		}
	}

	QMessageBox::information(NULL, "Title", m_msg);
	fout.close();


}

string UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}

void IRProc::dataIn()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this, tr("Select Data"), "", tr("Data (*.TSM)"));
	if (filename.isEmpty())
	{
		return;
	}
	char*  path;
	QByteArray t = filename.toLatin1(); // must
	path = t.data();

	ifstream fin(path);

	if (fin.fail())
	{
		QMessageBox::information(NULL, "Title", "No file!");
		exit(-1);
	}

	int pic_count;
	int w, h;
	string str1, str2, str3, str4, str5,str6,str7,str8;
	fin >> str1 >> str2 >> str3 >> str4 >> str5 >>str6>>str7>>str8>> w>>h>>pic_count;
	string name, gender;
	name = UTF8ToGB(str3.c_str()).c_str();
	gender = UTF8ToGB(str4.c_str()).c_str();


	g_cardID = QString::fromStdString(str1);
	g_scanID = QString::fromStdString(str2);
	g_name = QString::fromLocal8Bit(name.c_str());
	g_gender = QString::fromLocal8Bit(gender.c_str());
	g_ID = QString::fromStdString(str5);
	g_age = QString::fromStdString(str6);
	g_regTime = QString::fromStdString(str7) + ' ' + QString::fromStdString(str8);

	IMAGE_WIDTH = w;
	IMAGE_HEIGHT = h;

	int row = ui.tableWidget->rowCount();

	if (row >= g_pageSize)
	{
		row = 0;
		ui.tableWidget->setRowCount(0);
		ui.tableWidget->clearContents();
		g_curPage++;
		ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	}
	ui.tableWidget->insertRow(row);

	int index = row;

	ui.tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(index + 1)));
	ui.tableWidget->setItem(index, 1, new QTableWidgetItem(g_cardID));
	ui.tableWidget->setItem(index, 2, new QTableWidgetItem(g_scanID));
	ui.tableWidget->setItem(index, 3, new QTableWidgetItem(g_name));
	ui.tableWidget->setItem(index, 4, new QTableWidgetItem(g_gender));
	ui.tableWidget->setItem(index, 5, new QTableWidgetItem(g_ID));
	ui.tableWidget->setItem(index, 6, new QTableWidgetItem(g_age));
	ui.tableWidget->setItem(index, 7, new QTableWidgetItem(g_regTime));

	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss");

	ui.tableWidget->setItem(index, 9, new QTableWidgetItem(current_date));


	ui.tableWidget->selectRow(index);

	for (int i = 0; i < pic_count; ++i)
	{
			g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
			for (int j = 0; j < IMAGE_WIDTH*IMAGE_HEIGHT; ++j)
			{
				fin>>*(g_pData[i] + j);
			}

			g_win_width[i] = 16;
			g_bot[i] = 22;

	}

	for (g_picNum = 0; g_picNum < pic_count; g_picNum++)
	{
		g_temper[g_picNum].create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_32FC1);

		//data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);
		data2TemperCorrect(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100, inputEmiss, inputReflect, inputDis);

		g_TR[g_picNum] = calTR(g_temper[g_picNum]);

		data2Img(g_pData[g_picNum], g_img_gray[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], 0, g_filter_type, g_bot[g_picNum]);
		data2Img(g_pData[g_picNum], g_img[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], g_color_type, g_filter_type, g_bot[g_picNum]);
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
		g_img_show_flag[g_picNum] = 1;
	}

	changeLabel(g_picNum, IMAGE_PER_ROW);

	showImage(g_picNum);

	showThum();
	imgProc();
}



void IRProc::btn_show_func()
{
	if (ui.checkBox_3->isChecked())
	{
		ui.checkBox_3->setChecked(false);
		ui.stackedWidget->hide();
	}
	else
	{
		ui.checkBox_3->setChecked(true);
		ui.stackedWidget->show();
	}
}

void IRProc::btn_cross()
{
	cdlg = new CrossDlg;
	cdlg->show();
}

void IRProc::setUpAll()
{
	if (g_upAll_flag)
	{
		g_upAll_flag = 0;
		ui.update_all->setChecked(false);
	}
	else
	{
		g_upAll_flag = 1;
		ui.update_all->setChecked(true);

	}

}

void IRProc::showTemper()
{
	if (g_flag_showTemper)
	{
		g_flag_showTemper = 0;
		ui.show_temper->setChecked(false);
		ui.checkBox_4->setChecked(false);

	}
	else
	{
		g_flag_showTemper = 1;
		ui.show_temper->setChecked(true);
		ui.checkBox_4->setChecked(true);
	}

}


void IRProc::time_update()
{
	QDateTime current_time = QDateTime::currentDateTime();
	QString timestr = current_time.toString("yyyy-MM-dd hh:mm:ss      "); //设置显示的格式

	currentTimeLabel->setText(timestr); //设置label的文本内容为时间
	
}

void IRProc::btn_closeAll()
{


	qDeleteAll(ui.widget2->findChildren<MyLabel*>());
	qDeleteAll(ui.widget2->findChildren<QPushButton*>());

	qDeleteAll(ui.pageBigImg->findChildren<MyLabel*>());
	qDeleteAll(ui.pageBigImg->findChildren<QPushButton*>());

	for (int i = 0; i < g_picNum; i++)
		g_img_show_flag[i] = 0;

//	updateImage();

}

void IRProc::btn_closeOther()
{
	if (g_picNum == 0)
		return;

	qDeleteAll(ui.widget2->findChildren<MyLabel*>());
	qDeleteAll(ui.widget2->findChildren<QPushButton*>());
	qDeleteAll(ui.pageBigImg->findChildren<MyLabel*>());
	qDeleteAll(ui.pageBigImg->findChildren<QPushButton*>());


	for (int i = 0; i < g_picNum; i++)
		g_img_show_flag[i] = 0;

	g_img_show_flag[g_cur_img] = 1;
	changeLabel(g_picNum, IMAGE_PER_ROW);
	updateImage();

}


void IRProc::btn_hist()
{

	Mat srcImage;
	g_temper[g_cur_img].copyTo(srcImage);
	Mat dstImage; //读入的图片矩阵
	MatND dstHist; //直方图矩阵，对应老版本中的cvCreateHist（）
	int g_hdims = 64;     // 划分HIST的初始个数，越高越精确

	dstImage = Mat::zeros(256, 792, CV_8UC3);//每次都要初始化
	float hranges[] = { 0, 50 }; //灰度范围
	const float *ranges[] = { hranges };//灰度范围的指针

	if (g_hdims == 0)
	{
		printf("直方图条数不能为零！\n");
	}
	else
	{
		/*
		srcImage:读入的矩阵
		1:数组的个数为1
		0：因为灰度图像就一个通道，所以选0号通道
		Mat（）：表示不使用掩膜
		dstHist:输出的目标直方图
		1：需要计算的直方图的维度为1
		g_hdims:划分HIST的个数
		ranges:表示每一维度的数值范围
		*/
		//int channels=0;
		calcHist(&srcImage, 1, 0, Mat(), dstHist, 1, &g_hdims, ranges); // 计算直方图对应老版本的cvCalcHist

		/* 获取最大最小值 */
		double max = 0;
		minMaxLoc(dstHist, NULL, &max, 0, 0);// 寻找最大值及其位置，对应旧版本的cvGetMinMaxHistValue();

		/*  绘出直方图    */

		double bin_w = (double)dstImage.cols / g_hdims;  // hdims: 条的个数，则 bin_w 为条的宽度
		double bin_u = (double)dstImage.rows / max;  //// max: 最高条的像素个数，则 bin_u 为单个像素的高度

		// 画直方图
		for (int i = 0; i<g_hdims; i++)
		{
			Point p0 = Point(i*bin_w, dstImage.rows);//对应旧版本中的cvPoint（）

			int val = dstHist.at<float>(i);//注意一点要用float类型，对应旧版本中的 cvGetReal1D(hist->bins,i);
			Point p1 = Point((i + 1)*bin_w, dstImage.rows - val*bin_u);
			rectangle(dstImage, p0, p1, cvScalar(0, 255), 1, 8, 0);//对应旧版中的cvRectangle();
		}

		/*   画刻度   */
		char string[12];//存放转换后十进制数，转化成十进制后的位数不超过12位，这个根据情况自己设定
		//画纵坐标刻度（像素个数）
		//int kedu = 0;
		//for (int i = 1; kedu<max; i++)
		//{
		//	kedu = i*max / 10;//此处选择10个刻度
		//	_itoa(20+i, string, 10);//把一个整数转换为字符串，这个当中的10指十进制
		//	//在图像中显示文本字符串
		//	putText(dstImage, string, Point(0, dstImage.rows - kedu*bin_u), 1, 1, Scalar(0, 255, 255));//对应旧版中的cvPutText（）
		//}
		//画横坐标刻度（像素灰度值）
		int kedu = 0;
		for (int i = 0; kedu<250; i++)
		{
			kedu = 8 + i * 256 / 16;//此处选择间隔为20
			_itoa(24 + i, string, 10);//把一个整数转换为字符串
			//在图像中显示文本字符串
			putText(dstImage, string, cvPoint(kedu*(dstImage.cols / 256), dstImage.rows), 1, 1, Scalar(0, 255, 255));
		}
		namedWindow("温度分布", 1);
		imshow("温度分布", dstImage);
	}

}


void IRProc::changeMerGender()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();

	g_mer_gender= text;

	QString merFilePath = g_merge_path + g_mer_type[g_cur_img] + "/" + g_mer_gender;
	QDir dir;

	if (!dir.exists(merFilePath))
	{
		bool res = dir.mkpath(g_dataFolder);
		//		qDebug() << "新建目录是否成功" << res;
	}

	merFilePath = merFilePath + "/" + g_mer_pose[g_cur_img] + ".jpg";
	if (merFilePath.isEmpty())
	{
		QMessageBox msg(QMessageBox::Information, tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}
	merFilePath = QDir::toNativeSeparators(merFilePath);
	char*  path;
	QByteArray t = merFilePath.toLatin1(); // must
	path = t.data();


	Mat timg = imread(path);
	if (timg.empty())
	{
		QMessageBox msg(QMessageBox::Information, tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}



	cvtColor(timg, timg, CV_BGR2RGB);
	cv::resize(timg, g_mer_src[g_cur_img], Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src[g_cur_img].copyTo(g_mer[g_cur_img]);

	g_mer_ratio[g_cur_img] = 0.2;
	g_mer_hratio[g_cur_img] = g_mer_vratio[g_cur_img] = 1;
	g_offset_x[g_cur_img] = g_offset_y[g_cur_img] = 0;
	g_mer_flag[g_cur_img] = 1;

	//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();

	ui.slider_mer_ratio->setValue(g_mer_ratio[g_cur_img] * 100);
	ui.slider_mer_ratio2->setValue(g_mer_ratio[g_cur_img] * 100);

	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img] * 100));
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img] * 100));

}
void IRProc::changeMerPose()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();
	
	g_mer_pose[g_cur_img] = text;


	QString merFilePath = g_merge_path + g_mer_type[g_cur_img] + "/" + g_mer_gender;
	QDir dir;

	if (!dir.exists(merFilePath))
	{
		bool res = dir.mkpath(g_dataFolder);
		//		qDebug() << "新建目录是否成功" << res;
	}

	merFilePath = merFilePath + "/" + g_mer_pose[g_cur_img] + ".jpg";
	if (merFilePath.isEmpty())
	{
		QMessageBox msg(QMessageBox::Information, tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}
	merFilePath = QDir::toNativeSeparators(merFilePath);
	char*  path;
	QByteArray t = merFilePath.toLatin1(); // must
	path = t.data();


	Mat timg = imread(path);
	if (timg.empty())
	{
		QMessageBox msg(QMessageBox::Information, tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}

	

	cvtColor(timg, timg, CV_BGR2RGB);
	cv::resize(timg, g_mer_src[g_cur_img], Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src[g_cur_img].copyTo(g_mer[g_cur_img]);

	g_mer_ratio[g_cur_img] = 0.2;
	g_mer_hratio[g_cur_img] = g_mer_vratio[g_cur_img] = 1;
	g_offset_x[g_cur_img] = g_offset_y[g_cur_img] = 0;
	g_mer_flag[g_cur_img] = 1;

	//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();

	ui.slider_mer_ratio->setValue(g_mer_ratio[g_cur_img] * 100);
	ui.slider_mer_ratio2->setValue(g_mer_ratio[g_cur_img] * 100);

	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img] * 100));
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img] * 100));

}
void IRProc::changeMerType()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();

	if (text == "Human body") text = "1";
	if (text == "Meridian") text = "2";

	int type = text.toInt();

	g_mer_type[g_cur_img] = text;//1-人体；2-穴位；3-8？


	QString merFilePath = g_merge_path + g_mer_type[g_cur_img] + "/" + g_mer_gender;
	QDir dir;

	if (!dir.exists(merFilePath))
	{
		bool res = dir.mkpath(g_dataFolder);
		//		qDebug() << "新建目录是否成功" << res;
	}

	merFilePath = merFilePath + "/" + g_mer_pose[g_cur_img] + ".jpg";
	if (merFilePath.isEmpty())
	{
		QMessageBox msg(QMessageBox::Information,tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}
	merFilePath = QDir::toNativeSeparators(merFilePath);
	char*  path;
	QByteArray t = merFilePath.toLatin1(); // must
	path = t.data();


	Mat timg = imread(path);
	if (timg.empty())
	{
		QMessageBox msg(QMessageBox::Information, tr("Information"), merFilePath + " does not exist!");

		msg.exec();
		return;
	}

	cvtColor(timg, timg, CV_BGR2RGB);
	cv::resize(timg, g_mer_src[g_cur_img], Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src[g_cur_img].copyTo(g_mer[g_cur_img]);

	g_mer_ratio[g_cur_img] = 0.2;
	g_mer_hratio[g_cur_img] = g_mer_vratio[g_cur_img] = 1;
	g_offset_x[g_cur_img] = g_offset_y[g_cur_img] = 0;
	g_mer_flag[g_cur_img] = 1;

	//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();

	ui.slider_mer_ratio->setValue(g_mer_ratio[g_cur_img] * 100);
	ui.slider_mer_ratio2->setValue(g_mer_ratio[g_cur_img] * 100);

	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img]*100));
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img]*100));


}

void IRProc::updateMer()
{
	if (g_mer_hratio[g_cur_img] >= 1 && g_mer_vratio[g_cur_img] >= 1)
	{
		Mat acu_n;

		cv::resize(g_mer_src[g_cur_img], acu_n, Size(IMAGE_HEIGHT* g_mer_hratio[g_cur_img], IMAGE_WIDTH * g_mer_vratio[g_cur_img]), 0, 0);

		m_mx = IMAGE_HEIGHT* g_mer_hratio[g_cur_img] / 2 - IMAGE_HEIGHT / 2 + g_offset_x[g_cur_img];
		m_my = IMAGE_WIDTH * g_mer_vratio[g_cur_img] / 2 - IMAGE_WIDTH / 2 + g_offset_y[g_cur_img];


		if (m_mx > (IMAGE_HEIGHT* g_mer_hratio[g_cur_img] - IMAGE_HEIGHT)) m_mx = (IMAGE_HEIGHT* g_mer_hratio[g_cur_img] - IMAGE_HEIGHT) - 1;
		if (m_mx <0) m_mx = 0;
		if (m_my > (IMAGE_WIDTH * g_mer_vratio[g_cur_img] - IMAGE_WIDTH)) m_my = (IMAGE_WIDTH * g_mer_vratio[g_cur_img] - IMAGE_WIDTH) - 1;
		if (m_my <0) m_my = 0;

		Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);

		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

		roi1.copyTo(g_mer[g_cur_img]);
	}
	else
	{
		Mat mer(IMAGE_WIDTH,IMAGE_HEIGHT,CV_8UC3, Scalar(0, 0, 0));
		Mat acu_n;
		cv::resize(g_mer_src[g_cur_img], acu_n, Size(IMAGE_HEIGHT* g_mer_hratio[g_cur_img], IMAGE_WIDTH * g_mer_vratio[g_cur_img]), 0, 0);


		int h = IMAGE_HEIGHT < acu_n.cols ? IMAGE_HEIGHT : acu_n.cols;
		int w = IMAGE_WIDTH < acu_n.rows ? IMAGE_WIDTH : acu_n.rows;

		m_mx = IMAGE_HEIGHT* g_mer_hratio[g_cur_img] / 2 - h / 2+g_offset_x[g_cur_img];
		m_my = IMAGE_WIDTH * g_mer_vratio[g_cur_img] / 2 - w / 2 + g_offset_y[g_cur_img];


		if (m_mx >(IMAGE_HEIGHT* g_mer_hratio[g_cur_img] - h)) m_mx = (IMAGE_HEIGHT* g_mer_hratio[g_cur_img] - h) - 1;
		if (m_mx <0) m_mx = 0;
		if (m_my >(IMAGE_WIDTH * g_mer_vratio[g_cur_img] - w)) m_my = (IMAGE_WIDTH * g_mer_vratio[g_cur_img] - w) - 1;
		if (m_my <0) m_my = 0;

		Rect rect1(m_mx, m_my, h, w);
		//	Rect rect2(224, 224, 128, 128);
		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1


		int m_mx2 = IMAGE_HEIGHT / 2 - IMAGE_HEIGHT* g_mer_hratio[g_cur_img] / 2 + g_offset_x[g_cur_img];
		int m_my2 = IMAGE_WIDTH / 2 - IMAGE_WIDTH * g_mer_vratio[g_cur_img] / 2 + g_offset_y[g_cur_img];


		if (m_mx2 >(IMAGE_HEIGHT - IMAGE_HEIGHT* g_mer_hratio[g_cur_img])) m_mx2 = (IMAGE_HEIGHT  - IMAGE_HEIGHT* g_mer_hratio[g_cur_img]) - 1;
		if (m_mx2 <0) m_mx2 = 0;
		if (m_my2 >(IMAGE_WIDTH - IMAGE_WIDTH * g_mer_vratio[g_cur_img])) m_my2 = (IMAGE_WIDTH - IMAGE_WIDTH * g_mer_vratio[g_cur_img]) - 1;
		if (m_my2 <0) m_my2 = 0;


		Rect rect2(m_mx2, m_my2, h, w);
		Mat roi2;
		roi2=mer(rect2);

		addWeighted(roi1, 1, roi2, 0, 0., roi2);

		mer.copyTo(g_mer[g_cur_img]);
	}
	


}


void IRProc::btnMerWid()
{
	if (g_mer_flag[g_cur_img])
	{
		g_mer_hratio[g_cur_img] = g_mer_hratio[g_cur_img] + MER_RATIO_STEP;

		updateMer();
		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

}
void IRProc::btnMerNar()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_hratio[g_cur_img] - 0.01 > 0.01)
			g_mer_hratio[g_cur_img] = g_mer_hratio[g_cur_img] - MER_RATIO_STEP;
		else
			g_mer_hratio[g_cur_img] = 0.5;

		updateMer();
		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	
}
void IRProc::btnMerHigher()
{
	if (g_mer_flag[g_cur_img])
	{
		g_mer_vratio[g_cur_img] = g_mer_vratio[g_cur_img] + MER_RATIO_STEP;
		updateMer();
		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

}
void IRProc::btnMerLower()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_vratio[g_cur_img] - 0.01 > 0.01)
			g_mer_vratio[g_cur_img] = g_mer_vratio[g_cur_img] - MER_RATIO_STEP;
		else
			g_mer_vratio[g_cur_img] = 0.5;

		updateMer();
		updateImage();

	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

}
void IRProc::btnMerLeft()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_hratio[g_cur_img] > 1)
		{
			g_offset_x[g_cur_img] += MER_STEP;

		}
		else
		{
			g_offset_x[g_cur_img] -= MER_STEP;
		}
		updateMer();

		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

}
void IRProc::btnMerRight()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_hratio[g_cur_img] > 1)
		{
			g_offset_x[g_cur_img] -= MER_STEP;

		}
		else
		{
			g_offset_x[g_cur_img] += MER_STEP;
		}
		updateMer();

		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
}
void IRProc::btnMerUp()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_vratio[g_cur_img] > 1)
		{
			g_offset_y[g_cur_img] += MER_STEP;

		}
		else
		{
			g_offset_y[g_cur_img] -= MER_STEP;
		}
		updateMer();
		updateImage();
	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

}
void IRProc::btnMerDown()
{
	if (g_mer_flag[g_cur_img])
	{
		if (g_mer_vratio[g_cur_img] > 1)
		{
			g_offset_y[g_cur_img] -= MER_STEP;

		}
		else
		{
			g_offset_y[g_cur_img] += MER_STEP;
		}
		updateMer();
		updateImage();

	}
	else
	{
		m_msg = QString::fromLocal8Bit("请先打开融合图像\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	
	
}


void IRProc::sliderBot()
{
	int pos = ui.slider_bot->value();
	g_bot[g_cur_img] = pos*1.0/100;

	if (g_upAll_flag)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			g_bot[i] = g_bot[g_cur_img];
		}
	}

	updateImage();

	
	ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
	ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));

}

void IRProc::sliderMerRatio2()
{
	int pos = ui.slider_mer_ratio2->value();
	g_mer_ratio[g_cur_img] = pos*1.0 / 100;
	ui.slider_mer_ratio->setValue(pos);
	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img]*100));
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img]*100));
	updateImage();

}

void IRProc::sliderMerRatio()
{
	int pos = ui.slider_mer_ratio->value();
	g_mer_ratio[g_cur_img] = pos*1.0 / 100;
	ui.slider_mer_ratio2->setValue(pos);
	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img]*100));
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img]*100));
	updateImage();

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

void IRProc::btnMerDef()
{
	QString filename;
	filename = QFileDialog::getOpenFileName(this, tr("Select IMG"), "", tr("Data (*.jpg)"));
	if (filename.isEmpty())
	{
		return;
	}
	char*  path;
	QByteArray t = filename.toLatin1(); // must
	path = t.data();

	Mat timg = imread(path);

	cvtColor(timg, timg, CV_BGR2RGB);
	cv::resize(timg, g_mer_src[g_cur_img], Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src[g_cur_img].copyTo(g_mer[g_cur_img]);

	g_mer_ratio[g_cur_img] = 0.2;
//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();

}



void IRProc::userAreaFull()
{
	//QMessageBox::information(this, tr("Information"), QStringLiteral("<font size='26' color='white'>用户区最大化</font>"));

	ui.stackedWidget->hide();
	ui.toolBar->hide();
	ui.tabWidget->hide();
}

void IRProc::toolBarExpand()
{
	//QMessageBox::information(this, tr("Information"), QStringLiteral("展开所有工具栏"));
	ui.stackedWidget->show();
	ui.toolBar->show();
	ui.tabWidget->show();
}

void IRProc::sysSettingOp()
{
	adlg = new SetAuthDlg;

	adlg->show();
}

void IRProc::customize()
{

	QCheckBox *chbox = (QCheckBox*)this->sender();
	QString text = chbox->text();

	if (text == QString::fromLocal8Bit("显示状态栏"))
	{
		if (chbox->isChecked())
		{
			ui.statusBar->show();
		}
		else
		{
			ui.statusBar->hide();
		}

	}

	if (text == QString::fromLocal8Bit("显示数据区"))
	{
		if (chbox->isChecked())
		{
			ui.pageDataCal->show();
			ui.tabWidget->setCurrentWidget(ui.pageDataCal);
			ui.tabWidget->addTab(ui.pageDataCal, QString::fromLocal8Bit("数据测量"));
		}
		else
		{
			ui.pageDataCal->hide();\
			ui.tabWidget->setCurrentWidget(ui.pageDataCal);
			int index=ui.tabWidget->currentIndex();
			ui.tabWidget->setCurrentWidget(ui.pageImgThum);	
			ui.tabWidget->removeTab(index);
		}

	}
	if (text == QString::fromLocal8Bit("实时热辐射"))
	{
		if (g_flag_showTemper)
		{
			g_flag_showTemper = 0;
			ui.show_temper->setChecked(false);
			ui.checkBox_4->setChecked(false);
	
		}
		else
		{
			g_flag_showTemper = 1;
			ui.show_temper->setChecked(true);
			ui.checkBox_4->setChecked(true);
		}

	}
	if (text == QString::fromLocal8Bit("显示功能区"))
	{
		if (chbox->isChecked())
		{
			ui.stackedWidget->show();
		}
		else
		{
			ui.stackedWidget->hide();
		}

	}
	if (text == QString::fromLocal8Bit("记住密码"))
	{
		if (chbox->isChecked())
		{
			g_remember_flag = 1;
		}
		else
		{
			g_remember_flag = 0;
		}
		ofstream fout("config.ini");

		fout << g_ip.toStdString() << ' ' << g_port.toStdString() << ' ' << g_uport.toStdString() << ' ' << QString::number(g_step).toStdString() << ' ' << g_user.toStdString() << ' ' << g_passwd.toStdString() << ' ' << g_remember_flag;

		fout.close();

	}



	//	QMessageBox::information(this, tr("Information"), text);
}

void IRProc::changeWinWidth()
{


	QToolButton  *tb = (QToolButton*)this->sender();

	QString text = tb->text();

	g_win_width[g_cur_img] = text.toInt();

	if (g_upAll_flag)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			g_win_width[i] = g_win_width[g_cur_img];
		}
	}

	ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));

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
		g_curBig = 0;
		g_cur_img = g_bigIndex[g_curBig] = r;
		g_bigNum=1;
		g_big_show[g_curBig] = 1;

		changeLabel(g_picNum, IMAGE_PER_ROW);
		showImage(g_picNum);

		ui.stackedWidget_21->setCurrentWidget(ui.pageBigImg);

	}
	else
	{

		int r = name.toInt() - BIG_IMG_BASE;
		g_curBig = r;

		int i;
		for (i = 0; i < g_bigNum; i++)
		{
			if (g_bigIndex[i] == r)
			{
				g_big_show[i] = 0;
				break;
			}
		}

		for (i = 0; i < g_bigNum; i++)
		{
			if (g_big_show[i])
				break;
		}
		
		if (i == g_bigNum)
		{

		ui.stackedWidget_21->setCurrentWidget(ui.widget2);
		g_flagShowBigImg = 0;
		
		g_bigNum = 0;
		}
		
		changeLabel(g_picNum, IMAGE_PER_ROW);
		showImage(g_picNum);

	}


}

void IRProc::resetShape(int cur_img)
{
	for (int i = 0; i < COMMENT_PER_IMAGE; i++)
	{
		allshape[cur_img][i].comment = "None";
		allshape[cur_img][i].del_flag = false;
		allshape[cur_img][i].shape_type = 0;
		allshape[cur_img][i].lt_x = 0;
		allshape[cur_img][i].lt_y = 0;
		allshape[cur_img][i].rb_x = 0;
		allshape[cur_img][i].rb_y = 0;
		allshape[cur_img][i].t_aver = 0;
		allshape[cur_img][i].t_max = 0;
		allshape[cur_img][i].t_min = 0;
		allshape[cur_img][i].t_msd = 0;
	}

	g_shape_no[cur_img] = 0;

}



void IRProc::btnAnalyze()
{

	//QString filename;
	//filename = QFileDialog::getOpenFileName(this, tr("Select Data"), "", tr("Data (*.dat)"));
	//if (filename.isEmpty())
	//{
	//	return;
	//}
	//char*  path;
	//QByteArray t = filename.toLatin1(); // must
	//path = t.data();





	int row = ui.tableWidget->currentIndex().row();

	if (row == -1)
	{
		m_msg = QString::fromLocal8Bit("请选择用户\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

	g_cardID = ui.tableWidget->item(row, 1)->text();
	g_scanID = ui.tableWidget->item(row, 2)->text();
	g_age = ui.tableWidget->item(row, 6)->text();
	g_name = ui.tableWidget->item(row, 3)->text();
	g_gender = ui.tableWidget->item(row, 4)->text();
	g_ID = ui.tableWidget->item(row, 5)->text();

	if (g_gender == QString::fromLocal8Bit("男"))
	{
		ui.btn_male->setChecked(true);
		g_mer_gender = "Male";

	}
	else
	{
		ui.btn_female->setChecked(true);
		g_mer_gender = "Female";
	}

	g_regTime = ui.tableWidget->item(row, 7)->text();





	conDataBase();

	map<string, string> mapUserInfoResp;

	int ret = m_cli.get_info(g_scanID.toStdString(), mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		//	m_cli.close();
		 QMessageBox::information(NULL, "Title", m_msg);
		//mb_conn.EnableWindow(TRUE);
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息为空");
	}
	else
	{
		//std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();

		//it = mapUserInfoResp.begin();
		//QString rectmp;
		//it++;
		//rectmp = QString::fromLocal8Bit(it->second.c_str());
		//g_ID = rectmp;//证件号


		if (mapUserInfoResp.end() != mapUserInfoResp.find("pic"))
		{
			vecPngIDResp.clear();
			int size = split_vec(mapUserInfoResp["pic"].c_str(), vecPngIDResp, ',');
		}
		//return;
	}


	if (0 == vecPngIDResp.size())
	{
		m_msg = QString::fromLocal8Bit("图片index列表为空\n请先调用「获取信息」接口");
		 QMessageBox::information(NULL, "Title", m_msg);
		 return;
	}
	else
	{

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;

		QProgressDialog *progressDialog = new QProgressDialog(this);
		progressDialog->setWindowFlags(flags);

		QLabel *lb = new QLabel;
		lb->setStyleSheet("color:rgb(255,255,255)");
		QPushButton *bt = new QPushButton;
		bt->setStyleSheet("color:rgb(255,255,255)");

		progressDialog->setLabel(lb);
		progressDialog->setLabelText(QString::fromLocal8Bit("图像载入中..."));
		progressDialog->setCancelButton(bt);
		progressDialog->setCancelButtonText(QString::fromLocal8Bit("取消"));     //设置进度对话框的取消按钮的显示文字

		progressDialog->setWindowModality(Qt::WindowModal);
		progressDialog->setMinimumDuration(5);
		progressDialog->setWindowTitle(QString::fromLocal8Bit("图像载入中..."));




		int pic_size = 640*480+2;

		unsigned short *sPicData = (unsigned short*)malloc(pic_size * sizeof(short));

		pic_count = (int)vecPngIDResp.size();

		if (pic_count > IMGE_TOTAL_NUM) pic_count = IMGE_TOTAL_NUM;

		int num = pic_count;
	
		progressDialog->setRange(0, num);                    //设置进度条的范围,从0到num


		for (int i = 0; i < pic_count; ++i)
		{
		//	g_pData[i] = (unsigned short*)malloc(pic_size * sizeof(short));
			m_msg = QString::fromLocal8Bit("获取图片 ");
			m_msg.append(vecPngIDResp[i].c_str());

			//memset(sPicData, 0, PIC_SIZE*sizeof(short));
			if (!m_cli.get_png(g_scanID.toStdString(), vecPngIDResp[i], sPicData, pic_size))
			{
				m_msg.append(QString::fromLocal8Bit(" 图像为空\n"));
				//m_msg.append(m_cli.get_msg().c_str());
				QMessageBox::information(NULL, "Title", m_msg);
				m_cli.close();
				conDataBase();
				progressDialog->close();
				return;
				break;
			}
			else
			{
				m_msg.append(QString::fromLocal8Bit(" 成功\n"));

				IMAGE_WIDTH = sPicData[0];
				IMAGE_HEIGHT = sPicData[1];

				if (IMAGE_WIDTH != 384 && IMAGE_WIDTH != 640)
				{
					IMAGE_WIDTH = 384;
					IMAGE_HEIGHT = 288;

					g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];

					*(g_pData[i]) = sPicData[0];

					for (int j = 1; j < IMAGE_WIDTH*IMAGE_HEIGHT; ++j)
					{
						//outfile << " " << sPicData[j];
						*(g_pData[i] + j) = sPicData[j];
					}
				}
				else
				{
					g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];

					*(g_pData[i]) = sPicData[2];

					for (int j = 3; j < IMAGE_WIDTH*IMAGE_HEIGHT+2; ++j)
					{
						//outfile << " " << sPicData[j];
						*(g_pData[i] + j-2) = sPicData[j];
					}

				}
				
				//*(g_pData[i]) = sPicData[0];
				//
				//for (int j = 1; j < PIC_SIZE; ++j)
				//{
				//	//outfile << " " << sPicData[j];
				//	*(g_pData[i] + j) = sPicData[j];
				//}

				//	 QMessageBox::information(NULL, "Title", m_msg);

				progressDialog->setValue(i+1);
				if (progressDialog->wasCanceled())               //检测取消按钮是否被触发,如果触发,则退出循环并关闭进度条
					return;
			}
		}
	}

	imgProc();

	for (g_picNum = 0; g_picNum < pic_count; g_picNum++)
	{
		g_bot[g_picNum] = 22;
		g_win_width[g_picNum] = 12;

		g_temper[g_picNum].create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_32FC1);

		//data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);
		data2TemperCorrect(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100, inputEmiss, inputReflect, inputDis);

		g_TR[g_picNum] = calTR(g_temper[g_picNum]);

		data2Img(g_pData[g_picNum], g_img_gray[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], 0, g_filter_type, g_bot[g_picNum]);
		data2Img(g_pData[g_picNum], g_img[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_picNum], g_color_type, g_filter_type, g_bot[g_picNum]);
		g_img[g_picNum].copyTo(g_src[g_picNum]);
		g_img_gray[g_picNum].copyTo(g_src_gray[g_picNum]);

		g_mer[g_picNum] = Mat::zeros(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);

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

		g_img_show_flag[g_picNum] = 1;

		g_mer_hratio[g_picNum] = g_mer_vratio[g_picNum] = 1;
		g_mer_pose[g_picNum] = "Front";
		g_mer_type[g_picNum] = "1";

		resetShape(g_picNum);

	}

	changeLabel(g_picNum, IMAGE_PER_ROW);

	showImage(g_picNum);

	showThum();


	m_user[m_user_index] = g_name;
	m_scan[m_user_index] = g_scanID;

	ui.comboBox_user->clear();

	ui.comboBox_user->addItem(m_user[0], 0);
	ui.comboBox_user->addItem(m_user[1], 1);

	ui.comboBox_user->setCurrentIndex(m_user_index);

	ui.listWidget_date->clear();
	ui.listWidget_date->addItem(g_scanID);

	m_user_index = (m_user_index + 1) % 2;

	//updateImage();
}


void IRProc::showThum()
{
	if (g_picNum == 0) return;

	qDeleteAll(ui.pageImgThum->findChildren<QLabel*>());
	qDeleteAll(ui.pageImgThum->findChildren<QPushButton*>());

	int imgRow = 2;

	int rows = (IMGE_TOTAL_NUM - 1) / imgRow + 1;

	int count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imgRow; y++)
		{
			ThumLabel *lb = new ThumLabel;
			//lb->setText(QString::number(x * imgRow + y));
			lb->setObjectName(QString::number(x * imgRow + y + THUM_IMG_BASE));
			lb->setFrameShape(QFrame::Box);

			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
			lb->setAlignment(Qt::AlignTop|Qt::AlignLeft);
			lb->setFixedSize(60, 80);
			ui.gridLayout_3->addWidget(lb, x, y);

			connect(lb, SIGNAL(thumClicked()), this, SLOT(thumClicked()));
			count++;
			if (count > g_picNum) lb->setDisabled(true);
			if (count >= IMGE_TOTAL_NUM) break;
		}
		if (count >= IMGE_TOTAL_NUM) break;
	}

	for (int i = 0; i < g_picNum; i++)
	{
		QLabel *p = ui.pageImgThum->findChild<QLabel*>(QString::number(i + THUM_IMG_BASE));


		int with = p->width();
		int height = p->height();

		QPixmap pixmap = QPixmap::fromImage(g_qImgShow[i]);
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
		p->setPixmap(fitpixmap);
	}

}

void IRProc::thumClicked()
{
	changeLabel(g_picNum, IMAGE_PER_ROW);
	showImage(g_picNum);

}


void IRProc::changeLabel(int totalNum, int imagePerRow)//调整显示窗口数
{

	if (g_flagShowBigImg)
	{
		if (g_bigNum == 0) return;
		qDeleteAll(ui.pageBigImg->findChildren<MyLabel*>());
		qDeleteAll(ui.pageBigImg->findChildren<QPushButton*>());
		for (int i = 0; i < g_bigNum; i++)
		{
			int r = g_bigIndex[i];
			if (g_img_show_flag[r]&&g_big_show[i])
			{
				MyLabel *lb = new MyLabel;
				//lb->setMaximumSize(480, 640);
				//lb->setMinimumSize(480, 640);
				if (g_bigNum <= 2)
				{
					lb->setMaximumSize(BIG_WIDTH+60, BIG_HEIGHT+80);
					lb->setMinimumSize(BIG_WIDTH+60, BIG_HEIGHT+80);
				}
				else
				{
					lb->setMaximumSize(BIG_WIDTH, BIG_HEIGHT);
					lb->setMinimumSize(BIG_WIDTH, BIG_HEIGHT);
				}
				lb->setText(QString::number(r));
				lb->setObjectName(QString::number(r + BIG_IMG_BASE));
				lb->setFrameShape(QFrame::Box);
				lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
				connect(lb, SIGNAL(calData()), this, SLOT(calData()));
				connect(lb, SIGNAL(upInfo()), this, SLOT(upInfo()));
		
				//lb->setMouseTracking(true);
				ui.gridLayout_6->addWidget(lb, 0, i);

				QPushButton *bt = new QPushButton;

				bt->setText(QString::number(r));
				ui.gridLayout_6->addWidget(bt, 0, i, Qt::AlignRight | Qt::AlignTop);
				bt->setMinimumSize(16, 16);
				bt->setMaximumSize(16, 16);
				bt->setObjectName(QString::number(r+BIG_IMG_BASE));
				//bt->setStyleSheet(QLatin1String("color:rgb(0,0,0);border:2px"));
				
				connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));
			}
		}

		ui.stackedWidget_21->setCurrentWidget(ui.pageBigImg);
	}
	else
	{
		if (g_picNum == 0) return;

		qDeleteAll(ui.widget2->findChildren<MyLabel*>());
		qDeleteAll(ui.widget2->findChildren<QPushButton*>());

		int rows = (totalNum - 1) / imagePerRow + 1;

		int hei = ui.widget2->height() / rows - 10;
		int wid;
		if (totalNum >= imagePerRow)
			wid = ui.widget2->width() / imagePerRow - 10;
		else
			wid = ui.widget2->width() / totalNum - 10;

		int count = 0;
		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < imagePerRow; y++)
			{
				if (g_img_show_flag[x * imagePerRow + y])
				{
					MyLabel *lb = new MyLabel;
					lb->setText(QString::number(x * imagePerRow + y));
					lb->setObjectName(QString::number(x * imagePerRow + y));
					lb->setFrameShape(QFrame::Box);
					lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
					lb->setAlignment(Qt::AlignCenter);
					/*				if (hei >= 640)
					{
					lb->setFixedSize(480, 640);
					}
					else if (hei >= 560)
					{
					lb->setFixedSize(420, 560);
					}
					else */if (hei >= 480)
					{
						lb->setFixedSize(360, 480);
					}
					else if (hei >= 400)
					{
						lb->setFixedSize(300, 400);
					}
					else if (hei >= 320)
					{
						lb->setFixedSize(240, 320);
					}
					else if (hei >= 240)
					{
						lb->setFixedSize(180, 240);
					}
					else
					{
						lb->setFixedSize(120, 160);
					}

					ui.gridLayout_2->addWidget(lb, x, y);

					connect(lb, SIGNAL(calData()), this, SLOT(calData()));
					connect(lb, SIGNAL(upInfo()), this, SLOT(upInfo()));

					QPushButton *bt = new QPushButton;
					//lb->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

					bt->setText(QString::number(x *imagePerRow + y));
					ui.gridLayout_2->addWidget(bt, x, y, Qt::AlignRight | Qt::AlignTop);
					bt->setMinimumSize(16, 16);
					bt->setMaximumSize(16, 16);
					bt->setObjectName("bt" + QString::number(x * imagePerRow + y));
					//bt->setStyleSheet(QLatin1String("color:rgb(255,255,255);backgroud-color:rgb(0,0,0,0);border-width:2px;border-type:solid;"));
					/*bt->setFlat(true);
					bt->setAttribute(Qt::WA_TranslucentBackground);*/
					connect(bt, SIGNAL(clicked()), this, SLOT(imgChange()));
				}

				count++;
				if (count >= totalNum) break;
			}
			if (count >= totalNum) break;
		}

		ui.stackedWidget_21->setCurrentWidget(ui.widget2);

	}
}

void IRProc::upInfo()
{
	ui.tag_index->setText(QString::number(g_sel_tag[g_cur_img]+1));

	ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
	ui.temper_high->setText(QString::number(g_bot[g_cur_img]+g_win_width[g_cur_img]));

	ui.slider_bot->setValue(g_bot[g_cur_img] * 100);
	ui.slider_bot->update();

	ui.slider_mer_ratio->setValue(g_mer_ratio[g_cur_img] * 100);
	ui.mer_cur_ratio->setText(QString::number(g_mer_ratio[g_cur_img]*100));

	ui.slider_mer_ratio2->setValue(g_mer_ratio[g_cur_img] * 100);
	ui.mer_cur_ratio2->setText(QString::number(g_mer_ratio[g_cur_img]*100));

	switch (g_win_width[g_cur_img])
	{
	case 4:
		ui.btn_win_w4->setChecked(true);
		break;
	case 5:
		ui.btn_win_w5->setChecked(true);
		break;
	case 6:
		ui.btn_win_w6->setChecked(true);
		break;
	case 8:
		ui.btn_win_w8->setChecked(true);
		break;
	case 10:
		ui.btn_win_w10->setChecked(true);
		break;
	case 12:
		ui.btn_win_w12->setChecked(true);
		break;
	}

	if (g_mer_gender== "Male")
	{
		ui.btn_male->setChecked(true);
	}
	else
	{
		ui.btn_female->setChecked(true);
	}


	if (g_mer_pose[g_cur_img] == "Front")
	{
		ui.btn_pose_front->setChecked(true);
	}
	else if (g_mer_pose[g_cur_img] == "Back")
	{
		ui.btn_pose_back->setChecked(true);
	}
	else if (g_mer_pose[g_cur_img] == "Left")
	{
		ui.btn_pose_left->setChecked(true);
	}
	else if (g_mer_pose[g_cur_img] == "Right")
	{
		ui.btn_pose_right->setChecked(true);
	}

	QString typeName = "btn_mer_type" + g_mer_type[g_cur_img];

	QToolButton *bt = ui.pageMerge->findChild<QToolButton*>(typeName);

	if (bt != NULL)
		bt->setChecked(true);


	QString typeName2 = "btn_" + g_mer_type[g_cur_img];

	QToolButton *bt2 = ui.pageProc->findChild<QToolButton*>(typeName2);

	if (bt != NULL)
		bt2->setChecked(true);

	updateTag();

	if (g_shape_no[g_cur_img] == 0)
	{
		ui.max_TR->setText(QString::number(0));
		ui.min_TR->setText(QString::number(0));
		ui.av_TR->setText(QString::number(0));
		ui.max_max->setText(QString::number(0));

	}

}

void IRProc::calData()
{
	//QMessageBox::information(this, "size", QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_max) + "," + QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_min) + "," + QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_aver));
	ui.max_TR->setText(QString::number(allshape[g_cur_img][g_sel_tag[g_cur_img]].t_max - g_TR[g_cur_img],10,4));
	ui.min_TR->setText(QString::number(allshape[g_cur_img][g_sel_tag[g_cur_img]].t_min - g_TR[g_cur_img], 10, 4));
	ui.av_TR->setText(QString::number(allshape[g_cur_img][g_sel_tag[g_cur_img]].t_aver - g_TR[g_cur_img], 10, 4));
	if (g_shape_no[g_cur_img] == 1)
	{
		ui.max_max->setText(QString::number(allshape[g_cur_img][g_sel_tag[g_cur_img]].t_max, 10, 4));
	}
	else
	{
		ui.max_max->setText(QString::number(allshape[g_cur_img][g_sel_tag[g_cur_img]].t_max - allshape[g_cur_img][g_sel_tag[g_cur_img] - 1].t_max, 10, 4));
	}

	updateTag();
}

void IRProc::showImage(int pic_num)
{
	if (g_flagShowBigImg)
	{
		for (int i = 0; i < g_bigNum; i++)
		{
			if (g_img_show_flag[g_bigIndex[i]]&&g_big_show[i])
			{
				MyLabel *p = ui.pageBigImg->findChild<MyLabel*>(QString::number(g_bigIndex[i] + 20));

				p->draw_shape(g_shape_no[g_bigIndex[i]]);
				p->setOffset(g_offset[g_bigIndex[i]]);
				p->update();
			}
		}
	}
	else
	{
		for (int i = 0; i < g_picNum; i++)
		{
			if (g_img_show_flag[i])
			{
				MyLabel *p = ui.widget2->findChild<MyLabel*>(QString::number(i));

				p->draw_shape(g_shape_no[i]);
				p->setOffset(g_offset[i]);
				p->update();
			}

		}
	}

}

void IRProc::updateImage()
{
	if (g_upAll_flag)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			if (g_color_type)
			{
				g_src[i].copyTo(g_img[i]);
				data2Img(g_pData[i], g_img[i], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[i], g_color_type, g_filter_type, g_bot[i]);
				g_img[i].copyTo(g_src[i]);
				g_img[i] = g_img[i] * (1 - g_mer_ratio[i]) + g_mer[i] * g_mer_ratio[i];
				//draw_shape(g_img[i], allshape[i], g_shape_no[i]);
				QImage image = QImage((const unsigned char*)(g_img[i].data), g_img[i].cols, g_img[i].rows, QImage::Format_RGB888);
				g_qImgShow[i] = image.copy();
			}
			else
			{
				data2Img(g_pData[i], g_img_gray[i], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[i], 0, g_filter_type, g_bot[i]);
				g_img_gray[i].copyTo(g_src_gray[i]);
				//draw_shape(g_img_gray[i], allshape[i], g_shape_no[i]);
				QImage image_gray = QImage((const unsigned char*)(g_img_gray[i].data), g_img_gray[i].cols, g_img_gray[i].rows, QImage::Format_Grayscale8);
				g_qImgShow_gray[i] = image_gray.copy();
			}
		}

	}
	else
	{
		if (g_color_type)
		{
			data2Img(g_pData[g_cur_img], g_img[g_cur_img], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_cur_img], g_color_type, g_filter_type, g_bot[g_cur_img]);
			g_img[g_cur_img].copyTo(g_src[g_cur_img]);
			g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio[g_cur_img]) + g_mer[g_cur_img] * g_mer_ratio[g_cur_img];
			//draw_shape(g_img[g_cur_img], allshape[g_cur_img], g_shape_no[g_cur_img]);
			QImage image = QImage((const unsigned char*)(g_img[g_cur_img].data), g_img[g_cur_img].cols, g_img[g_cur_img].rows, QImage::Format_RGB888);
			g_qImgShow[g_cur_img] = image.copy();
		}
		else
		{
			data2Img(g_pData[g_cur_img], g_img_gray[g_cur_img], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width[g_cur_img], 0, g_filter_type, g_bot[g_cur_img]);
			g_img_gray[g_cur_img].copyTo(g_src_gray[g_cur_img]);
			//draw_shape(g_img_gray[g_cur_img], allshape[g_cur_img], g_shape_no[g_cur_img]);
			QImage image_gray = QImage((const unsigned char*)(g_img_gray[g_cur_img].data), g_img_gray[g_cur_img].cols, g_img_gray[g_cur_img].rows, QImage::Format_Grayscale8);
			g_qImgShow_gray[g_cur_img] = image_gray.copy();
		}
	}
	

	showImage(g_picNum);
}

void IRProc::wheelEvent(QWheelEvent*event)
{

	g_bot[g_cur_img] += 1.0*event->delta() / 120 * g_step;

	if (g_upAll_flag)
	{
		for (int i = 0; i < g_picNum; i++)
		{
			g_bot[i] = g_bot[g_cur_img];
		}
	}

	updateImage();

	ui.slider_bot->setValue(g_bot[g_cur_img] * 100);
	/*ui.slider_bot->m_displayLabel->setText(QString::number(g_bot));

	ui.slider_bot->m_displayLabel->move((ui.slider_bot->width() - ui.slider_bot->m_displayLabel->width())*g_bot / 50, 0);*/
	ui.slider_bot->update();

	ui.tmper_low->setText(QString::number(g_bot[g_cur_img]));
	ui.temper_high->setText(QString::number(g_bot[g_cur_img] + g_win_width[g_cur_img]));

}

void IRProc::colorTypeChange()
{
	g_color_type = (g_color_type + 1) % 3;
	updateImage();
}


void IRProc::setFilter(int)
{
	g_filter_type = ui.cbox_smooth->currentIndex();
	updateImage();
}


void IRProc::btn_sel()
{
	if (g_mouse_mode != 6)
	{
		g_mouse_mode = 6;
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
	}
}

void IRProc::btn_zoom()
{
	if (g_mouse_mode != 0)
	{
		g_mouse_mode = 0;
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 6;
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}

void IRProc::addPoint()
{
	if (g_mouse_mode != 1)
	{
		g_mouse_mode = 1;
		ui.btn_add_point->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_rect->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_rect2->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));

	}
	else
	{
		g_mouse_mode = 6;
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
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
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 6;
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
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
		ui.btn_bigger->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_smaller->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}
	else
	{
		g_mouse_mode = 6;
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
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
		g_mouse_mode = 6;
		ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));
		ui.btn_add_ellipse->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 141);"));
	}

}





//void IRProc::calPar(Mat &T, Shape &s)
//{
//	float topvalue = -999999, bottomvalue = 9999, aver = 0, sum = 0, sd = 0;
//	int num = (s.rb_x - s.lt_x)*(s.rb_y - s.lt_y);
//
//	if (s.shape_type != 1)
//	{
//		for (int i = s.lt_y; i < s.rb_y; i++)
//		{
//				float *p_tData = T.ptr<float>(i);
//				for (int j = s.lt_x; j < s.rb_x; j++)
//				{
//					float value = *(p_tData + j);
//					//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
//					bottomvalue = bottomvalue<value ? bottomvalue : value;
//					topvalue = topvalue>value ? topvalue : value;
//					sum += value;
//				}
//		}
//		aver = sum / num;
//		for (int i = s.lt_y; i < s.rb_y; i++)
//		{
//			float *p_tData = T.ptr<float>(i);
//			for (int j = s.lt_x; j < s.rb_x; j++)
//			{
//				float value = *(p_tData + j);
//				//dst.at<uchar>(j, HEIGHT - 1 - i) = g_tmpdst.at<uchar>(i, j);
//				sd += (value - aver)*(value - aver);
//			}
//		}
//		s.t_max = topvalue;;
//		s.t_min = bottomvalue;
//		s.t_aver = aver;
//		s.t_msd = sqrt(sd) / num;
//	}
//	
//}

void IRProc::btn_sysPar()
{
	dlg = new SettingDlg;

	dlg->show();
}

void IRProc::btn_camPar()
{
	camdlg = new CamParDlg;

	camdlg->show();
}


void IRProc::addData(int index, QString cardID, QString scanID, QString RegTime)
{
	ui.tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(index + 1)));
	ui.tableWidget->setItem(index, 1, new QTableWidgetItem(cardID));
	ui.tableWidget->setItem(index, 2, new QTableWidgetItem(scanID));
	ui.tableWidget->setItem(index, 7, new QTableWidgetItem(RegTime));

	std::string sScanID = scanID.toStdString();

	std::map<std::string, std::string> mapUserInfoResp;
	int ret = m_cli.get_info(sScanID, mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息失败\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();

		return;
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("获取用户信息为空");
		//	QMessageBox::information(NULL, "Title", m_msg);
		ui.tableWidget->setItem(index, 3, new QTableWidgetItem(""));
		ui.tableWidget->setItem(index, 4, new QTableWidgetItem(""));
		ui.tableWidget->setItem(index, 6, new QTableWidgetItem(""));
		return;
	}
	else
	{
		m_msg = QString::fromLocal8Bit("获取用户信息成功");
		//	 QMessageBox::information(NULL, "Title", m_msg);
		std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();
		//for (; it != mapUserInfoResp.end(); ++it)
		//{
		//	m_msg = QString::fromLocal8Bit("获取 ");
		//	m_msg.append(QString::fromLocal8Bit(it->first.c_str()));
		//	m_msg.append("\n");
		//	m_msg.append(QString::fromLocal8Bit(it->second.c_str()));
		//	// QMessageBox::information(NULL, "Title", m_msg);
		//	
		//}

		it = mapUserInfoResp.begin();
		QString rectmp;
		rectmp = it->second.c_str();
		g_age = rectmp;//年龄
		it++;
		rectmp = QString::fromLocal8Bit(it->second.c_str());
		g_ID = rectmp;//证件号
		it++;
		rectmp = QString::fromLocal8Bit(it->second.c_str());
		g_name = rectmp;//姓名
		it++;
		rectmp = it->second.c_str();//图像
		it++;
		rectmp = it->second.c_str();
		it++;
		rectmp = rectmp = it->second.c_str();
		if (rectmp == "0")
			g_gender = QString::fromLocal8Bit("男");//性别
		else
			g_gender = QString::fromLocal8Bit("女");

		ui.tableWidget->setItem(index, 3, new QTableWidgetItem(g_name));
		ui.tableWidget->setItem(index, 4, new QTableWidgetItem(g_gender));
		ui.tableWidget->setItem(index, 5, new QTableWidgetItem(g_ID));
		ui.tableWidget->setItem(index, 6, new QTableWidgetItem(g_age));

		if (mapUserInfoResp.end() != mapUserInfoResp.find("pic"))
		{
			vecPngIDResp.clear();
			int size = split_vec(mapUserInfoResp["pic"].c_str(), vecPngIDResp, ',');
		}
	}



}

void IRProc::conDataBase()
{
	;
	if (m_cli.init(g_ip.toStdString(), atoi(g_port.toStdString().c_str())))
	{
		m_msg = QString::fromLocal8Bit("连接成功");

		//QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("连接失败\n请确认IP或端口号");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	//2-
	std::string sPermissions;
	// 注意： 0表示测试客户端， 1表示正式客户端
	int iRet = m_cli.login_auth(g_user.toStdString().c_str(), g_passwd.toStdString().c_str(), sPermissions, iTestFlag);
	if (0 > iRet)
	{
		m_msg = QString::fromLocal8Bit("获取授权码失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("获取授权码成功\n");
		QString sAuth = QString::fromStdString(m_cli.get_auth().c_str());
		QString sRep1 = sAuth.mid(8, 16);
		QString sRep2('*', sRep1.length());
		sAuth.replace(sRep1, sRep2);
		if (0 == iRet)
		{
			m_msg.append(QString::fromLocal8Bit("测试环境: "));

		}
		//	m_msg.append(sAuth);

		int iPermissions = atoi(sPermissions.c_str());
		permissions_t pt;
		if (iPermissions & 0x01)
		{
			pt.p1 = true;
		}
		if (iPermissions & 0x02)
		{
			pt.p2 = true;
		}
		if (iPermissions & 0x04)
		{
			pt.p3 = true;
		}

		//m_msg.append(QString::fromLocal8Bit("\n权限: "));
		//m_msg.append(QString::fromLocal8Bit("图像扫描"));
		//if (pt.p1)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(√)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(×)"));
		//}
		//m_msg.append(QString::fromLocal8Bit(",图像分析"));
		//if (pt.p2)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(√)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(×)"));
		//}
		//m_msg.append(QString::fromLocal8Bit(",系统设置"));
		//if (pt.p3)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(√)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(×)"));
		//}

		if (pt.p1 || pt.p3)
		{
			g_log_flag = 1;
			//	QMessageBox::information(NULL, "Title", m_msg);

		}
		else
		{
			m_msg.append(QString::fromLocal8Bit("没有权限"));
			QMessageBox::information(NULL, "Title", m_msg);
		}

	}

}
void IRProc::updateData()
{

	std::map <std::string, std::string> mapParams;

	if (g_sel_mode == 0)
	{
		QDateTime current_time = QDateTime::currentDateTime();
		QString start_time = current_time.toString("yyyy-MM-dd"); //
		QString end_time = current_time.addDays(1).toString("yyyy-MM-dd"); //

		ui.dateEdit_start->setDateTime(current_time);
		ui.dateEdit_end->setDateTime(current_time.addDays(1));


		mapParams["data_type"] = "4";
		mapParams["page_size"] = QString::number(g_pageSize).toStdString();
		mapParams["page_num"] = QString::number(g_curPage).toStdString();
		//mapParams["name"] = "张三";
		//mapParams["cardid"] = "CARD100000000001";
		//mapParams["scanid"] = "SCAN001";
		mapParams["begin"] = start_time.toStdString();
		mapParams["end"] = end_time.toStdString();
	}
	else if (g_sel_mode == 1)
	{
		mapParams["data_type"] = "4";
		mapParams["page_size"] = QString::number(g_pageSize).toStdString();
		mapParams["page_num"] = QString::number(g_curPage).toStdString();
		//mapParams["name"] = "张三";
		//mapParams["cardid"] = "CARD100000000001";
		//mapParams["scanid"] = "SCAN001";
		mapParams["begin"] = ui.dateEdit_start->text().toStdString();
		mapParams["end"] = ui.dateEdit_end->text().toStdString();
	}
	else if (g_sel_mode == 2)
	{
		mapParams["data_type"] = "3";
		mapParams["page_size"] = QString::number(g_pageSize).toStdString();
		mapParams["page_num"] = QString::number(g_curPage).toStdString();
		if (ui.lineEdit_name->text() != "")
			mapParams["name"] = ui.lineEdit_name->text().toLocal8Bit();
		//mapParams["cardid"] = "CARD100000000001";
		if (ui.lineEdit_scanID->text() != "")
			mapParams["cardid"] = ui.lineEdit_scanID->text().toStdString();
		mapParams["page_size"] = QString::number(g_pageSize).toStdString();
		mapParams["page_num"] = QString::number(g_curPage).toStdString();
	}
	else
	{
		mapParams["data_type"] = "4";
		mapParams["page_size"] = QString::number(g_pageSize).toStdString();
		mapParams["page_num"] = QString::number(g_curPage).toStdString();
	}
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;




	int iRet = m_cli.get_listdata(sParams, sData);
	m_msg = "";

	if (0 < iRet)
	{
		//m_msg = "结果:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

		QString temp = lst[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();

		int num = dataNum<g_pageSize ? dataNum : g_pageSize;

		if (num>0)
		{
			if (g_show_progress)
			{
				progressDialog = new QProgressDialog(this);

				Qt::WindowFlags flags = Qt::Dialog;
				flags |= Qt::WindowCloseButtonHint;

				progressDialog->setWindowFlags(flags);

				QLabel *lb = new QLabel;
				lb->setStyleSheet("color:rgb(255,255,255)");
				QPushButton *bt = new QPushButton;
				bt->setStyleSheet("color:rgb(255,255,255)");

				progressDialog->setLabel(lb);
				progressDialog->setLabelText(QString::fromLocal8Bit("数据载入中..."));
				progressDialog->setCancelButton(bt);
				progressDialog->setCancelButtonText(QString::fromLocal8Bit("取消"));     //设置进度对话框的取消按钮的显示文字

				progressDialog->setWindowModality(Qt::WindowModal);
				progressDialog->setMinimumDuration(5);
				progressDialog->setWindowTitle(QString::fromLocal8Bit("数据载入中..."));
				progressDialog->setRange(0, num);                    //设置进度条的范围,从0到num

			}


			g_maxPage = (dataNum - 1) / g_pageSize + 1;


			lst[0] = lst[0].section('=', -1, -1);
			ui.tableWidget->setRowCount(lst.size());
			for (int i = 0; i != lst.size(); ++i)
			{
				//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

				addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
				if (g_show_progress)
				{
					progressDialog->setValue(i + 1);
					if (progressDialog->wasCanceled())               //检测取消按钮是否被触发,如果触发,则退出循环并关闭进度条
						return;
				}
			}
		}
		else
		{
			ui.tableWidget->setRowCount(0);
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("失败\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}

	if (g_show_progress)
	{
		g_show_progress = 0;
	}

}

void IRProc::btn_showAll()
{
	g_pageSize = ui.lineEdit_page_size->text().toInt();
	g_sel_mode = 3;

	std::map <std::string, std::string> mapParams;
	mapParams["data_type"] = "4";
	mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	//mapParams["page_num"] = QString::number(g_curPage).toStdString();

	//mapParams["name"] = ui.lineEdit_name->text().toStdString();
	////mapParams["cardid"] = "CARD100000000001";
	//mapParams["scanid"] = ui.lineEdit_scanID->text().toStdString();
	//mapParams["begin"] = ui.dateEdit_start->text().toStdString();
	//mapParams["end"] = ui.dateEdit_end->text().toStdString();
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;

	int iRet = m_cli.get_listdata(sParams, sData);
	m_msg = "";

	if (0 < iRet)
	{
		//m_msg = "结果:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

		//	QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), m_msg);
		//QString temp = lst[0].section('&', 3, 3);
		//int dataNum = temp.section('=', -1, -1).toInt();

		int dataNum = lst.size();


		g_maxPage = (dataNum - 1) / g_pageSize + 1;
		g_curPage = 1;
		ui.lineEdit_cur_page->setText(QString::number(g_curPage));

		lst[0] = lst[0].section('=', -1, -1);
		ui.tableWidget->setRowCount(dataNum<g_pageSize ? dataNum : g_pageSize);
		for (int i = 0; i != (dataNum<g_pageSize ? dataNum : g_pageSize); ++i)
		{
			//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

			addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("失败\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}

}


void IRProc::btn_nameSel()
{
	std::map <std::string, std::string> mapParams;

	mapParams["data_type"] = "3";
	//mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	//mapParams["page_num"] = QString::number(g_curPage).toStdString();
	if (ui.lineEdit_name->text() != "")
		mapParams["name"] = ui.lineEdit_name->text().toLocal8Bit();
	//mapParams["cardid"] = "CARD100000000001";
	if (ui.lineEdit_scanID->text() != "")
		mapParams["cardid"] = ui.lineEdit_scanID->text().toStdString();
	//mapParams["begin"] = ui.dateEdit_start->text().toStdString();
	//mapParams["end"] = ui.dateEdit_end->text().toStdString();
	std::string sParams = map_join(mapParams, '&', '=');
	std::string sData;
	;

	int iRet1 = m_cli.get_listdata(sParams, sData);
	m_msg = "";

	if (0 < iRet1)
	{
		//m_msg = "结果:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst_1;
		lst_1 = m_msg.split(';');

		QString temp = lst_1[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();

		g_maxPage = (dataNum - 1) / g_pageSize + 1;

		lst_1[0] = lst_1[0].section('=', -1, -1);
		if (dataNum>0)
			ui.tableWidget->setRowCount(0);
		else
		{
			m_msg = QString::fromLocal8Bit("未找到记录\n");
			QMessageBox::information(NULL, "Title", m_msg);
			return;
		}

		for (int i = 0; i != dataNum; ++i)
		{
			//	QMessageBox::information(NULL, "Title", lst_1[i].section(',',-3,-3));
			mapParams["cardid"] = lst_1[i].section(',', -3, -3).toStdString();
			mapParams["data_type"] = "4";
			sParams = map_join(mapParams, '&', '=');
			int iRet = m_cli.get_listdata(sParams, sData);

			m_msg = "";

			if (0 < iRet)
			{
				//m_msg = "结果:\n";
				m_msg.append(QString::fromLocal8Bit(sData.c_str()));
				//	QMessageBox::information(NULL, "Title", m_msg);
				QList<QString> lst;
				lst = m_msg.split(';');

				QString temp = lst[0].section('&', 0, 0);
				int dataNum = temp.section('=', -1, -1).toInt();

				if (dataNum == 0)
				{
					m_msg = QString::fromLocal8Bit("未找到记录\n");
					QMessageBox::information(NULL, "Title", m_msg);
					return;
				}

				g_maxPage = (dataNum - 1) / g_pageSize + 1;
				lst[0] = lst[0].section('=', -1, -1);

				for (int j = 0; j != dataNum; ++j)
				{
					//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));
					int row = ui.tableWidget->rowCount();

					if (row >= g_pageSize)
					{
						row = 0;
						ui.tableWidget->setRowCount(0);
						ui.tableWidget->clearContents();
						g_curPage++;
						ui.lineEdit_cur_page->setText(QString::number(g_curPage));
					}
					ui.tableWidget->insertRow(row);

					int index = row;

					addData(index, lst[j].section(',', 0, 0), lst[j].section(',', 1, 1), lst[j].section(',', -1, -1));

					ui.tableWidget->selectRow(index);
				}
			}
			else
			{
				m_msg = QString::fromLocal8Bit("查找失败\n");
				m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
				QMessageBox::information(NULL, "Title", m_msg);
				m_cli.close();
				conDataBase();
			}



		}
	}
	else
	{
		m_msg = QString::fromLocal8Bit("查找失败\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}




}


void IRProc::btn_dateSel()
{
	g_pageSize = ui.lineEdit_page_size->text().toInt();

	g_sel_mode = 1;
	std::map <std::string, std::string> mapParams;
	mapParams["data_type"] = "4";
	mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	//mapParams["page_num"] = QString::number(g_curPage).toStdString();
	//mapParams["name"] = "张三";
	//mapParams["cardid"] = "CARD100000000001";
	//mapParams["scanid"] = "SCAN001";
	mapParams["begin"] = ui.dateEdit_start->text().toStdString();
	mapParams["end"] = ui.dateEdit_end->text().toStdString();
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;

	int iRet = m_cli.get_listdata(sParams, sData);
	m_msg = "";

	if (0 < iRet)
	{
		//m_msg = "结果:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

	//	QMessageBox::information(NULL, "Title", m_msg);

		QString temp = lst[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();
		//	int dataNum = lst.size() - 1;


		g_maxPage = (dataNum - 1) / g_pageSize + 1;

		g_curPage = 1;
		ui.lineEdit_cur_page->setText(QString::number(g_curPage));

		lst[0] = lst[0].section('=', -1, -1);
		if (dataNum == 0)
		{
			m_msg = QString::fromLocal8Bit("未找到记录\n");
			QMessageBox::information(NULL, "Title", m_msg);
			return;
		}

		ui.tableWidget->setRowCount(dataNum<g_pageSize ? dataNum : g_pageSize);
		for (int i = 0; i != (dataNum<g_pageSize ? dataNum : g_pageSize); ++i)
		{
			//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

			addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("失败\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}


}


void IRProc::btn_start()
{
	g_curPage = 1;
	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	g_pageSize = ui.lineEdit_page_size->text().toInt();

	updateData();

}

void IRProc::btn_end()
{
	g_curPage = g_maxPage;
	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	//	g_pageSize = ui.lineEdit_page_size->text().toInt();

	updateData();

}



void IRProc::btn_pre()
{
	g_curPage--;
	if (g_curPage < 1) g_curPage = 1;
	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
//	g_pageSize = ui.lineEdit_page_size->text().toInt();

	updateData();

}

void IRProc::btn_next()
{
	g_curPage++;
	if (g_curPage > g_maxPage) g_curPage = g_maxPage;
	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	//	g_pageSize = ui.lineEdit_page_size->text().toInt();

	updateData();


}

void IRProc::btn_change()
{
	int row = ui.tableWidget->currentIndex().row();

	if (row == -1)
	{
		m_msg = QString::fromLocal8Bit("请选择用户\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}


	g_cardID = ui.tableWidget->item(row, 1)->text();
	g_scanID = ui.tableWidget->item(row, 2)->text();
	g_age = ui.tableWidget->item(row, 6)->text();
	g_name = ui.tableWidget->item(row, 3)->text();
	g_gender = ui.tableWidget->item(row, 4)->text();
	g_ID = ui.tableWidget->item(row, 5)->text();

	g_reg_flag = 0;

	rdlg = new RegDlg;

	rdlg->setWindowTitle(QString::fromLocal8Bit("修改"));
	rdlg->setWindowModality(Qt::ApplicationModal);
	rdlg->ui.lineEdit_card->setDisabled(true);
	rdlg->ui.lineEdit_5->setDisabled(true);
	rdlg->ui.btn_reg->setText(QString::fromLocal8Bit("修改"));
	rdlg->exec();


	//	conDataBase();

	if (g_reg_flag)
	{
		std::map<std::string, std::string> mapCardInfo;


		if ("" != g_ID)
		{
			mapCardInfo["identity"] = g_ID.toStdString();
		}
		if ("" != g_name)
		{
			QByteArray cdata = g_name.toLocal8Bit();
			mapCardInfo["name"] = string(cdata);
		}
		if ("" != g_gender)
		{
			QByteArray cdata = g_gender.toLocal8Bit();
			mapCardInfo["sex"] = string(cdata);
		}
		if ("" != g_age)
		{
			mapCardInfo["age"] = g_age.toStdString();
		}
		if ("" != g_cardID)
		{
			mapCardInfo["card_id"] = g_cardID.toStdString();
		}


		m_msg = QString::fromLocal8Bit("卡号: ");
		m_msg.append(g_cardID);
		//QMessageBox::information(NULL, "Title", m_msg);

		int iRet = m_cli.set_cardid(mapCardInfo, g_user.toStdString());
		if (0 < iRet)
		{
			m_msg = QString::fromLocal8Bit("注册卡信息成功\nID:");
			m_msg.append(g_cardID);

		}
		else
		{
			m_msg = QString::fromLocal8Bit("注册卡信息失败\n");
			m_msg.append(m_cli.get_msg().c_str());
			QMessageBox::information(NULL, "Title", m_msg);
			m_cli.close();
			conDataBase();
			return;
		}
		//QMessageBox::information(NULL, "Title", m_msg);


		std::map<std::string, std::string> mapUserInfo;
		mapUserInfo["scan_id"] = g_scanID.toStdString();
		//mapUserInfo["card_id"] = sCardID;
		mapUserInfo["pic"] = vec_join(vecPngIDReq, ',');
		mapUserInfo["user"] = g_user.toStdString();

		if (!m_cli.send_info(mapUserInfo))
		{
			m_msg = QString::fromLocal8Bit("发送用户信息失败\n");
			m_msg.append(m_cli.get_msg().c_str());
			QMessageBox::information(NULL, "Title", m_msg);
			m_cli.close();
			conDataBase();
			return;

		}
		else
		{
			m_msg = QString::fromLocal8Bit("修改成功");
		}

		QMessageBox::information(NULL, "Title", m_msg);

		ui.tableWidget->item(row, 1)->setText(g_cardID);
		ui.tableWidget->item(row, 2)->setText(g_scanID);
		ui.tableWidget->item(row, 6)->setText(g_age);
		ui.tableWidget->item(row, 3)->setText(g_name);
		ui.tableWidget->item(row, 4)->setText(g_gender);
	}

}

void IRProc::btn_del()
{
	int row = ui.tableWidget->currentIndex().row();

	if (row == -1)
	{
		m_msg = QString::fromLocal8Bit("请选择用户\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

	g_scanID = ui.tableWidget->item(row, 2)->text();

	int iRet = m_cli.del_scanid(g_scanID.toStdString());
	if (0 < iRet)
	{
		m_msg = QString::fromLocal8Bit("删除扫描ID成功\nID:");
		m_msg.append(g_scanID);
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("删除扫描ID失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}


	ui.tableWidget->removeRow(row);
	//ui.tableWidget->setRowCount(0);
	//updateData();
}


void IRProc::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);

	changeLabel(g_picNum, IMAGE_PER_ROW);
	showImage(g_picNum);
}

double IRProc::calTR(Mat &img)
{
	cv::Scalar     mean;
	cv::Scalar     dev;
	cv::meanStdDev(img, mean, dev);
	double  m = mean.val[0];
	double  s = dev.val[0];
	return m;
}