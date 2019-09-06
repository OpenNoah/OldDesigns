/**************************************
 *FILE    :mainw.h
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/14/2013
***************************************/

#ifndef _Z_MAINW_H
#define _Z_MAINW_H

#include <qobject.h>
#include <qwidget.h>
#include <qevent.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qiconview.h>
#include "apps.h"

class QIVI:public QIconViewItem {
 public:
	QIVI(QIconView * parent, const QString & text, const QPixmap & icon, class icon *ico);
	
 private:
	class icon *v_ico;
};

class MainW:public QWidget {
 Q_OBJECT
 public:
	MainW(QWidget * parent = 0, char *name = 0, WFlags f = 0);
	~MainW(void);

 public slots:
	void showIcons(bool);
	void reload(void);
	void iconClicked(QIVI *item);

 private:
	void resizeEvent(QResizeEvent * e);

	void initList(void);
	QListViewItem *initNode(QListViewItem * parent, int id,
				class icon * ico);
	QListViewItem *initMainNode(QListView * parent, int id, subdir * dir);

	apps *a;

	QListView *lv;
	QIconView *iv;
	QPushButton *pbq;
	QPushButton *pbIcons;
};

#endif
