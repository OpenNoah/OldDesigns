/**************************************
 *FILE    :mainw.cpp
 *PROJECT :eps-Qt
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :23/8/2013
***************************************/

#include <qmessagebox.h>
#include <qapplication.h>
#include <qfontdialog.h>
#include <qdatetime.h>
#include <qtimer.h>
#include "mainw.h"
#include "misc.h"
#include "styles.h"

#define qDebug(...)

#ifdef ARM
#define DEFW 600
#define DEFH 400
#define PBH 48
#define SPACE 4
#define INFOH 16
#else
#define DEFW 300
#define DEFH 200
#define PBH 24
#define SPACE 2
#define INFOH 16
#endif

MainW::MainW(QWidget *parent, char *name, WFlags f): QWidget(parent, name, f)
{
    qDebug("MainW constructor load at here!");

    info = new QLabel(this);
    
    q = new MLE(this);
    q->setReadOnly(true);
    q->setWordWrap(QMultiLineEdit::WidgetWidth);
    q->setWrapPolicy(QMultiLineEdit::Anywhere);
    connect(q, SIGNAL(answerA()), this, SLOT(answerA()));
    connect(q, SIGNAL(answerB()), this, SLOT(answerB()));
    connect(q, SIGNAL(answerC()), this, SLOT(answerC()));
    connect(q, SIGNAL(answerD()), this, SLOT(answerD()));
    connect(q, SIGNAL(forward()), this, SLOT(loadnext()));
    connect(q, SIGNAL(backward()), this, SLOT(loadprev()));
    connect(q, SIGNAL(home()), this, SLOT(home()));
    connect(q, SIGNAL(end()), this, SLOT(end()));
    connect(q, SIGNAL(normal()), this, SLOT(normal()));
    connect(q, SIGNAL(wrong()), this, SLOT(wrong()));
    connect(q, SIGNAL(jumpto()), this, SLOT(jumpto()));
    connect(q, SIGNAL(quit()), qApp, SLOT(quit()));
    
    QPopupMenu *menu = new QPopupMenu(this);
    menu->insertItem(TUNIC("正常模式 (&N)"), this, SLOT(normal()));
    menu->insertItem(TUNIC("错题模式 (&W)"), this, SLOT(wrong()));

    QPopupMenu *jump = new QPopupMenu(this);
    jump->insertItem(TUNIC("第一题 (&H&o&m&e)"), this, SLOT(home()));
    jump->insertItem(TUNIC("最后一题 (&E&n&d)"), this, SLOT(end()));
	jump->insertSeparator();
    jump->insertItem(TUNIC("选择题目 (&J)"), this, SLOT(jumpto()));
    menu->insertItem(TUNIC("题目跳转"), jump);

    menu->insertItem(TUNIC("清空错题"), this, SLOT(cleanall()));
	menu->insertSeparator();

    QPopupMenu *set = new QPopupMenu(this);
    set->insertItem(TUNIC("风格设置"), setstyles());
    set->insertItem(TUNIC("字体大小"), this, SLOT(fonts()));
    menu->insertItem(TUNIC("设置"), set);

	menu->insertSeparator();
    menu->insertItem(TUNIC("关于"), this, SLOT(about()));
	menu->insertSeparator();
    menu->insertItem(TUNIC("退出 (&Q)"), qApp, SLOT(quit()));
    
    opt = new QPushButton(this);
    opt->setText(TUNIC("选项"));
    opt->setPopup(menu);
    
	for (int i = 0; i < 4; i++) {
    	sel[i] = new QPushButton(this);
    	sel[i]->setText((QString)"&" + (QChar)('A' + i));
		sel[i]->setAccel('A' + i);
    }
    connect(sel[0], SIGNAL(clicked()), this, SLOT(answerA()));
    connect(sel[1], SIGNAL(clicked()), this, SLOT(answerB()));
    connect(sel[2], SIGNAL(clicked()), this, SLOT(answerC()));
    connect(sel[3], SIGNAL(clicked()), this, SLOT(answerD()));
    
	prev = new QPushButton(this);
    prev->setText(TUNIC("&上一题"));
	connect(prev, SIGNAL(clicked()), this, SLOT(loadprev()));

	next = new QPushButton(this);
    next->setText(TUNIC("&下一题"));
	connect(next, SIGNAL(clicked()), this, SLOT(loadnext()));
    
    setMinimumSize(DEFW, DEFH);

	t = time((time_t *)NULL);

	//for (int i = 0; i < 4; i++)
		//errcount[i] = 0;
	read(FILE, FILE ".cfg");
	load();

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(infoUpdate()));
	timer->start(500);

	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(cfgsave()));
}

#define readVar(qfile, var) ((qfile)->readBlock((char *)&(var), sizeof(var)) == sizeof(var))
void MainW::read(QString path, QString cfgpath)
{
    cfg = new QFile(cfgpath);
    if (!cfg->open(IO_ReadWrite)) {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("配置文件打开失败!"));
        qFatal("Open cfg file failed!");
	}
    la = readQuestions(path, cfg, &lw);
	cfg->at(0);
    char ch;
    if (!readVar(cfg, ch))
		goto _re;
	if (ch != CHECK)
		goto _re;
	if (!readVar(cfg, counta))
		goto _re;
	if (!readVar(cfg, countw))
		goto _re;
	if (!readVar(cfg, ch))
		goto _re;
	if (ch) {
		l = lw;
		count = countw;
	} else {
		l = la;
		count = counta;
	}
	if (!readVar(cfg, ch))
		goto _re;
	font((unsigned int)ch);
	unsigned int i;
	if (!readVar(cfg, i))
		goto _re;
	setCurrentStyle(i);
	return;
_re:
	counta = countw = 0;
	l = la;
	count = counta;
	font(13);
	cfgsave();
}

void MainW::cfgsave(void)
{
	if (l == la)
		counta = count;
	else if (l == lw)
		countw = count;
	else {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("发生了一个不可预料的错误!"));
		qFatal("An unexpected error happend!");
	}
	cfg->at(0);
	char ch;
	ch = CHECK;
    cfg->writeBlock(&ch, sizeof(ch));
    cfg->writeBlock((char *)&counta, sizeof(counta));
    cfg->writeBlock((char *)&countw, sizeof(countw));
	ch = l == lw;
    cfg->writeBlock(&ch, sizeof(ch));
	ch = (char)q->font().pointSize();
    cfg->writeBlock(&ch, sizeof(ch));
	unsigned int i = getCurrentStyle();
    cfg->writeBlock((char *)&i, sizeof(i));
    cfg->flush();
}

void MainW::cleanall(void)
{
	if (QMessageBox::warning(this, CAPTION(), TUNIC("确定要清除所有记录的错题???\n(你确定?...)"), TUNIC("确定"), TUNIC("不确定"), QString::null, 0, 1) != 0)
		return;
	for (unsigned int i = 0; i < la->count(); i++)
		la->at(i)->Clean();
	countw = 0;
	if (l == lw)
		count = 0;
	load();
}

void MainW::jumpto(void)
{
    qDebug("A NumberDialog should be displayed");
    NumberDialog dialog(1, l->count(), 1, l->count() / 100, count + 1, this);
	connect(&dialog, SIGNAL(valueChanged(int)), this, SLOT(load(int)));
    dialog.exec();
}

void MainW::load(void)
{
    if (l->count() == 0) {
		if (l == la) {
			QMessageBox::critical(0, TUNIC(TITLE), TUNIC("文件中不存在问题文本!"));
	    	qFatal("No question exist in the file");
		} else if (l == lw) {
			QMessageBox::warning(this, CAPTION(), TUNIC("暂时没有记录的错题,\n自动返回正常模式...\n(你在玩我?...)"));
			normal();
			return;
		} else {
			QMessageBox::critical(0, TUNIC(TITLE), TUNIC("发生了一个不可预料的错误!"));
	    	qFatal("An unexpected error happend!");
		}
	}
    q->setText(l->at(count)->q());
	if (l->at(count)->u() != question::N)
		q->append(TUNIC((QString)"\n你的答案: " + (char)('A' + l->at(count)->u()) + "\n正确答案: " + (char)('A' + l->at(count)->a())));
	infoUpdate();
	cfgsave();
}

void MainW::infoUpdate(void)
{
	QString str;
	if (l == la)
		str = TUNIC("正常模式");
	else if (l == lw)
		str = TUNIC("错题模式");
	else
		str = TUNIC("未知模式");
	str.append(TUNIC(" | 题目: ") + QString::number(count + 1) + "/" + QString::number(l->count()));
	//str.append(TUNIC("|正确率") + QString::number(100 - ((double)errcount[0] / (double)(errcount[1] == 0 ? 1 : errcount[1])) * 100, 'f', 1) + '%');
	//str.append(TUNIC("|首正率") + QString::number(100 - ((double)errcount[2] / (double)(errcount[3] == 0 ? 1 : errcount[3])) * 100, 'f', 1) + '%');
	int sec = time((time_t *)NULL) - t;
	QTime run(sec / 60 / 60, (sec / 60) % 60, sec % 60);
	str.append(TUNIC(" | 运行: ") + run.toString());
    info->setText(str);
}

void MainW::loadprev(void)
{
    if (count == 0) {
    	qDebug("This is the first question!");
		QMessageBox::information(this, CAPTION(), TUNIC("这已经是第一题了.\n(你是想内存溢出吗?..)"));
		return;
	}
	count--;
	load();
}

void MainW::loadnext(void)
{
    if (l->count() <= count + 1) {
    	qDebug("No more question exist in the file!");
		QMessageBox::information(this, CAPTION(), TUNIC("这已经是最后一题了.\n(后面木有题目了呦 QvQ)"));
		return;
	}
    count++;
	load();
}

void MainW::load(int n)
{
	count = n - 1;
	load();
}

void MainW::normal(void)
{
	if (l == la)
		return;
	else if (l == lw)
		countw = count;
	else {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("发生了一个不可预料的错误!"));
		qFatal("An unexpected error happend!");
	}
	l = la;
	count = counta;
	load();
}

void MainW::wrong(void)
{
	if (l == lw)
		return;
	else if (l == la)
		counta = count;
	else {
		QMessageBox::critical(0, TUNIC(TITLE), TUNIC("发生了一个不可预料的错误!"));
		qFatal("An unexpected error happend!");
	}
	l = lw;
	count = countw;
	load();
}

void MainW::answerA(void) {answer(question::A);}
void MainW::answerB(void) {answer(question::B);}
void MainW::answerC(void) {answer(question::C);}
void MainW::answerD(void) {answer(question::D);}

void MainW::answer(int ans)
{
    qDebug("User answerd " + QString::number(ans));

	//errcount[1]++;										//<Total answered> count up
	bool reg = l->at(count)->u() == question::N;		//Need to register the answer?
	//if (reg)											//If need to register the answer
		//errcount[3]++;									//<Total new questions> count up

	unsigned int off = 0;									//
	if (lw->count() != 0)								//If there are <wrong> questions
		off = lw->at(countw)->o();						//Store the offset of current <wrong> question

    if (l->at(count)->Answer(ans)) {					//If the user answer is correct
        qDebug("That's correct!... QvQ...");
        loadnext();										//Load next question
    } else {											//Else, the answer is not correct
        qDebug("Not correct... 23333333333");
		//errcount[0]++;									//<Answered wrongly> count up
		if (reg) {										//If I need to register the answer
			//errcount[2]++;								//<First time wrong> count up
			if (lw->count() != 1)						//If there were <wrong> questions before operation
				if (off != lw->at(countw)->o())			//If the offset at the same position has changed
					countw++;							//It means I need to count up the current position
			load();										//Reload current question
		}
	}
}

void MainW::about(void)
{
	if (QMessageBox::information(this, CAPTION(),
		#include "about.txt"
		, TUNIC("关于Qt"), TUNIC("关闭")) == 0)
		QMessageBox::aboutQt(this, CAPTION());
}

void MainW::fonts(void)
{
    qDebug("A NumberDialog should be displayed");
    NumberDialog dialog(11, 24, 1, 5, q->font().pointSize(), this);
	connect(&dialog, SIGNAL(valueChanged(int)), this, SLOT(font(int)));
    dialog.exec();
	cfgsave();
}

void MainW::font(unsigned int size)
{
	q->setFont(QFont(0, size));
}

#define RW (e->size().width())
#define RH (e->size().height())
#define PBY (RH - PBH)
void MainW::resizeEvent(QResizeEvent *e)
{
    qDebug("MainW resize event");
    
    info->setGeometry(0, 0, RW, INFOH);
    q->setGeometry(0, INFOH + SPACE, RW, RH - PBH - SPACE - INFOH - SPACE);
    opt->setGeometry(0, PBY, RW / 5 - SPACE / 2, PBH);
	prev->setGeometry(RW / 5 + SPACE / 2, PBY, RW / 5 - SPACE, PBH);
    for (int i = 0; i < 4; i++)
    	sel[i]->setGeometry(RW * 2 / 5 + SPACE / 2 + (RW  * 2 / 5 - SPACE) / 4 * i, PBY, (RW * 2 / 5 - SPACE) / 4, PBH);
	next->setGeometry(sel[3]->x() + sel[3]->width() + SPACE, PBY, RW - sel[3]->x() - sel[3]->width() - SPACE, PBH);
}

MainW::~MainW(void)
{
    qDebug("MainW deconstructor load at here!");
    delete l;
}
