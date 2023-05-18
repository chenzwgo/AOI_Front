#include "usermanagementuidlg.h"
#include "ui_usermanagementuidlg.h"
#include "uicommon/change.h"
#include "usermanagercfg.h"
#include "uicommon/commonutils.h"
#include "inotifyservice.h"
#include "competencesetdlg.h"
#include "usersetdlg.h"
#include "uibusinesscomm.h"

#define PLUGIN_PATH "plugins/usermanagementui"
#define PLUGIN_NAME "usermanagementui"

static const char *pluginText[] = {
    QT_TRANSLATE_NOOP( "UsermanagementuiDlg", "PluginName" ),
};

CUsermanagementuiDlg::CUsermanagementuiDlg(QDialog *parent) :
    QDialog(parent),
    m_pUserManagerCfg(NULL)
{
    this->setupUi(this);

    btnProMode->setChecked(true);
    QSize qSize = this->size();
    widget_left->setFixedWidth(qSize.width() / 7 * 6);
    verticalSpacer_down->changeSize(5, qSize.height() / 11 * 6, QSizePolicy::Fixed);
    btnCompetence->setVisible(false);
    btnUser->setVisible(false);

}

CUsermanagementuiDlg::~CUsermanagementuiDlg()
{

}

bool CUsermanagementuiDlg::init()
{
	CommonUtils::getIUIPlugin("toolbarplugin");
	m_pUserManagerCfg = UserManagerCfg::GetInstance();
	SendLoginInfo(MODE_LOGOUT, "");
	ManageUIView(MODE_LOGOUT, "");
    return true;
}

bool CUsermanagementuiDlg::deinit()
{
    return true;
}

bool CUsermanagementuiDlg::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
        if(parent != NULL)
        {
            QPoint point (parent->geometry().center() - QPoint (this->width() / 2,
                                                                this->height() / 2));
            move(point);
        }
    }
    else
    {
        resize(rect.width(), rect.height());
        move(rect.x(), rect.y());
    }
    show();
    return true;
}

bool CUsermanagementuiDlg::hideWindow()
{
    hide();
    return true;
}


void CUsermanagementuiDlg::fitSize(const QRect& rect)
{
    resize(rect.width(), rect.height());
}

bool CUsermanagementuiDlg::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);


    return true;
}

void CUsermanagementuiDlg::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath(PLUGIN_NAME);
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);

        retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                 __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void CUsermanagementuiDlg::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
    //屏闭Esc键
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}

void CUsermanagementuiDlg::on_btnLogin_clicked( bool bChecked )
{
    tEditInformationTedit->clear();
    btnCompetence->setVisible(false);
    btnUser->setVisible(false);

	QString strCurModel = LOGOUT;
	int iCurModel = MODE_LOGOUT;
	QString strName = "";
	QString strCurModelDesc = "";
	QString strPassWord = "";

    if(!bChecked)
    {
        btnLogin->setText(QString::fromLocal8Bit("登录"));
        editName->setText("");
        editPassword->setText("");

        editName->setEnabled(true);
        editPassword->setEnabled(true);
        btnProMode->setEnabled(true);
        btnDebugMode->setEnabled(true);
        btnEngineerMode->setEnabled(true);

		strCurModel = LOGOUT;
		iCurModel = MODE_LOGOUT;
    }
	else
	{
		strCurModel = ROOT_MODELS;
		iCurModel = MODE_ROOT;
		strCurModelDesc = QString::fromLocal8Bit("开发模式");
		strName = editName->text();
		strPassWord = editPassword->text();
		if (!CheckUser(strCurModel, strCurModelDesc, strName, strPassWord))
		{
			if (btnProMode->isChecked())
			{
				strCurModel = PRODUCTION_MODELS;
				iCurModel = MODE_PRODUCTION;
				strCurModelDesc = btnProMode->text();
			}
			else if (btnDebugMode->isChecked())
			{
				strCurModel = DEBUG_MODELS;
				iCurModel = MODE_DEBUG;
				strCurModelDesc = btnDebugMode->text();
			}
			else
			{
				strCurModel = ENGINEER_MODELS;
				iCurModel = MODE_ENGINEER;
				strCurModelDesc = btnEngineerMode->text();
			}

			if (!CheckUser(strCurModel, strCurModelDesc, strName, strPassWord))
			{
				btnLogin->setChecked(false);
				return;
			}
		}

		btnLogin->setText(QString::fromLocal8Bit("退出"));
		editName->setEnabled(false);
		editPassword->setEnabled(false);
		btnProMode->setEnabled(false);
		btnDebugMode->setEnabled(false);
		btnEngineerMode->setEnabled(false);
	}

    SendLoginInfo(iCurModel, strName);
	ManageUIView(iCurModel, strName);
    if (ROOT_MODELS == strCurModel || ENGINEER_MODELS == strCurModel)
    {
        btnCompetence->setVisible(true);
        btnUser->setVisible(true);
    }
    
}

bool CUsermanagementuiDlg::CheckUser( QString strLoginModel, QString strModelDesc, QString strName, QString strPwd )
{
    if (NULL == m_pUserManagerCfg)
    {
        tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("获取管理实例失败！"), UILOG_ERR);
        return false;
    }
    QMap<QString, QMap<QString, QString> >* mapUserInfo = m_pUserManagerCfg->GetUserInfo();
    if (NULL == mapUserInfo)
    {
        return NULL;
    }
    
    //查找对应的登录模式
	bool bFind = true;
    QMap<QString, QMap<QString, QString> >::Iterator tempIter = mapUserInfo->find(strLoginModel);
	if (tempIter == mapUserInfo->end())
	{
        tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("登录模式不存在！"), UILOG_ERR);
		return false;
	}

    //查找对应的用户名
    QMap<QString, QString>::Iterator tempIterUser = tempIter.value().find(strName);
	if (tempIterUser == tempIter.value().end())
	{
		bFind = false;
	}

	if (!bFind)
	{
		if (strLoginModel != ROOT_MODELS)
		{
            tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("用户名：") + strName + QString::fromLocal8Bit("不存在！"), UILOG_ERR);
		}
		return false;
	}
	
	tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("登录模式：") + strModelDesc, UILOG_INFO);
    tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("用户名：") + strName, UILOG_INFO);

    //查找对应的密码
    if (strPwd != tempIterUser.value())
    {
		if (strLoginModel != ROOT_MODELS)
		{
            tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("密码：错误！"), UILOG_ERR);
		}
        return false;
    }
    tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("密码：****"), UILOG_INFO);
    return true;
}

bool CUsermanagementuiDlg::SendUISetting( QString strPluginName, int iModel, QString strName, QMap<QString, STWidgetInfo>* mapInfo )
{
    INotifyService* pNotifyService = CommonUtils::getNotifyService();
    if (NULL == pNotifyService)
    {
        tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("消息服务插件加载失败！"), UILOG_ERR);
        return false;
    }

	QString wParam = QString("%1:%2").arg(iModel).arg(strName);
    return pNotifyService->send(strPluginName, "ShowUIView", (OS_LPARAM)&wParam, (OS_LPARAM)mapInfo);
}

void CUsermanagementuiDlg::on_btnCompetence_clicked()
{
    CompetenceSetDlg::GetInstance(this)->exec();
}

void CUsermanagementuiDlg::on_btnUser_clicked()
{
    UserSetDlg::GetInstance(this)->exec();
}

void CUsermanagementuiDlg::ManageUIView( int iLoginModel, QString strName )
{
	QWidget* pMainUIWidget = CommonUtils::getMainWindow();
	if (pMainUIWidget == NULL || m_pUserManagerCfg == NULL)
	{
		return;
	}

	QMap<QString, STWidgetInfo>* pInfo = m_pUserManagerCfg->GetWidgetID();
	if (pInfo == NULL)
	{
		return;
	}

	QMap<QString, STWidgetInfo>::iterator itInfo;
	for (itInfo = pInfo->begin(); itInfo != pInfo->end(); itInfo++)
	{
		QWidget* pChildWidget = FindChildWidget(pMainUIWidget, itInfo->stWidgetItem.strObjectName);
		if (pChildWidget != NULL)
		{
			ShowUIView(iLoginModel, pChildWidget, &itInfo.value());
		}
		else
		{
			SendUISetting(itInfo->stWidgetItem.strPluginName, iLoginModel, strName, pInfo);
			continue;
		}
	}
}

void CUsermanagementuiDlg::ShowUIView( int iLoginModel, QObject* pFatherWidget, STWidgetInfo* pInfo )
{
	if (pFatherWidget == NULL || pInfo == NULL)
	{
		return;
	}
	//pFatherWidget->setDisabled(pInfo->stWidgetItem.bDisable[iLoginModel]);
    QAction* pApc = dynamic_cast<QAction*>(pFatherWidget);
    QWidget* pWid = dynamic_cast<QWidget*>(pFatherWidget);
    if (NULL != pApc)
    {
        pApc->setDisabled(pInfo->stWidgetItem.bDisable[iLoginModel]);
    }
    else if (NULL != pWid)
    {
        pWid->setDisabled(pInfo->stWidgetItem.bDisable[iLoginModel]);
    }

	if (pInfo->mapItem.size() > 0)
	{
		QMap<QString, STWidgetInfo*>::Iterator itWidget = pInfo->mapItem.begin();

		for (; itWidget != pInfo->mapItem.end(); itWidget++)
		{
            //QWidget* pChildCtrl = pFatherWidget->findChild<QWidget*>(itWidget.value()->stWidgetItem.strObjectName);
            QObject *pChildCtrl= pFatherWidget->findChild<QObject*>(itWidget.value()->stWidgetItem.strObjectName);
			ShowUIView(iLoginModel, pChildCtrl, itWidget.value());
		}
	}
}

QWidget* CUsermanagementuiDlg::FindChildWidget( QObject* pObj, QString strChildName )
{
	if (pObj == NULL)
	{
		return NULL;
	}

	QWidget* pChildWidget = pObj->findChild<QWidget*>(strChildName);
	if (pChildWidget != NULL)
	{
		return pChildWidget;
	}
	else
	{
		QObjectList childlist = pObj->children();
		for (int i=0; i< childlist.size(); i++)
		{
			QString strName = childlist.at(i)->objectName();
			pChildWidget = FindChildWidget(childlist.at(i), strChildName);
			if (pChildWidget != NULL)
			{
				return pChildWidget;
			}
			else
			{
				continue;
			}
		}
	}
	return NULL;
}

bool CUsermanagementuiDlg::SendLoginInfo( int iModel, QString strName )
{
	INotifyService* pNotifyService = CommonUtils::getNotifyService();
	if (NULL == pNotifyService)
	{
        tEditInformationTedit->Addlogmsg(QString::fromLocal8Bit("消息服务插件加载失败！"), UILOG_ERR);
		return false;
	}
	CommonUtils::getIUIPlugin("customsettingplugin"); //
	QString wParam = QString("%1:%2").arg(iModel).arg(strName);
	CommonUtils::GetPluginMainWidget("mainui");
	CommonUtils::GetPluginMainWidget("customsettingplugin");
	//CommonUtils::GetPluginMainWidget("recipesuiplugin");
	pNotifyService->send("mainui", "logininfo", (OS_LPARAM)&wParam, NULL);
	pNotifyService->send("customsettingplugin", "logininfo", (OS_LPARAM)&wParam, NULL);
	pNotifyService->send("recipesuiplugin", "logininfo", (OS_LPARAM)&wParam, NULL);
	//
	QSettings setting(QString("config/loginmode.ini"), QSettings::IniFormat);
	setting.beginGroup("mode");
	setting.setValue("login", iModel);
	setting.endGroup();
	//
	 return true;
}

