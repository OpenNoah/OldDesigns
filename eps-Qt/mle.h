/**************************************
 *FILE    :mle.h
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :28/8/2013
***************************************/

#ifndef _MLE_H
#define _MLE_H

#include <qmultilineedit.h>

class MLE: public QMultiLineEdit
{
	Q_OBJECT

public:
	MLE(QWidget *parent = 0);
	~MLE(void);

signals:
    void answerA(void);
    void answerB(void);
    void answerC(void);
    void answerD(void);
	void forward(void);
	void backward(void);
	void home(void);
	void end(void);
	void normal(void);
	void wrong(void);
	void jumpto(void);
	void quit(void);

protected:
	void keyPressEvent(QKeyEvent *e);
};

#endif
