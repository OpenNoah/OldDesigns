/**************************************
 *FILE    :styles.h
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :29/8/2013
***************************************/

#ifndef _STYLES_H
#define _STYLES_H

#include <qobject.h>
#include <qpopupmenu.h>

class style: public QObject
{
	Q_OBJECT

public:
	style(QWidget *parent = 0);
	~style(void);
	unsigned int current(void) const {return v_current;}
	void setCurrent(unsigned int i);

	enum Styles {Windows = 0, Platinum = 1, Motif = 2, MotifPlus = 3, CDE = 4, SGI = 5, Interlace = 6};

public slots:
	void motif(void);
	void windows(void);
	void cde(void);
	void interlace(void);
	void motifplus(void);
	void sgi(void);
	void platinum(void);

private:
	unsigned int v_current;
};

QPopupMenu *setstyles(void);
unsigned int getCurrentStyle(void);
void setCurrentStyle(unsigned int i);

#endif
