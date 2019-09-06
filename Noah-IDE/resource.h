#ifndef RESOURCE_H
#define RESOURCE_H

#include <qpe/resource.h>
#include <qpe/qpeapplication.h>

#define APP_TITLE	tr("Noah-IDE重制版")
#define BASE_DIR	"np1000/Noah-IDE/"
#define CONF_FILE	"Noah-IDE"
#define INIT_DIR	"/mnt/mmc"//"/mnt/UsrDisk/Noah-IDE"
#define GRY_FILE	"_gry"
#define KONSOLE		(QPEApplication::qpeDir() + "bin/embeddedkonsole")
#define RES_LOAD(f)	Resource::loadPixmap(BASE_DIR f)
#define RES_LOAD_GRY(f)	Resource::loadPixmap(BASE_DIR f GRY_FILE)

#define CMD_RUN		(". /etc/profile; " + KONSOLE + " \"cd \'" + info.dirPath() + "\' && './" + project.Attr.project + "' " + args + "\" &")
#define CMD_COMPILE	(". /etc/profile; " + KONSOLE + " \"cd \'" + info.dirPath() + "\' && make\" &")
#define CMD_CLEAN	(". /etc/profile; " + KONSOLE + " \"cd \'" + info.dirPath() + "\' && make clean\" &")
#define CMD_ONEKEY	(". /etc/profile; " + KONSOLE + " \"cd \'" + info.dirPath() + "\' && make && sleep 2s && './" + project.Attr.project + "' " + args + "\" &")

#define	ID_FILE			0
#define	ID_PROJECT		1
#define	ID_EDIT			2
#define	ID_CPP			3
#define	ID_SETTINGS		4
#define	ID_SAVE			5
#define	ID_UNDO			6
#define	ID_REDO			7
#define	ID_TAB			8
#define	ID_COMPILE		9
#define	ID_MSG_WIN		10
#define	ID_EXIT			11
#define	ID_OPEN			12

#define ID_MENU_SUBNEW_C	0
#define ID_MENU_SUBNEW_CPP	1
#define ID_MENU_SUBNEW_H	2
#define ID_MENU_FILE_NEW	0
#define ID_MENU_FILE_OPEN	1
#define ID_MENU_FILE_SAVE	2
#define ID_MENU_FILE_SAVEAS	3
#define ID_MENU_FILE_SAVEALL	4
#define ID_MENU_FILE_CLOSE	5
#define ID_MENU_FILE_CLOSEALL	6
#define ID_MENU_FILE_QUIT	7

#define ID_MENU_SUBNEWPROJ_C	0
#define ID_MENU_SUBNEWPROJ_CPP	1
#define ID_MENU_SUBNEWPROJ_H	2
#define ID_MENU_PROJ_NEW	0
#define ID_MENU_PROJ_OPEN	1
#define ID_MENU_PROJ_SAVE	2
#define ID_MENU_PROJ_CLOSE	3
#define ID_MENU_PROJ_ADD	4
#define ID_MENU_PROJ_ADDNEW	5
#define ID_MENU_PROJ_REMOVE	6
#define ID_MENU_PROJ_PROPERT	7
#define ID_MENU_PROJ_PACKSRC	8
#define ID_MENU_PROJ_PACKIPK	9

#define ID_MENU_EDIT_UNDO	0
#define ID_MENU_EDIT_REDO	1
#define ID_MENU_EDIT_GOTO	2

#define ID_MENU_CPP_ONEKEY	0
#define ID_MENU_CPP_COMPILE	1
#define ID_MENU_CPP_RUN		2
#define ID_MENU_CPP_CLEAN	3

#define ID_MENU_SETTINGS_WIN	0
#define ID_MENU_SETTINGS_EDITOR	1
#define ID_MENU_SETTINGS_ALL	2

#define PIX_UNEDITED		RES_LOAD("unedited")
#define PIX_EDITED		RES_LOAD("edited")

#define PIX_FILE		RES_LOAD("menu_file")
#define PIX_COMPILE		RES_LOAD("menu_compile")
#define PIX_CPP			RES_LOAD("menu_cpp")
#define PIX_EDIT		RES_LOAD("menu_edit")
#define PIX_MSG_WIN		RES_LOAD("menu_msg_win")
#define PIX_OPEN		RES_LOAD("menu_open")
#define PIX_PROJECT		RES_LOAD("menu_project")
#define PIX_REDO		RES_LOAD("menu_undo")
#define PIX_SAVE		RES_LOAD("menu_save")
#define PIX_SETTINGS		RES_LOAD("menu_settings")
#define PIX_UNDO		RES_LOAD("menu_redo")
#define PIX_TAB			RES_LOAD("menu_tab")
#define PIX_EXIT		RES_LOAD("menu_exit")

#define PIX_FILE_GRY		RES_LOAD_GRY("menu_file")
#define PIX_COMPILE_GRY		RES_LOAD_GRY("menu_compile")
#define PIX_CPP_GRY		RES_LOAD_GRY("menu_cpp")
#define PIX_EDIT_GRY		RES_LOAD_GRY("menu_edit")
#define PIX_MSG_WIN_GRY		RES_LOAD_GRY("menu_msg_win")
#define PIX_OPEN_GRY		RES_LOAD_GRY("menu_open")
#define PIX_PROJECT_GRY		RES_LOAD_GRY("menu_project")
#define PIX_REDO_GRY		RES_LOAD_GRY("menu_undo")
#define PIX_SAVE_GRY		RES_LOAD_GRY("menu_save")
#define PIX_SETTINGS_GRY	RES_LOAD_GRY("menu_settings")
#define PIX_UNDO_GRY		RES_LOAD_GRY("menu_redo")
#define PIX_TAB_GRY		RES_LOAD_GRY("menu_tab")
#define PIX_EXIT_GRY		RES_LOAD_GRY("menu_exit")

#endif
