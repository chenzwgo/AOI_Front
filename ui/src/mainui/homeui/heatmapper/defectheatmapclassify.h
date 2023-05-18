#ifndef DEFECTHEATMAPCLASSIFY_H
#define DEFECTHEATMAPCLASSIFY_H
#include <QWidget>
#include <QChartView>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAbstractSeries>
#include <QMap>
#include <QCheckBox>

#include "maindefine.h"
#include "common_data.h"
QT_CHARTS_USE_NAMESPACE

class QHBoxLayout;
;
class DefectheatmapclassifyWidget:public QWidget
{
	Q_OBJECT
public:

     DefectheatmapclassifyWidget(QWidget * parent = NULL);
    ~DefectheatmapclassifyWidget();

	// ��ʼ������ͼ
	void initHeatMap(int RangeX, int RangeY,bool bByPiece = true);
	// ��������
	void addData(int index, double dposx, double dposy,bool Write = false);
	// �������
	void clearUI();
	void slotDateChanged(QString cDate);
	void updateUI(QString cdate, int worktimes);//���µ��������ͼ



private:
	// ����ͼ��
	void createImage();
	void writeDefectHeatMapFile(int itype, double dposx, double dposy);
	void readDefectHeatMapFile();
	void readDefectHeatMapFile(QString strDate,int worktimes);
	
	void initData();   // ��ʼ������
	void showHeatMapPoint();   //��ʾ������
	int changeIndex(int index);  //����ȱ��������Ӧ����ͼȱ������
protected slots:
    // ��ѡ�仯
    void slotSelectChanged(int checked);

private:
	bool m_bInit;
	QHBoxLayout* m_pHLayout;
	QChart *m_pChart;
	QMap<QCheckBox*, QScatterSeries*> m_mapCheckSeries;
	QVector<QImage> m_vectorImage;
	QMap<int, QScatterSeries*> m_mapSeries;
	QMap<int, string> m_mapDefectDes;//
	QMap<int, vector<STPoint> > m_mapPos;
	QMap<int ,QImage> m_mapImage;
	QMap<int, QCheckBox*> m_mapCheckBox;

};

#endif
