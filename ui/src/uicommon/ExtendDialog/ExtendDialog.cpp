
#include <QRect>
#include <QMessageBox>
#include "ExtendDialog.h"
#include "commonutils.h"
#define DEFAULT_BORDER_WIDTH (3)
#define DEFAULT_TITLE_HEIGHT (29)

ExtendDialog::ExtendDialog(bool bCusTitle, QWidget* qWidgetParent, Qt::WindowFlags flags, WidgetType type)
    :  m_pFrame(NULL)
    , m_pCusTitle(NULL)
    , m_pMainWidget(NULL)
    , m_type(type)
    , m_bCusTitle(bCusTitle)
    , m_iTitleHeight(DEFAULT_TITLE_HEIGHT)
    , m_iBorderWidth(DEFAULT_BORDER_WIDTH)
    , m_bCloseButtonEnable(true)
{
    setParent(qWidgetParent);
    if (!m_bCusTitle)
    {
        QDialog::setWindowFlags(Qt::Dialog | (flags & ~(Qt::FramelessWindowHint | Qt::CustomizeWindowHint)));
        return;
    }

    m_pFrame = new QFrame(this);

    QDialog::setWindowFlags(/*flags | */Qt::Dialog | Qt::FramelessWindowHint | Qt::CustomizeWindowHint);

    setTtileBar(flags);

    if (NULL != m_pFrame)
    {
        m_pFrame->setObjectName("Core_ExtendDialogFrame");
    }
}

ExtendDialog::~ExtendDialog()
{
    if (isVisible() || isActiveWindow())
    {
        //close();
    }
}

QWidget* ExtendDialog::GetMainWidget()
{
    return m_pMainWidget;
}

void ExtendDialog::AddMainWidget(QWidget* pWidget)
{
    m_pMainWidget = pWidget;

    if (NULL == m_pMainWidget)
    {
        return;
    }

    m_pMainWidget->setParent(this);
    m_pMainWidget->setWindowFlags(Qt::FramelessWindowHint);

    if (m_bCusTitle)
    {
        QRect widgetRect(m_iBorderWidth, m_iTitleHeight ,
                    rect().width()-2*m_iBorderWidth, rect().height() - m_iBorderWidth - m_iTitleHeight);
        m_pMainWidget->move(widgetRect.x(), widgetRect.y());
    }
    else
    {
        QRect widgetRectWithSysTitle(0, 0 ,
                    rect().width()-6, rect().height() - 32);
        m_pMainWidget->move(widgetRectWithSysTitle.x(), widgetRectWithSysTitle.y());
    }
}

void ExtendDialog::getFrameValue()
{
   // IMainFrameUIService* pMainFrame = NULL;
//    pMainFrame = (IMainFrameUIService*)(Startup::GetMainFrameService());

    //if (NULL == pMainFrame)
    {
        return;
    }

//    m_bCusTitle = pMainFrame->IsCusTitle();
//    if (m_bCusTitle)
//    {
//        m_iTitleHeight = pMainFrame->GetTitleHeight();
//        m_iBorderWidth = pMainFrame->GetBorderWidth();
//    }
}

bool ExtendDialog::IsCusTitle()
{
    return m_bCusTitle;
}

void ExtendDialog::setLayout(QLayout* playout)
{
    if (NULL == playout)
    {
        return;
    }

    QRect layoutRect(m_iBorderWidth, m_iTitleHeight ,
                rect().width()-2*m_iBorderWidth, rect().height() - m_iBorderWidth - m_iTitleHeight);
    playout->setGeometry(layoutRect);

    QDialog::setLayout(playout);
}


void ExtendDialog::setWindowFlags(Qt::WindowFlags flags)
{
    if (m_bCusTitle && (flags & Qt::WindowStaysOnTopHint))
    {
        qWarning ("%s, %s : Qt::WindowStaysOnTopHint set", __FILE__, __FUNCTION__);
        QDialog::setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }

    flags = windowFlags() | (flags & ~(Qt::CustomizeWindowHint|Qt::FramelessWindowHint));
    if (!m_bCusTitle)
    {
        QDialog::setWindowFlags(flags);
        return;
    }

    setTtileBar(flags);
}

void ExtendDialog::setWindowTitle(const QString& strTitle)
{
    if (!m_bCusTitle)
    {
        QDialog::setWindowTitle(strTitle);
        return;
    }

    if (NULL == m_pCusTitle)
    {
        return;
    }

    m_pCusTitle->setWindowTitle(strTitle);
}

void ExtendDialog::EnableCloseButton(bool bEnable)
{
    if (NULL == m_pCusTitle)
    {
        return;
    }

    m_bCloseButtonEnable = bEnable;
    m_pCusTitle->enableCloseBTN(bEnable);

    if (!bEnable)
    {
        qDebug("%s, %s : grabKeyboard() call", __FILE__, __FUNCTION__);
        grabKeyboard();
    }
    else
    {
        qDebug("%s, %s : releaseKeyboard() called", __FILE__, __FUNCTION__);
        releaseKeyboard();
    }
}

void ExtendDialog::setTtileBar(Qt::WindowFlags flags)
{
    if (NULL == m_pCusTitle)
    {
        m_pCusTitle = new TitleBar(this);
    }

    if (NULL == m_pCusTitle)
    {
        return;
    }

    if (flags & Qt::CustomizeWindowHint)
    {
        m_pCusTitle->showAllControl(false);
    }
    else
    {
        m_pCusTitle->showAllControl();
    }

    if (!(flags & Qt::WindowMinimizeButtonHint))
    {
        m_pCusTitle->showMinimizeBTN(false);
    }
    else
    {
        m_pCusTitle->showMinimizeBTN();
    }

    if (!(flags & Qt::WindowMaximizeButtonHint))
    {
        m_pCusTitle->showMaxRestoreBTN(false);
    }
    else
    {
        m_pCusTitle->showMaxRestoreBTN();
    }

    if (!(flags & Qt::WindowCloseButtonHint))
    {
        m_pCusTitle->showCloseBTN(false);
    }
    else
    {
        m_pCusTitle->showCloseBTN();
    }

    m_pCusTitle->showIcon(false);

    QRect titleRect(3,0,rect().width(), 29); // Fix the problem that in Arabic language, the close button in titlebar is too close to the left border of dialog
    m_pCusTitle->setRect(titleRect);
}

void ExtendDialog::changeSize(QSize widgetSize)
{
    if (!m_bCusTitle)
    {
        widgetSize.setWidth(widgetSize.width());
        widgetSize.setHeight(widgetSize.height());
    }
    else
    {
        widgetSize.setWidth(widgetSize.width()+2*m_iBorderWidth);
        widgetSize.setHeight(widgetSize.height()+m_iBorderWidth+m_iTitleHeight);
    }
    
    resize(widgetSize);
}

void ExtendDialog::resizeEvent(QResizeEvent* resizeevent)
{
    if (NULL == resizeevent)
    {
        return;
    }

    QSize& titleSize = const_cast<QSize&>(resizeevent->size());

    if (!m_bCusTitle && NULL != m_pMainWidget)
    {
       // titleSize.setWidth(titleSize.width()-8);
        //titleSize.setHeight(titleSize.height()-33);
        m_pMainWidget->resize(titleSize.width(), titleSize.height());
        return;
    }

    if (NULL != m_pFrame)
    {
        m_pFrame->resize(titleSize);
    }

    if (NULL != m_pCusTitle)
    {
        m_pCusTitle->resize(titleSize.width()-3, m_iTitleHeight);// Fix the problem that in Arabic language, the close button in titlebar is too close to the left border of dialog
    }

    if (m_type == NormalDialog && NULL != m_pMainWidget)
    {
        m_pMainWidget->resize(titleSize.width() -2*m_iBorderWidth, titleSize.height() - m_iBorderWidth -m_iTitleHeight);
    }
    CommonUtils::moveCentre(this);
}

void ExtendDialog::closeEvent( QCloseEvent * pCloseEvent )
{
    qDebug("%s, %s : entered", __FILE__, __FUNCTION__ );

    if (m_bCloseButtonEnable)
    {
        if(m_pCusTitle)
        {
          ////  m_pCusTitle->releaseOccupy();
        }
        qDebug("%s, %s : releaseKeyboard() call", __FILE__, __FUNCTION__ );
        releaseKeyboard(); // releasekeyboard no matter closebutton enable or not

        //  add Close button handle sends signal BEGIN
        qDebug("%s, %s : emit dlgCloseEvent ()", __FILE__, __FUNCTION__ );
        emit dlgCloseEvent ();
        // add Close button handle sends signal END
        return;
    }

    if (Qt::AltModifier == QApplication::keyboardModifiers ())
    {
        qDebug("%s, %s : close event ignored", __FILE__, __FUNCTION__ );
        pCloseEvent->ignore();
        return;
    }
    qDebug("%s, %s : releaseKeyboard() call", __FILE__, __FUNCTION__ );
    releaseKeyboard();

    // add Close button handle sends signal BEGIN
    qDebug("%s, %s : emit dlgCloseEvent ()", __FILE__, __FUNCTION__ );
    emit dlgCloseEvent ();
    // add Close button handle sends signal END
}

void ExtendDialog::keyPressEvent ( QKeyEvent * keyEvent )
{
    qDebug("%s, %s : entered", __FILE__, __FUNCTION__);

    if (NULL == m_pCusTitle || m_bCloseButtonEnable)
    {
        // standard event processing
        return QDialog::keyPressEvent(keyEvent);
    }

    qDebug("key press %d", keyEvent->key());
    if (Qt::Key_Escape ==  keyEvent->key())
    {
        qDebug("%s, %s : Escape key pressed", __FILE__, __FUNCTION__);
        return;
    }

    QDialog::keyPressEvent(keyEvent);
}

void ExtendDialog::showEvent(QShowEvent * showevent)
{
    qDebug("%s, %s : entered", __FILE__, __FUNCTION__);

    if ((m_type == MessageBoxDialog) && (NULL != showevent))
    {
        CommonUtils::moveCentre(this);
    }
    if ((NULL != m_pMainWidget) && (NULL != m_pMainWidget->parentWidget()))
    {
        if (MessageBoxDialog == m_type && !m_bCusTitle)
        {
            m_pMainWidget->parentWidget()->setFixedSize(m_pMainWidget->width(), m_pMainWidget->height());
        }
        QSize maxSize = m_pMainWidget->parentWidget()->maximumSize();
        QSize minSize = m_pMainWidget->parentWidget()->minimumSize();
        if (minSize != maxSize && !m_bCusTitle)
        {
            m_pMainWidget->parentWidget()->setMinimumSize(100, 100);
        }
    }

    if (!m_bCloseButtonEnable && QWidget::keyboardGrabber() != this)
    {
        qDebug("%s, %s : grabKeyBoard() called", __FILE__, __FUNCTION__);
        grabKeyboard();
    }

    QDialog::showEvent(showevent);
    return;
}

void ExtendDialog::hideEvent(QHideEvent * hidevent)
{
    Q_UNUSED(hidevent);
    qDebug("%s, %s : entered", __FILE__, __FUNCTION__);
    //if (!m_bCloseButtonEnable) //  releasekeyboard no matter closebutton
    {
        qDebug("%s, %s : releaseKeyboard() called", __FILE__, __FUNCTION__);
        releaseKeyboard();
    }

}

void ExtendDialog::getCornerRegion(QRegion& leftCornerRegion, QRegion& rightCornerRegion)
{
    const int iRectangleSize = 10;
    const int iEllipseSize = 18;

    QRegion uselessleftRegion(0, 0, iRectangleSize, iRectangleSize, QRegion::Rectangle);
    QRegion roundleftRegion(0, 0, iEllipseSize, iEllipseSize, QRegion::Ellipse);

    leftCornerRegion = uselessleftRegion.intersected(roundleftRegion);
    leftCornerRegion = uselessleftRegion.xored(leftCornerRegion);

    QRegion uselessrightRegion(rect().width()-iRectangleSize, 0, iRectangleSize, iRectangleSize, QRegion::Rectangle);
    QRegion roundrightRegion(rect().width()-iEllipseSize, 0, iEllipseSize, iEllipseSize, QRegion::Ellipse);

    rightCornerRegion = uselessrightRegion.intersected(roundrightRegion);
    rightCornerRegion = uselessrightRegion.xored(rightCornerRegion);
}

void ExtendDialog::paintEvent( QPaintEvent * pEvent )
{
    QDialog::paintEvent(pEvent);

    if (m_bCusTitle)
    {
        QRegion leftCornerRegion;
        QRegion rightCornerRegion;

        getCornerRegion(leftCornerRegion, rightCornerRegion);

        QRegion maskedRegion(0, 0, rect().width(), rect().height(), QRegion::Rectangle);
        maskedRegion = maskedRegion.subtracted(leftCornerRegion);
        maskedRegion = maskedRegion.subtracted(rightCornerRegion);
        setMask(maskedRegion);
    }
}


