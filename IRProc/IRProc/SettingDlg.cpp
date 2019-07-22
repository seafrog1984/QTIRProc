#include "SettingDlg.h"
#include <fstream>
#include <iostream> 
#include <QMessageBox>


using namespace std;



extern QString g_ip;
extern QString g_port;
extern QString g_uport;
extern float g_step;

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

void SettingDlg::btn_setPar()
{

	g_ip = ui.lineEdit_servIP->text();
	g_port = ui.lineEdit_port->text();
	g_uport = ui.lineEdit_uport->text();
	g_step = ui.lineEdit_step->text().toFloat();

	ofstream fout("config.ini");

	fout << g_ip.toStdString() << ' ' << g_port.toStdString() << ' ' << g_uport.toStdString()<<' '<<QString::number(g_step).toStdString();

	fout.close();

	QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("ÐÞ¸Ä³É¹¦"));


}