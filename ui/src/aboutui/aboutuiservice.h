#ifndef ABOUTUISERVICE_H
#define ABOUTUISERVICE_H

#include <QObject>
#include <QWidget>
#include "OS_DEFINE.h"

class AboutDlg;
class AboutUIService: public QObject
{
    Q_OBJECT
private:
    AboutUIService();
    virtual ~AboutUIService();
public:
    static AboutUIService* getInstance();
    static void releaseInstance();

    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void setPluginContainer(void *pPluginContainer);
    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strPluginName);
    QWidget* GetAboutWidget();

private:
    static AboutUIService* m_pInstance;
    AboutDlg *m_pAboutDlg;

};

#endif // ABOUTUISERVICE_H
