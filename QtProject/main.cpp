/**************************************
 *FILE    :main.cpp
 *PROJECT :QtProject
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/3/2013
***************************************/

//#define QPEAPP

#ifndef QPEAPP
#include <qapplication.h>
#else
#include <qpe/qpeapplication.h>
#endif
#include <qcdestyle.h>
#include <qtextcodec.h>
#include "class.h"
#include "misc.h"

int main(int argc, char *argv[])
{
#ifndef QPEAPP
    QApplication a(argc, argv);
#else
    QPEApplication a(argc, argv);
#endif
    //QTextCodec *codec = QTextCodec::codecForName("GB2312");
    a.setDefaultCodec(codec);
    MainW w;
    w.setCaption("QtProject");
    a.setStyle(new QCDEStyle());
#ifndef QPEAPP
    a.setMainWidget(&w);
    w.showMaximized();
#else
    a.showMainWidget(&w);
#endif
    return a.exec();
}
