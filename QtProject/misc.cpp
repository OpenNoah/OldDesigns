/**************************************
 *FILE    :misc.cpp
 *PROJECT :QtProject
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/6/2013
***************************************/

#include <qpe/qpeapplication.h>
#include <qpe/global.h>
#include <qcopchannel_qws.h>
#include <stdlib.h>
#include "misc.h"

QTextCodec *codec = QTextCodec::codecForName("GB2312");
Misc *misc = new Misc;

void Misc::IMEShow(void)
{
    Global::showInputMethod();
}

void Misc::IMEHide(void)
{
    Global::hideInputMethod();
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
    QCopChannel::send("QPE/Application/qpe", "raise()");
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
