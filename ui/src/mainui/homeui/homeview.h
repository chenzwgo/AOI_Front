#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include <QTime>
#include "maindefine.h"
#include <QMap>
#include <jthreadt.h>
#include "businesscommon.h"
#include "uidefine.h"
#include <QWidget>
#include <string>
#include "OS_DEFINE.h"
#include <QTranslator>
#include "uicommon/extendstackwidget.h"
#include "../showdefect/diagramscene.h"
#include "ProductUnion.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "dataprocess/cdataprocess.h"
//

namespace Ui {
class HomeView;
}
/*新ui修改*/
class QAbstractButton;
class SelectProductDlg;

class QGraphicsTextItem;

class QGraphicsView;

class CustomWidget;
class CVisionimagetoolDlg;
class DiagramScene;
class IFileVariable;
class CImageProduct;
class GraphWidget;
class ScriptLexer;
class CFeatures;
class IDefectQualification;
class IDefectRecordInterface;
class IBufferManager;
class QToolButton;
class QHBoxLayout;
class QButtonGroup;

class HomeView : public QWidget
{
    Q_OBJECT

public:
    explicit HomeView(QWidget *parent = 0);
    ~HomeView();
//
public:
    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void fitSize(const QRect& rect);

    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);
	void test(OS_LPARAM wparam, OS_LPARAM lparam);
public:
    static HomeView* GetInstance();
    static void DestroyInstance();
    
    static void uilog(string msg, int level);
    void updateSysStateBtn(int esState);
	void unInit();
    void update_dect(int stationIndex);//更新显示图像外框大小//
private:
    void setToolBtnPress(QString strBtnName = "");    
	void timerEvent(QTimerEvent *event);
public: signals:
    void sigupdateSysStateBtn(int esState);
public slots:
    void updateRunLog(const QString &runLog, int type = 0);
    void slotupdateSysStateBtn(int esState);
    void slotUpdatePos(QPointF pos);
    void slotUpdateImage( QString str,int iType);
    void slotUpdateOK(stProductSummary *resData );
    void slotUpdateTableOK(stProductSummary resData);
    void slotRightButtonClicked(QPointF pos);
    void slotInsert();
    void slotSaveImage();
    void slotThreshold();
	void slotsLoginModelChange(int iModel);
public slots:
    void timeOut();
	void updateWorkStatus(int state);  // 更新工作状态
	void slotUpdateUPH(bool bCurrentDate = true);
    void slotUpdefectLevel(int dvdefectAntArr[MAX_DEFECT_SIZE]);
	void slotUpdefectHeatMap(double dposx, double dposy);
	void slotUpdateDVDefectCnt();

	void slotStartWork();
	void slotCTime(double dCTime);
	void SlotDisplayImage(int iRow, int iCol, void* pImage);
	void slotProductInfo(CImageProduct* pProduct);
	void slotUpdateShowEnd(unsigned long long sn, enResFlag RestFlag, string str2Dcode);
	//
	void slotReShowProductInfo(CImageProduct* pProduct);
	//
    void slotCamera_show(QString strName,int iSence);
    void slotUpdateSelectPos(const QRectF pos,int iSence);
	void slotsUpdateUI(gHistogram* pHistogram, int iSize,int ial);
	void slotsIniOtherUI();
private slots:
   
    void on_toolButton_clear_log_clicked();  // 清除LOG
    void on_toolButton_clear_data_clicked();
    void on_pushButton_update_choose_clicked(int change = 0);

    //
	void on_toolButton_change_product_clicked();
    void on_tabWidget_defect_currentChanged(int index);
	void on_toolButton_reCheck_clicked();
	void on_pushButton_defect_day_clicked();
    void on_pushButton_defect_pices_clicked();
    void on_toolButton_change_class_clicked();
	void on_ToolBtns_clicked();
	void on_radioButton_firstcheck_clicked();
	void on_radioButton_secondcheck_clicked();
	/*新ui修改*/
	void on_buttonGroup_mainFlow_buttonClicked(QAbstractButton * button);
	void on_pushButton_page_cnt_clicked();
	void on_pushButton_page_area_clicked();
	void on_comboBox_date_data_currentIndexChanged(const QString &arg1);

	//
	void on_toolButton_prodcuctInfo_clicked();
	void on_toolButton_UPH_clicked();
	void on_toolButton_yield_clicked();
	void on_toolButton_ngchart_clicked();
	void on_toolButton_heatmap_2_clicked();
	//
	void on_pushButton_night_clicked();
	void on_pushButton_day_clicked();

	//
	void on_pushButton_heatmap_clicked();
	void on_pushButton_regioncnt_clicked();

	void on_pushButton_shang_start_clicked();
	void on_pushButton_clean_start_clicked();
	void on_pushButton_xia_start_clicked();
    void on_channelBtn_clicked();
    void on_toolButton_stopBeeAlarm_clicked();
	void on_pushButton_clearmode_clicked();
	
private:
	
    void setBackColor(QWidget *widget, const QBrush &abrush);
    void showEvent ( QShowEvent * event );
    void hideEvent ( QHideEvent * event );
	void RiorNgAlarm();//连续NG RI 报警

    void UpdateProduct();
    void SaveProduct();
	void getNgDefectType(stProductSummary* presSummary);
	void updateProductUPH();
    void updateDefectLevel();

	void updateDVDefectCnt();

    void UpdateFacilityStatus();//刷新设备状态-（卡，光源，相机）
    void saveDefectResult(QString strData);
	void saveEndResData(const QString &strData);
    void UpdateDispImage(int indexChannel, int iScene = 0);
	void readUPHFile(QString date = ""); //
	void writeUPHFile();
   
 
	void readDvToplimitCfg();
	void initToolBtns();
    void updateOutTipIO(int index);
    void updateProductDes();
	void addDateDataChoose();
	void setImageRoi(bool bShow = true);
    void showCutRoi(bool bChannel_A = true);//true正面a b c e f g

	void updateDayOrNightNgChart(bool bToday);
	void initStationUiCfg();

	void updateSueqence(int stationIndex);
    void ShowFlag();
	

/*新ui修改*/
public slots:
    //白天模式
    void SlotDayMode();
    //黑夜模式
    void SlotNightMode();
private:
    static HomeView* m_pInstance;
    Ui::HomeView *ui;

    QTimer* m_pTimer;
    int    m_pTimerDiskCheck;
    IFileVariable *m_fileVariable;

    QDateTime  m_runTime;      // 运行时长
    bool m_bCountProduceTime;  // 生产是否开始计时
    QDateTime m_startTime;     // 生产开始时间

	int m_curCamIndex;
	int m_currentChannel[CAMERA_NUMBER];      // 选中的要显示的图像通道索引

	QMutex m_mutex;


	QFile * m_pFile;
	
	//CRecord m_crecord;
    E_WORK_STATE m_preState;
	QDateTime m_StartWorkTime;	// 开始启动时间
	int m_iProduceTime;
    DiagramScene *m_scene;
    GraphWidget  *m_view;
    GraphWidget  *m_currentView;

    ImageSrcStrut *m_pIamgeSrc[IMAGE_STATION_MAX_CNT][SEQUE_NUMBER];
	stProductSummary m_resSummary;
	unsigned long long m_CurrentProductSn;
	unsigned long long m_LastProductSn;
	IDefectQualification* m_pIDefectQualification;
	stdefectLevelEnableCfg m_stdefectLevelEnableCfg;
	QPixmap m_Pix[2];
    IBufferManager           *m_pCameraBufferManager[2];
    QStringList     m_CamNameList;
    QStringList     m_ChannelNameList;

    QMenu*   m_pContextMenu;
    QAction* m_pActInsert;
	QAction* m_pActSaveImage;
    QAction* m_pActShowRoi;
    QAction* m_pActThreshold;
    QAction* m_pActFreezeImage;
    stUPHCfg m_UPHCfgs[UPHCFG_SIZES]; // 0 总数 ，1为白班 ，2 夜班 // 3  为白班第一个小时
    stUPHCfg m_UPHCfgsHistory[UPHCFG_SIZES]; // 0 总数 ，1为白班 ，2 夜班 显示非当天数据, combox 中选择的日期数据
    bool m_bFristCheckSpec;
    vector<STPoint> m_heatMapPoints;
    stDvToplimitCfg m_stDvToplimitCfg[DVTOPLIMITCFG_SIZE];
    int m_iSequenceNumber;//记录通道数更新
    bool m_bSaveLog;
    QString m_strLogPath;


    // scene index
    int m_scene1Index;  // 相机上下反置时，scene要反一下
    int m_scene2Index;
    QPolygon m_HomeViewCurPolygon[2][2];
    QString m_strMarkCurday;
    int     m_iDayClass;//标志当天的班次。 1为当天的白班 2 为 当天的夜班

    int m_iScenseIndex;
    int m_iImageChoose[IMAGE_STATION_MAX_CNT];  //记录选的默认通道
    int m_ResFlag[res_MaxNumber]; //记录连续结果个数
    int m_iChooseDay;//当前选择 1 白班或 2 夜班

    vector<GraphWidget *> graphWidgetVec;
    vector<DiagramScene *> diagramSceneVec;
    //vector<QHBoxLayout*>    m_ChannelbtnLayout;
    QHBoxLayout* m_ChannelbtnLayout;
    QButtonGroup* m_BtnGroup;
    map<int, vector<QToolButton* > >  btnVecMap;
    bool m_binit;
	string m_strLine;
	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
};

#endif // HOMEVIEW_H
