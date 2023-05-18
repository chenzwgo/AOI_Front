#ifndef ACQUISITIONPAGE_H
#define ACQUISITIONPAGE_H
#include "abstractpagewidget.h"
#include "ui_acquisitionui.h"
#include "ui_ImageConfigForm.h"
#include <QList>
#include <vector>

using namespace std;

class IllumChannelWdgt;
class InitLightWdgt;
class IllumChannelWdgt_new;
class InitLightWdgt_new;
class ExtendStackWidget;
class AcquisitionPage : public AbstractPageWidget
{
    Q_OBJECT
public:
    AcquisitionPage(QWidget *parent = 0);
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
public slots :
	void slotsLoginModelChange(int iModel);
	void slot_channelCntUpdate(int, int);
private slots:
    void on_currentChanged(int index);
	void on_PagetChanged(int index);
private:
    ExtendStackWidget *m_pExtendAcquisition;
	QList<ExtendStackWidget*> m_stationWidgets;
	vector<ExtendStackWidget* > StackWidgetVec;
	map<int ,vector<IllumChannelWdgt_new* > > ChannelWdgtVecMap;
};


class AcquisitionConfigPage : public AbstractPageWidget
{
    Q_OBJECT
public:

    AcquisitionConfigPage(QString strKey,QWidget *parent = 0);
    ~AcquisitionConfigPage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();

private slots:
    void on_autoCutImage();
    void on_manualCutImage();

private:
    QString m_strChannelName;

     Ui::acquisitionForm  *ui;
     int m_imageWidth;
     int m_imageHeight;
     bool m_bflag;
};


class ImageConfigPage : public AbstractPageWidget
{
    Q_OBJECT
public:

    ImageConfigPage(int index=0,QWidget *parent = 0);
    ~ImageConfigPage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
	void  onUpdate();
signals:
	void sig_channelCntUpdate(int stationIndex,int value );
private slots:
	void on_enableBlock();
	void on_BlockvalueChanged(int iValue);
	void on_CamvalueChanged(int iValue);
	void on_enableUpCam();
	void on_ImageConfigChanged(int index);
	void on_spinBox_channel_cnt_valueChanged(int value);
	void on_comboBox_imge_channel_currentIndexChanged(int index);
private:
    int m_index;
    Ui::ImageConfigForm  *ui;


};

#endif // ACQUISITIONPAGE_H
