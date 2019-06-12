#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IRProc.h"

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



public:
	void changeLabel(int totalNum,int imagePerRow);//������ʾ������
	void showImage(int totalNum);//��ʾͼ��

	void wheelEvent(QWheelEvent*event); //�����������¹��ֵ�ʱ������

	void mousePressEvent(QMouseEvent *event);//������¼���ʵ�ַŴ���С

};
