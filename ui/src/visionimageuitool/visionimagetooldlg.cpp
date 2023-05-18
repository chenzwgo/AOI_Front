#include "uitoolbox/visionimagetooldlg.h"
#include "uicommon/commonutils.h"
#include "imagewindow.h"

#define PLUGIN_PATH "plugins/visionimagetoolplugin"

static const char *pluginText[] = {
    QT_TRANSLATE_NOOP( "VisionimagetoolDlg", "PluginName" ),
};


CVisionimagetoolDlg::CVisionimagetoolDlg(QWidget *parent) :
    QWidget(parent) ,
    m_pImageWindow(NULL)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    m_pImageWindow = new ImageWindow();
    mainLayout->addWidget(m_pImageWindow);
    mainLayout->setMargin(0);
    setLayout(mainLayout);
    connect(m_pImageWindow, SIGNAL(sigItemMoveFinished()), this, SLOT(slotItemMoveFinished()));
    connect(m_pImageWindow, SIGNAL(sigItemRBtDoubleCliecked(QPointF &)), this, SLOT(slotItemRBtDoubleCliecked(QPointF&)));
    connect(m_pImageWindow, SIGNAL(sigItemSelectChanged(int)), this, SLOT(slotItemSelectChanged(int)));
    //m_pImageWindow->show();
}

CVisionimagetoolDlg::~CVisionimagetoolDlg()
{

}

bool CVisionimagetoolDlg::init()
{
    return true;
}

bool CVisionimagetoolDlg::deinit()
{
    return true;
}

bool CVisionimagetoolDlg::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog);
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

bool CVisionimagetoolDlg::hideWindow()
{
    hide();
    return true;
}


void CVisionimagetoolDlg::fitSize(const QRect& rect)
{
    resize(rect.width(), rect.height());
}

bool CVisionimagetoolDlg::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);


    return true;
}



void CVisionimagetoolDlg::InitUI()
{
}

bool CVisionimagetoolDlg::ReadConfig()
{
    return true;
}

void CVisionimagetoolDlg::SetLanguage(QString strlanguage)
{
    QString strModule = CommonUtils::getPluginLibraryPath();
    QString strLangFilePath  = strModule + '/' + PLUGIN_PATH+ '/'+strlanguage+LANG_POSTFIX;
    qDebug("%s:%d(%s) LangFile is \"%s\" ",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));


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

void CVisionimagetoolDlg::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
    //屏闭Esc键
    case Qt::Key_Escape:
        break;
    default:
        QWidget::keyPressEvent(e);
    }
}

void CVisionimagetoolDlg::showPictureInLabel( QString file_path )
{
    m_pImageWindow->showPictureInLabel(file_path);
}

void CVisionimagetoolDlg::showPictureInLabel( unsigned char *pixelsR, unsigned char *pixelsG, unsigned char *pixelsB,long width, long height )
{
    m_pImageWindow->showPictureInLabel(pixelsR, pixelsG, pixelsB, width, height);
}

void CVisionimagetoolDlg::adjustPictureSize()
{
    m_pImageWindow->adjustPictureSize();
}

void CVisionimagetoolDlg::clearPicture()
{
    m_pImageWindow;
}

void CVisionimagetoolDlg::showButton(int iHidden )
{
    m_pImageWindow->hiddenButton(iHidden);
}


void CVisionimagetoolDlg::showToolBar(int iHidden)
{
    m_pImageWindow->showToolBar(iHidden);
}

void CVisionimagetoolDlg::showStatusBar(int iHidden)
{
    m_pImageWindow->showStatusBar(iHidden);
}

void CVisionimagetoolDlg::showActionItem(int iHidden)
{
    m_pImageWindow->showActionItem(iHidden);
}

bool CVisionimagetoolDlg::addShapeItem(const QVariant &param, double dWidth, bool bIsMoveEnable, int id)
{
    return m_pImageWindow->addShapeItem(param, dWidth, bIsMoveEnable, id);
}

QList<QVariant> CVisionimagetoolDlg::queryShapeItem()
{
    return m_pImageWindow->queryShapeItem();
}

bool CVisionimagetoolDlg::removeShapeItem( int index, bool clear /*= false*/ )
{
    return m_pImageWindow->removeShapeItem(index, clear);
}

void CVisionimagetoolDlg::setRoiMaxCount( int iCount /*= -1*/ )
{
    m_pImageWindow->setRoiMaxCount(iCount);
}

void CVisionimagetoolDlg::slotItemMoveFinished()
{
    emit sigItemMoveFinished();
}

void CVisionimagetoolDlg::slotItemRBtDoubleCliecked( QPointF & point)
{
    emit sigItemRBtDoubleCliecked(point);
}

void CVisionimagetoolDlg::slotItemSelectChanged(int id)
{
    emit sigItemSelectChanged(id);
}

void CVisionimagetoolDlg::addManualToolBar(QWidget *pWidget)
{
    if (NULL != m_pImageWindow)
    {
        m_pImageWindow->setManualWidget(pWidget);
    }
}

void CVisionimagetoolDlg::setParamValue( QString strKey, double dValue, int iValue )
{
    if (0 == strKey.compare("showcross"))
    {
        if (NULL != m_pImageWindow)
        {
            //double dRatius = dValue;
            m_pImageWindow->showCross(true, dValue);
        }
    }
}

void CVisionimagetoolDlg::setItemSelect(int id)
{
    m_pImageWindow->setItemSelect(id);
}

unsigned char * CVisionimagetoolDlg::getImageLabelPtr()
{
    return getImageLabelPtr();
}

void CVisionimagetoolDlg::UpdateImage()
{
    UpdateImage();
}

void CVisionimagetoolDlg::setSaveImageFileName(QString strName)
{
    m_pImageWindow->setSaveImageFileName(strName);
}

ShowPictureWidget::ShowPictureWidget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QGridLayout();
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

ShowPictureWidget::~ShowPictureWidget()
{

}

void ShowPictureWidget::setShowPicture(int row, int column)
{
    m_row = row;
    m_column = column;

    if(m_allWidget.count() > 0)
    {
        for(int i = 0; i < m_allWidget.count(); i++)
        {
            mainLayout->removeWidget(m_allWidget[i]);
            delete m_allWidget[i];
            m_allWidget[i] = NULL;
        }
    }

    m_allWidget.clear();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            CVisionimagetoolDlg *addWidget = new CVisionimagetoolDlg;

            addWidget->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);
            addWidget->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
            mainLayout->addWidget(addWidget,i,j);
            m_allWidget.append(addWidget);

        }
    }
}

CVisionimagetoolDlg *ShowPictureWidget::getSpecifyWidget(int row,int column)
{
    if(row > m_row-1 || column > m_column-1)
    {
        return NULL;
    }
    int index = row * m_column + column;
    return m_allWidget[index];
}
void ShowPictureWidget::hiddenSpecifyWidget(int row,int column,bool isHidden)
{
    if(row > m_row-1 || column > m_column-1)
    {
        QMessageBox::information(this,"ERROR","get widget fail!");
        return ;
    }

    int index = row * (mainLayout->columnCount()) + column + 1;
    m_allWidget[index]->setHidden(isHidden);
    return;
}

void ShowPictureWidget::resizeWindowsSize()
{
    for (int i = 0; i < m_allWidget.size(); ++i) 
    {
        if(m_allWidget.at(i) != NULL)
        {
            m_allWidget.at(i)->adjustPictureSize();			
        }
    }
}
