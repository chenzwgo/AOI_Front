#include "RecipesUI.h"
#include "commonutils.h"
#include <stdio.h>
#include "change.h"
#include "ititlebar.h"
#include "iuicontainer.h"
#include "iuiplugin.h"
#include "uicommon/extendstackwidget.h"
#include "productparamui.h"
#include "recipesparamui.h"
#include "g12common.h"
#include "sendmessage.h"
/*新ui修改*/
#include <QGridLayout>

#define PLUGIN_NAME "RecipesUIPlugin"

RecipesUI* RecipesUI::m_pInstance = NULL;

RecipesUI::RecipesUI(QWidget *parent) :
    QDialog(parent),
    m_pProductParamUI(NULL),
    m_pRecipesParamUI(NULL)
{

}

RecipesUI::~RecipesUI()
{

}

RecipesUI* RecipesUI::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new RecipesUI;
    }

    return m_pInstance;
}

void RecipesUI::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

bool RecipesUI::init()
{
    if(m_pProductParamUI == NULL)
    {
        m_pProductParamUI = new ProductParamUI();
    }

    if(m_pRecipesParamUI == NULL)
    {
        m_pRecipesParamUI = new RecipesParamUI();
	}
	/*新ui修改*/
	//m_pExtendStackWidget = new ExtendStackWidget(this);
	m_pExtendStackWidget = new ExtendStackWidget();
	QGridLayout* pQGridLayout = new QGridLayout;
	pQGridLayout->setContentsMargins(0, 0, 0, 0);
	pQGridLayout->addWidget(m_pExtendStackWidget);
	setLayout(pQGridLayout);

    //m_pExtendStackWidget->addWidget("Recipes",m_pRecipesParamUI);
	//QString strDes = QString::fromLocal8Bit("产品参数(") + QString::fromStdString(getCurrentProgramName()) + ")";
   // m_pExtendStackWidget->addWidget(strDes,m_pProductParamUI);
    m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("产品参数"),m_pProductParamUI);
    return true;
}

bool RecipesUI::deinit()
{
    if (m_pExtendStackWidget !=NULL)
    {
        delete m_pExtendStackWidget;
        m_pExtendStackWidget = NULL;
    }
    return true;
}

bool RecipesUI::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QDialog::setWindowFlags(Qt::Dialog);
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
        if(m_pExtendStackWidget != NULL)
        {
            m_pExtendStackWidget->resize(rect.width(), rect.height());
        }
    }
    setObjectName("RecipesUIPlugin");
    show();
    return true;
}

bool RecipesUI::hideWindow()
{
    hide();
    return true;
}

void RecipesUI::setPluginContainer(void *pPluginContainer)
{
    Q_UNUSED(pPluginContainer);
}

void RecipesUI::fitSize(const QRect& rect)
{
    Q_UNUSED(rect);
}

bool RecipesUI::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);
	if ("logininfo" == event && 0 != wparam)
	{
		QString strLoginInfo = *(QString*)wparam;
		QStringList infoList = strLoginInfo.split(':');
		do
		{
			if (infoList.size() < 2)
			{
				return true;
			}
			int iLoginModel = infoList[0].toInt();
			switch (iLoginModel)
			{
			case 0://未登录、退出
			case 1://生产
			{
				SENDMSG_INSTANCE->SendLoginModelChange(1);
			}
			break;
			case 2://调机
			{
				SENDMSG_INSTANCE->SendLoginModelChange(iLoginModel);
			}
			break;
			case 3: // 程序员
			{
				SENDMSG_INSTANCE->SendLoginModelChange(iLoginModel);
			}
			break;
			case 4: // root
			{

				SENDMSG_INSTANCE->SendLoginModelChange(iLoginModel);
			}
			break;
			default:
				break;
			}

		} while (0);

	}
    return true;
}


void RecipesUI::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath(PLUGIN_NAME);
    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                  __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}

void RecipesUI::showEvent(QShowEvent *pEvent)
{
	QString strDes = QString::fromLocal8Bit("产品参数(") + QString::fromStdString(getCurrentProgramName()) + ")";
	m_pExtendStackWidget->setWidgetText(0, strDes);
	m_pProductParamUI->updateProductItem(QString::fromStdString(getCurrentProgramName()));
}

void RecipesUI::keyPressEvent( QKeyEvent *e )
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
