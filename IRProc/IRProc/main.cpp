#include "IRProc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	IRProc w;
	w.show();
	return a.exec();
}
