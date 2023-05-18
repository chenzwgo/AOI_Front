#ifndef NOTITLEDLG_H
#define NOTITLEDLG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class NoTitleDlg;
}

class NoTitleDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NoTitleDlg(QWidget *parent = 0);
    ~NoTitleDlg();
private:
	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void showEvent(QShowEvent *);
private:
	Ui::NoTitleDlg *ui;
	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	bool m_bPressed;//鼠标按下标志（不分左右键）
	QPoint m_ptPress;//鼠标按下的初始位置
};

#endif // NOTITLEDLG_H
