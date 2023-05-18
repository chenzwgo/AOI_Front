#include "defectclassificationui.h"
#include "commonutils.h"
#include <stdio.h>
#include "uicommon/extendstackwidget.h"
#include "defectexpression.h"
#include "keyfeatureswidget.h"


DefectClassificationUI::DefectClassificationUI(QWidget *parent) :
    QDialog(parent)
{
    init();
}

DefectClassificationUI::~DefectClassificationUI()
{
    deinit();
}



bool DefectClassificationUI::init()
{
    m_pExtendStackWidget = new ExtendStackWidget(this);
    m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("È±ÏÝÀàÐÍ"),new DefectExpression());

    /*
    ExtendStackWidget* m_pExtendFeatures = new ExtendStackWidget(this);
    m_pExtendStackWidget->addWidget("Key Features",m_pExtendFeatures);
    m_pExtendFeatures->addWidget("width", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("size", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("length", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("DiagSize", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("Aranking", new KeyFeaturesWidget());
    m_pExtendFeatures->addWidget("Aarea", new KeyFeaturesWidget());
    */

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
