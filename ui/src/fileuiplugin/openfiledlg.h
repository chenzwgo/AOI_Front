#ifndef OPENFILEDLG_H
#define OPENFILEDLG_H

#include <QDialog>
#include <QMouseEvent>
#include "fileuicommon.h"
#include <QMap>

namespace Ui {
class OpenFileDlg;
}

class OpenFileDlg : public QDialog
{
    Q_OBJECT

public:
	enum
	{
		SAVE_CURRENT = 0,
		NO_SAVE_CRENT,
		CANCEL
	};
    explicit OpenFileDlg(QWidget *parent = 0);
    ~OpenFileDlg();
	void InitUi(const QString& strCurrentFile, const QMap<QString, StProgramInfo>& mapProInfo);
	QString GetCurrentProgram();
	void SetDayModel();
	void SetNightModel();
private:
	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void showEvent(QShowEvent *);
private slots:
	void on_pushButton_ok_clicked();
	void on_pushButton_cancel_clicked();
private:
    Ui::OpenFileDlg *ui;
	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	bool m_bPressed;//鼠标按下标志（不分左右键）
	QPoint m_ptPress;//鼠标按下的初始位置
};

#endif // OPENFILEDLG_H
