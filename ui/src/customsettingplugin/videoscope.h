#ifndef VIDEOSCOPE_H
#define VIDEOSCOPE_H

#include <string>
#include <QWidget>
#include "jthreadt.h"
#include "jevent.h"
#include "g12globalenv.h"

using namespace std;

namespace Ui {
class videoscope;
}

class CurveGraph;
class stImageInfo;

class IArithmeticStatic;
class IArithmeticStaticMgr;
class IFileVariable;
class IBufferManager;
class ICardContInterp;

class videoscope : public QWidget
{
    Q_OBJECT

public:
    explicit videoscope(QWidget *parent = 0);
    ~videoscope();

    void init();
    void uninit();
private:
    void initUI();
    bool updateVideo(int valueIndex);
    void ImageProcess();
    void ThreadTrigger(void *param);
    void ThreadInitCam(void *param);

    void UpdateImageBuffer();
    void SetUIEnable(bool bEnable);
protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    
private slots:
    void on_pushButton_hold_clicked(bool checked);
    void on_pushButton_clear_clicked(bool checked);
    void on_pushButton_continues_clicked(bool checked);
    void slot_comboBox_channel_currentIndexChanged(int index);
    void on_comboBox_pro_type_currentIndexChanged(int index);
	void slot_comboBox_image_currentIndexChanged(int index);
    void on_doubleSpinBox_rate_valueChanged(double dValue);
    void on_spinBox_min_limit_valueChanged();
    void on_spinBox_max_limit_valueChanged();

    void on_showData();

    void slotInitUI();
signals:
    void sigShowData();
    void sigInitUI();

public:
    static void CallbackGrapFunction(void *, unsigned char *, int, int);
private:
    IArithmeticStaticMgr         *m_pIArithmeticStaticMgr;
	IArithmeticStatic            *m_pIArithmeticStatic;
    IFileVariable                *m_pCfgFile;

    JThreadTP<videoscope, void *, void>       *m_pTriggerThread;
    JThreadTP<videoscope, void *, void>       *m_pInitThread;

    Ui::videoscope *ui;
    CurveGraph   *m_pCurGraph;
    stImageInfo  *m_imageInfoBuffer;
    int           m_holdSize; //保持个数
    IBufferManager *m_pCameraBufferManager;
    ICardContInterp*  m_pCardConInterp;
    //JThreadTP<videoscope, void*, void>*    m_pThreadTrigger;
    int           m_iCurrentSelect;  //当前通道图像在相机中的位置
    int           m_iCurrentProType;
    int           m_iUnionWidth;
    int           m_iUnionHeight;

    bool          m_bTriggerRunning;
    bool          m_bInitCamRunning;
    int           m_duSleepTime;
	int           m_trigger_frequence;
    string        m_currentChName;
    JEvent        m_DispFinished;
    bool          m_bIsInit;
};

#endif // VIDEOSCOPE_H
