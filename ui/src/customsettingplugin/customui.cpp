#include "customui.h"
#include "commonutils.h"
#include <stdio.h>
#include <QRegExpValidator>
#include "iuicontainer.h"
#include "iuiplugin.h"
#include <QProcess>
#include "proxymanager.h"
#include "jtime.h"
#include "uicommon/change.h"
#include "uicommon/extendstackwidget.h"
#include "configui/configview.h"
#include "debugui/debugview.h"
//#include "illuminationcontroldlg.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
/*新UI修改*/
#include <QGridLayout>
#define  ENABLE_DV_MODE  1// 屏蔽DV 
CustomUi::CustomUi(QWidget *parent) :
    QWidget(parent),
    m_pConfigui(NULL),
    m_pDebugView(NULL),
	m_pExtendStackWidget(NULL),
    m_pECamTool(NULL)
{
	m_iMode = 0;
	m_pItem = NULL;
	m_Image = NULL;
    m_pConfigui = new ConfigView();
    m_pDebugView= new DebugView();
    //m_pIlluminationControlDlg = new IlluminationControlDlg();
	//m_pDebugView->SetLightInstance(m_pIlluminationControlDlg);
}

CustomUi::~CustomUi()
{
	if (m_pItem != NULL)
	{
		delete m_pItem;
		m_pItem = NULL;
	}
	if (m_Image != NULL)
	{
		delete m_Image;
		m_Image = NULL;
	}
	if (m_pConfigui !=NULL)
	{
		delete m_pConfigui;
		m_pConfigui = NULL;
	}
	if (m_pDebugView !=NULL)
	{
		delete m_pDebugView;
		m_pDebugView = NULL;
	}
	//if (NULL != m_pIlluminationControlDlg)
	//{
	//	delete m_pIlluminationControlDlg;
	//	m_pIlluminationControlDlg = NULL;
	//}
}
DebugView* CustomUi::getDebugView()
{
	return  m_pDebugView;
}
int g_videoindex = 0;
bool CustomUi::init()
{
	m_pExtendStackWidget = new ExtendStackWidget(this);
    /*新UI修改*/
	QGridLayout* pQGridLayout = new QGridLayout;
	pQGridLayout->addWidget(m_pExtendStackWidget);
	pQGridLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(pQGridLayout);
	
	m_pExtendStackWidget->setObjectName("obj_customsetting");
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("参数设置"),m_pConfigui,"obj_set_pamaner");

#ifndef  TEST_IN_SS
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("调机界面"),m_pDebugView);
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("工站点位"),"stationui","obj_set_station");
   // m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("轴IO调试"),"cardui","obj_set_card_debug");
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("卡轴配置"),"motionui","obj_set_cardcfg");
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("流程配置"),"mainflowui","obj_set_mainflowui");
    //QString strTitle = QString::fromLocal8Bit("温度控件");
#ifdef HISTORY_VERSION_1
    strTitle = QString::fromLocal8Bit("光源调试");
#endif
    //m_pExtendStackWidget->addWidget(strTitle,m_pIlluminationControlDlg,"obj_set_light_ctr");
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("相机设置"), "linecamerasettingui", "obj_set_cameracfg");
    
    //m_pECamTool->setObjectName("obj_camtool");
    //videoscope *pItem = new videoscope;
    //m_pECamTool->addWidget("video", pItem);
    //g_videoindex = m_pECamTool->indexOf(pItem);
   // m_pECamTool->addWidget("image", new ImageViewer());
    //m_pECamTool->setCurrentWidget(0);
    //m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("图像工具"), m_pECamTool);
#endif
	
   // m_pExtendStackWidget->setCurrentWidget(0);
	m_pECamTool = new ExtendStackWidget(this);

    return true;
}

bool CustomUi::deinit()
{
    if (NULL != m_pECamTool)
    {
		if (m_pECamTool->count() > 0)
		{
			videoscope *pItem = (videoscope *)m_pECamTool->widgetOf(g_videoindex);
			pItem->uninit();
		}

    }
    return true;
}

bool CustomUi::showWindow(QWidget *parent, QRect &rect, bool bPopFlag)
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
        if(m_pExtendStackWidget != NULL)
        {
            m_pExtendStackWidget->resize(rect.width(), rect.height());
        }
    }
    setObjectName("customsettingplugin");

    show();
    return true;
}

bool CustomUi::hideWindow()
{
    hide();
    return true;
}

void CustomUi::fitSize(const QRect &rect)
{
    resize(rect.width(), rect.height());
}

bool CustomUi::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);
    if (NOTIFY_PROGRAM_NEW == event || NOTIFY_PROGRAM_OPEN == event)//切换产品
	{
        if (NULL != m_pConfigui)
        {
            m_pConfigui->InitUI();
        }
		//if (NULL != m_pIlluminationControlDlg)
		//{
  //          m_pIlluminationControlDlg->ChangeProdutUpdateCfgPara();
		//}
	}
	//else if (NOTIFY_RESET_STROBOFLASH == event)
	//{
	//	if (NULL == m_pDebugView)
	//	{
	//		return false;
	//	}
	//	return m_pDebugView->ResetStroboflash();
	//}
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
			{

				if (2 == m_iMode)
				{
					m_pExtendStackWidget->removeWidget(0);
				}
				else if (3 <= m_iMode)
				{
					m_pECamTool->removeWidget(1);
					m_pECamTool->removeWidget(0);
					int icount = m_pExtendStackWidget->count() - 1;
					for (int i = icount; i >= 0; i--)
					{
						m_pExtendStackWidget->removeWidget(i);
					}
				}
				m_iMode = 0;
			}
			break;
			case 1://生产
			{
				IFileVariable *pFile = getCameraSetCfgFileVariable();
				if (pFile)
				{
					pFile->setValue(IMAGE_PARAM_SAVE_SIMAGE_ENABLE,0, true);//不保存原始图像
					//pFile->setValue(IMAGE_PARAM_H_SY_DETECT_ENABLE,1, true);//使能H_SY算法
					
				}
				m_pConfigui->on_radioButton_piece_by_piece_clicked();//流水线
				m_iMode = 1;
			}
			break;
			case 2://调机
			{
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("参数设置"), m_pConfigui, "obj_set_pamaner");
				m_pExtendStackWidget->setCurrentWidget(0);
				m_iMode = 2;
			}
			break;
			case 3: // 程序员
			case 4: // root
			{

				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("参数设置"), m_pConfigui, "obj_set_pamaner");

#if ENABLE_DV_MODE
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("工站点位"), "stationui", "obj_set_station");
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("轴IO调试"), "cardui", "obj_set_card_debug");
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("卡轴配置"), "motionui", "obj_set_cardcfg");
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("流程配置"), "mainflowui", "obj_set_mainflowui");
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("相机设置"), "linecamerasettingui", "obj_set_cameracfg");

				//videoscope *pItem = new videoscope;
				if (NULL == m_pItem)
				{
					m_pItem = new videoscope;
				}
				if (NULL == m_Image)
				{
					m_Image = new ImageViewer();
				}
				
				m_pECamTool->addWidget("video", m_pItem);
				g_videoindex = m_pECamTool->indexOf(m_pItem);
				
				m_pECamTool->addWidget("image", m_Image);
				m_pECamTool->setCurrentWidget(0);
				m_pExtendStackWidget->addWidget(QString::fromLocal8Bit("图像工具"), m_pECamTool);
#else
				//

#endif
				m_pExtendStackWidget->setCurrentWidget(0);
				m_iMode = 3;
			}
			break;
			default:
				break;
			}

		} while (0);

	}
	if ("next_product" == event)
	{
		m_pConfigui->on_pushButton_next_clicked();
	}
    return true;
}

void CustomUi::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath("customsettingplugin");
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);

        //retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                 __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
}



