#include "defectclassificationui.h"
#include "commonutils.h"
#include <stdio.h>
#include "change.h"
#include "ititlebar.h"
#include "iuicontainer.h"
#include "iuiplugin.h"
#include "uicommon/extendstackwidget.h"
#include "defectexpression.h"
#include "keyfeatureswidget.h"

#define PLUGIN_NAME "DefectClassificationUIPlugin"

DefectClassificationUI* DefectClassificationUI::m_pInstance = NULL;

DefectClassificationUI::DefectClassificationUI(QWidget *parent) :
    QDialog(parent)
{

}

DefectClassificationUI::~DefectClassificationUI()
{

}

DefectClassificationUI* DefectClassificationUI::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new DefectClassificationUI;
    }

    return m_pInstance;
}

void DefectClassificationUI::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

bool DefectClassificationUI::init()
{
    m_pExtendStackWidget = new ExtendStackWidget(this);
    ExtendStackWidget* m_pExtendFeatures = new ExtendStackWidget(this);
    m_pExtendStackWidget->addWidget("Formula",new DefectExpression());
    m_pExtendStackWidget->addWidget("Key Features",m_pExtendFeatures);
    m_pExtendFeatures->addWidget("width", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("size", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("length", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("DiagSize", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("Aranking", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("Aarea", new KeyFeaturesWidget());

    return true;
}

bool DefectClassificationUI::deinit()
{
    if (m_pExtendStackWidget !=NULL)
    {
        delete m_pExtendStackWidget;
        m_pExtendStackWidget = NULL;
    }
    return true;
}

bool DefectClassificationUI::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
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
    setObjectName("DefectClassificationUIPlugin");
    show();
    return true;
}

bool DefectClassificationUI::hideWindow()
{
    hide();
    return true;
}

void DefectClassificationUI::setPluginContainer(void *pPluginContainer)
{
    Q_UNUSED(pPluginContainer);
}

void DefectClassificationUI::fitSize(const QRect& rect)
{
    Q_UNUSED(rect);
}

bool DefectClassificationUI::notify(QString event, OS_WPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);

    return true;
}


void DefectClassificationUI::SetLanguage(QString strlanguage)
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

void DefectClassificationUI::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
        //ÆÁ±ÕEsc¼ü 
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}
