/**************************************
 *FILE    :numberdialog.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :25/8/2013
***************************************/

#include <qapplication.h>
#include <qinputdialog.h>
#include "numberdialog.h"
#include "misc.h"

#define qDebug(...)

LCDNumber::LCDNumber(int from, int to, int step, QWidget *parent, const char *name): QLCDNumber(parent, name)
{
    qDebug("LCDNumber constructor load at here!");
	v_from = from;
	v_to = to;
	v_step = step;
}

void LCDNumber::mousePressEvent(QMouseEvent *e)
{
	bool ok;
	Misc::IMESet(16);
	Misc::IMEShow();
	int i = QInputDialog::getInteger(qApp->mainWidget()->caption(), TUNIC("手动输入数值:"), (int)value(), v_from, v_to, v_step, &ok, this);
	Misc::IMEHide();
	if (ok)
		emit valueChanged(i);
}

LCDNumber::~LCDNumber(void)
{
    qDebug("LCDNumber deconstructor load at here!");
}

#define DEFH 100
#define DEFW 220
#define SLIH 24
#define PBH 24
#define SPACE 2

NumberDialog::NumberDialog(int minValue, int maxValue, int lineStep, int pageStep, int value, QWidget *parent, const char *name): QDialog(parent, name, true)
{
    qDebug("NumberDialog constructor load at here!");
    pageStep += pageStep == 0;

    lcd = new LCDNumber(minValue, maxValue, 1, this);
	lcd->display(value);

    s = new QScrollBar(minValue, maxValue, lineStep, pageStep, value, Horizontal, this);
    connect(s, SIGNAL(valueChanged(int)), lcd, SLOT(display(int)));
    connect(s, SIGNAL(valueChanged(int)), this, SLOT(valueEmit(int)));
	connect(lcd, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));

	accept = new QPushButton(this);
	accept->setText(TUNIC("确定"));
	connect(accept, SIGNAL(clicked()), this, SLOT(accept()));

	cancel = new QPushButton(this);
	cancel->setText(TUNIC("取消"));
	connect(cancel, SIGNAL(clicked()), this, SLOT(back()));

	orig = value;

	setCaption(qApp->mainWidget()->caption());
    setMinimumSize(DEFW, DEFH);
}

void NumberDialog::back(void)
{
	s->setValue(orig);
	reject();
}

#define RW (e->size().width())
#define RH (e->size().height())
void NumberDialog::resizeEvent(QResizeEvent *e)
{
    qDebug("NumberDialog resize event");
    lcd->setGeometry(0, 0, RW, RH - SLIH - SPACE - PBH - SPACE);
    s->setGeometry(0, RH - SLIH - SPACE - PBH, RW, SLIH);
	accept->setGeometry(0, RH - PBH, RW / 2 - SPACE / 2, PBH);
	cancel->setGeometry(RW / 2 + SPACE / 2, RH - PBH, RW / 2 - SPACE / 2, PBH);
}

NumberDialog::~NumberDialog(void)
{
    qDebug("NumberDialog deconstructor load at here!");
}
