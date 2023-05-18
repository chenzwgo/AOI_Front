#ifndef EXTENDSTACKWIDGET_H
#define EXTENDSTACKWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QTranslator>
#include <QVector>

#include "dllexport.h"
#include "uidefine.h"
#include "OS_DEFINE.h"

class ToolButton;
class QVBoxLayout;
class QSpacerItem;
class QStackedWidget;
class QGridLayout;
class QTranslator;
class QHBoxLayout;
class ToolButtonGroup;


#ifdef UI_UICOMMON
#define UICOMMON_API Q_DECL_EXPORT
#else
#define UICOMMON_API Q_DECL_IMPORT
#endif


class UICOMMON_API ExtendStackWidget : public QFrame
{
    Q_OBJECT
public:
    struct WidgetPair
    {
        QString strText;
        ToolButton* pbtn;
        QWidget* pwdg;
        QString strPluginGuid;
        WidgetPair()
        {
            pbtn = NULL;
            pwdg = NULL;
        }
        WidgetPair(QString Text,ToolButton* btn,QWidget* wdg,QString guid="")
        {
            strText = Text;
            pbtn = btn;
            pwdg = wdg;
            strPluginGuid = guid;
        }
    };
    explicit ExtendStackWidget(QWidget *parent = 0);
    ~ExtendStackWidget();

    // 增加
    void addWidget (QString strText,QString strPluginGuid,QString objName = "");
    void addWidget (QString strText,QWidget* widget,QString objName = "");
    void insertWidget(int index, QWidget* widget);
    void setWidgetText(int index,QString strText);
    void setCurrentWidget(int index);
    void setCurrentWidget(QWidget* widget);

    // 移除
    void removeWidget(QWidget* widget);
    void removeWidget(QString strText);
    void removeWidget(int index);
    //查询
    int	count () ;
    int	currentIndex () ;
    QWidget* currentWidget () ;
    int	indexOf(QWidget * widget ) ;
    QWidget* widgetOf(int index) ;
    // 使能灰化
    void enableWidget(QWidget* widget,bool enable =true);
    void enableWidget(QString strText,bool enable =true);
    void enableWidget(int index,bool enable =true);

    void setTitleBtnWidth(unsigned int iWidth);
 signals:
    void currentChanged ( int index );
private slots:
    void slot_onToolBtn_clicked();
private:

    QTranslator m_translator;
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QFrame *frame;
    QHBoxLayout *gridLayout;
    QStackedWidget *stackedWidget;
    ToolButtonGroup* ToolButtongroup;

private:
	void InitUI();
    ToolButton* newToolBtn(QString strText);
    QVector<WidgetPair> Vec;
};

#endif // CUSTOMUI_H
