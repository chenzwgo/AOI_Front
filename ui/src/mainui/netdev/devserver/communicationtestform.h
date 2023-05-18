#ifndef COMMUNICATIONTESTFORM_H
#define COMMUNICATIONTESTFORM_H

#include <QWidget>
#include <QTimer>
#include "jthreadt.h"
#include "uidefine.h"

namespace Ui {
class CommunicationTestForm;
}

struct STSendContext
{
    QString strFunCode;
    QString strBody;
    QString strDes;
};

class CSocketServer;
class CommunicationTestForm : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationTestForm(QString strCameraName, QWidget *parent = 0);
    ~CommunicationTestForm();
protected:
    void hideEvent(QHideEvent *e);
private:
    void ThreadLoopSend();
    void GetUiSendContext(STSendContext& stContext);
    void SendContext(const STSendContext& stContext);
    void readFunCfg();
    void readFunCfgInI();
public:
    static int iCallBackPrintLog(unsigned int enNotifyType, QString strMsg);
    void PrintLog(QString strLog, UILogLevel iLevel);
signals:
    void sigMyLog(QString strLog, int iLevel);
private slots:
    void slotTimeOut();
    void slotMyLog(QString strLog, int iLevel);
    void on_pushButton_send_clicked();
    void on_pushButton_clearLog_clicked();
    void on_checkBox_loop_clicked(bool bCkecked);
    void on_comboBox_funcCode_currentIndexChanged(int index);
private:
    Ui::CommunicationTestForm *ui;
    const QString          m_strCameraName;
    CSocketServer         *m_pSocketServer;
    QTimer                *m_pQtimer;
    JThreadT<CommunicationTestForm> *m_pThreadLoopSend;
    bool                             m_bLoopSend;
    int                              m_iLoopTime;
    STSendContext                    m_stSendContext;
    static CommunicationTestForm * m_pInstance;
    QMap<QString,STSendContext*> m_SendContextMap;
};

#endif // COMMUNICATIONTESTFORM_H
