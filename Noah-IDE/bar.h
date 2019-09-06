#ifndef _BAR_H
#define _BAR_H

#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

class Bar: public QMenuBar
{
	friend class MainW;

public:
	Bar(QWidget *parent);

protected:
	QPopupMenu *menuFile, *menuProject, *menuEdit, \
		*menuCpp, *menuSettings;
};

#endif
