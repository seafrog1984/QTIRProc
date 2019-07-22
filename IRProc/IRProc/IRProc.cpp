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


#define IMAGE_WIDTH  384//ԭʼͼ���
#define IMAGE_HEIGHT 288//ԭʼͼ���

#define IMAGE_PER_ROW 5  //ÿ����ʾͼ����
#define IMGE_TOTAL_NUM 12 //��ʾ��ͼ������
#define COMMENT_PER_IMAGE 30 //ÿ��ͼ��ı�ע��
#define BIG_IMG_BASE 20 //��ʾ��ͼ��Label��Ż���
#define THUM_IMG_BASE 30 //��ʾ����ͼ��Label��Ż���

int g_flagShowBigImg = 0;//��ʾ��ͼ����Сͼ�ı�־�� 1-��ͼ��0-Сͼ
int g_picNum = 0;//��ȡ��ͼ������
int g_currentBigNum = 0;//��ǰ��ͼ�±�
int g_cur_img = 0;//��ǰ������ͼ���±�

double g_ratio[IMGE_TOTAL_NUM];//ͼ��Ŵ���
int g_img_show_flag[IMGE_TOTAL_NUM] = { 0 };//ͼ����ʾ��־

unsigned short *g_pData[IMGE_TOTAL_NUM];//ԭʼ����

Mat g_mer_src;//ԭʼ�ں�ͼ��
Mat g_mer=Mat::zeros(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);;//�ں�ͼ��
Mat g_src[IMGE_TOTAL_NUM]; // opencvͼ��ԭʼ- ��ɫ
Mat g_src_gray[IMGE_TOTAL_NUM];//opencvͼ��ԭʼ- �Ҷ�
Mat g_img[IMGE_TOTAL_NUM];//opencvͼ��-��ɫ
Mat g_img_gray[IMGE_TOTAL_NUM];//opencvͼ��-�Ҷ�
Mat g_temper[IMGE_TOTAL_NUM];//�¶Ⱦ���
double g_TR[IMGE_TOTAL_NUM] = { 0 };//ͼ����¶Ⱦ�����
QImage g_qImgShow[IMGE_TOTAL_NUM];//Qtԭʼͼ��-��ɫ
QImage g_qImgShow_gray[IMGE_TOTAL_NUM];//Qtԭʼͼ��-�Ҷ�

QPoint g_offset[IMGE_TOTAL_NUM] = {};//��ʾͼ���ƫ����

float g_referTemper = 25;//�ο��¶�
int g_win_width = 16;//����
int g_color_type = 2;// ��ɫģʽ 0-�Ҷȣ�1-��ͳα��ɫ��2-TTM
int g_filter_type = 2;//�˲�ģʽ 0-��ֵ��1-ֱ��ͼ���⣻2-���˲�
float g_bot = 22;//�ϲ�ο�ֵ����ʼ22

float g_step = 0.01;

Shape allshape[IMGE_TOTAL_NUM][COMMENT_PER_IMAGE] = { { false, 0, 0, 0, 0, 0, 0, 0, 0, 0, "None" } };
int g_shape_no[IMGE_TOTAL_NUM] = { 0 };//ͼ���ϱ�ע������

int g_mouse_mode=6;//����¼�ģʽ�� 0-�Ŵ���С����ס�ƶ��� Ĭ�ϣ�1-�ӵ㣻2-���Σ�3-Բ�Ǿ��Σ�4-��Բ��5-ɾ����6-ѡ��ģʽ
int g_flag_showTemper=0;//��ʾ�¶ȱ�־�� 0-����ʾ��1-��ʾ

QString g_merge_path;
QString g_mer_gender="Male";//�ں�ͼ�Ա�
QString g_mer_pose = "Front";// �ں�ͼ����
QString g_mer_type="ACU";//�ں�ͼ���
double g_mer_ratio = 0;//�ں϶�
double g_mer_hratio = 1, g_mer_vratio = 1;//�ں�ͼˮƽ��ֱ�Ŵ���


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

QString g_name = "����";
QString g_age = "20";
QString g_gender = "��";
QString g_ID = "id003";
QString g_scanID = "SCAN0016";
QString g_cardID = "CARD100000000003";

int g_pageSize = 10;
int g_maxPage = 2;
int g_curPage = 1;

int pic_count = 0;

extern int g_reg_flag=0;

double g_cur_max=0;
double g_cur_min = 0;
double g_cur_mean = 0;



IRProc::IRProc(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this);


	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(21,85,141);color:rgb(255,255,255);}"); //���ñ�ͷ����ɫ
	ui.tableWidget->horizontalHeader()->setSectionsClickable(false);
	//ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//���õ���ѡ��һ��
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);	//����ÿ�����ݲ��ɱ༭
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);	//����ֻ��ѡ��һ�У�����ѡ�����


	//setWindowTitle(tr("Main Window"));

	connect(ui.imgProcAct, &QAction::triggered, this, &IRProc::imgProc);
	connect(ui.imgMergeAct, &QAction::triggered, this, &IRProc::imgMerge);
	connect(ui.dataManageAct, &QAction::triggered, this, &IRProc::dataManage);
	connect(ui.sysSettingAct, &QAction::triggered, this, &IRProc::sysSetting);
	connect(ui.btn_sys_par, SIGNAL(clicked()), this, SLOT(btn_sysPar()));

	connect(ui.userAreaBt, SIGNAL(clicked()), this, SLOT(userAreaFull()));
	connect(ui.toolBarExpandBt, SIGNAL(clicked()), this, SLOT(toolBarExpand()));
	connect(ui.btn_set_auth, SIGNAL(clicked()), this, SLOT(sysSettingOp()));
	connect(ui.btn_analyze, SIGNAL(clicked()), this, SLOT(btnAnalyze()));
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
	ui.btn_sel->setStyleSheet(QLatin1String("background-color: rgb(21, 86, 200);"));

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


	connect(ui.btn_del, SIGNAL(clicked()), this, SLOT(btn_del()));
	connect(ui.btn_change, SIGNAL(clicked()), this, SLOT(btn_change()));
	connect(ui.btn_pre, SIGNAL(clicked()), this, SLOT(btn_pre()));
	connect(ui.btn_next, SIGNAL(clicked()), this, SLOT(btn_next()));
	connect(ui.btn_start, SIGNAL(clicked()), this, SLOT(btn_start()));
	connect(ui.btn_end, SIGNAL(clicked()), this, SLOT(btn_end()));
	connect(ui.btn_date_sel, SIGNAL(clicked()), this, SLOT(btn_dateSel()));
	connect(ui.btn_name_sel, SIGNAL(clicked()), this, SLOT(btn_nameSel()));
	connect(ui.btn_show_all, SIGNAL(clicked()), this, SLOT(btn_showAll()));

	connect(ui.cbox_smooth, SIGNAL(currentIndexChanged(int)), this, SLOT(setFilter(int)));



	ui.slider_mer_ratio->setMinimum(0);
	ui.slider_mer_ratio->setMaximum(100);
	ui.slider_mer_ratio->setSingleStep(1);
	ui.slider_mer_ratio->setValue(20);

	connect(ui.slider_mer_ratio, SIGNAL(valueChanged(int)), this, SLOT(sliderMerRatio()));
	connect(ui.slider_mer_ratio2, SIGNAL(valueChanged(int)), this, SLOT(sliderMerRatio2()));
	
	ui.slider_bot->setMinimum(0);
	ui.slider_bot->setMaximum(500);
	ui.slider_bot->setSingleStep(1);
	ui.slider_bot->setValue(220);
	ui.slider_bot->type = 1;

	connect(ui.slider_bot, SIGNAL(valueChanged(int)), this, SLOT(sliderBot()));


	//	changeLabel(g_picNum, IMAGE_PER_ROW);
	ui.cbox_smooth->addItem(QStringLiteral("��ֵ"), 0);
	ui.cbox_smooth->addItem(QStringLiteral("ֱ��ͼ"), 1);
	ui.cbox_smooth->addItem(QStringLiteral("ԭʼ"), 2);


	ui.toolBar->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
		"background-color: rgb(19, 35, 67);"));

	//	changeLabel(10, IMAGE_PER_ROW);

	g_mouse_mode = 6;
	g_flag_showTemper = 0;
	this->setMouseTracking(true);
	ui.widget2->setMouseTracking(true);

	QDir dir;
	g_merge_path = dir.currentPath() + "/Refer_image/";


	m_mx = IMAGE_HEIGHT* g_mer_hratio / 2 - IMAGE_HEIGHT / 2;
	m_my = IMAGE_WIDTH * g_mer_vratio / 2 - IMAGE_WIDTH / 2;

	ui.lineEdit_cur_page->setText(QString::number(g_curPage));
	ui.lineEdit_page_size->setText(QString::number(g_pageSize));

	updateData();


	statusBar();
	currentTimeLabel = new QLabel; // ����QLabel�ؼ�
	currentTimeLabel->setStyleSheet("color:rgb(255,255,255);");
	//ui.statusBar->addWidget(currentTimeLabel); //��״̬����Ӵ˿ؼ�
	ui.statusBar->addPermanentWidget(currentTimeLabel); //��״̬����Ӵ˿ؼ�
	QTimer *timer = new QTimer(this);
	timer->start(1000); //ÿ��1000ms����timeout���ź�
	connect(timer, SIGNAL(timeout()), this, SLOT(time_update()));


}

void IRProc::btn_cross()
{
	cdlg = new CrossDlg;
	cdlg->show();
}

void IRProc::showTemper()
{
	if (g_flag_showTemper)
	{
		g_flag_showTemper = 0;
	}
	else
	{
		g_flag_showTemper = 1;
	}

}


void IRProc::time_update()
{
	QDateTime current_time = QDateTime::currentDateTime();
	QString timestr = current_time.toString("yyyy-MM-dd hh:mm:ss      "); //������ʾ�ĸ�ʽ

	currentTimeLabel->setText(timestr); //����label���ı�����Ϊʱ��
	
}

void IRProc::btn_closeAll()
{


	qDeleteAll(ui.widget2->findChildren<QLabel*>());
	qDeleteAll(ui.widget2->findChildren<QPushButton*>());

	for (int i = 0; i < g_picNum; i++)
		g_img_show_flag[i] = 0;

//	updateImage();

}

void IRProc::btn_closeOther()
{
	if (g_picNum == 0)
		return;

	qDeleteAll(ui.widget2->findChildren<QLabel*>());
	qDeleteAll(ui.widget2->findChildren<QPushButton*>());

	//g_picNum = 1;

	//memcpy(g_pData[0], g_pData[g_cur_img], IMAGE_HEIGHT*IMAGE_WIDTH*sizeof(short));
	//g_temper[g_cur_img].copyTo(g_temper[0]);

	//g_img[g_cur_img].copyTo(g_img[0]);
	//g_img_gray[g_cur_img].copyTo(g_img_gray[0]);
	//g_src[g_cur_img].copyTo(g_src[0]);
	//g_src_gray[g_cur_img].copyTo(g_src_gray[0]);
	//g_qImgShow[0] = g_qImgShow[g_cur_img].copy();
	//g_qImgShow_gray[0] = g_qImgShow_gray[g_cur_img].copy();

	//g_ratio[0] = g_ratio[g_cur_img];

	//g_cur_img = 0;

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
	Mat dstImage; //�����ͼƬ����
	MatND dstHist; //ֱ��ͼ���󣬶�Ӧ�ϰ汾�е�cvCreateHist����
	int g_hdims = 64;     // ����HIST�ĳ�ʼ������Խ��Խ��ȷ

	dstImage = Mat::zeros(256, 792, CV_8UC3);//ÿ�ζ�Ҫ��ʼ��
	float hranges[] = { 0, 50 }; //�Ҷȷ�Χ
	const float *ranges[] = { hranges };//�Ҷȷ�Χ��ָ��

	if (g_hdims == 0)
	{
		printf("ֱ��ͼ��������Ϊ�㣡\n");
	}
	else
	{
		/*
		srcImage:����ľ���
		1:����ĸ���Ϊ1
		0����Ϊ�Ҷ�ͼ���һ��ͨ��������ѡ0��ͨ��
		Mat��������ʾ��ʹ����Ĥ
		dstHist:�����Ŀ��ֱ��ͼ
		1����Ҫ�����ֱ��ͼ��ά��Ϊ1
		g_hdims:����HIST�ĸ���
		ranges:��ʾÿһά�ȵ���ֵ��Χ
		*/
		//int channels=0;
		calcHist(&srcImage, 1, 0, Mat(), dstHist, 1, &g_hdims, ranges); // ����ֱ��ͼ��Ӧ�ϰ汾��cvCalcHist

		/* ��ȡ�����Сֵ */
		double max = 0;
		minMaxLoc(dstHist, NULL, &max, 0, 0);// Ѱ�����ֵ����λ�ã���Ӧ�ɰ汾��cvGetMinMaxHistValue();

		/*  ���ֱ��ͼ    */

		double bin_w = (double)dstImage.cols / g_hdims;  // hdims: ���ĸ������� bin_w Ϊ���Ŀ��
		double bin_u = (double)dstImage.rows / max;  //// max: ����������ظ������� bin_u Ϊ�������صĸ߶�

		// ��ֱ��ͼ
		for (int i = 0; i<g_hdims; i++)
		{
			Point p0 = Point(i*bin_w, dstImage.rows);//��Ӧ�ɰ汾�е�cvPoint����

			int val = dstHist.at<float>(i);//ע��һ��Ҫ��float���ͣ���Ӧ�ɰ汾�е� cvGetReal1D(hist->bins,i);
			Point p1 = Point((i + 1)*bin_w, dstImage.rows - val*bin_u);
			rectangle(dstImage, p0, p1, cvScalar(0, 255), 1, 8, 0);//��Ӧ�ɰ��е�cvRectangle();
		}

		/*   ���̶�   */
		char string[12];//���ת����ʮ��������ת����ʮ���ƺ��λ��������12λ�������������Լ��趨
		//��������̶ȣ����ظ�����
		//int kedu = 0;
		//for (int i = 1; kedu<max; i++)
		//{
		//	kedu = i*max / 10;//�˴�ѡ��10���̶�
		//	_itoa(20+i, string, 10);//��һ������ת��Ϊ�ַ�����������е�10ָʮ����
		//	//��ͼ������ʾ�ı��ַ���
		//	putText(dstImage, string, Point(0, dstImage.rows - kedu*bin_u), 1, 1, Scalar(0, 255, 255));//��Ӧ�ɰ��е�cvPutText����
		//}
		//��������̶ȣ����ػҶ�ֵ��
		int kedu = 0;
		for (int i = 0; kedu<250; i++)
		{
			kedu = 8 + i * 256 / 16;//�˴�ѡ����Ϊ20
			_itoa(24 + i, string, 10);//��һ������ת��Ϊ�ַ���
			//��ͼ������ʾ�ı��ַ���
			putText(dstImage, string, cvPoint(kedu*(dstImage.cols / 256), dstImage.rows), 1, 1, Scalar(0, 255, 255));
		}
		namedWindow("�¶ȷֲ�", 1);
		imshow("�¶ȷֲ�", dstImage);
	}

}


void IRProc::changeMerGender()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();

	g_mer_gender = text;

}
void IRProc::changeMerPose()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();
	
	g_mer_pose = text;

}
void IRProc::changeMerType()
{

	QToolButton  *tb = (QToolButton*)this->sender();
	QString text = tb->text();

	int type = text.toInt();

	if (type == 1)
	{
		g_mer_type = "HB";
	}
	else if (type == 2)
	{
		g_mer_type = "ACU";
	}
	else
	{
		g_mer_type = text;
	}

	QString merFilePath = g_merge_path + g_mer_gender + "/" + g_mer_type + "/" + g_mer_pose + ".jpg";

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
	cv::resize(timg, g_mer_src, Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src.copyTo(g_mer);

	g_mer_ratio = 0.2;
	//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();


}

void IRProc::btnMerWid()
{
	Mat acu_n;
	g_mer_hratio = g_mer_hratio + 0.2;
	cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

	m_mx = IMAGE_HEIGHT* g_mer_hratio / 2 - IMAGE_HEIGHT / 2;
	m_my = IMAGE_WIDTH * g_mer_vratio / 2 - IMAGE_WIDTH / 2;
	Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);
	//	Rect rect2(224, 224, 128, 128);

	Mat roi1;
	acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	roi1.copyTo(g_mer);
	updateImage();

}
void IRProc::btnMerNar()
{
	Mat acu_n;

	if (g_mer_hratio - 1.2 > 0.0000001)
		g_mer_hratio = g_mer_hratio - 0.2;
	else
		g_mer_hratio = 1;

	cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

	m_mx = IMAGE_HEIGHT* g_mer_hratio / 2 - IMAGE_HEIGHT / 2;
	m_my = IMAGE_WIDTH * g_mer_vratio / 2 - IMAGE_WIDTH / 2;
	Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);
	//	Rect rect2(224, 224, 128, 128);

	Mat roi1;
	acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	roi1.copyTo(g_mer);
	updateImage();
}
void IRProc::btnMerHigher()
{
	Mat acu_n;
	g_mer_vratio = g_mer_vratio + 0.2;
	cv::resize(g_mer_src, acu_n, Size( IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

	m_mx = IMAGE_HEIGHT* g_mer_hratio / 2 - IMAGE_HEIGHT/2;
	m_my = IMAGE_WIDTH * g_mer_vratio / 2 - IMAGE_WIDTH/2;
	Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);
	//	Rect rect2(224, 224, 128, 128);

	Mat roi1;
	acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	roi1.copyTo(g_mer);
	updateImage();

}
void IRProc::btnMerLower()
{
	Mat acu_n;
	
	if (g_mer_vratio - 1.2 > 0.0000001)
		g_mer_vratio = g_mer_vratio - 0.2;
	else
		g_mer_vratio = 1;

	cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

	m_mx = IMAGE_HEIGHT* g_mer_hratio / 2 - IMAGE_HEIGHT / 2;
	m_my = IMAGE_WIDTH * g_mer_vratio / 2 - IMAGE_WIDTH / 2;
	Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);
	//	Rect rect2(224, 224, 128, 128);

	Mat roi1;
	acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

	roi1.copyTo(g_mer);
	updateImage();

}
void IRProc::btnMerLeft()
{
	if (g_mer_hratio > 1)
	{
		Mat acu_n;

		cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

		m_mx = m_mx + 20;
		if (m_mx >= (IMAGE_HEIGHT* g_mer_hratio - IMAGE_HEIGHT)) m_mx = (IMAGE_HEIGHT* g_mer_hratio - IMAGE_HEIGHT) - 1;

		Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);

		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

		roi1.copyTo(g_mer);
		updateImage();
	}



}
void IRProc::btnMerRight()
{
	if (g_mer_hratio > 1)
	{
		Mat acu_n;

		cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

		m_mx = m_mx - 20;
		if (m_mx <0) m_mx = 0;

		Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);

		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

		roi1.copyTo(g_mer);
		updateImage();
	}
	

}
void IRProc::btnMerUp()
{
	if (g_mer_vratio > 1)
	{
		Mat acu_n;

		cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

		m_my = m_my + 20;
		if (m_my >= (IMAGE_WIDTH * g_mer_vratio - IMAGE_WIDTH)) m_my = (IMAGE_WIDTH * g_mer_vratio - IMAGE_WIDTH) - 1;
	
		Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);

		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

		roi1.copyTo(g_mer);
		updateImage();

	}

	


}
void IRProc::btnMerDown()
{
	if (g_mer_vratio > 1)
	{
		Mat acu_n;

		cv::resize(g_mer_src, acu_n, Size(IMAGE_HEIGHT* g_mer_hratio, IMAGE_WIDTH * g_mer_vratio), 0, 0);

		m_my = m_my - 20;
		if (m_my <0) m_my = 0;


		Rect rect1(m_mx, m_my, IMAGE_HEIGHT, IMAGE_WIDTH);

		Mat roi1;
		acu_n(rect1).copyTo(roi1); // copy the region rect1 from the image to roi1

		roi1.copyTo(g_mer);
		updateImage();
	}
	
}


void IRProc::sliderBot()
{
	int pos = ui.slider_bot->value();
	g_bot = pos*1.0/10;
	updateImage();

}

void IRProc::sliderMerRatio2()
{
	int pos = ui.slider_mer_ratio2->value();
	g_mer_ratio = pos*1.0 / 100;
	ui.slider_mer_ratio->setValue(pos);
	updateImage();

}

void IRProc::sliderMerRatio()
{
	int pos = ui.slider_mer_ratio->value();
	g_mer_ratio = pos*1.0 / 100;
	ui.slider_mer_ratio2->setValue(pos);
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
	cv::resize(timg, g_mer_src, Size(IMAGE_HEIGHT, IMAGE_WIDTH), 0, 0);
	g_mer_src.copyTo(g_mer);

	g_mer_ratio = 0.2;
//	g_img[g_cur_img] = g_img[g_cur_img] * (1 - g_mer_ratio) + g_mer * g_mer_ratio;

	updateImage();

}



void IRProc::userAreaFull()
{
	QMessageBox::information(this, tr("Information"), QStringLiteral("<font size='26' color='white'>�û������</font>"));

}

void IRProc::toolBarExpand()
{
	QMessageBox::information(this, tr("Information"), QStringLiteral("չ�����й�����"));
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

	if (text == QString::fromLocal8Bit("��ʾ״̬��"))
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

	if (text == QString::fromLocal8Bit("��ʾ������"))
	{
		if (chbox->isChecked())
		{
			ui.pageDataCal->show();
			ui.tabWidget->setCurrentWidget(ui.pageDataCal);
		}
		else
		{
			ui.pageDataCal->hide();
			ui.tabWidget->setCurrentWidget(ui.pageImgThum);
		}

	}

	//	QMessageBox::information(this, tr("Information"), text);
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
	QString name = sender()->objectName();//�����ź��ߵĶ�����



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
		lb->setObjectName(QString::number(r + BIG_IMG_BASE));
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
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // �������
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // ����������

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
		m_msg = QString::fromLocal8Bit("��ѡ���û�\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}

	g_cardID = ui.tableWidget->item(row, 1)->text();
	g_scanID = ui.tableWidget->item(row, 2)->text();
	g_age = ui.tableWidget->item(row, 6)->text();
	g_name = ui.tableWidget->item(row, 3)->text();
	g_gender = ui.tableWidget->item(row, 4)->text();

	ui.comboBox_user->addItem(g_name);

	ui.listWidget_date->addItem(g_scanID);


	conDataBase();

	map<string, string> mapUserInfoResp;

	int ret = m_cli.get_png_id(g_scanID.toStdString(), mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("��ȡ�û���Ϣʧ��\n");
		m_msg.append(m_cli.get_msg().c_str());
		//	m_cli.close();
		 QMessageBox::information(NULL, "Title", m_msg);
		//mb_conn.EnableWindow(TRUE);
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("��ȡ�û���ϢΪ��");
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
		m_msg = QString::fromLocal8Bit("ͼƬindex�б�Ϊ��\n���ȵ��á���ȡ��Ϣ���ӿ�");
		 QMessageBox::information(NULL, "Title", m_msg);
		 return;
	}
	else
	{

		unsigned short sPicData[PIC_SIZE]; // = (unsigned short*)malloc(PIC_SIZE * sizeof(short));

		pic_count = (int)vecPngIDResp.size();

		if (pic_count > IMGE_TOTAL_NUM) pic_count = IMGE_TOTAL_NUM;
		for (int i = 0; i < pic_count; ++i)
		{
			g_pData[i] = (unsigned short*)malloc(PIC_SIZE * sizeof(short));
			m_msg = QString::fromLocal8Bit("��ȡͼƬ ");
			m_msg.append(vecPngIDResp[i].c_str());

			//memset(sPicData, 0, PIC_SIZE*sizeof(short));
			if (!m_cli.get_png(g_scanID.toStdString(), vecPngIDResp[i], sPicData, PIC_SIZE))
			{
				m_msg.append(QString::fromLocal8Bit(" ͼ��Ϊ��\n"));
				//m_msg.append(m_cli.get_msg().c_str());
				QMessageBox::information(NULL, "Title", m_msg);
				m_cli.close();
				conDataBase();
				return;
				break;
			}
			else
			{
				m_msg.append(QString::fromLocal8Bit(" �ɹ�\n"));

				g_pData[i] = new unsigned short[IMAGE_WIDTH*IMAGE_HEIGHT];
				*(g_pData[i]) = sPicData[0];
				for (int j = 1; j < PIC_SIZE; ++j)
				{
					//outfile << " " << sPicData[j];
					*(g_pData[i] + j) = sPicData[j];
				}

				//	 QMessageBox::information(NULL, "Title", m_msg);
			}
		}
	}

	for (g_picNum = 0; g_picNum < pic_count; g_picNum++)
	{
		g_temper[g_picNum].create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_32FC1);

		data2Temper(g_pData[g_picNum], g_temper[g_picNum], IMAGE_HEIGHT, IMAGE_WIDTH, 100);

		g_TR[g_picNum] = calTR(g_temper[g_picNum]);

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
		g_img_show_flag[g_picNum] = 1;
	}

	changeLabel(g_picNum, IMAGE_PER_ROW);

	showImage(g_picNum);

	showThum();
	//updateImage();
}


void IRProc::showThum()
{
	if (g_picNum == 0) return;

	qDeleteAll(ui.pageImgThum->findChildren<QLabel*>());
	qDeleteAll(ui.pageImgThum->findChildren<QPushButton*>());

	int imgRow = 2;

	int rows = (g_picNum - 1) / imgRow + 1;

	int count = 0;
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < imgRow; y++)
		{
			QLabel *lb = new QLabel;
			lb->setText(QString::number(x * imgRow + y));
			lb->setObjectName(QString::number(x * imgRow + y + THUM_IMG_BASE));
			lb->setFrameShape(QFrame::Box);

			lb->setStyleSheet(QLatin1String("backgroud-color:rgb(255,255,255);border:0px;"));
			lb->setAlignment(Qt::AlignCenter);
			lb->setFixedSize(60, 80);
			ui.gridLayout_3->addWidget(lb, x, y);

		//	connect(lb, SIGNAL(calData()), this, SLOT(calData()));
			count++;
			if (count >= g_picNum) break;
		}
		if (count >= g_picNum) break;
	}

	for (int i = 0; i < g_picNum; i++)
	{
		QLabel *p = ui.pageImgThum->findChild<QLabel*>(QString::number(i + THUM_IMG_BASE));


		int with = p->width();
		int height = p->height();

		QPixmap pixmap = QPixmap::fromImage(g_qImgShow[i]);
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // �������
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // ����������
		p->setPixmap(fitpixmap);
	}

}


void IRProc::changeLabel(int totalNum, int imagePerRow)//������ʾ������
{
	if (g_picNum == 0) return;

	qDeleteAll(ui.widget2->findChildren<QLabel*>());
	qDeleteAll(ui.widget2->findChildren<QPushButton*>());
	
	int rows = (totalNum - 1) / imagePerRow + 1;

	int hei = ui.widget2->height() / rows - 10;
	int wid = hei * 3 / 4;

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
				if (hei >= 640)
				{
					lb->setFixedSize(480, 640);
				}
				else if (hei >= 560)
				{
					lb->setFixedSize(420, 560);
				}
				else if (hei >= 480)
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
			}
		
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
			if (g_img_show_flag[x * imagePerRow + y])
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
			}
			count++;
			if (count >= totalNum) break;
		}
		if (count >= totalNum) break;
	}

}

void IRProc::calData()
{
	//QMessageBox::information(this, "size", QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_max) + "," + QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_min) + "," + QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_aver));
	ui.max_TR->setText(QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_max - g_TR[g_cur_img],10,4));
	ui.min_TR->setText(QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_min - g_TR[g_cur_img], 10, 4));
	ui.av_TR->setText(QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_aver - g_TR[g_cur_img], 10, 4));
	if (g_shape_no[g_cur_img] == 0)
	{
		ui.max_max->setText(QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_max, 10, 4));
	}
	else
	{
		ui.max_max->setText(QString::number(allshape[g_cur_img][g_shape_no[g_cur_img]].t_max - allshape[g_cur_img][g_shape_no[g_cur_img] - 1].t_max, 10, 4));
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
		p->update();

	}
	else
	{
		//changeLabel(g_picNum, IMAGE_PER_ROW);
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
	for (int i = 0; i < g_picNum; i++)
	{
		if (g_color_type)
		{
			data2Img(g_pData[i], g_img[i], IMAGE_HEIGHT, IMAGE_WIDTH, g_win_width, g_color_type, g_filter_type, g_bot);
			g_img[i].copyTo(g_src[i]);
			g_img[i] = g_img[i] * (1 - g_mer_ratio) + g_mer* g_mer_ratio;
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

	ui.slider_bot->setValue(g_bot * 10);

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


void IRProc::addData(int index, QString cardID, QString scanID, QString RegTime)
{
	ui.tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(index + 1)));
	ui.tableWidget->setItem(index, 1, new QTableWidgetItem(cardID));
	ui.tableWidget->setItem(index, 2, new QTableWidgetItem(scanID));
	ui.tableWidget->setItem(index, 7, new QTableWidgetItem(RegTime));

	std::string sScanID = scanID.toStdString();
	;
	std::map<std::string, std::string> mapUserInfoResp;
	int ret = m_cli.get_info(sScanID, mapUserInfoResp);
	if (-1 == ret)
	{
		m_msg = QString::fromLocal8Bit("��ȡ�û���Ϣʧ��\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();

		return;
	}
	else if (0 == ret)
	{
		m_msg = QString::fromLocal8Bit("��ȡ�û���ϢΪ��");
		//	QMessageBox::information(NULL, "Title", m_msg);
		ui.tableWidget->setItem(index, 3, new QTableWidgetItem(""));
		ui.tableWidget->setItem(index, 4, new QTableWidgetItem(""));
		ui.tableWidget->setItem(index, 6, new QTableWidgetItem(""));
		return;
	}
	else
	{
		m_msg = QString::fromLocal8Bit("��ȡ�û���Ϣ�ɹ�");
		//	 QMessageBox::information(NULL, "Title", m_msg);
		std::map<std::string, std::string>::iterator it = mapUserInfoResp.begin();
		//for (; it != mapUserInfoResp.end(); ++it)
		//{
		//	m_msg = QString::fromLocal8Bit("��ȡ ");
		//	m_msg.append(QString::fromLocal8Bit(it->first.c_str()));
		//	m_msg.append("\n");
		//	m_msg.append(QString::fromLocal8Bit(it->second.c_str()));
		//	// QMessageBox::information(NULL, "Title", m_msg);
		//	
		//}

		it = mapUserInfoResp.begin();
		QString rectmp;
		rectmp = it->second.c_str();
		g_age = rectmp;//����
		it++;
		rectmp = QString::fromLocal8Bit(it->second.c_str());
		g_ID = rectmp;//֤����
		it++;
		rectmp = QString::fromLocal8Bit(it->second.c_str());
		g_name = rectmp;//����
		it++;
		rectmp = it->second.c_str();//ͼ��
		it++;
		rectmp = it->second.c_str();
		it++;
		rectmp = rectmp = it->second.c_str();
		if (rectmp == "0")
			g_gender = QString::fromLocal8Bit("��");//�Ա�
		else
			g_gender = QString::fromLocal8Bit("Ů");

		ui.tableWidget->setItem(index, 3, new QTableWidgetItem(g_name));
		ui.tableWidget->setItem(index, 4, new QTableWidgetItem(g_gender));
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
		m_msg = QString::fromLocal8Bit("���ӳɹ�");

		//QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("����ʧ��\n��ȷ��IP��˿ں�");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}
	//2-
	std::string sPermissions;
	// ע�⣺ 0��ʾ���Կͻ��ˣ� 1��ʾ��ʽ�ͻ���
	int iRet = m_cli.login_auth(g_user.toStdString().c_str(), g_passwd.toStdString().c_str(), sPermissions, iTestFlag);
	if (0 > iRet)
	{
		m_msg = QString::fromLocal8Bit("��ȡ��Ȩ��ʧ��\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("��ȡ��Ȩ��ɹ�\n");
		QString sAuth = QString::fromStdString(m_cli.get_auth().c_str());
		QString sRep1 = sAuth.mid(8, 16);
		QString sRep2('*', sRep1.length());
		sAuth.replace(sRep1, sRep2);
		if (0 == iRet)
		{
			m_msg.append(QString::fromLocal8Bit("���Ի���: "));

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

		//m_msg.append(QString::fromLocal8Bit("\nȨ��: "));
		//m_msg.append(QString::fromLocal8Bit("ͼ��ɨ��"));
		//if (pt.p1)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}
		//m_msg.append(QString::fromLocal8Bit(",ͼ�����"));
		//if (pt.p2)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}
		//m_msg.append(QString::fromLocal8Bit(",ϵͳ����"));
		//if (pt.p3)
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}
		//else
		//{
		//	m_msg.append(QString::fromLocal8Bit("(��)"));
		//}

		if (pt.p1 || pt.p3)
		{
			g_log_flag = 1;
			//	QMessageBox::information(NULL, "Title", m_msg);

		}
		else
		{
			m_msg.append(QString::fromLocal8Bit("û��Ȩ��"));
			QMessageBox::information(NULL, "Title", m_msg);
		}

	}

}
void IRProc::updateData()
{

	std::map <std::string, std::string> mapParams;
	mapParams["data_type"] = "4";
	mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	mapParams["page_num"] = QString::number(g_curPage).toStdString();
	//mapParams["name"] = "����";
	//mapParams["cardid"] = "CARD100000000001";
	//mapParams["scanid"] = "SCAN001";
	//mapParams["begin"] = "2018-12-01 00:00:00";
	//mapParams["end"] = "2019-12-01 00:00:00";
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;
	;
	int iRet = m_cli.get_listdata(sParams, sData);

	if (0 < iRet)
	{
		//m_msg = "���:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

		QString temp = lst[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();

		g_maxPage = (dataNum - 1) / g_pageSize + 1;


		lst[0] = lst[0].section('=', -1, -1);
		ui.tableWidget->setRowCount(lst.size());
		for (int i = 0; i != lst.size(); ++i)
		{
			//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

			addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("ʧ��\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}
}

void IRProc::btn_showAll()
{
	std::map <std::string, std::string> mapParams;
	mapParams["data_type"] = "4";
	//mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	//mapParams["page_num"] = QString::number(g_curPage).toStdString();

	//mapParams["name"] = ui.lineEdit_name->text().toStdString();
	////mapParams["cardid"] = "CARD100000000001";
	//mapParams["scanid"] = ui.lineEdit_scanID->text().toStdString();
	//mapParams["begin"] = ui.dateEdit_start->text().toStdString();
	//mapParams["end"] = ui.dateEdit_end->text().toStdString();
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;
	;
	int iRet = m_cli.get_listdata(sParams, sData);

	if (0 < iRet)
	{
		//m_msg = "���:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

		QString temp = lst[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();

		g_maxPage = (dataNum - 1) / g_pageSize + 1;


		lst[0] = lst[0].section('=', -1, -1);
		ui.tableWidget->setRowCount(lst.size());
		for (int i = 0; i != lst.size(); ++i)
		{
			//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

			addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("ʧ��\n");
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
	if (0 < iRet1)
	{
		//m_msg = "���:\n";
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
			m_msg = QString::fromLocal8Bit("δ�ҵ���¼\n");
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
				//m_msg = "���:\n";
				m_msg.append(QString::fromLocal8Bit(sData.c_str()));
				//	QMessageBox::information(NULL, "Title", m_msg);
				QList<QString> lst;
				lst = m_msg.split(';');

				QString temp = lst[0].section('&', 0, 0);
				int dataNum = temp.section('=', -1, -1).toInt();

				if (dataNum == 0)
				{
					m_msg = QString::fromLocal8Bit("δ�ҵ���¼\n");
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
				m_msg = QString::fromLocal8Bit("����ʧ��\n");
				m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
				QMessageBox::information(NULL, "Title", m_msg);
				m_cli.close();
				conDataBase();
			}



		}
	}
	else
	{
		m_msg = QString::fromLocal8Bit("����ʧ��\n");
		m_msg.append(QString::fromLocal8Bit(m_cli.get_msg().c_str()));
		QMessageBox::information(NULL, "Title", m_msg);
		m_cli.close();
		conDataBase();
	}




}


void IRProc::btn_dateSel()
{
	std::map <std::string, std::string> mapParams;
	mapParams["data_type"] = "4";
	mapParams["page_size"] = QString::number(g_pageSize).toStdString();
	mapParams["page_num"] = QString::number(g_curPage).toStdString();
	//mapParams["name"] = "����";
	//mapParams["cardid"] = "CARD100000000001";
	//mapParams["scanid"] = "SCAN001";
	mapParams["begin"] = ui.dateEdit_start->text().toStdString();
	mapParams["end"] = ui.dateEdit_end->text().toStdString();
	std::string sParams = map_join(mapParams, '&', '=');

	std::string sData;
	;
	int iRet = m_cli.get_listdata(sParams, sData);

	if (0 < iRet)
	{
		//m_msg = "���:\n";
		m_msg.append(QString::fromLocal8Bit(sData.c_str()));
		//QMessageBox::information(NULL, "Title", m_msg);
		QList<QString> lst;
		lst = m_msg.split(';');

		QString temp = lst[0].section('&', 0, 0);
		int dataNum = temp.section('=', -1, -1).toInt();

		g_maxPage = (dataNum - 1) / g_pageSize + 1;


		lst[0] = lst[0].section('=', -1, -1);
		if (dataNum == 0)
		{
			m_msg = QString::fromLocal8Bit("δ�ҵ���¼\n");
			QMessageBox::information(NULL, "Title", m_msg);
			return;
		}

		ui.tableWidget->setRowCount(lst.size());
		for (int i = 0; i != dataNum; ++i)
		{
			//		QMessageBox::information(NULL, "Title", lst[i].section(',',1,1));

			addData(i, lst[i].section(',', 0, 0), lst[i].section(',', 1, 1), lst[i].section(',', -1, -1));
		}

	}
	else
	{
		m_msg = QString::fromLocal8Bit("ʧ��\n");
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
	g_pageSize = ui.lineEdit_page_size->text().toInt();

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
		m_msg = QString::fromLocal8Bit("��ѡ���û�\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}


	g_cardID = ui.tableWidget->item(row, 1)->text();
	g_scanID = ui.tableWidget->item(row, 2)->text();
	g_age = ui.tableWidget->item(row, 6)->text();
	g_name = ui.tableWidget->item(row, 3)->text();
	g_gender = ui.tableWidget->item(row, 4)->text();

	rdlg = new RegDlg;

	rdlg->setWindowTitle(QString::fromLocal8Bit("�޸�"));
	rdlg->setWindowModality(Qt::ApplicationModal);
	rdlg->ui.lineEdit_card->setDisabled(false);
	rdlg->ui.lineEdit_5->setDisabled(false);
	rdlg->ui.btn_reg->setText(QString::fromLocal8Bit("�޸�"));
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


		m_msg = QString::fromLocal8Bit("����: ");
		m_msg.append(g_cardID);
		//QMessageBox::information(NULL, "Title", m_msg);

		int iRet = m_cli.set_cardid(mapCardInfo, g_user.toStdString());
		if (0 < iRet)
		{
			m_msg = QString::fromLocal8Bit("ע�Ῠ��Ϣ�ɹ�\nID:");
			m_msg.append(g_cardID);

		}
		else
		{
			m_msg = QString::fromLocal8Bit("ע�Ῠ��Ϣʧ��\n");
			m_msg.append(m_cli.get_msg().c_str());
			m_cli.close();
			conDataBase();

		}
		//QMessageBox::information(NULL, "Title", m_msg);


		std::map<std::string, std::string> mapUserInfo;
		mapUserInfo["scan_id"] = g_scanID.toStdString();
		//mapUserInfo["card_id"] = sCardID;
		mapUserInfo["pic"] = vec_join(vecPngIDReq, ',');
		mapUserInfo["user"] = g_user.toStdString();

		if (!m_cli.send_info(mapUserInfo))
		{
			m_msg = QString::fromLocal8Bit("�����û���Ϣʧ��\n");
			m_msg.append(m_cli.get_msg().c_str());
			m_cli.close();
			conDataBase();

		}
		else
		{
			m_msg = QString::fromLocal8Bit("�޸ĳɹ�");
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
		m_msg = QString::fromLocal8Bit("��ѡ���û�\n");
		QMessageBox::information(NULL, "Title", m_msg);
		return;
	}


	g_scanID = ui.tableWidget->item(row, 2)->text();



	;
	int iRet = m_cli.del_scanid(g_scanID.toStdString());
	if (0 < iRet)
	{
		m_msg = QString::fromLocal8Bit("ɾ��ɨ��ID�ɹ�\nID:");
		m_msg.append(g_scanID);
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else
	{
		m_msg = QString::fromLocal8Bit("ɾ��ɨ��IDʧ��\n");
		m_msg.append(m_cli.get_msg().c_str());

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
	return s;
}