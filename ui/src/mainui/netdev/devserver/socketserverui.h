#ifndef _SOCKETSERVERUIUI_H_
#define _SOCKETSERVERUIUI_H_

#include <QWidget>
#include <QTranslator>
#include "OS_DEFINE.h"
#include "ui_socketserverui.h"
#include <QMap>
#include "socketcommon.h"
#include "socketserver.h"
#include "socketservercfg.h"


class ISMCameraManager;
class QTimer;
class QMenu;
class QAction;
class ClientCfg;
class Socketserverui : public QWidget ,public Ui::socketserverui
{
    Q_OBJECT

public:
    enum
    {
        CameraAtribute_CamName = 0,
        CameraAtribute_CamIP,
        CameraAtribute_CamPort,
        CameraAtribute_bOnline,
        CameraAtribute_End
    };
    Socketserverui(QWidget *parent = 0);
    ~Socketserverui();
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();

    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);
protected:
    void keyPressEvent( QKeyEvent *e );
private:
    QTranslator m_translator;

protected:
    void showEvent(QShowEvent * e);
    void hideEvent(QHideEvent * e);
private slots:
    void slotTimeout();
    void slotActConnect();
    void slotActDisConnect();
    void slotActDebug();
    void slotActAdd();
    void slotActDel();
    void slotActAlter();
    void on_tableWidget_clientList_customContextMenuRequested ( const QPoint & pos );
    void on_pushButton_clearLog_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_startServer_clicked();
    void on_pushButton_stopServer_clicked();

private:
    bool initCameralistTable();
    void SetTableRow(int iRow, const clientCfg& clientCfg);
    void enableServerUi(bool bEnable);
private:
    CSocketServer         *m_pSocketServer;
    CSocketServerCfg      *m_pSocketServerCfg;
    QTimer                *m_pQTimer;
    QMenu                 *m_pMenu;
    QAction               *m_pActConnect;
    QAction               *m_pActDisconnect;
    QAction               *m_pActDebug;
    QAction               *m_pActAdd;
    QAction               *m_pActDel;
    QAction               *m_pActAlter;
    QMap<QString, bool>    m_mapCamStatus;
};

#endif // _SOCKETSERVERUIUI_H_

