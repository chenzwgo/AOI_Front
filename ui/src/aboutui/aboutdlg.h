#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QTranslator>
#include "OS_DEFINE.h"

namespace Ui {
class AboutDlg;
}

class AboutDlg : public QDialog
{
    Q_OBJECT
    
private:
    explicit AboutDlg(QWidget *parent = 0);
    ~AboutDlg();

   

    void readAboutInfo();
protected:
    void keyPressEvent(QKeyEvent *e);

public:
    static AboutDlg* getInstance();
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
    Ui::AboutDlg *ui;
    static AboutDlg * m_pInstance;    
    QTranslator m_translator;
};

#endif // ABOUTDLG_H
