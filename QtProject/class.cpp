/**************************************
 *FILE    :class.cpp
 *PROJECT :QtProject
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :6/5/2013
***************************************/

#include <qapplication.h>
#include <qtextstream.h>
//#include <qtextcodec.h>
#include <qfont.h>
#include <qfile.h>
#include "misc.h"
#include "class.h"

#define DATAFILE "/opt/QtPalmtop/Settings/QtProject.txt"

#define ERSVALID (e->size().isValid())
#define ERSEMPTY (e->size().isEmpty())
#define ERSNULL (e->size().isNull())
#define ERSHEIGHT (e->size().height())
#define ERSWIDTH (e->size().width())

#define MLESETFONT() setFont(QFont("SimSun", 13, QFont::Bold))

#define PBALLOC(m, n) new QPushButton(codec->toUnicode(n), (m))
#define PBSETFONT() setFont(QFont("Noah", 14, QFont::Bold))
#define PBCONNECT(m, n) connect(pb, SIGNAL(clicked()), (m), SLOT(n))
#define PBSAVESLOT() connect(pb, SIGNAL(clicked()), this, SLOT(save()))
#define PBENSIGNAL(m, n) connect((m), SIGNAL(n), pb, SLOT(setEnabled(bool)))
#define PBDISABLED() pb->setEnabled(false)

/*QLE::QLE(QWidget *parent, const char *name): QLineEdit(parent, name) {}

void QLE::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() & LeftButton)
        misc->IMEShow();
}

QMLE::QMLE(QWidget *parent, const char *name): QMultiLineEdit(parent, name) {}

void QMLE::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() & LeftButton)
        misc->IMEShow();
}

G1::G1(QWidget *parent, const char *name): QWidget(parent, name)
{
    mle = new QMultiLineEdit(this);
    mle->setGeometry(0, 0, 90, 60);
    l = new QLabel(this);
    l->setGeometry(90, 0, 30, 60);
    connect(mle, SIGNAL(textChanged()), this, SLOT(setText()));
    setMinimumSize(120, 60);
}

void G1::resizeEvent(QResizeEvent *e)
{
    if (ERSEMPTY || ERSNULL) {
        qDebug("Resize invalid!");
        return;
    }
    mle->setGeometry(0, 0, ERSWIDTH / 3 * 2, ERSHEIGHT);
    l->setGeometry(ERSWIDTH / 3 * 2, 0, ERSWIDTH / 3, ERSHEIGHT);
}

void G1::clear(void)
{
    mle->clear();
}

void G1::setText(void)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    qDebug(codec->fromUnicode(mle->text()));
    l->setText(mle->text());
}*/

MainW::MainW(QWidget *parent, const char *name): QWidget(parent, name)
{
    //g1 = new G1(this);
    //g1->setGeometry(0, 0, g1->minimumWidth(), g1->minimumHeight());
    mle = new QMultiLineEdit(this);
    mle->MLESETFONT();
    mle->setGeometry(0, 0, 120, 60);
    g = new QGrid(4, QGrid::Vertical, this);
    g->setSpacing(1);
    //g->setGeometry(0, g1->minimumHeight(), g1->minimumWidth(), 110);
    g->setGeometry(0, 60, 120, 110);

    QPushButton *pb;
    pb = PBALLOC(g, "打开软键盘");
    pb->PBSETFONT();
    PBCONNECT(misc, IMEShow());

    pb = PBALLOC(g, "清空输入框");
    pb->PBSETFONT();
    PBCONNECT(mle, clear());

    pb = PBALLOC(g, "启用电量检测");
    pb->PBSETFONT();
    PBCONNECT(misc, enableBat());

    pb = PBALLOC(g, "禁用电量检测");
    pb->PBSETFONT();
    PBCONNECT(misc, disableBat());

    pb = PBALLOC(g, "复制");
    pb->PBSETFONT();
    PBCONNECT(mle, copy());
    PBENSIGNAL(mle, copyAvailable(bool));
    PBDISABLED();

    pb = PBALLOC(g, "粘贴");
    pb->PBSETFONT();
    PBCONNECT(mle, paste());

    pb = PBALLOC(g, "撤消");
    pb->PBSETFONT();
    PBCONNECT(mle, undo());
    PBENSIGNAL(mle, undoAvailable(bool));
    PBDISABLED();

    pb = PBALLOC(g, "重做");
    pb->PBSETFONT();
    PBCONNECT(mle, redo());
    PBENSIGNAL(mle, redoAvailable(bool));
    PBDISABLED();

    pb = PBALLOC(g, "退出到桌面");
    pb->PBSETFONT();
    PBCONNECT(misc, quitToQpe());

    pb = PBALLOC(g, "锁定屏幕");
    pb->PBSETFONT();
    PBSAVESLOT();
    PBCONNECT(misc, lockScreen());

    pb = PBALLOC(g, "关机");
    pb->PBSETFONT();
    PBSAVESLOT();
    PBCONNECT(misc, shutdown());

    pb = PBALLOC(g, "退出程序~");
    pb->PBSETFONT();
    PBCONNECT(qApp, quit());

    //setMinimumSize(g1->minimumWidth(), g1->minimumHeight() + 110);
    setMinimumSize(120, 60 + 110);
    load();
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(save()));
}

void MainW::save(void)
{
    QFile f(DATAFILE);
    if (f.open(IO_WriteOnly)) {
        QTextStream t(&f);
        t << mle->text();
    }
}

void MainW::load(void)
{
    QFile f(DATAFILE);
    if (f.open(IO_ReadOnly)) {
        //QDataStream t(&f);
        QCString s;
        char l[4097];
        //while (!t.eof()) {
        //    s += t.readLine();
        //    if (!t.eof())
        //        s += '\n';
        //}
        while (!f.atEnd()) {
            int p = f.readBlock(l, 4096);
            l[p] = '\0';
            s += l;
        }
        mle->setText(codec->toUnicode(s));
    }
}

void MainW::resizeEvent(QResizeEvent *e)
{
    if (ERSEMPTY || ERSNULL) {
        qDebug("Resize invalid!");
        return;
    }
    mle/*g1*/->setGeometry(0, 0, ERSWIDTH, ERSHEIGHT - 110);
    g->setGeometry(0, ERSHEIGHT - 110, ERSWIDTH, 110);
}
