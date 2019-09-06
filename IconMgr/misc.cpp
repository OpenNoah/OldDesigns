/**************************************
 *FILE    :misc.cpp
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/7/2013
***************************************/

#include <qapplication.h>
#include <qcopchannel_qws.h>
#include <stdlib.h>
#include "misc.h"

QTextCodec *codec = QTextCodec::codecForName("UTF-8");
Misc *misc = new Misc;

void Misc::IMEShow(void)
{
	QCopChannel::send("QPE/IME", "showInputMethod()");
}

void Misc::IMEHide(void)
{
	QCopChannel::send("QPE/IME", "hideInputMethod()");
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
