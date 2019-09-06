/**************************************
 *FILE    :question.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :23/8/2013
***************************************/

#include <qlist.h>
#include <qfile.h>
#include <qstring.h>
#include <qiodevice.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include "question.h"
#include "misc.h"

#define qDebug(...)

question::question(QList<question> *lw, QTextStream *ts, QFile *f, int o)
{
    qDebug("question constructor load at here!");
    v_your = N;
    while (!ts->atEnd()) {
        QString str(TUNIC(ts->readLine()));
        if (str.stripWhiteSpace().isEmpty())
            continue;
        if (str.startsWith("^")) {
            v_valid = 1;
            if (!f->at(o)) {
				QMessageBox::critical(0, TUNIC(TITLE), TUNIC("更改配置文件指针位置失败!"));
                qFatal("Change file offset failed!");
			}
            char your;
            if (f->readBlock(&your, 1) == 1) {
                qDebug("Got <%d> from cfg file", (int)your);
                if ((int)your >= N && (int)your <= D)
                    v_your = (int)your;
            } else {
                char t = N;
                f->writeBlock(&t, 1);
            }
            f->flush();
            v_file = f;
            v_off = o;
            QChar ans = str.at(1);
            v_ans = ans.latin1() - 'A';
			v_lw = lw;
            return;
        }
        if (!v_que.isEmpty())
            v_que += '\n';
        v_que += str;
    }
    v_valid = 0;
}

bool question::Answer(int ans)
{
	if (v_your != N || v_ans == ans)
		return v_ans == ans;
    v_your = ans;
    if (!v_file->at(v_off)) {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("更改配置文件指针位置失败!"));
        qFatal("Change file offset failed!");
	}
    char your = ans;
    v_file->writeBlock(&your, 1);
    v_file->flush();
	unsigned int i;
	for (i = 0; i < v_lw->count(); i++)
		if (v_lw->at(i)->o() > v_off)
			break;
	v_lw->insert(i, this);
    return false;
}

void question::Clean(void)
{
	if (v_your == N)
		return;
    v_your = N;
    if (!v_file->at(v_off)) {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("更改配置文件指针位置失败!"));
        qFatal("Change file offset failed!");
	}
    char your = N;
    v_file->writeBlock(&your, 1);
    v_file->flush();
	for (unsigned int i = 0; i < v_lw->count(); i++)
		if (v_lw->at(i)->o() == v_off) {
			v_lw->take();
			break;
		}
}

question::~question(void)
{
    qDebug("question deconstructor load at here!");
}

QList<question> *readQuestions(QString path, QFile *out, QList<question> **lw)
{
    QList<question> *la = new QList<question>;
	*lw = new QList<question>;
    la->setAutoDelete(true);
    QFile f(path);
    if (!f.open(IO_ReadOnly)) {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("文件打开失败!"));
        qFatal("Open file failed!");
	}
    QTextStream ts(&f);
    ts.setEncoding(QTextStream::Latin1);
    int i = 64;							//First 64 bytes will be used to store app settings
    while (1) {
        question *q = new question(*lw, &ts, out, i++);
        if (!q->v()) {
            delete q;
            break;
        }
        la->append(q);
		if (!q->c())
			(*lw)->append(q);
    }
    f.close();
    return la;
}

QList<question> *readWrongQuestions(QList<question> *la)
{
    QList<question> *lw = new QList<question>;
    lw->setAutoDelete(true);
	for (unsigned int i = 0; i < la->count(); i++)
		if (!la->at(i)->c())
			lw->append(la->at(i));
	return lw;
}
