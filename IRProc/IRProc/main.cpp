#include "IRProc.h"
#include <QtWidgets/QApplication>
#include "LogDlg.h"

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	//IRProc w;
	//w.show();
	LogDlg w;
	w.show();
	return a.exec();
}
