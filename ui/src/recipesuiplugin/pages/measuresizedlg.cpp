#include "measuresizedlg.h"
#include "ui_measuresizedlg.h"
#include "filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "callsdk.h"
#include <QFileDialog>
#include "uitoolbox/visionimagetooldlg.h"
#include "common_roi.h"
#include "g12arithmetic/iarithmeticmeasuresize.h"

MeasureSizeDlg::MeasureSizeDlg(QWidget *parent) :
	AbstractPageWidget(parent),
    ui(new Ui::MeasureSizeDlg)
	, m_imageInfoBuffer(NULL)
{
    ui->setupUi(this);
}

MeasureSizeDlg::~MeasureSizeDlg()
{
    delete ui;
}

void MeasureSizeDlg::onEnable(bool enable)
{

}

void MeasureSizeDlg::onSave()
{
	IFileVariable* pFileVariable = getCurrentRoiVariable(m_strProductName.toStdString());
	if (pFileVariable != NULL)
	{
		int roix1 = ui->spinBox_roix1->value();
		int roiy1 = ui->spinBox_roiy1->value();
		int roix2 = ui->spinBox_roix2->value();
		int roiy2 = ui->spinBox_roiy2->value();
	
		pFileVariable->setValue(ROI, ROI_X1, roix1, true);
		pFileVariable->setValue(ROI, ROI_X2, roix2, true);
		pFileVariable->setValue(ROI, ROI_Y1, roiy1, true);
		pFileVariable->setValue(ROI, ROI_Y2, roiy2, true);

		reInitSDKFlag();
	}
}

void MeasureSizeDlg::onCancel()
{
	onUpdate();
}

void MeasureSizeDlg::onUpdate()
{
	int roix1 = 0;
	int roix2 = 0;
	int roiy1 = 0;
	int roiy2 = 0;
	
	IFileVariable* pFileVariable = getCurrentRoiVariable(m_strProductName.toStdString());
	if (pFileVariable != NULL)
	{					
		roix1 = pFileVariable->getValue(ROI, ROI_X1).toInt();
		roix2 = pFileVariable->getValue(ROI, ROI_X2).toInt();
		roiy1 = pFileVariable->getValue(ROI, ROI_Y1).toInt();
		roiy2 = pFileVariable->getValue(ROI, ROI_Y2).toInt();
	}
		
	ui->spinBox_roix1->setValue(roix1);
	ui->spinBox_roiy1->setValue(roiy1);
	ui->spinBox_roix2->setValue(roix2);
	ui->spinBox_roiy2->setValue(roiy2);
}

void MeasureSizeDlg::updateProductName(const QString& strProductName)
{
	AbstractPageWidget::updateProductName(strProductName);
	ui->label_programName->setText(strProductName);
}

void MeasureSizeDlg::on_pushButton_loadImage_clicked()
{
	m_strImageName = QFileDialog::getOpenFileName(this,
		tr("Open Factors File"),
		"E:\\image",
		"Image Files(*.jpg *.png *.bmp)",
		0);
	if (NULL != m_imageInfoBuffer)
	{
		delete m_imageInfoBuffer;
		m_imageInfoBuffer = NULL;
	}
}

void MeasureSizeDlg::on_pushButton_SetROI_clicked()
{
	int iRoiX1 = 0;
	int iRoiY1 = 0;
	int iRoiX2 = 100;
	int iRoiY2 = 100;

	IFileVariable* pFileVariable = getCurrentRoiVariable(m_strProductName.toStdString());
	if (pFileVariable != NULL)
	{
		iRoiX1 = pFileVariable->getValue(ROI, ROI_X1).toInt();
		iRoiY1 = pFileVariable->getValue(ROI, ROI_Y1).toInt();
		iRoiX2 = pFileVariable->getValue(ROI, ROI_X2).toInt();
		iRoiY2 = pFileVariable->getValue(ROI, ROI_Y2).toInt();
	}

	setRoiParam(iRoiX1, iRoiY1, iRoiX2, iRoiY2);

	ui->spinBox_roix1->setValue(iRoiX1);
	ui->spinBox_roiy1->setValue(iRoiY1);
	ui->spinBox_roix2->setValue(iRoiX2);
	ui->spinBox_roiy2->setValue(iRoiY2);
}

void MeasureSizeDlg::reInitSDKFlag()
{
	IArithmeticMeasureSize* m_pIArithmeticMeasureSize = (IArithmeticMeasureSize*)GetSDKInstance("g12arithmetic", IArithmeticMeasureSize::IID_MEASURE_SIZE);
	if (m_pIArithmeticMeasureSize != NULL)
	{
		m_pIArithmeticMeasureSize->uninit();
	}
}

void MeasureSizeDlg::setRoiParam(int &iRoiX1, int &iRoiY1, int &iRoiX2, int &iRoiY2)
{
	QDialog dlg;
	dlg.resize(800, 1000);
	QHBoxLayout pLayout;
	CVisionimagetoolDlg imageDlg;
	dlg.setLayout(&pLayout);
	pLayout.addWidget(&imageDlg);
	imageDlg.setRoiMaxCount(1);
	QList<QVariant> dataListI;
	QPointF pointCenter(0, 0);
	QPointF pointB;
	QPointF pointE;

	pointB.setX(iRoiX1);
	pointB.setY(iRoiY1);
	pointE.setX(iRoiX2);
	pointE.setY(iRoiY2);
	dataListI << EnShape_Rect1 << pointCenter << 0 << 0 << 0 << pointB << pointE;
	imageDlg.addShapeItem(dataListI);

	imageDlg.showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
	imageDlg.showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);
	if (NULL != m_imageInfoBuffer)
	{
		imageDlg.showPictureInLabel(m_imageInfoBuffer->pixelR, NULL, NULL, m_imageInfoBuffer->width, m_imageInfoBuffer->height);
	}
	else
	{
		imageDlg.showPictureInLabel(m_strImageName);
	}
	dlg.exec();

	QList<QVariant> dataList = imageDlg.queryShapeItem();
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
			iRoiX1 = posB.x();
			iRoiY1 = posB.y();
			iRoiX2 = posE.x();
			iRoiY2 = posE.y();
		}
	}
}
