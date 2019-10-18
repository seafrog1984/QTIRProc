#include "SettingDlg.h"
#include <fstream>
#include <iostream> 
#include <QMessageBox>


using namespace std;


extern QString g_user;
extern QString g_passwd;
extern QString g_ip;
extern QString g_port;
extern QString g_uport;
extern float g_step;

extern int g_remember_flag;

int g_code[7] = { 4, 9, 6, 2, 8, 7, 3 };



SettingDlg::SettingDlg(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);


	ui.lineEdit_servIP->setText(g_ip);
	ui.lineEdit_port->setText(g_port);
	ui.lineEdit_uport->setText(g_uport);
	ui.lineEdit_step->setText(QString::number(g_step));

	connect(ui.btn_set_Par, SIGNAL(clicked()), this, SLOT(btn_setPar()));
}

SettingDlg::~SettingDlg()
{
}

void encryption(string& c, int a[]){

	for (int i = 0, j = 0; c[j]; j++, i = (i + 1) % 7){

		c[j] += a[i];

		if (c[j] > 122) c[j] -= 90;
	}
}


void SettingDlg::btn_setPar()
{

	g_ip = ui.lineEdit_servIP->text();
	g_port = ui.lineEdit_port->text();
	g_uport = ui.lineEdit_uport->text();
	g_step = ui.lineEdit_step->text().toFloat();

	string s = g_passwd.toStdString();
	encryption(s, g_code);


	ofstream fout("config.ini");

	fout << g_ip.toStdString() << ' ' << g_port.toStdString() << ' ' << g_uport.toStdString() << ' ' << QString::number(g_step).toStdString() << ' ' << g_user.toStdString() << ' ' << s/*g_passwd.toStdString()*/ << ' ' << g_remember_flag;

	fout.close();

	QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("ÐÞ¸Ä³É¹¦"));


}