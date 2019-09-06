/**************************************
 *FILE    :misc.h
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/7/2013
***************************************/

#ifndef _Z_MISC_H
#define _Z_MISC_H

#include <qtextcodec.h>
#include <qobject.h>

#define FUNICODE (codec->fromUnicode(n))
#define TUNICODE (codec->toUnicode(n))

extern QTextCodec *codec;

class Misc:public QObject {
	Q_OBJECT public slots: void IMEShow(void);
	void IMEHide(void);
	void disableBat(void);
	void enableBat(void);
	void quitToQpe(void);
	void shutdown(void);
	void lockScreen(void);
};

extern Misc *misc;

#endif
