#ifndef _MAINUI_DLG_H_
#define _MAINUI_DLG_H_
#include <QWidget>
#include <string>
#include "OS_DEFINE.h"
#include <QTranslator>
#include "uicommon/extendstackwidget.h"
using namespace std;

namespace Ui {
class mainuiDlg;
}

class HomeView;
class CMainuiDlg : public QWidget
{
    Q_OBJECT

private:
    explicit CMainuiDlg(QWidget *parent = 0);
	~CMainuiDlg();
public:
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);
public:
	static CMainuiDlg* GetInstance();
	static void DestroyInstance();
	static CMainuiDlg* m_psInstance;
    static void uilog(string msg, int level);
    void updateSysStateBtn(int esState);
private:
    void setToolBtnPress(QString strBtnName ="");
public: signals:
    void sigupdateSysStateBtn(int esState);
public slots:
    void updateRunLog(const QString &runLog, int type = 0);
    void slotupdateSysStateBtn(int esState);

private:
    Ui::mainuiDlg *ui;
    ExtendStackWidget *m_pExtendStackWidget;
    QTranslator m_translator;
    HomeView* m_HomeView;
};



#endif // _MAINUI_DLG_H_

