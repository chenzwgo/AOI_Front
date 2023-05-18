#ifndef _TEXTPUSHBUTTON_H
#define _TEXTPUSHBUTTON_H

#include <QPushButton>
#include <QPainter>

struct StAxisIcon
{
    StAxisIcon() {};
    StAxisIcon(const QString& tmpIconType, const QString& tmpIconTip, bool tmpDirRev)
    {
        iconType = tmpIconType;
        iconTip = tmpIconTip;
        bDirRev = tmpDirRev;
    }
    QString iconType;  // 图标类型(左右型,前后型,上下型,旋转型等)
    QString iconTip;   // 图标提示(+X,-Z等)
    bool bDirRev;       // 方向取反
};

class TextPushButton : public QPushButton
{
public:
    enum EM_TipPos {enLeftTop, enRightTop, enLeftBottom, enRightBottom};
    TextPushButton(QWidget *parent = 0): QPushButton(parent) {};
    ~TextPushButton(void) {};

    void setIconInfo(const StAxisIcon &axisIconInfo)
    {
        QString iconPath = ":/uicustomwidget/image/icon_";
        QString symbol = axisIconInfo.bDirRev ? "+" : "-";
        m_iconPath = iconPath+axisIconInfo.iconType+".bmp";
        setIcon(QIcon(m_iconPath));
        setIconSize(QSize(56, 46));

        m_iconTip = symbol+axisIconInfo.iconTip;
        if (0 == axisIconInfo.iconType.compare("left"))
        {
            m_tipPos = enLeftTop;
        }
        else if (0 == axisIconInfo.iconType.compare("right"))
        {
            m_tipPos = enRightTop;
        }
        else if (0 == axisIconInfo.iconType.compare("up"))
        {
            m_tipPos = enRightTop;
        }
        else if (0 == axisIconInfo.iconType.compare("down"))
        {
            m_tipPos = enRightBottom;
        }
        else if (0 == axisIconInfo.iconType.compare("front"))
        {
            m_tipPos = enRightTop;
        }
        else if (0 == axisIconInfo.iconType.compare("back"))
        {
            m_tipPos = enRightBottom;
        }
        else if (0 == axisIconInfo.iconType.compare("clockwise"))
        {
            m_tipPos = enRightBottom;
        }
        else if (0 == axisIconInfo.iconType.compare("anticlockwise"))
        {
            m_tipPos = enLeftBottom;
        }
        else
        {
            m_tipPos = enLeftTop;
        }
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        QPushButton::paintEvent(event);
        QPainter painter(this) ;

        if (this->isEnabled())
        {
            painter.setPen(QColor("black"));
        }
        else
        {
            painter.setPen(QColor("Gray"));
        }

        QRect textRect = rect();
        switch (m_tipPos)
        {
        case enLeftTop:
            {
                textRect.setTopLeft(QPoint(rect().left()+6, rect().top()+4));
                painter.drawText(textRect,Qt::AlignLeft | Qt::AlignTop,m_iconTip);
            }
            break;
        case enRightTop:
            {
                textRect.setTopRight(QPoint(rect().right()-6, rect().top()+4));
                painter.drawText(textRect,Qt::AlignRight | Qt::AlignTop,m_iconTip);
            }
            break;
        case enLeftBottom:
            {
                textRect.setBottomLeft(QPoint(rect().left()+6, rect().bottom()-4));
                painter.drawText(textRect,Qt::AlignLeft | Qt::AlignBottom,m_iconTip);
            }
            break;
        case enRightBottom:
            {
                textRect.setBottomRight(QPoint(rect().right()-6, rect().bottom()-4));
                painter.drawText(textRect,Qt::AlignRight | Qt::AlignBottom,m_iconTip);
            }
            break;
        default:
            break;
        }
    }

private:
    EM_TipPos m_tipPos;
    QString m_iconPath;
    QString m_iconTip;
};

#endif
