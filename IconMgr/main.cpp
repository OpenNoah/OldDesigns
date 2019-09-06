/**************************************
 *FILE    :main.cpp
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/7/2013
***************************************/

//#define QPEAPP

#ifndef QPEAPP
#include <qapplication.h>
#else
#include <qpe/qpeapplication.h>
#endif
//#include <qcdestyle.h>
#include "mainw.h"
//#include "class.h"
#include "misc.h"

int main(int argc, char *argv[])
{
#ifndef QPEAPP
	QApplication a(argc, argv);
#else
	QPEApplication a(argc, argv);
#endif
	a.setDefaultCodec(codec);
	a.setFont(QFont("SimSun", 15, QFont::Bold));
	MainW w;
	w.setCaption(codec->toUnicode("图标管理程序"));
	//a.setStyle(new QCDEStyle());
#ifndef QPEAPP
	a.setMainWidget(&w);
	w.showMaximized();
#else
	a.showMainWidget(&w, !TRUE);
#endif
	return a.exec();
}
