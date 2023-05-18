#include "uitoolbox/msgbox.h"
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include "uicommon/commonutils.h"
#include "rtdefine.h"

MsgBox *MsgBox::m_pInstance = NULL;
QString MsgBox::m_lastText = "";
MsgBox::MsgBox(QWidget *parent)
    : QDialog(parent)
	, m_textEdit(NULL)
{
	initDialog();
}


MsgBox::~MsgBox(void)
{
}

void MsgBox::information( const QString &text, bool bShowTime )
{
    if (getInstance()->isVisible() && m_lastText == text)
    {
        return;
    }
    
    m_lastText = text;

    QString tmp;
    if (bShowTime)
    {
        tmp = CommonUtils::GetCurrentTimeString();
	    tmp.append(": ");
    }
	tmp.append(text);
	getInstance()->m_textEdit->appendPlainText(tmp);
	getInstance()->show();
}

void MsgBox::warning( const QString &text, bool bShowTime )
{
    if (getInstance()->isVisible() && m_lastText == text)
    {
        return;
    }

    m_lastText = text;

    QString tmp;
    if (bShowTime)
    {
        tmp = CommonUtils::GetCurrentTimeString();
        tmp.append(": ");
    }
    tmp.append(text);

	QString fmtmsg="<font color=\"blue\">";
	fmtmsg.append(tmp);
	fmtmsg.append("</font>");
	getInstance()->m_textEdit->appendHtml(fmtmsg);
	getInstance()->show();
}

void MsgBox::critical( const QString &text, bool bShowTime )
{
    if (getInstance()->isVisible() && m_lastText == text)
    {
        return;
    }

    m_lastText = text;

    QString tmp;
    if (bShowTime)
    {
        tmp = CommonUtils::GetCurrentTimeString();
        tmp.append(": ");
    }
	tmp.append(text);

	QString fmtmsg="<font color=\"red\">";
	fmtmsg.append(tmp);
	fmtmsg.append("</font>");
	getInstance()->m_textEdit->appendHtml(fmtmsg);
	getInstance()->show();
}


MsgBox * MsgBox::getInstance(QWidget *parent)
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new MsgBox(parent);
	}
	return m_pInstance;
}

void MsgBox::initDialog()
{
	QVBoxLayout *verticalLayout;
	QPlainTextEdit *textEdit;

	this->resize(400, 300);
	this->setWindowTitle("Warn");
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);

	verticalLayout = new QVBoxLayout(this);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
	textEdit = new QPlainTextEdit(this);
	textEdit->setMaximumBlockCount(1000);
	m_textEdit = textEdit;
	textEdit->setObjectName(QString::fromUtf8("textEdit"));

	verticalLayout->addWidget(textEdit);
}

void MsgBox::showEvent( QShowEvent * event )
{

}

void MsgBox::hideEvent( QHideEvent * event )
{
    m_textEdit->clear();
}

