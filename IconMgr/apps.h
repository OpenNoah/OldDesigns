/**************************************
 *FILE    :apps.h
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/15/2013
***************************************/

#ifndef _Z_READ_H
#define _Z_READ_H

#include <qstring.h>
#include <qstringlist.h>
#include <qpixmap.h>

#define DEFPATH "/opt/QtPalmtop/apps/"
#define IMGPATH "/opt/QtPalmtop/pics/"
#define DIRPATH "/opt/QtPalmtop/apps2/"

#define LANGNUM 2
#define en 0
#define zh_CN 1

#define OTHERICONS 2
#define ICON_Normal 2
#define ICON_Shadow 3

#define FOCUSICONS 1
#define ICON_Focus 4

class icon {
 public:
	icon(const char *path);
	~icon(void);
	QString path(void) const;
	QString name(const int lang) const;
	QPixmap icons(const int type) const;
	QString icons_path(const int type) const;
	QString idname(void) const;
	QString type(void) const;
	QString other(void) const;
	class subdir *dir(void) const;

 private:
	// Path
	 QString v_path;

	// Store data get from the file
	QString v_exec;
	QString v_name[LANGNUM];
	QString v_comment[LANGNUM];
	QPixmap v_icons[LANGNUM + OTHERICONS + FOCUSICONS];
	QString v_icons_path[LANGNUM + OTHERICONS + FOCUSICONS];
	QString v_idname;
	int v_appid;
	int v_tapid;
	QString v_type;
	QString v_other;
	bool v_wifi;
	class subdir *v_dir;
};

class subdir {
 public:
	// Constructor and deconstructor
	subdir(const char *path);
	~subdir(void);

	// Path
	QString path(void) const;

	// Data get from .directory
	QString name(const int lang) const;
	QPixmap icons(const int type) const;
	QString icons_path(const int type) const;
	QString idname(void) const;
	QString type(void) const;
	QString other(void) const;

	// Icons
	int count(void) const;
	class icon *icon(const int i) const;

 private:
	// Path
	 QString v_path;

	// Store data get from .directory
	QString v_name[LANGNUM];
	QPixmap v_icons[LANGNUM + OTHERICONS];
	QString v_icons_path[LANGNUM + OTHERICONS];
	QString v_idname;
	QString v_type;
	QString v_other;

	// Store icons
	int v_count;
	class icon **v_icon;
};

class res {
 public:
	res(const char *path = DEFPATH "resourcemanager.app");
	~res(void);
	QString path(void) const;
	int count(void) const;
	QString suffix(const int i) const;
	QString target(const int i) const;

 private:
	 QString v_path;
	QString v_other;
	int v_count;
	QStringList v_suffix;
	QStringList v_target;
};

class apps {
 public:
	apps(const char *path = DEFPATH);
	~apps(void);
	QString path(void) const;
	int count(void) const;
	class subdir *dir(const int i) const;
	class res *res(void) const;

 private:
	 QString v_path;
	int v_count;
	class subdir **v_dir;
	class res *v_res;
};

#endif
