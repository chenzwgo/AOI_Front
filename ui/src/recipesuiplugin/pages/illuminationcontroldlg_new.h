#ifndef ILLUMINATIONCONTROLDLG_NEW_H
#define ILLUMINATIONCONTROLDLG_NEW_H

#include <QWidget>
#include <QString>
#include "ui_illchannelform_new.h"
#include "ui_inilight_new.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "./vscameratools/camera/ilinecamera.h"
#include "g12globalenv.h"
#include "abstractpagewidget.h"
#include "lightcontrol_new.h"
class ExtendStackWidget;
class CameraCtrlWdgt;
class IllumChannelWdgt_new;
class InitLightWdgt_new;


class iMotion;
class ICard;
class ICardContInterp;
class InitLightWdgt_new :public AbstractPageWidget
{
	Q_OBJECT
private:
	enum LIGHT_INDEX
	{
		LIGHT_INDEX_A = 0,
		LIGHT_INDEX_B = 1,
		LIGHT_INDEX_C = 2,
		LIGHT_INDEX_D = 3,
	};
public:
    InitLightWdgt_new(int index = 0, QWidget *parent = 0);
    ~InitLightWdgt_new();
	void initUi();

    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();

private slots:
	//void on_checkBox_stroboflash_on_clicked(bool checked);	//	 频闪使能
	void on_pushButton_writedata_clicked();		//   写入配置到控制器
	void on_pushButton_read_clicked();			//   从控制器读取配置
	void on_pushButton_clear_clicked();			//   清除控制器计数
	void on_pushButton_clear2_clicked();			//   清除控制器计数
	void on_pushButton_svae_set_clicked();		//   保存设置到文件
	void on_pushButton_save_as_clicked();		//   设置另存为
	void on_pushButton_loadcfg_clicked();		//   加载配置文件
	void on_pushButton_clear_print_clicked();	//   清空以下数据

	private slots:
	void on_pushButton_0_cd1_clicked();
	void on_pushButton_0_cd2_clicked();
	void on_pushButton_0_cd3_clicked();
	void on_pushButton_0_cd4_clicked();
	void on_pushButton_0_cd5_clicked();
    void on_pushButton_0_cd6_clicked();

	void on_pushButton_1_cd1_clicked();
	void on_pushButton_1_cd2_clicked();
	void on_pushButton_1_cd3_clicked();
	void on_pushButton_1_cd4_clicked();
	void on_pushButton_1_cd5_clicked();
	void on_pushButton_1_cd6_clicked();

	void on_pushButton_2_cd1_clicked();
	void on_pushButton_2_cd2_clicked();
	void on_pushButton_2_cd3_clicked();
	void on_pushButton_2_cd4_clicked();
	void on_pushButton_2_cd5_clicked();
	void on_pushButton_2_cd6_clicked();

	void on_pushButton_3_cd1_clicked();
	void on_pushButton_3_cd2_clicked();
	void on_pushButton_3_cd3_clicked();
	void on_pushButton_3_cd4_clicked();
	void on_pushButton_3_cd5_clicked();
	void on_pushButton_3_cd6_clicked();

	void on_spinBox_light_cnt_valueChanged(int i);

	void on_pushButton_0_getlight_clicked();
	void on_pushButton_1_getlight_clicked();
	void on_pushButton_2_getlight_clicked();
	void on_pushButton_3_getlight_clicked();

signals:
    void updateSenceName(string);
private:
    void updateUI();
	bool SaveAllData();
	void getUiData();
	void saveIpAdd();
	void updateIPAddUI();
	void updateLightInfoAddUI();
private:
	stEleCfg m_stEleCfg;
    Ui::ilightForm_new  *ui;
    string            m_currentSence;
	lightcontrol_new* m_plightcontrol_new;
	bool m_bstroboflash; //频闪模式模式
	int m_index;
};

class IllumChannelWdgt_new :public AbstractPageWidget
{
    Q_OBJECT
public:
    IllumChannelWdgt_new(QString strSence, QString Channel,QWidget *parent = 0);
    ~IllumChannelWdgt_new();
	bool ChangeProdutUpdate();

    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();

	void updateLightCfgUi();

public slots:
    void on_pushButton_send_clicked();
	void on_pushButton_SendMutFlash_clicked();
	void on_pushButton_SendAllMutFlash_clicked();
    void on_pushButton_saveFile_clicked();
	void on_pushButton_clear_clicked(); 
    void on_pushButton_model_set_clicked(bool checked);
    void slotUpdateSenceName(string strSenceName);

	void on_radioButton_section1_clicked();
	void on_radioButton_section2_clicked();
	void on_radioButton_section3_clicked();
	void on_radioButton_section4_clicked();
	void on_radioButton_section5_clicked();

	// 通知到新光源控制器下发配方,和旧的同步
	void on_save_config(int cameraIndex);

protected:
    virtual void showEvent(QShowEvent *event);
private:

    QString m_strChannel;
	bool ReadCfgToUi();
	bool UiToCfg();
    void updateUI(stIllumChannelCfg *pValue);
    bool checkIsError();
	Channel_Index ChannelNameToIndex(QString strChannelName);
	void readAllCfg();
private:
    QMap<QString,QLabel*> m_mapLabel;
	stIllumChannelCfg m_IllumChannelValue[Scan_line_Size];
    Ui::illForm_new  *ui;
	bool WriteDataToLight();

    int m_ctrlType;
    unsigned int m_FirstLine;
    unsigned int m_SecondLine;
    string       m_currentSence;
	lightcontrol_new* m_plightcontrol_new;
	Scan_line_num m_iSection;
	Channel_Index m_Channel_Index;
};
#endif // ILLUMINATIONCONTROLDLG_NEW_H
