#ifndef AREACAMPAGE_H
#define AREACAMPAGE_H

#include "abstractpagewidget.h"
#include <QWidget>

#include "jthreadt.h"

namespace Ui {
class CAreaCamPage;
}

class lightcontrol_new;
class motioncontrol_new;

class CAreaCamPage : public AbstractPageWidget
{
    Q_OBJECT

public:
    explicit CAreaCamPage(QString strKey,QWidget *parent = 0);
    ~CAreaCamPage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
protected:
	void hideEvent(QHideEvent *event);
signals:
	void sigSendImage(void* );
	void sig_channelCntUpdate(int, int);
//protected slots:
//	void hideEvent();
private slots:
	void on_CamvalueChanged(int iValue);
	void on_comboBox_cam_name_currentIndexChanged(const QString &text);
	void on_pushButton_snap_clicked();
	void on_pushButton_continue_clicked();
	void on_pushButton_stop_clicked();
	void on_image_display(void* pBuffer);
private:
	void ThreadTrigger(void *param);
private:
    Ui::CAreaCamPage *ui;
    QString m_strKeyName;
	bool m_bRunRealtime; 
	string m_strCamSn;
	JThreadTP<CAreaCamPage, void *, void> *m_pTriggerThread;
	int m_w; 
	int m_h;
    lightcontrol_new* m_plightcontrol_new;
    motioncontrol_new* m_pmotioncontrol;
	int m_itrigNetid;			//   ÔË¶¯¿ØÖÆÆ÷ netid
};

#endif // AREACAMPAGE_H
