#ifndef OTHERPAGE_H
#define OTHERPAGE_H
#include "abstractpagewidget.h" 
#include "ui_netlightcommunicationdlg.h"
#include "ui_pccommunicationdlg.h"
class NetLightCommunicationDlg;
class PcCommunicationDlg;
class ExtendStackWidget;
class OtherPage : public AbstractPageWidget
{
    Q_OBJECT
public:
	OtherPage(QWidget *parent = 0);
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();

private slots:
    void on_currentChanged(int index);
private:
	NetLightCommunicationDlg* m_pNetLightCommunicationDlg;
	PcCommunicationDlg*       m_pPcCommunicationDlg;
    ExtendStackWidget *m_pExtendAcquisition;

};
#endif // ACQUISITIONPAGE_H
