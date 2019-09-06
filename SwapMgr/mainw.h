/**************************************
 *FILE    :mainw.h
 *PROJECT :SwapMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :25/7/2013
***************************************/

#ifndef _Z_MAINW_H
#define _Z_MAINW_H

#include <qobject.h>
#include <qlabel.h>
#include <qslider.h>
#include <qpushbutton.h>

class MainW: public QObject
{
    Q_OBJECT

public:
    MainW(QWidget *p);

private slots:
    void load(void);
    void change(int v);
    void apply(void);
    void enable(bool e);

private:
    QLabel *lFile;
    QLabel *lSize;
    QSlider *s;
    QPushButton *pbApply;
    QPushButton *pbReload;
    QPushButton *pbOnOff;
    QPushButton *pbQuit;
    QWidget *parent;
};

#endif
