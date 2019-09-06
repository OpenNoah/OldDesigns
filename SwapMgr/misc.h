/**************************************
 *FILE    :misc.h
 *PROJECT :Common
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

class Misc: public QObject
{
    Q_OBJECT

public:
    bool setSwapFile(QString path);
    QString swapFile(void);
    unsigned int swapFileSize(void);
    bool changeSwapFileSize(unsigned int size);
    bool swapFileEnable(void);

public slots:
    void IMEShow(void);
    void IMEHide(void);
    void disableBat(void);
    void enableBat(void);
    void quitToQpe(void);
    void shutdown(void);
    void lockScreen(void);
    void setSwapFileEnable(bool e);

private:
    QString v_swapFilePath;
};

extern Misc *misc;

#endif
