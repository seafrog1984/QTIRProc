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


};
