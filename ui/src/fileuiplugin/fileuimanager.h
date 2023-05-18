
#ifndef FILEUIMANAGER_H
#define FILEUIMANAGER_H
#include "OS_DEFINE.h"
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QDialog>
#include "ui_newfiledlg.h"
#include "fileuicommon.h"


class INotifyService;
class FileUiManager :public QWidget
{
    Q_OBJECT
public:
    FileUiManager(QWidget *parent = 0);
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void setPluginContainer(void *pPluginContainer);
    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage); 
private:

	void onFileOpen();
	void onFileNew();
	void onFileSave();
    void onFileSaveAs();
    void onFileModify();
	// 设置当前程序名
	void setCurrentProgram(const QString &programName);
	// 获取程序描述
	QString readProgramDesc(const QString &programName);
	// 写入程序描述
	void writeProgramDesc(const QString &programName, const QString &programDesc);
private:
	void SetDayModel();
	void SetNightModel();
	
protected slots:
	void slotShowMessageOpen();
	void slotShowMessageNew();

private:
	QString m_currentProgram;
	QMap<QString,StProgramInfo> m_mapProInfo;
    QDialog* m_newDlg;
    Ui::newfiledlg* m_newUI;
	INotifyService* m_pNotifyService; //发UI信号句柄
	bool m_bDayModel;
};


#endif
