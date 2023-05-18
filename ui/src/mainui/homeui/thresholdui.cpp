#include "thresholdui.h"
#include "ui_thresholdui.h"
#include "../showdefect/graphwidget.h"
#include "../showdefect/diagramscene.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QGraphicsLayout>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <QStringList>
#include <QHBoxLayout>
#include <QSplineSeries>


#include "uitoolbox/visionimagetooldlg.h"
#include "common_roi.h"

QT_CHARTS_USE_NAMESPACE

Thresholdui::Thresholdui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Thresholdui)
{
	m_pImageSrc = NULL;
	m_imageView = NULL;
	m_pQChartView = NULL;
	memset(Histogramdata, 0,sizeof(Histogramdata));
    ui->setupUi(this);
	initUI();
	
}

Thresholdui::~Thresholdui()
{
	if (m_ImageThreshold.imageAddr != NULL)
	{
		delete m_ImageThreshold.imageAddr;
		m_ImageThreshold.imageAddr = NULL;
	}
    delete ui;
}

void Thresholdui::setImageData(ImageSrcStrut *pImageSrc)
{
	m_pImageSrc = pImageSrc;
	m_imageView->showPictureInLabel(m_pImageSrc->imageAddr, NULL, NULL, m_pImageSrc->iImageWide, m_pImageSrc->iImageHight);

}

void Thresholdui::initUI()
{

	ui->spinBox_Threshold_min->setValue(128);
	ui->spinBox_Threshold_max->setValue(255);

	QVBoxLayout *layoutImage = new QVBoxLayout;
	m_layoutHistogram = new QHBoxLayout;

	DiagramScene* m_scene = new DiagramScene(enScene::Scene_First);
	//m_imageView = new GraphWidget(m_scene);
	m_imageView = new CVisionimagetoolDlg(this);
	layoutImage->addWidget(m_imageView);
	layoutImage->setContentsMargins(0, 0, 0, 0);
	ui->widget_Threshold_image->setLayout(layoutImage);
	m_imageView->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
	m_imageView->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);

	ui->widget_histogram->setLayout(m_layoutHistogram);
	connect(m_scene, SIGNAL(sigCoordinate(QPointF)), this, SLOT(slot_UpdatePos(QPointF)));

}

void Thresholdui::showEvent(QShowEvent* pEvent)
{
	on_checkBox_Threshold_enable_clicked(false);
	on_checkBox_enableRoi_clicked(false);
	on_pushButton_threshold_clicked();
}

void Thresholdui::on_checkBox_Threshold_enable_clicked(bool bcheck)
{

	ui->spinBox_Threshold_min->setEnabled(bcheck);
	ui->spinBox_Threshold_max->setEnabled(bcheck);
	//if (bcheck)
	//{	
	//	on_pushButton_threshold_clicked();
	//}
	//else
	//{
	//	m_imageView->showPictureInLabel(m_pImageSrc->imageAddr, NULL, NULL, m_pImageSrc->iImageWide, m_pImageSrc->iImageHight);
	//
	//}
}

void Thresholdui::on_checkBox_enableRoi_clicked(bool bCheck)
{
	if (m_pImageSrc == NULL)
	{
		return;
	}
	ui->lineEdit_roi_x1->setEnabled(bCheck);
	ui->lineEdit_roi_y1->setEnabled(bCheck);
	ui->lineEdit_roi_x2->setEnabled(bCheck);
	ui->lineEdit_roi_y2->setEnabled(bCheck);
	ui->pushButton_setRoi_value->setEnabled(bCheck);
	if (bCheck)
	{
		QList<QVariant> dataListI;
		QPointF pointCenter(0, 0);
		QPointF pointB;
		QPointF pointE;

		pointB.setX(0);
		pointB.setY(0);
		pointE.setX(500);
		pointE.setY(500);
		dataListI << EnShape_Rect1 << pointCenter << 0 << 0 << 0 << pointB << pointE;
		m_imageView->addShapeItem(dataListI);

		ui->lineEdit_roi_x1->setText(QString::number(0));
		ui->lineEdit_roi_y1->setText(QString::number(0));
		ui->lineEdit_roi_x2->setText(QString::number(500));
		ui->lineEdit_roi_y2->setText(QString::number(500));
	}
	else
	{
		ui->lineEdit_roi_x1->setText(QString::number(0));
		ui->lineEdit_roi_y1->setText(QString::number(0));
		ui->lineEdit_roi_x2->setText(QString::number(m_pImageSrc->iImageWide));
		ui->lineEdit_roi_y2->setText(QString::number(m_pImageSrc->iImageHight));
	}
}

void Thresholdui::on_pushButton_setRoi_value_clicked()
{
	int x1;
	int y1;
	int x2;
	int y2;
	QList<QVariant> dataList = m_imageView->queryShapeItem();
	QList<QVariant>::iterator itL = dataList.begin();
	STRoiParam pRoiItem;
	for (int index = 0; itL != dataList.end(); itL++, index++)
	{
		QList<QVariant> dataArray = itL->toList();
		pRoiItem.roiType = (E_RoiType)dataArray.at(0).toInt();
		//获取更新后信息
		if (EnShape_Rect == pRoiItem.roiType)
		{
			QPointF posB = dataArray.at(5).toPointF();
			QPointF posE = dataArray.at(6).toPointF();
			x1 = posB.x();
			y1 = posB.y();
			x2 = posE.x();
			y2 = posE.y();
		}
	}
	ui->lineEdit_roi_x1->setText(QString::number(x1));
	ui->lineEdit_roi_y1->setText(QString::number(y1));
	ui->lineEdit_roi_x2->setText(QString::number(x2));
	ui->lineEdit_roi_y2->setText(QString::number(y2));
}

void Thresholdui::slot_UpdatePos(QPointF pos)
{
	int iGrayValue2 = 0;
	int x = pos.x();
	int y = pos.y();

	if (x >= m_pImageSrc->iImageWide || y >= m_pImageSrc->iImageHight)
	{
		return;
	}

	if (NULL != m_imageView)
	{
		iGrayValue2 = *(m_pImageSrc->imageAddr + y*m_pImageSrc->iImageWide + x);	
	}
	QString str = QString("X = %1,Y=%2,G=%3").arg(pos.x()).arg(pos.y()).arg(iGrayValue2);
	ui->label_threshold_curPos->setText(str);
}


void Thresholdui::updateHistogramUI()
{
	if (m_pQChartView != NULL)
	{
		m_layoutHistogram->removeWidget(m_pQChartView);
		delete m_pQChartView;
		m_pQChartView = NULL;
	}


	QSplineSeries* m_series = new QSplineSeries(this);
	QPen green(Qt::red);
	green.setWidth(3);
	m_series->setPen(green);

	unsigned int max = 0;
	for (int i = 0; i < sizeof(Histogramdata) / sizeof(Histogramdata[0]); i++)
	{
		if (Histogramdata[i] > 0)
		{
			m_series->append(i, Histogramdata[i]);
		}
		if (Histogramdata[i] > max)
		{
			max = Histogramdata[i];
		}

	}
	
	QChart* pQChart = new QChart();
	QValueAxis* m_axis = new QValueAxis;
	pQChart->addSeries(m_series);
	pQChart->createDefaultAxes();
	pQChart->setAxisX(m_axis, m_series);
	m_axis->setTickCount(16);
	pQChart->axisX()->setRange(0, 255);
	pQChart->axisY()->setRange(0, max);

	pQChart->setTitle(QString::fromLocal8Bit("灰度直方图"));
	pQChart->legend()->hide();
	pQChart->setAnimationOptions(QChart::AllAnimations);

	m_pQChartView = new QChartView(pQChart);
	m_pQChartView->setStyleSheet("background-color: rgba(199, 201, 229, 0);");

	m_layoutHistogram->setContentsMargins(0, 0, 0, 0);
	m_layoutHistogram->addWidget(m_pQChartView);

}

void Thresholdui::on_pushButton_threshold_clicked()
{
	bool bEnable = ui->checkBox_Threshold_enable->isChecked();
	bool bEnableROI = ui->checkBox_enableRoi->isChecked();
	unsigned char min = ui->spinBox_Threshold_min->value();
	unsigned char max = ui->spinBox_Threshold_max->value();

	if (!bEnable)
	{
		m_imageView->showPictureInLabel(m_pImageSrc->imageAddr, NULL, NULL, m_pImageSrc->iImageWide, m_pImageSrc->iImageHight);
		return;
	}
	if (bEnableROI)
	{

		int x1 = ui->lineEdit_roi_x1->text().toInt();
		int y1 = ui->lineEdit_roi_y1->text().toInt();
		int x2 = ui->lineEdit_roi_x2->text().toInt();
		int y2 = ui->lineEdit_roi_y2->text().toInt();

		int iWidth = abs(x2 - x1);
		int iHeight = abs(y2 - y1);

		memset(Histogramdata, 0, sizeof(Histogramdata));
		unsigned char* imageAddrR = new unsigned char[m_pImageSrc->iImageHight*m_pImageSrc->iImageWide];
		unsigned char* imageAddrG = new unsigned char[m_pImageSrc->iImageHight*m_pImageSrc->iImageWide];
		memcpy(imageAddrR, m_pImageSrc->imageAddr, m_pImageSrc->iImageHight*m_pImageSrc->iImageWide);
		memcpy(imageAddrG, m_pImageSrc->imageAddr, m_pImageSrc->iImageHight*m_pImageSrc->iImageWide);

		for (long y = 0; y < iHeight; y++)
		{
			for (long x = 0; x < iWidth; x++)
			{
				unsigned char* pdata = m_pImageSrc->imageAddr + (y1+y)*m_pImageSrc->iImageWide + (x1 + x);
				unsigned char* pdataR = imageAddrR + (y1 + y)*m_pImageSrc->iImageWide + (x1 + x);;
				unsigned char* pdataG = imageAddrG + (y1 + y)*m_pImageSrc->iImageWide + (x1 + x);;
				//unsigned char* pdataB = imageAddrB + y*m_pImageSrc->iImageWide + x;
				if (*pdata >= min && *pdata <= max)
				{
					*pdataR = 255; *pdataG = 0; //*pdataB = 0;
					Histogramdata[*pdata] = Histogramdata[*pdata] + 1;

				}
				else
				{
					*pdataR = *pdata; *pdataG = *pdata;// *pdataB = *pdata;
				}
			}
		}

		m_imageView->showPictureInLabel(imageAddrR, imageAddrG, imageAddrG, m_pImageSrc->iImageWide, m_pImageSrc->iImageHight);
		delete imageAddrR;
		delete imageAddrG;


	}
	else
	{
		memset(Histogramdata, 0, sizeof(Histogramdata));
		unsigned char* imageAddrR = new unsigned char[m_pImageSrc->iImageHight*m_pImageSrc->iImageWide];
		unsigned char* imageAddrG = new unsigned char[m_pImageSrc->iImageHight*m_pImageSrc->iImageWide];
		//unsigned char* imageAddrB = new unsigned char[m_pImageSrc->iImageHight*m_pImageSrc->iImageWide];
		for (long y = 0; y < m_pImageSrc->iImageHight; y++)
		{
			for (long x = 0; x < m_pImageSrc->iImageWide; x++)
			{
				unsigned char* pdata = m_pImageSrc->imageAddr + y*m_pImageSrc->iImageWide + x;
				unsigned char* pdataR = imageAddrR + y*m_pImageSrc->iImageWide + x;
				unsigned char* pdataG = imageAddrG + y*m_pImageSrc->iImageWide + x;
				//unsigned char* pdataB = imageAddrB + y*m_pImageSrc->iImageWide + x;
				if (*pdata > min && *pdata < max)
				{
					*pdataR = 255; *pdataG = 0; //*pdataB = 0;
					Histogramdata[*pdata] = Histogramdata[*pdata] + 1;

				}
				else
				{
					*pdataR = *pdata; *pdataG = *pdata;// *pdataB = *pdata;
				}			
			}
		}
		m_imageView->showPictureInLabel(imageAddrR, imageAddrG, imageAddrG, m_pImageSrc->iImageWide, m_pImageSrc->iImageHight);
		delete imageAddrR;
		delete imageAddrG;

		
	}
	updateHistogramUI();
	
}
