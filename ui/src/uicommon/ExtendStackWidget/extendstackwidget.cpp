#include "extendstackwidget.h"
#include "iuicontainer.h"
#include "iuiplugin.h"
#include "tool_button.h"
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QGridLayout>
#include <QTranslator>
#include <QHBoxLayout>
#include "uicommon/commonutils.h"

ExtendStackWidget::ExtendStackWidget(QWidget *parent) :
    QFrame(parent)
{
    InitUI();    
}

ToolButton*  ExtendStackWidget::newToolBtn(QString strText)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ToolButton* toolBtnDebug2 = new ToolButton(frame_2);
    toolBtnDebug2->setObjectName(QString::fromUtf8("toolBtnDebug2"));
    sizePolicy.setHeightForWidth(toolBtnDebug2->sizePolicy().hasHeightForWidth());
    toolBtnDebug2->setSizePolicy(sizePolicy);
    toolBtnDebug2->setMinimumSize(QSize(100, 30));
    //toolBtnDebug2->setMaximumSize(QSize(65, 30));
    toolBtnDebug2->setStyleSheet(QString::fromUtf8("QToolButton {\n"
        "background:rgb(234, 234, 235);\n"
        "border-radius:6px;}\n"
        "QToolButton:hover{\n"
        "background:rgb(228, 228, 229);\n"
        "border-radius:6px;}\n"
        "QToolButton:checked{\n"
        "background-color: rgb(204, 233, 189);\n"
        "border-radius:6px;}\n"
        "QToolButton:pressed{\n"
        "background:rgb(249, 227, 212);\n"
        "border-radius:6px;\n"
        "padding-top :1px;}"));
    toolBtnDebug2->setIconSize(QSize(68, 60));
    toolBtnDebug2->setCheckable(true);
    toolBtnDebug2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBtnDebug2->setAutoRaise(true);
    toolBtnDebug2->setText(strText);
    connect(toolBtnDebug2,SIGNAL(clicked()),this,SLOT(slot_onToolBtn_clicked()));
    return toolBtnDebug2;

    //ui->horizontalLayout->addWidget(toolBtnDebug2);
}

ExtendStackWidget::~ExtendStackWidget()
{
    for(int i = 0;i <Vec.size() ;i++)
    {
        if (Vec.at(i).pwdg != NULL)
        {
            if (Vec.at(i).strPluginGuid.length() >0)
            {
                QWidget* pPluginWidget = (QWidget*)CommonUtils::GetPluginMainWidget(Vec.at(i).strPluginGuid);
                if (pPluginWidget != NULL)
                {
                    pPluginWidget->setParent(NULL);
                }
            }
            else
            {
                stackedWidget->removeWidget(Vec.at(i).pwdg);
            }
        }     
    } 
}


void ExtendStackWidget::InitUI()
{
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setObjectName(QString::fromUtf8("ExtendStackWidget_verticalLayout"));
    frame_2 = new QFrame(this);
    frame_2->setObjectName(QString::fromUtf8("ExtendStackWidget_frame_2"));
    frame_2->setFrameShape(QFrame::StyledPanel);
    frame_2->setFrameShadow(QFrame::Raised);
    horizontalLayout = new QHBoxLayout(frame_2);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->setObjectName(QString::fromUtf8("ExtendStackWidget_horizontalLayout"));
    horizontalSpacer = new QSpacerItem(327, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    verticalLayout->addWidget(frame_2);
    frame = new QFrame(this);
    frame->setObjectName(QString::fromUtf8("ExtendStackWidget_frame"));
    frame->setStyleSheet(QString::fromUtf8(""));
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Raised);
    gridLayout = new QHBoxLayout(frame);
    frame->setLayout(gridLayout);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setObjectName(QString::fromUtf8("ExtendStackWidget_gridLayout"));
    stackedWidget = new QStackedWidget(frame);
    stackedWidget->setObjectName(QString::fromUtf8("ExtendStackWidget_stackedWidget"));
    gridLayout->addWidget(stackedWidget);
    verticalLayout->addWidget(frame);
    stackedWidget->setCurrentIndex(0);
	stackedWidget->setCurrentWidget(0);
    ToolButtongroup = new ToolButtonGroup(); 
    QMetaObject::connectSlotsByName(this);
}

void ExtendStackWidget::addWidget( QString strText,QWidget* widget,QString objName )
{
    ToolButton* newBtn =  newToolBtn(strText);
	if (!objName.isEmpty()&& NULL !=newBtn)
	{
		newBtn->setObjectName(objName);
	}
    stackedWidget->addWidget(widget);
    horizontalLayout->removeItem(horizontalSpacer);
    horizontalLayout->addWidget(newBtn);
    horizontalLayout->addItem(horizontalSpacer);  
    ToolButtongroup->addToolButton(newBtn);
    Vec.push_back(WidgetPair(strText,newBtn,widget));
}

void ExtendStackWidget::addWidget(QString strText, QString strPluginGuid, QString objName )
{
    ToolButton* newBtn =  newToolBtn(strText);
	if (!objName.isEmpty() && NULL != newBtn)
	{
		newBtn->setObjectName(objName);
	}
    QWidget* tmp = new QWidget(stackedWidget);
    stackedWidget->addWidget(tmp);
    horizontalLayout->removeItem(horizontalSpacer);
    horizontalLayout->addWidget(newBtn);
    horizontalLayout->addItem(horizontalSpacer);  
    ToolButtongroup->addToolButton(newBtn);
    Vec.push_back(WidgetPair(strText,newBtn,NULL,strPluginGuid));
}

void ExtendStackWidget::insertWidget( int index, QWidget* widget )
{
    ToolButton* newBtn =  newToolBtn("");
    stackedWidget->insertWidget(index,widget);
    horizontalLayout->removeItem(horizontalSpacer);
    horizontalLayout->insertWidget(index,newBtn);
    horizontalLayout->addItem(horizontalSpacer);  
    ToolButtongroup->addToolButton(newBtn);
    Vec.insert(index,WidgetPair("",newBtn,widget));
}

void ExtendStackWidget::setWidgetText(int index,QString strText)
{
    if (index >= Vec.size())
    {
        return;
    }
    if (Vec[index].pbtn == NULL)
    {
        return;
    }
    Vec[index].pbtn->setText(strText);
    Vec[index].strText = strText;
}

int ExtendStackWidget::count()
{
    return Vec.size();
}

int ExtendStackWidget::currentIndex()
{
    return stackedWidget->currentIndex();
}

QWidget* ExtendStackWidget::currentWidget()
{
    return stackedWidget->currentWidget();
}

int ExtendStackWidget::indexOf( QWidget * widget )
{
    return stackedWidget->indexOf(widget);
}

QWidget* ExtendStackWidget::widgetOf( int index )
{
    return stackedWidget->widget(index);
}

void ExtendStackWidget::enableWidget( QWidget* widget,bool enable /*=true*/ )
{
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).pwdg == widget)
        {
            break;
        }
    }
    if (i <Vec.size())
    {
        Vec.at(i).pwdg->setEnabled(enable);
        Vec.at(i).pbtn->setEnabled(enable);
    }
    return;
}

void ExtendStackWidget::enableWidget( QString strText,bool enable /*=true*/ )
{
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).strText == strText)
        {
            break;
        }
    }
    if (i <Vec.size())
    {
        Vec.at(i).pwdg->setEnabled(enable);
        Vec.at(i).pbtn->setEnabled(enable);
    }
    return;
}

void ExtendStackWidget::enableWidget( int index,bool enable /*=true*/ )
{
    if (index <Vec.size())
    {
        Vec.at(index).pwdg->setEnabled(enable);
        Vec.at(index).pbtn->setEnabled(enable);
    }
    return;
}

void ExtendStackWidget::setTitleBtnWidth( unsigned int iWidth )
{
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).pbtn != NULL)
        {
            Vec.at(i).pbtn->setFixedWidth(iWidth);
        }
    }
}

void ExtendStackWidget::removeWidget( QWidget* widget )
{
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).pwdg == widget)
        {
            break;
        }
    }
    if (i <Vec.size())
    {
        stackedWidget->removeWidget(Vec.at(i).pwdg);
        horizontalLayout->removeWidget(Vec.at(i).pbtn);
        Vec.remove(i);
    }
    return;
}

void ExtendStackWidget::removeWidget( QString strText )
{
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).strText == strText)
        {
            break;
        }
    }
    if (i <Vec.size())
    {
        stackedWidget->removeWidget(Vec.at(i).pwdg);
        horizontalLayout->removeWidget(Vec.at(i).pbtn);
        Vec.remove(i);
    }
    return;

}

void ExtendStackWidget::removeWidget( int index )
{
    if (index <Vec.size())
    {
        stackedWidget->removeWidget(Vec.at(index).pwdg);
        horizontalLayout->removeWidget(Vec.at(index).pbtn);
        Vec.remove(index);
    }
    return;
    
}

void ExtendStackWidget::setCurrentWidget( int index )
{
    if (index <Vec.size())
    {
        stackedWidget->setCurrentIndex(index);
        Vec.at(index).pbtn->setChecked(true);
    }
    return;
}

void ExtendStackWidget::setCurrentWidget( QWidget* widget )
{
    if (widget ==NULL)
    {
        return;
    }
    int i = 0;
    for ( ;i <Vec.size() ;i++)
    {
        if (Vec.at(i).pwdg == widget )
        {
            stackedWidget->setCurrentIndex(i);
            Vec.at(i).pbtn->setChecked(true);
            return;
        }
    }
    return;
}

void ExtendStackWidget::slot_onToolBtn_clicked()
{
    QToolButton* p = qobject_cast<QToolButton*>(sender());
    if (p == NULL)
    {
        return ;
    }
    QSize test = stackedWidget->size();
    for (int i = 0 ;i <Vec.size();i++)
    {
        if (Vec.at(i).pbtn == p )
        {
            if (Vec.at(i).pwdg != NULL && Vec.at(i).strPluginGuid.length() == 0)
            {
                stackedWidget->setCurrentWidget(Vec.at(i).pwdg);
                emit currentChanged(i);
            }
            else
            {
                QWidget* pWidget = stackedWidget->widget(i);
                QWidget* pPluginWidget = (QWidget*)CommonUtils::GetPluginMainWidget(Vec.at(i).strPluginGuid);
                if (pPluginWidget == NULL)
                {
                    QMessageBox::warning(this,"Warn",QString("»ñÈ¡%1²å¼þÊ§°Ü£¡").arg(Vec.at(i).strPluginGuid));
                }
                else
                {  
                    if (pWidget != pPluginWidget )
                    {
                        stackedWidget->removeWidget(pWidget);
                        stackedWidget->insertWidget(i,pPluginWidget);
                        Vec[i].pwdg = pPluginWidget;
                        pWidget = pPluginWidget;
                    }
                    stackedWidget->setCurrentWidget(pWidget);   
                    emit currentChanged(i);
                }                
            }
            break;            
        }
    }   
}


