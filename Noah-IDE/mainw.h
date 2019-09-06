#ifndef _MAINW_H
#define _MAINW_H

#include <qwidget.h>
#include <qmultilineedit.h>
#include <qlistbox.h>
#include <qstringlist.h>
#include <qstring.h>
#include <qlist.h>
#include <qfiledialog.h>
#include "bar.h"
#include "resource.h"

class MainW: public QWidget
{
	Q_OBJECT

	friend class Bar;

public:
	MainW(QWidget *parent = 0);

protected slots:
	void updateFileList(void);
	void updateEditor(void);
	void updateLayout(void);
	void editorUpdate(void);
	void listUpdate(void);
	void listUpdate(QListBoxItem *item) {listUpdate();}
	void undo(void) {editor->undo();}
	void redo(void) {editor->redo();}
	QStringList openWithDir(const QString& path);
	void open(void) {openWithDir(QString::null);}
	bool open(const QString& path);
	void open(QStringList& list);
	QString newFileWithDir(const QString& path);
	void newFile(void) {newFileWithDir(QString::null);}
	bool newFile(const QString& path);
	void save(void) {save(fileList->currentItem());}
	void save(int i);
	void saveAs(void);
	void saveAll(void);
	void close(void) {close(fileList->currentItem());}
	void close(int i);
	void close(const QString& path) {close(fileOpened(path));}
	void closeAll(void);
	void forceClose(void) {forceClose(fileList->currentItem());}
	void forceClose(int i);
	void forceClose(const QString& path) {forceClose(fileOpened(path));}
	bool closeAllCheck(void);
	void openProject(void);
	void openProject(const QString& path);
	bool closeProjectCheck(void);
	void closeProject(void);
	void saveProject(void);
	void projectAdd(void);
	void projectAdd(const QString& path);
	void projectAdd(QStringList& paths);
	void projectAddNew(void);
	void undoAvailable(bool e);
	void redoAvailable(bool e);
	void win(void);
	void tab(void);
	void quit(void);
	void aboutToQuit(void);

	void run(void);
	void compile(void);
	void clean(void);
	void oneKey(void);

protected:
	virtual void resizeEvent(QResizeEvent *e);

	struct {
		bool opened, readOnly, changed;
		QString path;
		struct {
			QString Auth, BOPT, COPT, Lang, project, version;
			bool autoMake;
		} Attr;
		struct {
			int count;
			QStringList File;
		} Files;
	} project;
	bool showList;
	Bar *bar;
	QMultiLineEdit *editor;
	QListBox *fileList;
	QStringList *filePath;
	QStringList *fileContext;
	QList<bool> *fileReadOnly, *fileChanged;

private:
	void projectInit(void);
	int fileOpened(const QString& path) {return filePath->findIndex(path);}
	int fileAdded(const QString& path) {return project.Files.File.findIndex(path);}
	QString relativePath(const QString& path);
	QString getArgs(bool *ok);
};

#endif
