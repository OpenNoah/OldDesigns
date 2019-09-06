#include <qapplication.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qpe/config.h>
#include <qfileinfo.h>
#include <stdlib.h>
#include <qinputdialog.h>
#include "mainw.h"
#include "bar.h"
#include "resource.h"

//#define qDebug(...)

MainW::MainW(QWidget *parent): QWidget(parent)
{
	setCaption(APP_TITLE);

	showList = true;
	bar = new Bar(this);
	undoAvailable(false);
	redoAvailable(false);
	fileList = new QListBox(this);
	editor = new QMultiLineEdit(this);
	editor->setReadOnly(true);
	editor->setDefaultTabStop(4);
	filePath = new QStringList;
	fileContext = new QStringList;
	fileReadOnly = new QList<bool>;
	fileReadOnly->setAutoDelete(true);
	fileChanged = new QList<bool>;
	fileChanged->setAutoDelete(true);
	closeAll();

	connect(fileList, SIGNAL(selectionChanged()), this, SLOT(updateEditor()));
	connect(fileList, SIGNAL(currentChanged(QListBoxItem *)), this, SLOT(listUpdate(QListBoxItem *)));
	connect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(undoAvailable(bool)));
	connect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(redoAvailable(bool)));
	connect(editor, SIGNAL(textChanged()), this, SLOT(editorUpdate()));
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

	projectInit();
}

// Widgets

void MainW::listUpdate(void)
{
	if (!fileList->count() || fileList->currentItem() == -1) {
		disconnect(editor, SIGNAL(textChanged()), \
				this, SLOT(editorUpdate()));
		setCaption(APP_TITLE);
		bar->setItemEnabled(ID_SAVE, false);
		bar->setItemEnabled(ID_TAB, false);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_CLOSE, false);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_CLOSEALL, false);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVE, false);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVEAS, false);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVEALL, false);
		bar->menuEdit->setItemEnabled(ID_MENU_EDIT_GOTO, false);
		editor->setText(QString::null);
		editor->setReadOnly(true);
		connect(editor, SIGNAL(textChanged()), \
				this, SLOT(editorUpdate()));
	} else {
		if (*fileChanged->at(fileList->currentItem()))
			setCaption(APP_TITLE + " - [" + *filePath->at(fileList->currentItem()) + " *]");
		else
			setCaption(APP_TITLE + " - [" + *filePath->at(fileList->currentItem()) + "]");
		bar->setItemEnabled(ID_SAVE, *fileChanged->at(fileList->currentItem()));
		bar->setItemEnabled(ID_TAB, !*fileReadOnly->at(fileList->currentItem()));
		bar->menuFile->setItemEnabled(ID_MENU_FILE_CLOSE, true);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_CLOSEALL, true);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVE, *fileChanged->at(fileList->currentItem()));
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVEAS, true);
		bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVEALL, true);
		bar->menuEdit->setItemEnabled(ID_MENU_EDIT_GOTO, true);
		editor->setReadOnly(*fileReadOnly->at(fileList->currentItem()));
		editor->setEdited(*fileChanged->at(fileList->currentItem()));
	}
}

void MainW::editorUpdate(void)
{
	if (fileList->currentItem() == -1)
		return;
	*fileContext->at(fileList->currentItem()) = editor->text();
	if (*fileChanged->at(fileList->currentItem()) == editor->edited())
		return;
	*fileChanged->at(fileList->currentItem()) = editor->edited();
	bar->setItemEnabled(ID_SAVE, editor->edited());
	bar->menuFile->setItemEnabled(ID_MENU_FILE_SAVE, editor->edited());
	disconnect(fileList, SIGNAL(selectionChanged()), \
			this, SLOT(updateEditor()));
	disconnect(fileList, SIGNAL(currentChanged(QListBoxItem *)), \
			this, SLOT(listUpdate(QListBoxItem *)));
	if (*fileChanged->at(fileList->currentItem())) {
		fileList->changeItem(PIX_EDITED, fileList->currentText(), fileList->currentItem());
		setCaption(APP_TITLE + " - [" + *filePath->at(fileList->currentItem()) + " *]");
	} else {
		fileList->changeItem(PIX_UNEDITED, fileList->currentText(), fileList->currentItem());
		setCaption(APP_TITLE + " - [" + *filePath->at(fileList->currentItem()) + "]");
	}
	connect(fileList, SIGNAL(selectionChanged()), \
			this, SLOT(updateEditor()));
	connect(fileList, SIGNAL(currentChanged(QListBoxItem *)), \
			this, SLOT(listUpdate(QListBoxItem *)));
}

void MainW::updateFileList(void)
{
	fileList->clear();
	for (QStringList::Iterator it = filePath->begin(); \
			it != filePath->end(); it++) {
		QFileInfo info(*it);
		fileList->insertItem(PIX_UNEDITED, info.fileName());
	}
}

void MainW::updateEditor(void)
{
	disconnect(editor, SIGNAL(textChanged()), \
			this, SLOT(editorUpdate()));
	if (!fileList->count())
		return;
	if (fileList->currentItem() == -1)
		fileList->setSelected(0, true);
	editor->setText(*fileContext->at(fileList->currentItem()));
	connect(editor, SIGNAL(textChanged()), \
			this, SLOT(editorUpdate()));
}

// File operations

QStringList MainW::openWithDir(const QString& path)
{
	QStringList paths;
	if (path.isEmpty())
		paths = QFileDialog::getOpenFileNames("C/C++ (*.c; *.cpp);;Header (*.h);;All Files (*)", INIT_DIR, this, 0, tr("打开文件"));
	else
		paths = QFileDialog::getOpenFileNames("C/C++ (*.c; *.cpp);;Header (*.h);;All Files (*)", path, this, 0, tr("打开文件"));
	open(paths);
	return paths;
}

bool MainW::open(const QString& path)
{
	if (path.isEmpty())
		return false;
	if (fileOpened(path) != -1) {
		fileList->setSelected(fileOpened(path), true);
		return true;
	}
	QFileInfo info(path);
	if (!info.isReadable()) {
		QMessageBox::critical(0, APP_TITLE, path + tr("\nFile is not readable!"));
		return false;
	}
	bool readOnly = !info.isWritable();
	if (readOnly)
		QMessageBox::warning(0, APP_TITLE, path + tr("\nFile is read-only!"));
	QFile f(path);
	f.open(IO_ReadOnly);
	QTextStream t(&f);
	filePath->append(path);
	fileContext->append(t.read());
	fileReadOnly->append(new bool(readOnly));
	fileChanged->append(new bool(false));
	f.close();
	updateFileList();
	fileList->setSelected(fileList->count() - 1, true);
	return true;
}

void MainW::open(QStringList& paths)
{
	if (paths.isEmpty())
		return;
	for (QStringList::Iterator it = paths.begin(); it != paths.end(); it++)
		open(*it);
}

bool MainW::newFile(const QString& path)
{
	if (path.isEmpty())
		return false;
	QFile f(path);
	if (f.exists()) {
		switch (QMessageBox::information(0, APP_TITLE, \
				path + tr("\nFile already exists, overwrite?"), \
				QMessageBox::Yes, QMessageBox::No)) {
		case QMessageBox::No:
			return false;
		}
	}
	if (!f.open(IO_WriteOnly)) {
		QMessageBox::critical(0, APP_TITLE, \
				path + tr("\nCreate file failed!"));
		return false;
	}
	f.close();
	forceClose(path);
	open(path);
	return true;
}

QString MainW::newFileWithDir(const QString& path)
{
	QFileDialog dialog(INIT_DIR, tr("C/C++ (*.c; *.cpp);;头文件 (*.h);;All Files (*)"), this, 0, true);
	if (!path.isEmpty())
		dialog.setDir(path);
	dialog.setCaption(tr("新建文件"));
	QString filePath;
	if (dialog.exec())
		filePath = dialog.selectedFile();
	if (filePath.isEmpty())
		return QString::null;
	if (dialog.selectedFilter() == tr("C/C++ (*.c; *.cpp)")) {
		if (filePath.right(4) != ".cpp" && filePath.right(2) != ".c")
			filePath.append(".cpp");
	} else if (dialog.selectedFilter() == tr("头文件 (*.h)")) {
		if (filePath.right(2) != ".h")
			filePath.append(".h");
	}
	if (!newFile(filePath))
		return QString::null;
	return filePath;
}

void MainW::save(int i)
{
	if (i == -1 || !(*fileChanged->at(i)))
		return;
	QFile f(*filePath->at(i));
	if (!f.open(IO_WriteOnly)) {
		QMessageBox::critical(0, APP_TITLE, \
				*filePath->at(i) + tr("\nOpen file failed!"));
		return;
	}
	QTextStream t(&f);
	t << *fileContext->at(i);
	disconnect(fileList, SIGNAL(selectionChanged()), \
			this, SLOT(updateEditor()));
	disconnect(fileList, SIGNAL(currentChanged(QListBoxItem *)), \
			this, SLOT(listUpdate(QListBoxItem *)));
	*fileChanged->at(i) = false;
	fileList->changeItem(PIX_UNEDITED, fileList->text(i), i);
	connect(fileList, SIGNAL(selectionChanged()), \
			this, SLOT(updateEditor()));
	connect(fileList, SIGNAL(currentChanged(QListBoxItem *)), \
			this, SLOT(listUpdate(QListBoxItem *)));
	listUpdate();
}

void MainW::saveAs(void)
{
	if (fileList->currentItem() == -1)
		return;
	// Need to known which filter chose
	QFileDialog dialog(*filePath->at(fileList->currentItem()), "C/C++ (*.c; *.cpp);;Header (*.h);;All Files (*)", this, 0, true);
	dialog.setCaption(tr("另存为"));
	dialog.setMode(QFileDialog::AnyFile);
	QString path;
	if (dialog.exec())
		path = dialog.selectedFile();
	if (path.isEmpty())
		return;
	if (dialog.selectedFilter() == tr("C/C++ (*.c; *.cpp)")) {
		if (path.right(4) != ".cpp" && path.right(2) != ".c")
			path.append(".cpp");
	} else if (dialog.selectedFilter() == tr("头文件 (*.h)")) {
		if (path.right(2) != ".h")
			path.append(".h");
	}
	QFile f(path);
	if (f.exists()) {
		switch (QMessageBox::information(0, APP_TITLE, \
				path + tr("\nFile already exists, overwrite?"), \
				QMessageBox::Yes, QMessageBox::No)) {
		case QMessageBox::No:
			return;
		}
	}
	if (!f.open(IO_WriteOnly)) {
		QMessageBox::critical(0, APP_TITLE, \
				path + tr("\nCreate file failed!"));
		return;
	}
	QTextStream t(&f);
	t << editor->text();
	filePath->append(path);
	fileContext->append(editor->text());
	fileReadOnly->append(new bool(false));
	fileChanged->append(new bool(false));
	f.close();
	updateFileList();
	fileList->setSelected(fileList->count() - 1, true);
}

void MainW::saveAll(void)
{
	for (unsigned int i = 0; i < fileList->count(); i++)
		save(i);
}

void MainW::close(int i)
{
	if (i == -1)
		return;
	if (*fileChanged->at(i)) {
		switch (QMessageBox::information(0, APP_TITLE, \
				*filePath->at(i) + tr("\nSave changes to file?"), \
				QMessageBox::Yes, QMessageBox::No, \
				QMessageBox::Cancel)) {
		case QMessageBox::Cancel:
			return;
		case QMessageBox::Yes:
			save();
		}
	}
	forceClose(i);
}

bool MainW::closeAllCheck(void)
{
	if (fileList->count()) {
		QString files;
		for (unsigned int i = 0; i < fileChanged->count(); i++)
			if (*fileChanged->at(i))
				files.append(*filePath->at(i) + '\n');
		if (!files.isEmpty())
			switch (QMessageBox::information(0, APP_TITLE, \
					files + tr("保存更改?"), \
					QMessageBox::Yes, QMessageBox::No, \
					QMessageBox::Cancel)) {
			case QMessageBox::Cancel:
				return false;
			case QMessageBox::Yes:
				saveAll();
			}
	}
	return true;
}

void MainW::closeAll(void)
{
	if (!closeAllCheck())
		return;
	filePath->clear();
	fileContext->clear();
	fileReadOnly->clear();
	fileChanged->clear();
	fileList->clear();
	listUpdate();		// clear() won't trigger currentChanged() event
}

void MainW::forceClose(int i)
{
	if (i == -1)
		return;
	filePath->remove(filePath->at(i));
	fileContext->remove(fileContext->at(i));
	fileReadOnly->remove(i);
	fileChanged->remove(i);
	fileList->removeItem(i);
	fileList->setSelected(i, true);
	updateEditor();
}

// Project related

void MainW::projectInit(void)
{
	project.opened = false;
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_SAVE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_CLOSE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADD, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADDNEW, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_REMOVE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PROPERT, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKSRC, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKIPK, false);
	Config cfg(CONF_FILE, Config::User);
	if (!cfg.isValid()) {
		qDebug(CONF_FILE ": Cannot read conf file!");
		return;
	}
	cfg.setGroup("Last");
	if (!cfg.readBoolEntry("opened", false))
		return;
	QString path = (cfg.readEntry("path", QString::null));
	openProject(path);
}

void MainW::openProject(void)
{
	QString path;
	if (project.opened) {
		QFileInfo info(project.path);
		path = info.dirPath();
	} else
		path = INIT_DIR;
	path = QFileDialog::getOpenFileName(path, tr("Project (*.bdp)"), this, 0, tr("打开工程"));
	if (!path.isEmpty()) {
		closeProject();
		openProject(path);
	}
}

void MainW::openProject(const QString& path)
{
	if (path.isEmpty())
		return;
	QFileInfo info(path);
	if (!info.isReadable()) {
		QMessageBox::critical(0, APP_TITLE, \
				path + tr("\n无法读取工程文件!"));
		return;
	}
	project.readOnly = !info.isWritable();
	if (project.readOnly)
		QMessageBox::warning(0, APP_TITLE, \
				path + tr("\n只读工程文件!"));
	Config cfg(path, Config::File);
	if (!cfg.isValid()) {
		QMessageBox::critical(0, APP_TITLE, \
				path + tr("\n无效的工程文件!"));
		return;
	}
	project.opened = true;
	project.changed = false;
	project.path = info.absFilePath();
	cfg.setGroup("Attr");
	project.Attr.Auth = cfg.readEntry("Auth", QString::null);
	project.Attr.BOPT = cfg.readEntry("BOPT", QString::null);
	project.Attr.COPT = cfg.readEntry("COPT", QString::null);
	project.Attr.Lang = cfg.readEntry("Lang", QString::null);
	project.Attr.autoMake = cfg.readBoolEntry("autoMake", true);
	project.Attr.project = cfg.readEntry("project", "HelloWorld");
	project.Attr.version = cfg.readEntry("version", "0.0.1");
	cfg.setGroup("Files");
	project.Files.count = cfg.readNumEntry("count", 0);
	project.Files.File.clear();
	for (int i = 0; i < project.Files.count; i++) {
		QFileInfo fileInfo(info.dirPath() + '/' + cfg.readEntry("File" + QString::number(i), QString::null));
		project.Files.File.append(fileInfo.absFilePath());
	}
	open(project.Files.File);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_CLOSE, true);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKSRC, true);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKIPK, true);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_SAVE, false);
	if (project.readOnly) {
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADD, false);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADDNEW, false);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_REMOVE, false);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_PROPERT, false);
	} else {
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADD, true);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADDNEW, true);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_REMOVE, true);
		bar->menuProject->setItemEnabled(ID_MENU_PROJ_PROPERT, true);
	}
}

bool MainW::closeProjectCheck(void)
{
	if (!project.opened)
		return true;
	if (project.changed) {
		switch (QMessageBox::information(0, APP_TITLE, \
				project.path + tr("\nSave changes to project?"), \
				QMessageBox::Yes, QMessageBox::No, \
				QMessageBox::Cancel)) {
		case QMessageBox::Cancel:
			return false;
		case QMessageBox::Yes:
			saveProject();
		}
	}
	return true;
}

void MainW::closeProject(void)
{
	if (!project.opened)
		return;
	if (!closeProjectCheck())
		return;
	for (int i = 0; i < project.Files.count; i++)
		close(fileOpened(*project.Files.File.at(i)));
	project.opened = false;
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_SAVE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_CLOSE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADD, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_ADDNEW, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_REMOVE, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PROPERT, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKSRC, false);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_PACKIPK, false);
}

void MainW::saveProject(void)
{
	if (!project.opened)
		return;
	project.changed = false;
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_SAVE, false);
	Config cfg(project.path, Config::File);
	cfg.setGroup("Attr");
	cfg.writeEntry("Auth", project.Attr.Auth);
	cfg.writeEntry("BOPT", project.Attr.BOPT);
	cfg.writeEntry("COPT", project.Attr.COPT);
	cfg.writeEntry("Lang", project.Attr.Lang);
	cfg.writeEntry("autoMake", project.Attr.autoMake);
	cfg.writeEntry("project", project.Attr.project);
	cfg.writeEntry("version", project.Attr.version);
	cfg.setGroup("Files");
	cfg.writeEntry("count", project.Files.count);
	for (int i = 0; i < project.Files.count; i++)
		cfg.writeEntry("File" + QString::number(i), relativePath(*project.Files.File.at(i)));
}

void MainW::projectAdd(void)
{
	if (!project.opened)
		return;
	QFileInfo info(project.path);
	QStringList paths = openWithDir(info.dirPath());
	projectAdd(paths);
}

void MainW::projectAdd(const QString& path)
{
	if (path.isEmpty())
		return;
	if (fileAdded(path) != -1)
		return;
	project.changed = true;
	project.Files.count++;
	project.Files.File.append(path);
	bar->menuProject->setItemEnabled(ID_MENU_PROJ_SAVE, true);
	return;
}

void MainW::projectAdd(QStringList &paths)
{
	if (paths.isEmpty())
		return;
	for (QStringList::Iterator it = paths.begin(); it != paths.end(); it++)
		projectAdd(*it);
}

void MainW::projectAddNew(void)
{
	QFileInfo info(project.path);
	QString path = newFileWithDir(info.dirPath());
	if (path.isEmpty())
		return;
	projectAdd(path);
}

// Commands

void MainW::run(void)
{
	if (!project.opened)
		return;
	QFileInfo info(project.path);
	bool ok;
	QString args = getArgs(&ok);
	//if (!ok)
	//	return;
	system(QString(CMD_RUN));
}

void MainW::compile(void)
{
	if (!project.opened)
		return;
	QFileInfo info(project.path);
	system(QString(CMD_COMPILE));
}

void MainW::clean(void)
{
	if (!project.opened)
		return;
	QFileInfo info(project.path);
	system(QString(CMD_CLEAN));
}

void MainW::oneKey(void)
{
	if (!project.opened)
		return;
	QFileInfo info(project.path);
	bool ok;
	QString args = getArgs(&ok);
	//if (!ok)
	//	return;
	system(QString(CMD_ONEKEY));
}

// Misc

QString MainW::relativePath(const QString& path)
{
	// Only relative up to 2 levels
	QFileInfo info(project.path);
	if (path.startsWith(info.dirPath() + '/'))
		return "." + path.mid(info.dirPath().length());
	info.setFile(info.dirPath());
	if (path.startsWith(info.dirPath() + '/'))
		return ".." + path.mid(info.dirPath().length());
	return path;
}

void MainW::undoAvailable(bool e)
{
	bar->setItemEnabled(ID_UNDO, e);
	bar->menuEdit->setItemEnabled(ID_MENU_EDIT_UNDO, e);
}

void MainW::redoAvailable(bool e)
{
	bar->setItemEnabled(ID_REDO, e);
	bar->menuEdit->setItemEnabled(ID_MENU_EDIT_REDO, e);
}

void MainW::win(void)
{
	showList = !showList;
	updateLayout();
}

void MainW::tab(void)
{
	if (editor->isReadOnly())
		return;
	editor->setEdited(true);
	editor->insert("\t");
	editorUpdate();		// insert() won't trigger textChanged() event
}

void MainW::quit(void)
{
	if (!closeProjectCheck())
		return;
	if (!closeAllCheck())
		return;
	qApp->quit();
}

void MainW::aboutToQuit(void)
{
	Config cfg(CONF_FILE, Config::User);
	cfg.setGroup("Last");
	cfg.writeEntry("opened", project.opened);
	if (!project.opened)
		return;
	cfg.writeEntry("path", project.path);
}

QString MainW::getArgs(bool *ok)
{
	QString args = QInputDialog::getText(APP_TITLE, tr("运行参数"), QString::null, ok);
	if (*ok)
		return args;
	else
		return QString::null;
}

#define W	size().width()
#define H	size().height()
#define LIST_W	(W / 4)
#define EDIT_W	(W - LIST_W)
#define BAR_H	24
#define BAR_Y	(H - BAR_H)
void MainW::updateLayout(void)
{
	if (showList) {
		fileList->show();
		editor->setGeometry(LIST_W, 0, EDIT_W, H - BAR_H);
	} else {
		fileList->hide();
		editor->setGeometry(0, 0, W, H - BAR_H);
	}
}

#undef W
#undef H
#define W	e->size().width()
#define H	e->size().height()
void MainW::resizeEvent(QResizeEvent *e)
{
	bar->move(0, BAR_Y);
	bar->setMaximumSize(W, BAR_H);
	fileList->setGeometry(0, 0, LIST_W, H - BAR_H);
	updateLayout();
}
