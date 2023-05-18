#include "flickerlabel.h"

flickerLabel::flickerLabel(QWidget* parent):QLabel(parent)
{
    m_pTimer = new QTimer(this);
    m_istep = 0;
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
	connect(this, SIGNAL(sigShowLableMsg(QString)), this, SLOT(slotShowLableMsg(QString)));
	connect(this, SIGNAL(sigClearLableMsg()), this, SLOT(slotClearLableMsg()));
}

flickerLabel::~flickerLabel()
{
    if(m_pTimer != NULL)
    {
        m_pTimer->stop();;
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

void flickerLabel::slotShowLableMsg(QString strmsg)
{
    if(m_pTimer->isActive())
    {
        m_pTimer->stop();
    }
    m_msg = strmsg;
    m_istep = 0;
    m_pTimer->start(500);

}

void flickerLabel::slotClearLableMsg()
{
    if(m_pTimer->isActive())
    {
        m_pTimer->stop();
    }
    m_msg ="";
    m_istep = 0;
    this->setText("");
    this->setStyleSheet( "background:none");
}

void flickerLabel::slotTimeout()
{
    if(m_istep %2==0)
    {
        this->setText(m_msg);
		QString styleSheet = "background:rgb(255, 128, 128)";
        this->setStyleSheet(styleSheet);
    }
    else
    {
        this->setText(m_msg);
        this->setStyleSheet("background:none");
    }

//    if(m_istep >= 6)
//    {
//        m_pTimer->stop();
//    }
    m_istep++;

}
