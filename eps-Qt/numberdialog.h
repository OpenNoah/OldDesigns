/**************************************
 *FILE    :numberdialog.h
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :25/8/2013
***************************************/

#ifndef _NUMBERDIALOG_H
#define _NUMBERDIALOG_H

#include <qdialog.h>
#include <qscrollbar.h>
#include <qlcdnumber.h>
#include <qpushbutton.h>

class LCDNumber: public QLCDNumber
{
	Q_OBJECT

public:
	LCDNumber(int from, int to, int step, QWidget *parent = 0, const char *name = 0);
	~LCDNumber(void);

signals:
	void valueChanged(int);

protected:
	void mousePressEvent(QMouseEvent *e);

private:
	int v_from;
	int v_to;
	int v_step;
};

class NumberDialog: public QDialog
{
    Q_OBJECT

public:
    NumberDialog(int minValue, int maxValue, int lineStep, int pageStep, int value, QWidget *parent = 0, const char *name = 0);
    ~NumberDialog(void);

signals:
    void valueChanged(int);

private slots:
	void setValue(int v) {s->setValue(v);}
    void valueEmit(int v) {emit valueChanged(v);}
	void back(void);

private:
    void resizeEvent(QResizeEvent *e);

    int orig;
    LCDNumber *lcd;
    QScrollBar *s;
    QPushButton *accept, *cancel;
};

#endif
