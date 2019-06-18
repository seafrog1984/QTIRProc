#pragma once
#include <QtWidgets/QMainWindow>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "ui_IRProc.h"
#include "string.h"

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

private:
	Ui::IRProcClass ui;

	void imgProc();//ͼ����
	void imgMerge();//ͼ���ں�
	void dataManage();//���ݹ���
	void sysSetting();//ϵͳ����

public slots:

	void userAreaFull();//ϵͳ����-�û������
	void toolBarExpand();//ϵͳ����-չ�����й�����
	void sysSettingOp();//ϵͳ����-Ȩ������
	void customize();//ϵͳ����-����

	void imgChange();//ͼ�����Ͻǰ�ť
	void btnAnalyze();//��ͼ��ת������ҳ��
	void updateImage();//����ͼ��
	void colorTypeChange();
	void changeWinWidth();
	void setStep();//���öϲ㲽��
	void setFilter(int);//�����˲���ʽ
	void addPoint();//����
	void addRect();//������
	void addCircle();//��Բ��
	void addEllipse();//����Բ

public:
	void changeLabel(int totalNum,int imagePerRow);//������ʾ������
	void showImage(int totalNum);//��ʾͼ��

	void wheelEvent(QWheelEvent*event); //�����������¹��ֵ�ʱ������

	//void mousePressEvent(QMouseEvent *event);//������¼���ʵ�ַŴ���С

	//void mouseRelease(QMouseEvent *event);

	void calPar(Mat &T, Shape &s);//ͳ�������С��

//	void draw_shape(Mat& img, Shape allshape[], int shape_no);//��ʾ��ǩ



};
