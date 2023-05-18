#ifndef _USERMANAGEMENTUI_DLG_H_
#define _USERMANAGEMENTUI_DLG_H_

#include <QDialog>
#include <QTranslator>
#include "OS_DEFINE.h"
#include"ui_usermanagementuidlg.h"
#include "usermanagercfg.h"

class QRegExpValidator;
class UserManagerCfg;
class CUsermanagementuiDlg : public QDialog ,public Ui::usermanagementuiDlg
{
    Q_OBJECT

protected:
    void keyPressEvent( QKeyEvent *e );
public:
    CUsermanagementuiDlg(QDialog *parent = 0);
    ~CUsermanagementuiDlg();
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void fitSize(const QRect& rect);
    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);
private slots:
    void on_btnLogin_clicked(bool bChecked);
    void on_btnCompetence_clicked();
    void on_btnUser_clicked();
private:
    bool CheckUser(QString strLoginModel, QString strModelDesc, QString strName, QString strPwd);
	bool SendUISetting(QString strPluginName, int iModel, QString strName, QMap<QString, STWidgetInfo>* mapInfo);
	bool SendLoginInfo(int iModel, QString strName);
	void ManageUIView(int iLoginModel, QString strName);
	void ShowUIView(int iLoginModel, QObject* pFatherWidget, STWidgetInfo* pInfo);
	QWidget* FindChildWidget(QObject* pObj, QString strChildName);
private:
    QTranslator m_translator;
    UserManagerCfg* m_pUserManagerCfg;
};

#endif // _USERMANAGEMENTUI_DLG_H_

