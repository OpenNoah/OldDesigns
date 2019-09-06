/**************************************
 *FILE    :question.h
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :23/8/2013
***************************************/

#ifndef _QUESTION_H
#define _QUESTION_H

#include <qtextstream.h>
#include <qstring.h>
#include <qlist.h>
#include <qfile.h>

#define TITLE "¥ÌÃ‚¡∑œ∞ <GT-Soft>"

class question
{
public:
	enum Selections {A = 0, B = 1, C = 2, D = 3, N = -1};
	question(QList<question> *lw, QTextStream *ts, QFile *f, int o);	//Constructor
	~question(void);							//Deconstructor
	QString q(void) const {return v_que;}		//Question
	int a(void) const {return v_ans;}			//Answer
	int u(void) const {return v_your;}			//Your answer
	bool v(void) const {return v_valid;}		//Data valid
	bool Answer(int ans);						//User's answer, and is it correct?
	bool c(void) const {return v_your == N;}	//Is that answer correct?
	void Clean(void);							//Clean data recorded
	unsigned int o(void) const {return v_off;}	//Offset

private:
	bool v_valid;
    unsigned int v_off;
	int v_ans;
	int v_your;
	QList<question> *v_lw;
    QFile *v_file;
	QString v_que;
};

QList<question> *readQuestions(QString path, QFile *out, QList<question> **lw);
QList<question> *readWrongQuestions(QList<question> *la);

#endif
