/**************************************
 *FILE    :mainw.h
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :23/8/2013
***************************************/

#ifndef _MAINW_H
#define _MAINW_H

#include <qobject.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qfile.h>
#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <time.h>
#include "question.h"
#include "numberdialog.h"
#include "mle.h"

#define TITLE "¥ÌÃ‚¡∑œ∞ <GT-Soft>"
#define FILE "/Settings/eps"
#define CHECK 0x05

class MainW: public QWidget
{
    Q_OBJECT

public:
    MainW(QWidget *parent = 0, char *name = 0, WFlags f = 0);
    ~MainW(void);

private slots:
	void infoUpdate(void);
    void loadprev(void);
    void loadnext(void);
	void cleanall(void);
	void jumpto(void);
	void load(int n);
	void normal(void);
	void wrong(void);
    void answerA(void);
    void answerB(void);
    void answerC(void);
    void answerD(void);
	void about(void);
	void home(void) {load(1);}
	void end(void) {load(l->count());}
	void fonts(void);
	void font(int size) {font((unsigned int)size);}
	void font(unsigned int size);
	void cfgsave(void);

private:
	void read(QString path, QString cfgpath);
    void answer(int ans);
	void load(void);
    void resizeEvent(QResizeEvent *e);

    unsigned int count, counta, countw;
	time_t t;
    QList<question> *l, *la, *lw;
	//unsigned int errcount[4];			//Answered wrongly, total answered, first time wrong, total new questions
    QLabel *info;
    MLE *q;
    QPushButton *opt;
    QPushButton *sel[4];
	QPushButton *prev;
	QPushButton *next;
	QFile *cfg;
};

#endif
