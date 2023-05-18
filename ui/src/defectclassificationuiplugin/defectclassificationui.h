#ifndef DEFECTCLASSIFICATIONUI_H
#define DEFECTCLASSIFICATIONUI_H

#include <QDialog>
#include <QTranslator>
#include "OS_DEFINE.h"

namespace Ui {
class DefectClassificationUI;
}

class  ExtendStackWidget;

class DefectClassificationUI : public QDialog
{
    Q_OBJECT
    
private:
    explicit DefectClassificationUI(QWidget *parent = 0);
    ~DefectClassificationUI();

protected:
    void keyPressEvent(QKeyEvent *e);

public:
    static DefectClassificationUI* getInstance();
    static void releaseInstance();

    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void setPluginContainer(void *pPluginContainer);
    void fitSize(const QRect& rect);
    
    bool notify(QString event, OS_WPARAM wparam, OS_LPARAM lparam);
     void SetLanguage(QString strlanguage);
    

private:
    static DefectClassificationUI * m_pInstance;    
    ExtendStackWidget *m_pExtendStackWidget;
    QTranslator m_translator;
};

#endif // DEFECTCLASSIFICATIONUI_H
