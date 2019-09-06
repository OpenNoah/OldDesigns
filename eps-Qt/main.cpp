/**************************************
 *FILE    :main.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :23/8/2013
***************************************/

#include <qapplication.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <unistd.h>
#include "mainw.h"
#include "misc.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setDefaultCodec(codec);
    a.setFont(QFont("SimSun", 15, QFont::Bold));
	if (argc == 2) {
		unlink(FILE);
		if (symlink(argv[1], FILE) != 0) {
			QMessageBox::critical(0, TUNIC(TITLE), TUNIC("无法创建链接文件!"));
			qFatal("Cannot create symbolic link!");
		}
		unlink(FILE ".cfg");
		if (symlink(QString(argv[1]) + ".cfg", FILE ".cfg") != 0) {
			QMessageBox::critical(0, TUNIC(TITLE), TUNIC("无法创建链接文件(配置)!"));
			qFatal("Cannot create symbolic link (cfg file)!");
		}
	}
    MainW w;
    w.setCaption(TUNIC(TITLE));
    a.setMainWidget(&w);
    w.showMaximized();
    return a.exec();
}
