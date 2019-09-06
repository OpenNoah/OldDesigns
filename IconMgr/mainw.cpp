/**************************************
 *FILE    :mainw.cpp
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/14/2013
***************************************/

#include <qapplication.h>
#include <qcolor.h>
#include "mainw.h"
#include "misc.h"

bool v_showIcons = FALSE;

QIVI::QIVI(QIconView * parent, const QString & text, const QPixmap & icon, class icon *ico): QIconViewItem(parent, text, icon)
{
	qDebug("QIVI constructor load at here!");
	v_ico = ico;
}

MainW::MainW(QWidget * parent, char *name, WFlags f): QWidget(parent, name, f)
{
	qDebug("MainW constructor load at here!");
	a = new apps();

	lv = new QListView(this);
	lv->addColumn("ID");
	lv->addColumn(codec->toUnicode("名称[中文]"));
	lv->addColumn("Name[en]");
	lv->addColumn(codec->toUnicode("路径/Path"));
	lv->setAllColumnsShowFocus(TRUE);
	lv->setGeometry(0, 0, 320, 190);

	//iv = new QIconView(this);
	//iv->setGeometry(0, 0, 320, 190);
	//connect(iv, SIGNAL(clicked(QIconViewItem *)), this, SLOT(iconClicked(QIVI *)));

	initList();

	pbIcons = new QPushButton(codec->toUnicode("显示图标"), this);
	pbIcons->setToggleButton(TRUE);
	pbIcons->setGeometry(0, 190, 160, 30);
	connect(pbIcons, SIGNAL(toggled(bool)), this, SLOT(showIcons(bool)));

	pbq = new QPushButton(codec->toUnicode("退出"), this);
	pbq->setGeometry(160, 190, 160, 30);
	connect(pbq, SIGNAL(clicked()), qApp, SLOT(quit()));

	setMinimumSize(320, 220);
}

MainW::~MainW(void)
{
	qDebug("MainW deconstructor load at here!");
	delete a;
}

void MainW::resizeEvent(QResizeEvent * e)
{
	lv->setGeometry(0, 0, e->size().width(), e->size().height() - 30);
	//iv->setGeometry(0, 0, e->size().width(), e->size().height() - 30);
	pbIcons->setGeometry(0, e->size().height() - 30, e->size().width() / 2,
			     30);
	pbq->setGeometry(e->size().width() / 2, e->size().height() - 30,
			 e->size().width() / 2, 30);
}

QListViewItem *MainW::initNode(QListViewItem * parent, int id, class icon * ico)
{
	QListViewItem *lvi =
	    new QListViewItem(parent, QString::number(id), ico->name(zh_CN),
			      ico->name(en), ico->path());
	//QIVI *ivi = new QIVI(iv, ico->name(zh_CN), ico->icons(zh_CN), ico);
	if (v_showIcons) {
		lvi->setPixmap(1, ico->icons(zh_CN));
		lvi->setPixmap(2, ico->icons(en));
	}
	if (ico->dir() != NULL)
		for (int i = 0; i < ico->dir()->count(); i++)
			initNode(lvi, i, ico->dir()->icon(i));
	return lvi;
}

QListViewItem *MainW::initMainNode(QListView * parent, int id, subdir * dir)
{
	QListViewItem *lvi =
	    new QListViewItem(parent, QString::number(id), dir->name(zh_CN),
			      dir->name(en), dir->path());
#ifdef SHOWICONS
	lvi->setPixmap(1, dir->icons(zh_CN));
	lvi->setPixmap(2, dir->icons(en));
#endif
	for (int i = 0; i < dir->count(); i++)
		initNode(lvi, i, dir->icon(i));
	return lvi;
}

#define RES a->res()
void MainW::initList(void)
{
	for (int i = 0; i < a->count(); i++)
		initMainNode(lv, i, a->dir(i));
	QListViewItem *lvi =
	    new QListViewItem(lv, QString::number(-1),
			      codec->toUnicode("文件关联"), "Resource",
			      RES->path());
	for (int i = 0; i < RES->count(); i++)
		new QListViewItem(lvi, QString::number(i), RES->suffix(i), "=",
				  RES->target(i));
}

void MainW::reload(void)
{
	lv->clear();
	initList();
}

void MainW::showIcons(bool e)
{
	v_showIcons = e;
	reload();
}

void MainW::iconClicked(QIVI *item)
{
	qDebug("GOT an item!");
}
