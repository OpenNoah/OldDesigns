#include <qpe/qpeapplication.h>
#include <qtextcodec.h>
#include "mainw.h"

QTextCodec *codec = QTextCodec::codecForName("UTF-8");

// Comments
int main(int argc, char *argv[])
{
	QPEApplication a(argc, argv);
	a.setDefaultCodec(codec);
	a.setFont(QFont("SimSun", 13, QFont::Bold));
	MainW m;
	a.showMainWidget(&m, FALSE);
	qDebug("Application exec() here");
	return a.exec();
}
