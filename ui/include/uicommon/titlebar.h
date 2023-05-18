#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QApplication>
#include <QtGui>
#include <QLayout>
#include <QStyle>
#include <QFrame>
#include <QWidget>
#include "OS_DEFINE.h"
#include "dllexport.h"
class QToolButton;
class QLabel;


class RT_API TitleBar : public QFrame
{
    Q_OBJECT
public:
    TitleBar(QWidget *parent);
    void setRect(QRect &rect);

    void showAllControl(bool bShow=true);

    // 设置隐藏或者显示属性
    void showMaxRestoreBTN(bool bShow=true);
    void showMinimizeBTN(bool bShow=true);
    void showCloseBTN(bool bShow=true);
    void showIcon(bool bShow=true);

    // 设置灰化属性
    void enableMaxRestoreBTN(bool bEnable=true);
    void enableMinimizeBTN(bool bEnable=true);
    void enableCloseBTN(bool bEnable=true);
    void enableIcon(bool bEnable=true);

    void setTitle(const QString strTitle);
    void setWindowTitle(const QString strTitle);

    bool isMax();
//    void releaseOccupy();

private slots:
    void showMin();
    void showMaxRestore();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);

//private:
//    static MiniState** s_ppMatrixOccupy;
//    static int s_iVerticalCnt;
//    static int s_iHorizontalCnt;
//    void moveToMiniPos();
//    void setMiniState();
//    void setOccupyState(bool bReduce, QRect& rectState);

private:
    QToolButton *m_pMinimize;
    QToolButton *m_pMaximize;
    QToolButton *m_pRestore;
    QToolButton *m_pClose;
    QLabel *m_pIconLabel;
    QLabel *m_pTitleLabel;

    QHBoxLayout *m_pHBoxLayout;

    QPoint m_startPos;
    QPoint m_clickPos;

    QWidget *m_pParent;
    bool m_bMaxNormal;
    bool m_bLeftMousePressed;
    QRect m_lastRestore;
};



#endif // TITLEBAR_H
