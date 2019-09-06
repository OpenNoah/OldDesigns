/**************************************
 *FILE    :mle.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :28/8/2013
***************************************/

#include "mle.h"

#define qDebug(...)

MLE::MLE(QWidget *parent): QMultiLineEdit(parent)
{
    qDebug("MLE constructor load at here!");
}

MLE::~MLE(void)
{
    qDebug("MLE deconstructor load at here!");
}

void MLE::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case 'A':
	case 'a':
		emit answerA();
		break;
	case 'B':
	case 'b':
		emit answerB();
		break;
	case 'C':
	case 'c':
		emit answerC();
		break;
	case 'D':
	case 'd':
		emit answerD();
		break;
	case 'N':
	case 'n':
		emit normal();
		break;
	case 'W':
	case 'w':
		emit wrong();
		break;
	case 'J':
	case 'j':
		emit jumpto();
		break;
	case 'Q':
	case 'q':
		emit quit();
		break;
	case Key_Left:
		emit backward();
		break;
	case Key_Right:
		emit forward();
		break;
	case Key_Up:
		emit backward();
		break;
	case Key_Down:
		emit forward();
		break;
	case Key_Home:
		emit home();
		break;
	case Key_End:
		emit end();
		break;
	}
}
