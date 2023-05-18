#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H
#include "g12common.h"
#include "g12globalenv.h"
#include "maindefine.h"

#include <QFileInfo>
#include <QFrame>
#include <QSet>
#include <QMap>
#include <set>
namespace Ui {
class ConfigView;
}

class ConfigView : public QFrame
{
    Q_OBJECT

public:
    explicit ConfigView(QFrame *parent = 0);
    ~ConfigView();
	void updateCameraSet();

    void InitUI();
public slots:
	void on_pushButton_OK_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_Enable_OK_clicked();
    void on_pushButton_Enable_Cancel_clicked();

	void on_pushButton_warn_Ok_clicked();
	void on_pushButton_warn_Cancel_clicked();
	void on_pushButton_clear_time_clicked();

    //void on_pbtn_cut_ok_clicked();
    //void on_pbtn_cut_cancel_clicked();
    void on_pushButton_save_browse_clicked();
    void on_pushButton_read_browse_clicked();
    void on_pushButton_Image_OK_clicked();
    void on_pushButton_Image_Cancel_clicked();
    void on_pushButton_product_new_clicked();
    void on_pushButton_product_open_clicked();
    void on_pushButton_product_save_clicked();
    void on_pushButton_product_modify_clicked();

    //单独修改通道名称
    void on_pushButton_name1_browse_clicked();
    void on_pushButton_name2_browse_clicked();
    void on_pushButton_name3_browse_clicked();
    void on_pushButton_name4_browse_clicked();
    void on_pushButton_name5_browse_clicked();
    void on_pushButton_name6_browse_clicked();
    void on_pushButton_name7_browse_clicked();
    void on_pushButton_name8_browse_clicked();
	void on_pushButton_image_sn_browse_clicked();
	void on_pushButton_history_data_browse_clicked();

    //离线图片遍历设置
    void on_pushButton_forward_clicked();
    void on_pushButton_next_clicked();
    void on_pushButton_do_clicked();
    void on_pushButton_refresh_clicked();
	void on_pushButton_do_aoto_clicked();
	void on_pushButton_do_aoto_2_clicked();
	
    void on_pushButton_browse_h_config_file_clicked();
    void on_pushButton_browse_h_log_path_clicked();
    void on_pushButton_H_alg_OK_clicked();
    void on_pushButton_H_alg_Cancel_clicked();

    void on_radioButton_pingpong_clicked();
    void on_radioButton_piece_by_piece_clicked();
    void on_radioButton_debug_clicked();
    void on_pushButton_pp_select_clicked();
    void on_pushButton_pbp_select_clicked();
    void on_pushButton_debug_select_clicked();
	void on_pushButton_data_OK_clicked();
	void on_pushButton_data_Cancel_clicked();

	void on_pushButton_browse_ClassifyResult_file_clicked();
	void on_pushButton_ProductResult_path_clicked();
	void on_pushButton_ui_log_path_clicked();
	//
	void on_pushButton_move_right_clicked();
	void on_pushButton_move_stop_clicked();
	void on_pushButton_mov_left_clicked();

	void on_pushButton_pro_sn_clicked();
  
	void on_pushButton_save_choose_front_clicked();
	void on_pushButton_work_time_start_clicked();

	void on_comboBox_station_index_currentIndexChanged(int index);
	void on_pushButton_online_clicked();
	void on_pushButton_offline_clicked();
	void on_pushButton_DvOutTime_clicked();
	void ModifyJsonFile(string key,int value,int station);
	int ReadJsonFile(string key,int station);

	void writeDvTimeOutFlag(bool bOpen);
	void on_pushButton_cgblockOutTime_clicked();
private:
    bool GetChannelFileList(QString path);
    QList<QFileInfo> GetFileList(QString path);
	void parseFileInfoList(QList<QFileInfo> fileInfoList);
	void offlineChangeProduct(string type);
    void UpdateOfflineFile();
    QString GetFileName();
    string getSelectFlowName(int index);
	bool ChannelImageSort(int iVSort[4], int iVsize, QList<QFileInfo> tmpList1, QList<QFileInfo> tmpList2, QList<QFileInfo> tmpList3, QList<QFileInfo> tmpList4, QList<QFileInfo> &tmpList);
protected:
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QHideEvent *event);

private:
    Ui::ConfigView *ui;
	void CalculateToUi();
    QList<QFileInfo> m_fileList[SEQUE_NUMBER];
    int  m_currentImageFileIndex;  //离线图片索引
    int  m_ImageFilesCount;  //离线图片中产品总个数
	unsigned int m_icurrentSN;

	set<int> m_snSet;
	QList<int> m_snList;
	QMap<int, QList<QString> > m_NameListMap;
	QList<QString> m_fileNameList;
};

#endif // CONFIGVIEW_H
