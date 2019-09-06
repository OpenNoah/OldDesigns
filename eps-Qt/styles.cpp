/**************************************
 *FILE    :styles.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :29/8/2013
***************************************/

#include <qapplication.h>
#include <qmotifstyle.h>
#include <qwindowsstyle.h>
#include <qcdestyle.h>
#include <qinterlacestyle.h>
#include <qmotifplusstyle.h>
#include <qsgistyle.h>
#include <qplatinumstyle.h>
#include "styles.h"
#include "misc.h"

#define qDebug(...)

style *s;

style::style(QWidget *parent): QObject(parent)
{
    qDebug("style constructor load at here!");
	v_current = Windows;
}

style::~style(void)
{
    qDebug("style deconstructor load at here!");
}

void style::setCurrent(unsigned int i)
{
	v_current = i;
	switch (i) {
	case Windows:
		windows();
		break;
	case Platinum:
		platinum();
		break;
	case Motif:
		motif();
		break;
	case MotifPlus:
		motifplus();
		break;
	case CDE:
		cde();
		break;
	case SGI:
		sgi();
		break;
	case Interlace:
		interlace();
		break;
	}
}

void style::motif(void)
{
    qDebug("Motif style load at here!");
	//qApp->setStyle(new QMotifStyle);
	v_current = Motif;
}

void style::windows(void)
{
    qDebug("Windows style load at here!");
	qApp->setStyle(new QWindowsStyle);
	v_current = Windows;
}

void style::cde(void)
{
    qDebug("CDE style load at here!");
	//qApp->setStyle(new QCDEStyle);
	v_current = CDE;
}

void style::interlace(void)
{
    qDebug("Interlace style load at here!");
	//qApp->setStyle(new QInterlaceStyle);
	v_current = Interlace;
}

void style::motifplus(void)
{
    qDebug("MotifPlus style load at here!");
	//qApp->setStyle(new QMotifPlusStyle);
	v_current = MotifPlus;
}

void style::sgi(void)
{
    qDebug("SGI style load at here!");
	//qApp->setStyle(new QSGIStyle);
	v_current = SGI;
}

void style::platinum(void)
{
    qDebug("Platinum style load at here!");
	//qApp->setStyle(new QPlatinumStyle);
	v_current = Platinum;
}

QPopupMenu *setstyles(void)
{
	s = new style;
    QPopupMenu *menu = new QPopupMenu();
    menu->insertItem(TUNIC("Windows 风格"), s, SLOT(windows()));
    //menu->insertItem(TUNIC("Platinum 风格"), s, SLOT(platinum()));
    //menu->insertItem(TUNIC("Motif 风格"), s, SLOT(motif()));
    //menu->insertItem(TUNIC("MotifPlus 风格"), s, SLOT(motifplus()));
    //menu->insertItem(TUNIC("CDE 风格"), s, SLOT(cde()));
    //menu->insertItem(TUNIC("SGI 风格"), s, SLOT(sgi()));
    //menu->insertItem(TUNIC("Interlace 风格"), s, SLOT(interlace()));
	return menu;
}

unsigned int getCurrentStyle(void) {return s->current();}

void setCurrentStyle(unsigned int i) {s->setCurrent(i);}
