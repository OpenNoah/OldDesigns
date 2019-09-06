/**************************************
 *FILE    :class.h
 *PROJECT :QtProject
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/4/2013
***************************************/

#ifndef _Z_CLASS_H
#define _Z_CLASS_H

#include <qmultilineedit.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qwidget.h>
#include <qevent.h>
#include <qlabel.h>
#include <qgrid.h>

/*class QLE: public QLineEdit
{
public:
    QLE(QWidget *parent = 0, const char *name = 0);
protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
};

class QMLE: public QMultiLineEdit
{
public:
    QMLE(QWidget *parent = 0, const char *name = 0);
protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
};

class G1: public QWidget
{
    Q_OBJECT
public:
    G1(QWidget *parent = 0, const char *name = 0);
public slots:
    void clear(void);
protected:
    void resizeEvent(QResizeEvent *e);
private slots:
    void setText(void);
private:
    class QMultiLineEdit *mle;
    class QLabel *l;
};*/

class MainW: public QWidget
{
    Q_OBJECT
public:
    MainW(QWidget *parent = 0, const char *name = 0);
public slots:
    void save(void);
    void load(void);
protected:
    void resizeEvent(QResizeEvent *e);
private:
    //G1 *g1;
    QMultiLineEdit *mle;
    QGrid *g;
};

#endif
