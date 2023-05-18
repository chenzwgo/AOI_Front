#ifndef MEASURESIZE_DLG_H
#define MEASURESIZE_DLG_H

#include <QWidget>
#include "abstractpagewidget.h"

namespace Ui {
class MeasureSizeDlg;
}

class stImageInfo;
class MeasureSizeDlg : public AbstractPageWidget
{
    Q_OBJECT

public:
    explicit MeasureSizeDlg(QWidget *parent = 0);
    ~MeasureSizeDlg();

	void  onEnable(bool enable);
	void  onSave();
	void  onCancel();
	void  onUpdate();
	virtual void updateProductName(const QString& strProductName);

private:
	void reInitSDKFlag();
	void setRoiParam(int &iRoiX1, int &iRoiY1, int &iRoiX2, int &iRoiY2);

private slots:
    void on_pushButton_loadImage_clicked();
	void on_pushButton_SetROI_clicked();

private:
	int m_imageWidth;
	int m_imageHeight;
	bool m_bflag;
	QString m_strImageName;
	stImageInfo *m_imageInfoBuffer;
    Ui::MeasureSizeDlg *ui;
};

#endif // MEASURESIZE_DLG_H
