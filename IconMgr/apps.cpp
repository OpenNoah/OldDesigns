/**************************************
 *FILE    :apps.cpp
 *PROJECT :IconMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/15/2013
***************************************/

#include <qapplication.h>
#include <qtextstream.h>
#include <qfile.h>
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <malloc.h>
#include <string>
#include "apps.h"

#define qDebug(str)

icon::icon(const char *path)
{
    qDebug("Icon constructor load at here!");
    v_path = path;
    v_dir = NULL;

    // Read the file: build stream structure
    QFile f(v_path);
    if (!f.open(IO_ReadOnly))
        return;
    QTextStream t(&f);
    t.setEncoding(QTextStream::UnicodeUTF8);

    // Read the file
    while (!t.atEnd()) {
        // Read 1 line from the file
        QString str(t.readLine());

        // If the line is the sign "[Desktop Entry]" than ignore it
        QString find("[Desktop Entry]");
        if (str.find(find, FALSE) != -1)
            continue;

        // If cannot find '=' than ignore it
        int pos = str.find('=');
        if (pos == -1) {
            v_other += str;
            qDebug("Unknown string!!");
            qDebug(str);
            continue;
        }

        // Otherwise, put the contexts into variables
        QString set = str.left(pos).stripWhiteSpace();
        QString con = str.right(str.length() - pos - 1).stripWhiteSpace();
        if (set == "Exec")
            v_exec = con;
        else if (set == "Name" || set == "Name[en]")
            v_name[en] = con;
        else if (set == "Name[zh_CN]")
            v_name[zh_CN] = con;
        else if (set == "Comment" || set == "Comment[en]")
            v_comment[en] = con;
        else if (set == "Comment[zh_CN]")
            v_comment[zh_CN] = con;
        else if (set == "Icon[en]") {
            if (con.right(strlen(".png")) != ".png")
                con.append(".png");
            v_icons[en].load(IMGPATH + con);
            v_icons_path[en] = IMGPATH + con;
        } else if (set == "Icon") {
            if (con.right(strlen(".png")) != ".png")
                con.append(".png");
            v_icons[zh_CN].load(IMGPATH + con);
            v_icons_path[zh_CN] = IMGPATH + con;
        } else if (set == "NormalImg") {
            v_icons[ICON_Normal].load(IMGPATH + con);
            v_icons_path[ICON_Normal] = IMGPATH + con;
        } else if (set == "ShadowIcon") {
            v_icons[ICON_Shadow].load(IMGPATH + con);
            v_icons_path[ICON_Shadow] = IMGPATH + con;
        } else if (set == "FocusIcon") {
            v_icons[ICON_Focus].load(IMGPATH + con);
            v_icons_path[ICON_Focus] = IMGPATH + con;
        } else if (set == "IDName")
            v_idname = con;
        else if (set == "AppId")
            v_appid = con.toInt();
        else if (set == "TapId")
            v_tapid = con.toInt();
        else if (set == "Type") {
            v_idname = con;
            if (con == "Subdirectory")
                v_dir = new class subdir(DIRPATH + v_exec);
        } else if (set == "WifiCheck")
            v_wifi = con == "0";
        else {
            v_other += str;
            qDebug("Unknown string!!");
            qDebug(str);
        }
    }
}

icon::~icon(void)
{
    qDebug("Icon deconstructor load at here!");
    delete v_dir;
}

QString icon::path(void) const {return v_path;}
QString icon::name(int lang) const {return v_name[lang];}
QPixmap icon::icons(int type) const {return v_icons[type];}
QString icon::icons_path(int type) const {return v_icons_path[type];}
QString icon::idname(void) const {return v_idname;}
QString icon::type(void) const {return v_type;}
QString icon::other(void) const {return v_other;}
class subdir* icon::dir(void) const {return v_dir;}

subdir::subdir(const char *path)
{
    qDebug("SubDir constructor load at here!");
    v_path = path;

    // Read .directory: build stream structure
    QFile f(v_path + "/.directory");
    if (!f.open(IO_ReadOnly))
        return;
    QTextStream t(&f);
    t.setEncoding(QTextStream::UnicodeUTF8);

    // Read .directory
    while (!t.atEnd()) {
        // Read 1 line from the file
        QString str(t.readLine());

        // If the line is the sign "[Desktop Entry]" than ignore it
        QString find("[Desktop Entry]");
        if (str.find(find, FALSE) != -1)
            continue;

        // If cannot find '=' than ignore it
        int pos = str.find('=');
        if (pos == -1) {
            v_other += str;
            qDebug("Unknown string!!");
            qDebug(str);
            continue;
        }

        // Otherwise, put the contexts into variables
        QString set = str.left(pos).stripWhiteSpace();
        QString con = str.right(str.length() - pos - 1).stripWhiteSpace();
        if (set == "Name" || set == "Name[en]")
            v_name[en] = con;
        else if (set == "Name[zh_CN]")
            v_name[zh_CN] = con;
        else if (set == "Icon[en]") {
            v_icons[en].load(IMGPATH + con);
            v_icons_path[en] = IMGPATH + con;
        } else if (set == "Icon") {
            v_icons[zh_CN].load(IMGPATH + con);
            v_icons_path[zh_CN] = IMGPATH + con;
        } else if (set == "NormalImg") {
            v_icons[ICON_Normal].load(IMGPATH + con);
            v_icons_path[ICON_Normal] = IMGPATH + con;
        } else if (set == "ShadowIcon") {
            v_icons[ICON_Shadow].load(IMGPATH + con);
            v_icons_path[ICON_Shadow] = IMGPATH + con;
        } else if (set == "IDName")
            v_idname = con;
        else if (set == "Type")
            v_idname = con;
        else {
            v_other += str;
            qDebug("Unknown string!!");
            qDebug(str);
        }
    }

    //Read icons in the directory
    v_count = 0;
    v_icon = NULL;
    DIR *dir;
    dir = opendir(path);
    dirent *dent;
    while ((dent = readdir(dir)) != NULL) {
        QString dname(dent->d_name);
        if (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0 && strcmp(dname.right(strlen(".desktop")).latin1(), ".desktop") == 0 && strcmp(dent->d_name, ".directory") != 0) {
            v_count++;
            std::string p(path);
            if (v_path.right(1) != "/")
                p += "/";
            p += dent->d_name;
            v_icon = (class icon **)realloc(v_icon, v_count * sizeof(class icon *));
            *(v_icon + v_count - 1) = new class icon(p.c_str());
        }
    }
    closedir(dir);
}

subdir::~subdir(void)
{
    qDebug("SubDir deconstructor load at here!");
    for (int i = 0; i < v_count; i++)
        delete *(v_icon + i);
}

QString subdir::path(void) const {return v_path;}
QString subdir::name(const int lang) const {return v_name[lang];}
QPixmap subdir::icons(const int type) const {return v_icons[type];}
QString subdir::icons_path(const int type) const {return v_icons_path[type];}
QString subdir::idname(void) const {return v_idname;}
QString subdir::type(void) const {return v_type;}
QString subdir::other(void) const {return v_other;}
int subdir::count(void) const {return v_count;}
class icon* subdir::icon(const int i) const {return *(v_icon + i);}

res::res(const char *path)
{
    qDebug("Res constructor load at here!");
    v_path = path;
    v_count = 0;

    // Read the file: build stream structure
    QFile f(v_path);
    if (!f.open(IO_ReadOnly))
        return;
    QTextStream t(&f);
    t.setEncoding(QTextStream::UnicodeUTF8);

    // Read the file
    while (!t.atEnd()) {
        // Read 1 line from the file
        QString str(t.readLine());

        // If the line is the sign "[MineType]" than ignore it
        QString find("[MineType]");
        if (str.find(find, FALSE) != -1)
            continue;

        // If cannot find '=' than ignore it
        int pos = str.find('=');
        if (pos == -1) {
            v_other += str;
            qDebug("Unknown string!!");
            qDebug(str);
            continue;
        }

        // Otherwise, put the contexts into variables
        v_count++;
        //v_suffix = (class QString **)realloc(v_suffix, v_count * sizeof(class QString *));
        v_suffix.append(str.left(pos).stripWhiteSpace());
        //v_target = (class QString **)realloc(v_target, v_count * sizeof(class QString *));
        v_target.append(str.right(str.length() - pos - 1).stripWhiteSpace());
    }
}

res::~res(void)
{
    qDebug("Res deconstructor load at here!");
}

QString res::path(void) const {return v_path;}
int res::count(void) const {return v_count;}
QString res::suffix(const int i) const {return *(v_suffix.at(i));}
QString res::target(const int i) const {return *(v_target.at(i));}

apps::apps(const char *path)
{
    qDebug("Apps constructor load at here!");
    v_path = path;
    v_count = 0;
    v_dir = NULL;
    v_res = new class res(v_path + "resourcemanager.app");
    DIR *dir;
    if (!(dir = opendir(path)))
        return;
    dirent *dent;
    while ((dent = readdir(dir)) != NULL)
        if (strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0 && strcmp(dent->d_name, "resourcemanager.app") != 0) {
            v_count++;
            std::string p(path);
            p += dent->d_name;
            v_dir = (class subdir **)realloc(v_dir, v_count * sizeof(class subdir *));
            *(v_dir + v_count - 1) = new class subdir(p.c_str());
        }
    closedir(dir);
}

apps::~apps(void)
{
    qDebug("Apps deconstructor load at here!");
    delete v_res;
    for (int i = 0; i < v_count; i++)
        delete *(v_dir + i);
}

QString apps::path(void) const {return v_path;}
int apps::count(void) const {return v_count;}
class subdir* apps::dir(const int i) const {return *(v_dir + i);}
class res* apps::res(void) const {return v_res;}
