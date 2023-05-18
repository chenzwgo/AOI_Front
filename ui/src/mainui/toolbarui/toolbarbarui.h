#ifndef TOOLBARBARUI_H
#define TOOLBARBARUI_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class ToolBarBarUi;
}

class QAbstractButton;
class ToolBarBarUi : public QWidget
{
    Q_OBJECT

public:
	enum IDToolButtonClicked
	{
		IDT_TOOLBTN_HOME = 0,
		IDT_TOOLBTN_ANALYSIS,
		IDT_TOOLBTN_DEBUD,
		IDT_TOOLBTN_SETTING,
		IDT_TOOLBTN_LOGIN,
		IDT_TOOLBTN_HELP,
		IDT_TOOLBTN_END
	};
	enum ColorModelStyle
	{
		IMODEL_DAY = 0,
		IMODEL_NIGHT
	};
    explicit ToolBarBarUi(QWidget *parent = 0);
    ~ToolBarBarUi();
	void SetLoginInfo(const QString& strLoginInfo);
signals:
	void sigToolButtonClicked(int iIndex);
	void sigChangeModel(int iModel);
private slots:
    void on_toolButton_close_clicked();
    void on_toolButton_min_clicked();
	void on_buttonGroup_buttonClicked(QAbstractButton * button);
	void on_toolButton_help_clicked();
    void SlotTimer();
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void showEvent(QShowEvent * e);
    void hideEvent(QHideEvent * e);
private:
    QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
    bool m_bPressed;//鼠标按下标志（不分左右键）
    QPoint m_ptPress;//鼠标按下的初始位置
    QTimer* m_pTimer;
private:
    Ui::ToolBarBarUi *ui;
	QString  m_strDayBenginTime;
	QString  m_strNightBenginTime;
	bool     m_bFirstShow;
};

#endif // TOOLBARBARUI_H
