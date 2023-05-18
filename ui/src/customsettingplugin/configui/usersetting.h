#ifndef USERSETTING_H
#define USERSETTING_H

#include <QGroupBox>
#include <QMap>
#include <QComboBox>
#include "settingdefine.h"

class QPushButton;
class QStackedWidget;
namespace Ui {
class UserSetting;
}

class UserSetting : public QGroupBox
{
    Q_OBJECT

public:
    explicit UserSetting(QWidget *parent = 0);
    ~UserSetting();

protected:
    void showEvent(QShowEvent *);

private slots:
    void on_pushBtnOk_clicked();

    void on_pushBtnCancel_clicked();

	void on_comboBox_currentIndexChanged(int index);
private:
    Ui::UserSetting *ui;
	bool m_bInit;
	bool m_bModify;
	bool m_bConnetIndexChanged;
	bool m_bMulStackedWidget;
	SettingInfo m_stSetting;
	QPushButton *m_pButtonOk;
	QPushButton *m_pButtonCancle;
	QComboBox	*m_pCombox;
	QStackedWidget *m_pStackedWidget;
	QMap<QString, ControlSetting> m_mapControl;
private:
	inline void InitControl(QObject *obj);
	void Modify( bool bmodify );
	void ShowData();
	void SaveData();
public:
	void Init();			//³õÊ¼»¯GroupBox¿Ø¼þ
};

#endif // USERSETTING_H
