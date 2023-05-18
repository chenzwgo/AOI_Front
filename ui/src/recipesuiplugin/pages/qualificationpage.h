#ifndef QUALIFICATIONPAGE_H
#define QUALIFICATIONPAGE_H
#include "abstractpagewidget.h"
#include <QObject>
#include <QTableView>
#include <QStandardItemModel>
#include "ui_generalform.h"
#include "maindefine.h"
#include "defect/idefectlist.h"
#include "defect/idefectlistmanage.h"
class ExtendStackWidget;
class QualificationPage : public AbstractPageWidget
{
	Q_OBJECT
public:
    QualificationPage(QWidget *parent = 0);
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
private slots:
	void on_currentChanged(int index);
private:
    ExtendStackWidget *m_pExtendQualification;
	ExtendStackWidget *m_pDefectLevel;
    ExtendStackWidget *m_pExtend1stCheck;
    ExtendStackWidget *m_pExtend2ndCheck;
};

class DefectQualificationPage : public AbstractPageWidget
{
    Q_OBJECT
public:

    DefectQualificationPage(QString strKey, QWidget *parent = 0);
    ~DefectQualificationPage();
    void  onEnable(bool enable);
    void  onSave();
    void  onCancel();
    void  onUpdate();
private:
    void init();
    void updateUIValue();
    void saveUIValue();

public slots:
	void slot_enableChanged();

private:
    QString m_strChannelName;
    QString  m_strTypeName;
    QTableView         *m_tableView;
    QStandardItemModel *m_defect_model;
    //bool defectTypeEnableArr[DEFECT_TYPE_SIZE];
    vector<bool> m_defectTypeEnableVec;
	map<string, bool> m_mapDefectEnable;
	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
	vector<pair<string,bool>> m_defectEnableVec;
	vector<DefectInfo> m_defectInfoList;
	vector<int> m_indexVec;
};


class GeneralPage : public AbstractPageWidget
{
	Q_OBJECT
public:

	GeneralPage(QWidget *parent = 0);
	~GeneralPage();
	void  onEnable(bool enable);
	void  onSave();
	void  onCancel();
	void  onUpdate();
signals:
	void sig_enableChanged();
private slots:
void on_pushButton_area_warning_clearCnt_clicked();
private:
	void onUpdate_AreaWarningUI();
	void onSave_AreaWarningUI();
	void updateDefectEnablePage();
    Ui::GeneralForm  *ui;
	stDvToplimitCfg m_stDvToplimitCfg[DVTOPLIMITCFG_SIZE];

	vector<string> m_defectNameVec;
	vector<string> m_defectEnNameVec;
	vector<pair<string,bool> > m_defectEnableVec;
	vector<pair<string ,QCheckBox* >> m_defectEnableCheckBoxVec;
};
#endif // QUALIFICATIONPAGE_H
