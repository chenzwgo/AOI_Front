#ifndef _EXTEND_DIALOG_H_
#define _EXTEND_DIALOG_H_

#include <QDialog>
#include <QFrame>
#include <QMessageBox>
#include "OS_DEFINE.h"
#include "titlebar.h"

typedef enum enWidgetType
{
    NormalDialog = 0x00000001,
    EmbedDialog = 0x00000002,
    MessageBoxDialog = 0x00000004,
    CustomDialog = 0x00000008
}WidgetType;

class TitleBar;
class RT_API ExtendDialog : public QDialog
{
   
    Q_OBJECT
    
public:

    ExtendDialog(bool bCusTitle=false, QWidget* qWidgetParent = NULL, Qt::WindowFlags flags = Qt::Dialog, WidgetType = NormalDialog);
    virtual ~ExtendDialog();

    QWidget* GetMainWidget();
  
    void setWindowTitle(const QString& strTitle); //cover the implementation of BaseClass's setWindowTitle
    void AddMainWidget(QWidget* pWidget);
    void setWindowFlags(Qt::WindowFlags flags);
    void setLayout(QLayout* playout);
   
    void EnableCloseButton(bool bEnable);
    bool IsCusTitle();

public slots:
    void changeSize(QSize widgetSize);

signals:
    /* This signal is added, to handle X button press for xx dialogs */
  
    void dlgCloseEvent();
  
protected:
    void resizeEvent(QResizeEvent* resizeevent);
    void paintEvent(QPaintEvent * pEvent);
    void closeEvent(QCloseEvent * pCloseEvent);
    void keyPressEvent ( QKeyEvent * keyEvent );
    void showEvent(QShowEvent * showevent);
    void hideEvent(QHideEvent * hidevent);

private:
    void setTtileBar(Qt::WindowFlags flags);
    void getCornerRegion(QRegion& leftCornerRegion, QRegion& rightCornerRegion);
    void getFrameValue();

private:
    QFrame* m_pFrame;
    TitleBar* m_pCusTitle;
    QWidget* m_pMainWidget;
    WidgetType m_type;
    bool m_bCusTitle;
    int m_iTitleHeight;
    int m_iBorderWidth;
    bool m_bCloseButtonEnable;
};

class RT_API MpMessageBox : public QMessageBox
{
   
    Q_OBJECT
 
public:
    MpMessageBox(QWidget* pWidgetParent)
        : QMessageBox(pWidgetParent)
    {
    }

signals:
    void changeSize(QSize boxSize);

protected:
    void resizeEvent(QResizeEvent* resizeevent)
    {
        QSize boxSize = resizeevent->size();
        emit changeSize(boxSize);

        QMessageBox::resizeEvent(resizeevent);
    }
};


#endif

