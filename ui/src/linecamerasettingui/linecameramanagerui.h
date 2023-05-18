#ifndef LINECAMERAMANAGERUI_H
#define LINECAMERAMANAGERUI_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <string>
#include <QTranslator>
#include <QCheckBox>
#include <QMessageBox>

#include "vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/ilinecamera.h"
#include "proxymanager.h"
#include "jthreadt.h"
#include "jevent.h"
#include "common_data.h"

using namespace std;

class CVisionimagetoolDlg;
class CurveGraph;
class iMotion;
class ICard;
class ICardContInterp;
class ImageCalibProcess;
class CalibRatio;

namespace Ui {
class CLineCameraManagerUI;
}
class CParamWidgetCreate
{
public:
    enum ReaLTimeAtrribute
    {
        VS_CAMERA_NAME = 0,        //相机名称
        VS_CAMERA_ID,              //相机ID
        VS_CAMERA_CHANNEL,         //通道数
        VS_CAMERA_EXPOSURE,        //曝光
        VS_CAMERA_GAIN,            //增益
        VS_CAMERA_FREQ,            //行频
        VS_CAMERA_BLOCK_SIZE,      //每次处理的最小大小
        VS_CAMERA_WIDTH,           //图像大小- 宽
        VS_CAMERA_SNAP_HEIGHT,     //图像大小- 单帧
        VS_CAMERA_REAL_HEIGHT,     //图像大小- 实时
        VS_CAMERA_TRIGEREXT,       //外触发
        VS_CAMERA_TRIGER_TYPE,     //触发类型（内部触发、外部IO、编码器触发）
        VS_CAMERA_TRIGERPOLARITY,  //上升沿，下降沿
        VS_CAMERA_IS_INVERT,       //是否翻转
    };
    CParamWidgetCreate(){};

    QWidget *CreateEditorWidget( QWidget *parent, int type);
};
class CLineCameraManagerUI : public QWidget
{
    Q_OBJECT
public:
    enum ErrorCode
    {
        L_UI_OK = 0,
        L_UI_UNINIT,
        L_UI_GET_IMAGE_FAILED,
    };
public:
    CLineCameraManagerUI(QWidget *parent = 0);
    ~CLineCameraManagerUI();
public:
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();

    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);

public:
    static void CallbackGrapFunction(void *, unsigned char *, int, int);

signals:
    void signalShowImage();  ////
    void signalUpdateUI();
    void signalCancelWait();
    void signalFinished();
public slots:
    void on_treeWidget_itemDoubleClicked( QTreeWidgetItem * item, int column ) ;
    void on_pushButton_save_clicked ( bool checked = false ) ;
    void on_pushButton_get_image_clicked();
    void on_pushButton_singlePhoto_clicked();
    void on_pushButton_realTimeDisplay_clicked(bool checked);
    void on_pushButton_Refresh_clicked();
    void on_comboBox_Camera_Select_currentIndexChanged ( int index );
    void on_pushButton_browse_clicked();
    void on_spinBox_min_col_valueChanged();
    void on_spinBox_max_col_valueChanged();
    void on_spinBox_min_gray_valueChanged();
    void on_spinBox_max_gray_valueChanged();
    void on_spinBox_min_limit_valueChanged();
    void on_spinBox_max_limit_valueChanged();
    void slotShowImage();
    void slotUpdateUI();
    void slotWaitCancel();
    void slotWaitCancel(int iR);
    void slotWaitFinished();
    void on_checkBox_C1_clicked(bool checked);
    void on_checkBox_C2_clicked(bool checked);
    void on_checkBox_C3_clicked(bool checked);
    void on_checkBox_C4_clicked(bool checked);
    void on_checkBox_C5_clicked(bool checked);
    void on_checkBox_C6_clicked(bool checked);
    void on_checkBox_C7_clicked(bool checked);
    void on_checkBox_C8_clicked(bool checked);
    void on_pushButton_test_clicked(bool checked);
    void on_pushButton_calib_clicked(bool checked);
    void on_pushButton_signal_clicked(bool checked);
    void on_pushButton_calib_pluse_ratio_clicked(bool checked);
private:
    void UpdateUI();
    void CreateTreeData();
    void InitCamListUI();
    void InitUI();
    void InitCam();
    void SaveParam();
    void ThreadDisplay(void*);
    void ThreadSnap(void*);
    void ThreadInitCam(void* param);
    void ThreadTriger(void* param);
    bool GrapOneImage();
    void timerEvent(QTimerEvent *event);
    void checkCameraStatus();
    void UpdateCurrentCamera(string szCamerName);
    //void UpdateImageTable();
    bool bIsModify();
    bool SetUIEnable(bool bEnable);
    void SnapFinished();

    bool OpenHighSpeedIo(bool bOpen, int iChannel);		// 打开高速IO
    void showEvent(QShowEvent * event);
    void hideEvent(QHideEvent * event);
    void GetCameraInParam(LINE_CAM_PARAM &info);
    void ClearLightCrlNumber();
    void UpdateCamStatus(QString strInfo);

    bool initCardConInterpObj();
    bool startTrigger(int iNumbs/* = -1*/);
    bool stopTrigger();
private:
    Ui::CLineCameraManagerUI *ui;
    QTranslator m_translator;

private:
    LINE_CAM_PARAM  m_pCameraParam;
    ILineCameraFactoryManage *m_pCameraManager;
    ILineCamera              *m_pCurrentCamera;

    iMotion*          m_pMotion;
    ICard *           m_pCard;
    ICardContInterp*  m_pCardConInterp;

    string                m_currentCameraName;
    JThreadTP<CLineCameraManagerUI, void*, void>*    m_pTriggerThread;
    JThreadTP<CLineCameraManagerUI, void*, void>*    m_pThreadInit;
    JThreadTP<CLineCameraManagerUI,void*,void>*    m_pThreadDisplay;
    JThreadTP<CLineCameraManagerUI,void*,void>*    m_pThreadSnap;
    JEvent                                    m_realEvent;
    JEvent                                    m_displayEvent;
    JEvent                                    m_arriveEvent;
    bool                                      m_bRunning;
    bool                                      m_bTriggerRunning;
    bool                                      m_bInitRunning;
    list<stImageInfo> m_ImageList;
    stImageInfo  m_ImageMemory;
    bool         m_bShowRawVidio[MAX_CHANNEL_COUNT];
    QPointF      m_mousePoint;
    unsigned long  m_Counter;

    JMutex m_Counterlock;
    JMutex m_lock;
    JMutex m_imageLock;

    CVisionimagetoolDlg *m_pShowImageWidget;
    int    m_errorCode;
    QMessageBox  m_waitDlg;
    QWidget      *m_pLightCtrlWidget;
    CurveGraph   *m_pCurGraph;
    vector<string> m_ValidCamList;

    int          m_statusTID;   //状态刷新定时器ID
    int          m_waitTID;     //操作阻塞等待定时器ID
    int          m_waitTimeOutID;     //操作阻塞等待定时器ID
    ImageCalibProcess *m_pCalibWidget;
    CalibRatio *m_pCalibRatioWidget;

    int m_channelCount;
    int m_recvChannel;
    bool m_bIsGGCard;
};

#endif // LINECAMERAMANAGERUI_H
