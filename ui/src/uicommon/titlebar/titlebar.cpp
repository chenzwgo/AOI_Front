
#include "titlebar.h"
#include <QToolButton>
#include <QLabel>
#include <QToolButton>
#include <QDesktopWidget>
#define SAFE_SHOW(a,bShow) if(a){ a->setVisible(bShow);}

//#define MINIMIZED_LENGTH (160)
//#define MINIMIZED_HEIGHT (30)
//MiniState** TitleBar::s_ppMatrixOccupy = NULL;
//int TitleBar::s_iVerticalCnt = 0;
//int TitleBar::s_iHorizontalCnt = 0;

TitleBar::TitleBar(QWidget *parent)
    : m_pMinimize(NULL)
    , m_pMaximize(NULL)
    , m_pRestore(NULL)
    , m_pClose(NULL)
    , m_pIconLabel(NULL)
    , m_pTitleLabel(NULL)
    , m_pHBoxLayout(NULL)
    , m_pParent(parent)
    , m_bMaxNormal(false)
    , m_bLeftMousePressed(false)
{
    setParent(parent);
    m_lastRestore = parentWidget()->geometry();

    m_pMinimize = new QToolButton(this);
    m_pMaximize = new QToolButton(this);
    m_pRestore  = new QToolButton(this);
    m_pClose    = new QToolButton(this);

    m_pMinimize->setFixedSize(21,21);
    m_pMaximize->setFixedSize(21,21);
    m_pRestore->setFixedSize(21,21);
    m_pClose->setFixedSize(21,21);

    m_pIconLabel = new QLabel(this);
    m_pIconLabel->setMinimumHeight(16);
    QString strpath = QApplication::applicationDirPath();
    strpath.append("/plugins/xframeplugin/startup.png");
    QPixmap iconPixmap(strpath);
    m_pIconLabel->setPixmap(iconPixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    m_pTitleLabel = new QLabel(this);
    m_pTitleLabel->setMinimumHeight(15);
    m_pTitleLabel->setMidLineWidth(30);

    m_pTitleLabel->setText("title");

    m_pHBoxLayout = new QHBoxLayout(this);
    m_pHBoxLayout->setContentsMargins(1,0,5,0);

#if defined(Q_WS_MAC)

    m_pHBoxLayout->addWidget(m_pClose);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pMinimize);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pRestore);
    m_pHBoxLayout->addWidget(m_pMaximize);
    m_pHBoxLayout->addStretch();
    m_pHBoxLayout->addWidget(m_pIconLabel);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pTitleLabel);
    m_pHBoxLayout->addStretch();
#else
    m_pHBoxLayout->addWidget(m_pIconLabel);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pTitleLabel);
    m_pHBoxLayout->addStretch();
    m_pHBoxLayout->addWidget(m_pMinimize);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pMaximize);
    m_pHBoxLayout->addWidget(m_pRestore);
    m_pHBoxLayout->setSpacing(2);
    m_pHBoxLayout->addWidget(m_pClose);
#endif

    setLayout(m_pHBoxLayout);

    m_pRestore->hide();

#if defined(Q_WS_MAC)
    m_pIconLabel->setObjectName("Mac_Core_IconLabelTitle");
    m_pTitleLabel->setObjectName("Mac_Core_LabelTitle");
    m_pMinimize->setObjectName("Mac_Core_ButtonMinimize");
    m_pMaximize->setObjectName("Mac_Core_ButtonMaximize");
    m_pRestore->setObjectName("Mac_Core_ButtonRestore");
    m_pClose->setObjectName("Mac_Core_ButtonClose");
#else
    m_pIconLabel->setObjectName("Core_IconLabelTitle");
    m_pTitleLabel->setObjectName("Core_LabelTitle");
    m_pMinimize->setObjectName("Core_ButtonMinimize");
    m_pMaximize->setObjectName("Core_ButtonMaximize");
    m_pRestore->setObjectName("Core_ButtonRestore");
    m_pClose->setObjectName("Core_ButtonClose");
#endif

    setFocusPolicy(Qt::NoFocus);
    m_pIconLabel->setFocusPolicy(Qt::NoFocus);
    m_pTitleLabel->setFocusPolicy(Qt::NoFocus);
    m_pMinimize->setFocusPolicy(Qt::NoFocus);
    m_pMaximize->setFocusPolicy(Qt::NoFocus);
    m_pRestore->setFocusPolicy(Qt::NoFocus);
    m_pClose->setFocusPolicy(Qt::NoFocus);

    m_bMaxNormal = false;

    connect(m_pClose, SIGNAL( clicked() ), parent, SLOT(close() ) );
    connect(m_pMinimize, SIGNAL( clicked() ), this, SLOT(showMin() ) );
    connect(m_pMaximize, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
    connect(m_pRestore, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
}

void TitleBar::setRect(QRect &rect)
{
    setFixedHeight(rect.height());
    setGeometry(rect);
  //  setMouseTracking(true);
}

void TitleBar::showMin()
{
    parentWidget()->showMinimized();
}

void TitleBar::showMaxRestore()
{
    if (m_bMaxNormal)
    {
        parentWidget()->setGeometry(m_lastRestore);
        parentWidget()->update();
   //     parentWidget()->showNormal();
        m_bMaxNormal = !m_bMaxNormal;
        m_pRestore->hide();
        m_pMaximize->show();
    }
    else
    {
        QDesktopWidget *pDesktop = QApplication::desktop();
        m_lastRestore = parentWidget()->geometry();
        if (NULL != pDesktop)
        {
            QRect titlerect = pDesktop->availableGeometry(parentWidget());
            #ifndef Q_WS_X11
                        parentWidget()->setGeometry(titlerect);
            #else
                        parentWidget()->setGeometry(titlerect.x(),titlerect.y(),titlerect.width()-1,titlerect.height()-1);
            #endif
                        parentWidget()->update();
        }
     //   parentWidget()->showMaximized();
        m_bMaxNormal = !m_bMaxNormal;
        m_pMaximize->hide();
        m_pRestore->show();
    }
}

bool TitleBar::isMax()
{
    return m_bMaxNormal;
}
void TitleBar::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    if (m_pIconLabel->geometry().contains(point))
    {
        return;
    }
    m_startPos = event->globalPos();
    m_clickPos = mapToParent(event->pos());
    m_bLeftMousePressed = true;
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_bLeftMousePressed = false;
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    QFrame::mouseMoveEvent(event);
    if (m_bMaxNormal)
        return;
    if (m_bLeftMousePressed)
    {
        parentWidget()->move(parentWidget()->mapToParent(mapToParent(event->pos())) - m_clickPos);
    }
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (m_pMaximize->isHidden() && m_pRestore->isHidden())
    {
        qDebug("Maximize button is hidden");
        return;
    }
    QPoint point = e->pos();
    if (m_pIconLabel->geometry().contains(point))
    {
        qDebug("DoubleClick on titlebar Icon");
        parentWidget()->close();
    //    qApp->closeAllWindows();
        return;
    }
    showMaxRestore();
}


void TitleBar::showAllControl(bool bShow)
{
    SAFE_SHOW(m_pIconLabel,bShow);
    SAFE_SHOW(m_pTitleLabel,bShow);
    if (m_bMaxNormal)
    {
        SAFE_SHOW(m_pRestore,bShow);
    }
    else
    {
        SAFE_SHOW(m_pMaximize,bShow);
    }
    SAFE_SHOW(m_pMinimize,bShow);
    SAFE_SHOW(m_pClose,bShow);
}

void TitleBar::showMaxRestoreBTN(bool bShow)
{
    if (m_bMaxNormal)
    {
        if(NULL != m_pRestore)
        {
            m_pRestore->setVisible(bShow);
        }
    }
    else
    {
        if(NULL != m_pMaximize)
        {
            m_pMaximize->setVisible(bShow);
        }
    }
}

void TitleBar::showMinimizeBTN(bool bShow)
{
    if (NULL != m_pMinimize)
    {
        m_pMinimize->setVisible(bShow);
    }
}

void TitleBar::showCloseBTN(bool bShow)
{
    if (NULL != m_pClose)
    {
        m_pClose->setVisible(bShow);
    }
}

void TitleBar::showIcon(bool bShow)
{
    if (NULL != m_pIconLabel)
    {
        m_pIconLabel->setVisible(bShow);
    }
}

void TitleBar::enableMaxRestoreBTN(bool bEnable)
{
    if(NULL != m_pRestore
       && NULL != m_pMaximize)
    {
        m_pRestore->setEnabled(bEnable);
        m_pMaximize->setEnabled(bEnable);
    }
}

void TitleBar::enableMinimizeBTN(bool bEnable)
{
    if (NULL != m_pMinimize)
    {
        m_pMinimize->setEnabled(bEnable);
    }
}

void TitleBar::enableCloseBTN(bool bEnable)
{
    if (NULL != m_pClose)
    {
        m_pClose->setEnabled(bEnable);
    }
}

void TitleBar::setTitle(const QString strTitle)
{
    if (NULL != m_pIconLabel)
    {
        m_pTitleLabel->setText(" "+strTitle);
    }
}

void TitleBar::setWindowTitle(const QString strTitle)
{
    setTitle(strTitle);
}

//void TitleBar::releaseOccupy()
//{
//    QRect rectOccupy = parentWidget()->geometry();
//
//    setOccupyState(true, rectOccupy);
//
//    return;
//}
//
//void TitleBar::setOccupyState(bool bReduce, QRect& rectState)
//{
//    int iFindCnt = 0;
//    for (int i=0; i<s_iVerticalCnt; i++)
//    {
//        for (int j=0; j<s_iHorizontalCnt; j++)
//        {
//            if (s_ppMatrixOccupy[i][j].rect == rectState)
//            {
//                if (bReduce)
//                {
//                    s_ppMatrixOccupy[i][j].iOccupy--;
//                }
//                else
//                {
//                    s_ppMatrixOccupy[i][j].iOccupy++;
//                }
//
//                iFindCnt = 4;
//            }
//            else if (s_ppMatrixOccupy[i][j].rect.intersects(rectState))
//            {
//
//                if (bReduce)
//                {
//                    s_ppMatrixOccupy[i][j].iOccupy--;
//                }
//                else
//                {
//                    s_ppMatrixOccupy[i][j].iOccupy++;
//                }
//
//                iFindCnt++;
//            }
//        }
//
//        if (iFindCnt>=4)
//        {
//            break;
//        }
//    }
//}
//
//void TitleBar::moveToMiniPos()
//{
//    m_restoreRect = parentWidget()->geometry();
//
//    int i = 0;
//    int j = 0;
//    if (NULL == s_ppMatrixOccupy)
//    {
//        QDesktopWidget* pDesktop = QApplication::desktop();
//        QRect desktoprect = pDesktop->availableGeometry(parentWidget());
//
//        s_iVerticalCnt = desktoprect.height()/MINIMIZED_HEIGHT;
//        s_iHorizontalCnt = desktoprect.width()/MINIMIZED_LENGTH;
//        s_ppMatrixOccupy = new MiniState*[(unsigned int)s_iVerticalCnt];
//        for (int k=0; k<s_iVerticalCnt; k++)
//        {
//            s_ppMatrixOccupy[k] = new MiniState[(unsigned int)s_iHorizontalCnt];
//        }
//
//        for (i=0; i<s_iVerticalCnt; i++)
//        {
//            for (j=0; j<s_iHorizontalCnt; j++)
//            {
//                s_ppMatrixOccupy[i][j].rect.setX(desktoprect.x() + MINIMIZED_LENGTH*j);
//                s_ppMatrixOccupy[i][j].rect.setY(desktoprect.bottom() - MINIMIZED_HEIGHT*(i+1));
//                s_ppMatrixOccupy[i][j].rect.setWidth(MINIMIZED_LENGTH);
//                s_ppMatrixOccupy[i][j].rect.setHeight(MINIMIZED_HEIGHT);
//                s_ppMatrixOccupy[i][j].iOccupy = 0;
//                s_ppMatrixOccupy[i][j].p = NULL;
//            }
//        }
//    }
//
//    bool bFind = false;
//    for (i=0; i<s_iVerticalCnt; i++)
//    {
//        for (j=0; j<s_iHorizontalCnt; j++)
//        {
//            if (s_ppMatrixOccupy[i][j].iOccupy == 0)
//            {
//                s_ppMatrixOccupy[i][j].iOccupy++;
//                s_ppMatrixOccupy[i][j].p = parentWidget();
//                bFind = true;
//                break;
//            }
//        }
//
//        if (bFind)
//        {
//            break;
//        }
//    }
//
//    parentWidget()->setGeometry(s_ppMatrixOccupy[i][j].rect);
//    parentWidget()->move(s_ppMatrixOccupy[i][j].rect.x(), s_ppMatrixOccupy[i][j].rect.y());
//    m_bMiniMized = true;
//}
//
//void TitleBar::setMiniState()
//{
//
//}

