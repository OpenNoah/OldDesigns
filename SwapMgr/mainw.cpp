/**************************************
 *FILE    :mainw.cpp
 *PROJECT :SwapMgr
 *AUTHOR  :zhiyb @ GT-Soft
 *CREATED :25/7/2013
***************************************/

#include <qpe/qpeapplication.h>
#include <qmessagebox.h>
#include "mainw.h"
#include "misc.h"

// Swap related. Unit: MiB
#define SWAPFILE "/mnt/mmc/.swap"
#define MINIMUM 10
#define MAXIMUM 2048

#define qDebug(...)

#define TEXTMIB(v) (codec->toUnicode("当前大小: " + QString::number(v) + " MiB, 最大值: " + QString::number((double)MAXIMUM / 1024, 'g', 3) + " GiB"))
#define TEXTGIB(v) (codec->toUnicode("当前大小: " + QString::number((double)v / 1024, 'g', 3) + " GiB, 最大值: " + QString::number((double)MAXIMUM / 1024, 'g', 3) + " GiB"))

#define DEFWIDTH 320
#define DEFHEIGHT (240 - 26)

bool loading = false;

MainW::MainW(QWidget *p)
{
    qDebug("MainW constructor load at here!");

    parent = p;
    
    if (!misc->setSwapFile(SWAPFILE))
    	qFatal("Cannot open " SWAPFILE " !");

    lFile = new QLabel(codec->toUnicode("虚拟内存文件: " SWAPFILE), parent);
    lFile->setAlignment(AlignCenter);
    lFile->setGeometry(0, 0, DEFWIDTH, 30);
    lSize = new QLabel(TEXTMIB(MINIMUM), parent);
    lSize->setAlignment(AlignCenter);
    lSize->setGeometry(0, 30, DEFWIDTH, 30);

    s = new QSlider(MINIMUM, MAXIMUM, 20, MINIMUM, Horizontal, parent);
    s->setGeometry(0, 60, DEFWIDTH, (DEFHEIGHT - 60) / 2);

    pbApply = new QPushButton(codec->toUnicode("应用更改"), parent);
    pbApply->setGeometry(0, 60 + (DEFHEIGHT - 60) / 2, DEFWIDTH / 4, (DEFHEIGHT - 60) / 2);

    pbReload = new QPushButton(codec->toUnicode("重新载入"), parent);
    pbReload->setGeometry(DEFWIDTH / 4, 60 + (DEFHEIGHT - 60) / 2, DEFWIDTH / 4, (DEFHEIGHT - 60) / 2);

    pbOnOff = new QPushButton(codec->toUnicode("开启/关闭"), parent);
    pbOnOff->setToggleButton(true);
    pbOnOff->setGeometry(DEFWIDTH / 2, 60 + (DEFHEIGHT - 60) / 2, DEFWIDTH / 4, (DEFHEIGHT - 60) / 2);

    pbQuit = new QPushButton(codec->toUnicode("退出程序"), parent);
    pbQuit->setGeometry(DEFWIDTH * 3 / 4, 60 + (DEFHEIGHT - 60) / 2, DEFWIDTH / 4, (DEFHEIGHT - 60) / 2);

    connect(s, SIGNAL(sliderMoved(int)), this, SLOT(change(int)));
    connect(s, SIGNAL(valueChanged(int)), this, SLOT(change(int)));
    connect(pbApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(pbReload, SIGNAL(clicked()), this, SLOT(load()));
    connect(pbOnOff, SIGNAL(toggled(bool)), this, SLOT(enable(bool)));
    connect(pbQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

    load();
}

void MainW::load(void)
{
    qDebug("MainW load slot");
    loading = true;
    s->setValue(misc->swapFileSize() / 1024 / 1024);
    pbOnOff->setOn(misc->swapFileEnable());
    loading = false;
}

void MainW::change(int v)
{
    qDebug("MainW change slot");
    qDebug("New value: " + QString::number(v));
    if (v < 1024)
        lSize->setText(TEXTMIB(v));
    else
        lSize->setText(TEXTGIB(v));
}

void MainW::apply(void)
{
    qDebug("MainW apply slot");
    if (QMessageBox::information(parent, codec->toUnicode("提示"), codec->toUnicode("在你点过 \"Ok\" 按钮之后,\n文件大小开始更改.\n这可能需要花费很长时间,\n请耐心等待."), QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
        return;
    parent->setEnabled(false);
    if (!misc->changeSwapFileSize(s->value() * 1024 * 1024))
        QMessageBox::critical(parent, codec->toUnicode("错误"), codec->toUnicode("修改文件大小时发生了一个错误,\n可能是因为空间不足."), QMessageBox::Ok, QMessageBox::NoButton);
    else
        QMessageBox::information(parent, codec->toUnicode("成功"), codec->toUnicode("虚拟内存文件大小修改成功"), QMessageBox::Ok, QMessageBox::NoButton);
    parent->setEnabled(true);
}

void MainW::enable(bool e)
{
    if (!loading)
        misc->setSwapFileEnable(e);
}
