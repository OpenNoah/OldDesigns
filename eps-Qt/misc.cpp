/**************************************
 *FILE    :misc.cpp
 *PROJECT :Common
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :7/6/2013
***************************************/

#include <qcopchannel_qws.h>
#include "misc.h"

QTextCodec *codec = QTextCodec::codecForName("GB2312");

Misc *misc = new Misc;

void Misc::IMEShow(void)
{
    QCopChannel::send("QPE/IME", "showInputMethod()");
}

void Misc::IMEHide(void)
{
    QCopChannel::send("QPE/IME", "hideInputMethod()");
}

void Misc::IMESet(unsigned short i)
{
    QCopChannel::send("QPE/IME", "setInputMethods(unsigned short)", i);
}
