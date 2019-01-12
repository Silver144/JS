#include "jobshop.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	jobshop w;
	w.show();
	return a.exec();
}
