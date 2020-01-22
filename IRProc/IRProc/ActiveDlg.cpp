#include "ActiveDlg.h"
#include <fstream>
#include <iostream>  
#include<QMessageBox>

extern int g_active_flag;
extern int g_time_flag;

using namespace std;



ActiveDlg::ActiveDlg(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);

	char mac[200];
	getMac(mac);

	QString qt_mac = QString::fromStdString(mac);

	ui.lineEdit_Mac->setText(qt_mac);

	connect(ui.pushButton_Active, SIGNAL(clicked()), this, SLOT(btn_active()));//激活
	connect(ui.pushButton_Active, SIGNAL(clicked()), this, SLOT(btn_active()));//取消

}

ActiveDlg::~ActiveDlg()
{
}

void ActiveDlg::btn_active()
{
	char *mac = new char[200];
	char *sn = new char[200];

	m_mac = ui.lineEdit_Mac->text();
	m_sn = ui.lineEdit_SN->text();

	QByteArray ba = m_mac.toLatin1(); // must
	mac = ba.data();

	QByteArray ba2 = m_sn.toLatin1(); // must
	sn = ba2.data();


	if (CheckSN(mac, sn))
	{
		g_active_flag = 1;
	}
	else
	{
		g_active_flag = 0;
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("激活码不正确！"));

	}

	if (CheckTime(sn))
	{
		g_time_flag = 1;
	}
	else
	{
		g_time_flag = 0;
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("注册码已失效！"));

	}

	if (g_active_flag&&g_time_flag)
	{
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("激活成功！"));
		ofstream fout("ac.dat");
		fout << sn;

		this->close();
	}

}

void ActiveDlg::btn_cancel()
{
	this->close();
}