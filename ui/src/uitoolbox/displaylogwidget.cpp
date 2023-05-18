#include "uitoolbox/displaylogwidget.h"
#include "uidefine.h"
#include "uicommon/commonutils.h"
#include "msgbox.h"

DisplayLogWidget::DisplayLogWidget(QWidget* parent):QPlainTextEdit(parent)
{
    SetMaxDisplaycount();
    connect(this,SIGNAL(addmsgsig(QString,int,bool,bool)),this,SLOT(slotAddmsg(QString,int,bool,bool)));
}

DisplayLogWidget::~DisplayLogWidget()
{

}

void DisplayLogWidget::ClearLog()
{
    clear();
}

void DisplayLogWidget::SetMaxDisplaycount(int maxcount)
{
    setMaximumBlockCount(maxcount);
}

void DisplayLogWidget::Addlogmsg(QString msg, UILogLevel error, bool bsavefile, bool bshowTime)
{
    emit addmsgsig(msg,error,bsavefile,bshowTime);
}

void DisplayLogWidget::slotAddmsg(QString msg, int errorlevel, bool bsavefile, bool bshowTime)
{
    QString tmp;
    if (bshowTime)
    {
        tmp = CommonUtils::GetCurrentTimeString();
        tmp.append(": ");
    }

    tmp.append(msg);

	QString fmtmsg;
	switch (errorlevel)
	{
	case UILOG_INFO:
		{
			fmtmsg="<font color=\"black\">";
			fmtmsg.append(tmp);
			fmtmsg.append("</font>");
		}
		break;
    case  UILOG_WARN:
		{
			fmtmsg="<font color=\"blue\">";
			fmtmsg.append(tmp);
			fmtmsg.append("</font>");
		}
		break;
    case  UILOG_ERR:
		{
			fmtmsg="<font color=\"red\">";
			fmtmsg.append(tmp);
			fmtmsg.append("</font>");
		}
		break;
    case UILOG_POP:
        {
            fmtmsg="<font color=\"black\">";
            fmtmsg.append(tmp);
            fmtmsg.append("</font>");
        }
        break;
    case UILOG_POPRED:
        {
            fmtmsg="<font color=\"red\">";
            fmtmsg.append(tmp);
            fmtmsg.append("</font>");
        }
        break;
	default:
        {
            fmtmsg="<font color=\"black\">";
            fmtmsg.append(tmp);
            fmtmsg.append("</font>");
        }
        break;
	}
	this->appendHtml(fmtmsg);
    if (errorlevel == UILOG_POP)
    {
         MsgBox::information(msg, bshowTime);
    }
    else if (errorlevel == UILOG_POPRED)
    {
         MsgBox::critical(msg, bshowTime);
    }

    if(bsavefile)
    {
        qDebug() <<tmp;
    }
    return;
}

