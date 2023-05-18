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
	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
};

#endif // NOTITLEDLG_H
