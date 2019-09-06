/**************************************
 *FILE	:main.cpp
 *PROJECT :SwapMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :25/7/2013
***************************************/

#include <qpe/qpeapplication.h>
#include "mainw.h"
#include "misc.h"

int main(int argc, char *argv[])
{
	QPEApplication a(argc, argv);
	a.setDefaultCodec(codec);
	a.setFont(QFont("SimSun", 15, QFont::Bold));
	QWidget w;
	w.setCaption(codec->toUnicode("虚拟内存管理程序"));
	MainW m(&w);
	a.showMainWidget(&w, FALSE);
	return a.exec();
}
