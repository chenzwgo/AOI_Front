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
	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	bool m_bPressed;//��갴�±�־���������Ҽ���
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
};

#endif // OPENFILEDLG_H
