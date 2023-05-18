#include "iolabel.h"

IOLabel::IOLabel(QWidget *parent):QLabel(parent)
{
    m_bState = -1;
    connect(this,SIGNAL(signalSetState(bool)),this,SLOT(slotSetState(bool)));
	slotSetState(false);
}

IOLabel::~IOLabel()
{

}

void IOLabel::setState(bool bOn)
{
    if (m_bState != bOn)
    {
        m_bState = bOn;
        emit signalSetState(bOn);
    }
}

void IOLabel::slotSetState(bool bOn)
{
    if (bOn)
    {
        setPixmap(QPixmap(":/uicustomwidget/image/fourthOn.png"));
    }
    else
    {
        setPixmap(QPixmap(":/uicustomwidget/image/fourthOff.png"));
    }
}

