#pragma once
#include <QtWidgets/QMainWindow>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "ui_IRProc.h"
#include "string.h"
#include "SetAuthDlg.h"
#include "SettingDlg.h"
#include "RegDlg.h"
#include "CrossDlg.h"
#include "ThumLabel.h"

struct shape{
	bool del_flag;
	int shape_type; //1.�㣬2.���� 3. Բ 4.��Բ
	int lt_x, lt_y;//���Ͻ�����
	int rb_x, rb_y;//���½�����
	double t_max;
	double t_min;
	double t_aver;
	double t_msd;
	std::string comment;//ע��
};
typedef struct shape Shape;

using namespace cv;

class IRProc : public QMainWindow
{
	Q_OBJECT

public:
	IRProc(QWidget *parent = Q_NULLPTR);

	void conDataBase();//�������ݿ�
	void updateData();
	void addData(int index, QString cardID, QString scanID, QString RegTime);

private:
	Ui::IRProcClass ui;

	void imgProc();//ͼ����
	void imgMerge();//ͼ���ں�
	void dataManage();//���ݹ���
	void sysSetting();//ϵͳ����
	virtual void resizeEvent(QResizeEvent * event);

public slots:

	void userAreaFull();//ϵͳ����-�û������
	void toolBarExpand();//ϵͳ����-չ�����й�����
	void sysSettingOp();//ϵͳ����-Ȩ������
	void customize();//ϵͳ����-����
	void btn_sysPar();//ϵͳ��������
	void btn_show_func();//��ʾ������

	void imgChange();//ͼ�����Ͻǰ�ť
	void btnAnalyze();//��ͼ��ת������ҳ��
	void updateImage();//����ͼ��
	void colorTypeChange();
	void changeWinWidth();//�޸Ĵ���
	void setFilter(int);//�����˲���ʽ
	void addPoint();//����
	void addRect();//������
	void addCircle();//��Բ��
	void addEllipse();//����Բ
	void btn_closeAll();//�ر�����
	void btn_closeOther();//�ر�����
	void sliderBot();//�ϲ�
	void btn_hist();//ֱ��ͼ
	void btn_cross();//�����߷���
	void btn_sel();//ѡ��
	void btn_zoom();//�Ŵ���С
	void showTemper();//��ʾ�¶�
	void saveMeasure();//�������
	void setUpAll();//ͬ���ϲ�

	void btnMerDef();//�Զ����ں�ͼ
	void sliderMerRatio();//�ں϶ȵ���
	void sliderMerRatio2();//�ں϶ȵ���
	void btnMerWid();//�ں�ͼ���
	void btnMerNar();//�ں�ͼ��խ
	void btnMerHigher();//�ں�ͼ���
	void btnMerLower();//�ں�ͼ�䰫
	void btnMerLeft();//�ں�ͼ����
	void btnMerRight();//�ں�ͼ����
	void btnMerUp();//�ں�ͼ����
	void btnMerDown();//�ں�ͼ����
	void changeMerGender();//�����Ա�
	void changeMerPose();//������λ
	void changeMerType();//��������

	
	void btn_del();
	void btn_change();
	void btn_pre();
	void btn_next();
	void btn_start();
	void btn_end();
	void btn_dateSel();
	void btn_nameSel();
	void btn_showAll();
	void dataOut();//���ݵ���
	void dataIn();//���ݵ���

	void calData();
	void time_update();
	void thumClicked();

public:
	void changeLabel(int totalNum,int imagePerRow);//������ʾ������
	void showImage(int totalNum);//��ʾͼ��

	void wheelEvent(QWheelEvent*event); //�����������¹��ֵ�ʱ������

	//void mousePressEvent(QMouseEvent *event);//������¼���ʵ�ַŴ���С

	//void mouseRelease(QMouseEvent *event);

	//void calPar(Mat &T, Shape &s);//ͳ�������С��
	double calTR(Mat &img);//����ͼ�������

	void showThum();//��ʾ����ͼ

//	void draw_shape(Mat& img, Shape allshape[], int shape_no);//��ʾ��ǩ

	int m_mx, m_my;

	SettingDlg *dlg;
	SetAuthDlg *adlg;
	RegDlg *rdlg;
	CrossDlg *cdlg;

	QString m_msg;

	QLabel *currentTimeLabel;//��ʾϵͳʱ��
};
