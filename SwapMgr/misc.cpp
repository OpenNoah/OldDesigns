/**************************************
 *FILE    :misc.cpp
 *PROJECT :Common
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/7/2013
***************************************/

#include <qpe/qpeapplication.h>
#include <qpe/global.h>
#include <qcopchannel_qws.h>
#include <qfile.h>
#include <qtextstream.h>
#include <stdlib.h>
#include <unistd.h>
#include "misc.h"

QTextCodec *codec = QTextCodec::codecForName("UTF-8");
Misc *misc = new Misc;

void Misc::IMEShow(void)
{
    //Global::showInputMethod();
}

void Misc::IMEHide(void)
{
    //Global::hideInputMethod();
}

void Misc::disableBat(void)
{
    system("rm -f /dev/jz-pw");
}

void Misc::enableBat(void)
{
    system("mknod /dev/jz-pw c 10 18 &> /dev/null");
}

void Misc::quitToQpe(void)
{
    //QCopChannel::send("QPE/Application/qpe", "raise()");
    qApp->quit();
}

void Misc::shutdown(void)
{
    system("pwrff");
}

void Misc::lockScreen(void)
{
    system("/mnt/mmc/Noah-IDE/Lock/Lock.sh*");
}

bool Misc::setSwapFile(QString path)
{
    v_swapFilePath = path;
    QFile f(v_swapFilePath);
    if (!f.exists()) {
        if (!f.open(IO_ReadWrite))
            return false;
        f.close();
        if (truncate(v_swapFilePath, 10 * 1024 * 1024) != 0)
            return false;
        if (system("mkswap " + v_swapFilePath) != 0)
            return false;
    }
    return true;
}

void Misc::setSwapFileEnable(bool e)
{
    if (e)
        system("swapon " + v_swapFilePath);
    else
        system("swapoff " + v_swapFilePath);
}

QString Misc::swapFile(void) {return v_swapFilePath;}

unsigned int Misc::swapFileSize(void)
{
    QFile f(v_swapFilePath);
    return f.size();
}

bool Misc::changeSwapFileSize(unsigned int size)
{
    bool e = swapFileEnable();
    if (e)
        setSwapFileEnable(false);
    if (truncate(v_swapFilePath, size) != 0)
        return false;
    if (system("mkswap " + v_swapFilePath) != 0)
        return false;
    if (e)
        setSwapFileEnable(true);
    return true;
}

bool Misc::swapFileEnable(void)
{
    // Read /proc/swaps
    QFile f("/proc/swaps");
    if (!f.open(IO_ReadOnly))
        return false;
    QTextStream t(&f);

	// Ignore the first description line from the file
    QString str(t.readLine());
    while (!t.atEnd()) {
		// Read 1 line from the file
        QString str(t.readLine());

        // If cannot find ' ' than ignore it
        int pos = str.find(' ');
        if (pos == -1) {
            qWarning("Unknown string from /proc/swaps");
            qWarning(str);
            continue;
        }

        // Otherwise, put the path into variables
        QString path = str.left(pos).stripWhiteSpace();

		// If it is the same path
		if (path == v_swapFilePath)
            return true;
	}
    return false;
}
