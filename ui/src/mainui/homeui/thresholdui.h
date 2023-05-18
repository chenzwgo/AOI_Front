#ifndef THRESHOLDUI_H
#define THRESHOLDUI_H

#include <QDialog>
#include <QChartView>
#include <QHBoxLayout>
#include "maindefine.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Thresholdui;
}

class GraphWidget;
class CVisionimagetoolDlg;

class Thresholdui : public QDialog
{
    Q_OBJECT

public:
    explicit Thresholdui(QWidget *parent = 0);
	~Thresholdui();
    void setImageData(ImageSrcStrut *pImageSrc);
private:
	void initUI();
protected slots:
	void showEvent(QShowEvent* pEvent);
private slots:
	void on_checkBox_Threshold_enable_clicked(bool bcheck);
	void on_pushButton_threshold_clicked();
	void on_checkBox_enableRoi_clicked(bool bCheck);
	void on_pushButton_setRoi_value_clicked();
	void slot_UpdatePos(QPointF pos);

private:
	void updateHistogramUI();
private:
    Ui::Thresholdui *ui;
	ImageSrcStrut *m_pImageSrc;
	ImageSrcStrut m_ImageThreshold;
	//GraphWidget* m_imageView;
	CVisionimagetoolDlg* m_imageView;

	QHBoxLayout *m_layoutHistogram ;
	QChartView* m_pQChartView;
	unsigned int Histogramdata[256];


};

#endif // THRESHOLDUI_H
