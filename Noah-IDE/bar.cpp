#include <qobject.h>
#include <qapplication.h>
#include "bar.h"
#include "resource.h"

#define qDebug(...)

Bar::Bar(QWidget *parent): QMenuBar(parent)
{
	qDebug("Bar constructor load at here!");

	menuFile = new QPopupMenu(this);
	menuFile->insertItem(tr("新建"), parent, SLOT(newFile()), ID_MENU_FILE_NEW);
	menuFile->insertItem(tr("打开"), parent, SLOT(open()), 0, ID_MENU_FILE_OPEN);
	menuFile->insertSeparator();
	menuFile->insertItem(tr("保存"), parent, SLOT(save()), 0, ID_MENU_FILE_SAVE);
	menuFile->insertItem(tr("另存为"), parent, SLOT(saveAs()), 0, ID_MENU_FILE_SAVEAS);
	menuFile->insertItem(tr("全部保存"), parent, SLOT(saveAll()), 0, ID_MENU_FILE_SAVEALL);
	menuFile->insertSeparator();
	menuFile->insertItem(tr("关闭"), parent, SLOT(close()), 0, ID_MENU_FILE_CLOSE);
	menuFile->insertItem(tr("全部关闭"), parent, SLOT(closeAll()), 0, ID_MENU_FILE_CLOSEALL);
	menuFile->insertSeparator();
	menuFile->insertItem(tr("退出"), parent, SLOT(quit()), 0, ID_MENU_FILE_QUIT);

	menuProject = new QPopupMenu(this);
	menuProject->insertItem(tr("新建工程"), parent, SLOT(newProject()), 0, ID_MENU_PROJ_NEW);
	menuProject->insertItem(tr("打开工程"), parent, SLOT(openProject()), 0, ID_MENU_PROJ_OPEN);
	menuProject->insertItem(tr("保存工程"), parent, SLOT(saveProject()), 0, ID_MENU_PROJ_SAVE);
	menuProject->insertItem(tr("关闭工程"), parent, SLOT(closeProject()), 0, ID_MENU_PROJ_CLOSE);
	menuProject->insertSeparator();
	menuProject->insertItem(tr("添加文件"), parent, SLOT(projectAdd()), 0, ID_MENU_PROJ_ADD);
	menuProject->insertItem(tr("添加新文件"), parent, SLOT(projectAddNew()), 0, ID_MENU_PROJ_ADDNEW);
	menuProject->insertItem(tr("移除文件"), parent, SLOT(projectRemove()), 0, ID_MENU_PROJ_REMOVE);
	menuProject->insertItem(tr("工程属性"), parent, SLOT(projectProperties()), 0, ID_MENU_PROJ_PROPERT);
	menuProject->insertSeparator();
	menuProject->insertItem(tr("源码文件打包"), parent, SLOT(packSrc()), 0, ID_MENU_PROJ_PACKSRC);
	menuProject->insertItem(tr("制作ipk"), parent, SLOT(packIPK()), 0, ID_MENU_PROJ_PACKIPK);

	menuEdit = new QPopupMenu(this);
	menuEdit->insertItem(tr("撤销"), parent, SLOT(undo()), 0, ID_MENU_EDIT_UNDO);
	menuEdit->insertItem(tr("重做"), parent, SLOT(redo()), 0, ID_MENU_EDIT_REDO);
	menuEdit->insertSeparator();
	menuEdit->insertItem(tr("跳转到"), parent, SLOT(jumpTo()), 0, ID_MENU_EDIT_GOTO);

	menuCpp = new QPopupMenu(this);
	menuCpp->insertItem(tr("一键运行"), parent, SLOT(oneKey()), 0, ID_MENU_CPP_ONEKEY);
	menuCpp->insertItem(tr("编译"), parent, SLOT(compile()), 0, ID_MENU_CPP_COMPILE);
	menuCpp->insertItem(tr("运行"), parent, SLOT(run()), 0, ID_MENU_CPP_RUN);
	menuCpp->insertSeparator();
	menuCpp->insertItem(tr("清除"), parent, SLOT(clean()), 0, ID_MENU_CPP_CLEAN);

	menuSettings = new QPopupMenu(this);
	menuSettings->insertItem(tr("全屏编辑"), parent, SLOT(win()), 0, ID_MENU_SETTINGS_WIN);
	menuSettings->insertSeparator();
	menuSettings->insertItem(tr("编辑器"), parent, SLOT(setEditor()), 0, ID_MENU_SETTINGS_EDITOR);
	menuSettings->insertItem(tr("全局"), parent, SLOT(settings()), 0, ID_MENU_SETTINGS_ALL);

	insertItem(tr("文件"), menuFile, ID_FILE);
	insertItem(tr("工程"), menuProject, ID_PROJECT);
	insertItem(tr("编辑"), menuEdit, ID_EDIT);
	insertItem(tr("运行"), menuCpp, ID_CPP);
	insertItem(tr("选项"), menuSettings, ID_SETTINGS);
	insertItem(PIX_SAVE, parent, SLOT(save()), 0, ID_SAVE);
	insertItem(PIX_UNDO, parent, SLOT(undo()), 0, ID_UNDO);
	insertItem(PIX_REDO, parent, SLOT(redo()), 0, ID_REDO);
	insertItem(PIX_TAB, parent, SLOT(tab()), 0, ID_TAB);
	insertItem(PIX_COMPILE, parent, SLOT(oneKey()), 0, ID_COMPILE);
	insertItem(PIX_MSG_WIN, parent, SLOT(win()), 0, ID_MSG_WIN);
	//insertItem(PIX_EXIT, parent, SLOT(quit()), 0, ID_EXIT);
}
