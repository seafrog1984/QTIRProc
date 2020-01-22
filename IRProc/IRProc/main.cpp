#include "IRProc.h"
#include <QtWidgets/QApplication>
#include "LogDlg.h"
#include "ActiveDlg.h"
#include <fstream>
#include <iostream>  
#include<QMessageBox>

using namespace std;

int g_active_flag = 0;
int g_time_flag = 0;

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);

	ifstream fin("ac.dat");
	char str[200];

	if (fin.fail())
	{
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("³ÌÐòÎ´¼¤»î£¡"));
		//	exit(-1);
	}
	else
	{
		fin >> str;
	}
	//	AfxMessageBox(_T(str));

	char mac[200];
	getMac(mac);
	//	AfxMessageBox(_T(mac));

	if (CheckSN(mac, str))
	{
		g_active_flag = 1;
	}
	else
	{
		g_active_flag = 0;

		ActiveDlg w;
		w.show();

		return a.exec();
	}

	if (CheckTime(str))
	{
		g_time_flag = 1;
	}
	else
	{
		g_time_flag = 0;
		QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("×¢²áÂëÒÑÊ§Ð§£¡"));

		ActiveDlg w;
		w.show();

		return a.exec();
	}


	if (g_active_flag&&g_time_flag)
	{
		LogDlg w;
		w.show();

		return a.exec();
	}


	//IRProc w;
	//w.show();
	//LogDlg w;
	//w.show();
	//return a.exec();
}
