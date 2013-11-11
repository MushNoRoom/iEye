#include "iEyeToolBar.h"
#include "iEyePosMonitor.h"
#include <QWidget>
#include <QAction>
namespace iEye
{
iEyeToolBar::iEyeToolBar(QWidget* parent)
    :QToolBar(parent),
      kb_en_act(new QAction("Disable Virtual Keyboard", this))
{

    addAction(kb_en_act);
    QObject::connect(kb_en_act, SIGNAL(triggered()), SLOT(onKeyEnPreesed()));
    QToolBar::addAction("Move Up", this, SLOT(respondToUP()));
    QToolBar::addAction("Move Left", this, SLOT(respondToML()));
    QToolBar::addAction("Center", this, SLOT(respondToCenter()));
    QToolBar::addAction("Move Down", this, SLOT(respondToDOWN()));
    QToolBar::addAction("Move Right", this, SLOT(respondToMR()));
}

iEyeToolBar::~iEyeToolBar()
{
    delete kb_en_act;
}

void iEyeToolBar::onKeyEnPreesed()
{
    if (iEyePosMonitor::kb_en == true)
    {
        iEyePosMonitor::kb_en = false; //
        kb_en_act->setText("Enable Virtual Keyboard");
    }
    else
    {
        iEyePosMonitor::kb_en = true; //
        kb_en_act->setText("Disable Virtual Keyboard");
    }
}

void iEyeToolBar::respondToML()
{
    emit need2MoveScene(iEye::MoveLeft);
}

void iEyeToolBar::respondToMR()
{
    emit need2MoveScene(iEye::MoveRight);
}

void iEyeToolBar::respondToUP()
{
    emit need2MoveScene(iEye::MoveUp);
}

void iEyeToolBar::respondToDOWN()
{
    emit need2MoveScene(iEye::MoveDown);
}

void iEyeToolBar::respondToCenter()
{
    emit need2MoveScene(iEye::MoveCenter);
}
}
