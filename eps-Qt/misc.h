/**************************************
 *FILE    :misc.h
 *PROJECT :Common
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :7/6/2013
***************************************/

#ifndef _Z_MISC_H
#define _Z_MISC_H

#include <qobject.h>
#include <qtextcodec.h>

#define FUNIC(n) (codec->fromUnicode(n))
#define TUNIC(n) (codec->toUnicode(n))

#define CAPTION() (qApp->mainWidget()->caption())

extern QTextCodec *codec;

class Misc: public QObject
{
    Q_OBJECT

public slots:
    static void IMEShow(void);
    static void IMEHide(void);
    static void IMESet(unsigned short);
};

#endif
