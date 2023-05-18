#include "calibratio.h"
#include "ui_calibratio.h"
#include "common_roi.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "callsdk.h"
#include <QFileDialog>

CalibRatio::CalibRatio(QWidget *parent) :
    QWidget(parent),
    m_pCalibInstance(NULL),
    m_pImageData(NULL),
    ui(new Ui::CalibRatio)
{
    ui->setupUi(this);
    init();
}

CalibRatio::~CalibRatio()
{
    delete ui;
}

void CalibRatio::init()
{
    m_pCalibInstance = (IArithmeticCalibration*)GetSDKInstance("g12arithmetic", IArithmeticCalibration::IDD_ARITHMETICCALIBRATION);

    QList<QVariant> dataListI;
    dataListI << EnShape_Rect << QPointF(0,0) << 0 << 0 << 0 << QPointF(100, 100) << QPointF(1000, 1000);
    ui->widget_show->addShapeItem(dataListI);
    ui->widget_show->setRoiMaxCount(1);

    ui->widget_show->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
    ui->widget_show->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);

    //double dSourceRatio = 0.0;
    double dSourcePixelRatio = 0.0;
    //double dCircleSize = 0.0;
    //int iThrMin = 0;
    //int iThrMax = 0;
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL != pFile)
    {
        //ui->doubleSpinBox_size->setValue(dCircleSize);
        dSourcePixelRatio = pFile->getValue(CAMERAPARAM_MOVEUNIT).toDouble();
        ui->doubleSpinBox_pixel_ratio->setValue(dSourcePixelRatio);
        //ui->doubleSpinBox_ratio->setValue(dSourceRatio);
        //ui->spinBox_th_min->setValue(iThrMin);
        //ui->spinBox_th_max->setValue(iThrMax);
    }
}

void CalibRatio::showImage()
{
    string strFileName = ui->lineEdit_file->text().toStdString();
    bool bIsShow = ui->checkBox_enable->isChecked();
    int minTh = ui->spinBox_th_min->value();
    int maxTh = ui->spinBox_th_max->value();
    stImageInfo image;
    if (NULL == m_pCalibInstance)
    {
        m_pCalibInstance = (IArithmeticCalibration*)GetSDKInstance("g12arithmetic", IArithmeticCalibration::IDD_ARITHMETICCALIBRATION);
    }
    if (NULL == m_pImageData)
    {
        m_pImageData = new stImageInfo;
        m_pCalibInstance->readImage(strFileName, m_pImageData);
    }
    QList<QVariant> dataList = ui->widget_show->queryShapeItem();
    QList<QVariant>::iterator itL = dataList.begin();
    STRoiParam pRoiItem;
    QPointF pointB;
    QPointF pointE;
    for (int index = 0; itL != dataList.end(); itL++, index++)
    {
        QList<QVariant> dataArray = itL->toList();
        pRoiItem.roiType = (E_RoiType)dataArray.at(0).toInt();
        //获取更新后信息
        if (EnShape_Rect == pRoiItem.roiType)
        {
            pointB = dataArray.at(5).toPointF();
            pointE = dataArray.at(6).toPointF();
        }
    }
    m_pCalibInstance->getImage(m_pImageData, &image, pointB.y(), pointB.x(), pointE.y(), pointE.x(), bIsShow, minTh, maxTh);
    ui->widget_show->showPictureInLabel(image.pixelR, image.pixelG, image.pixelB, image.width, image.height);
}

void CalibRatio::on_pushButton_browse_clicked(bool checked /*= false*/)
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"),
        "E:\\image",
        "Image Files(*.jpg *.png *.bmp)",
        0);
    if (!fileName.isNull())
    {
        ui->lineEdit_file->setText(fileName);
    }
    ui->widget_show->showPictureInLabel(fileName);
    if (NULL == m_pImageData)
    {
        m_pImageData = new stImageInfo;
    }
    m_pCalibInstance->readImage(fileName.toStdString(), m_pImageData);
}

void CalibRatio::on_pushButton_calc_clicked(bool checked /*= false*/)
{
    QList<QVariant> dataList = ui->widget_show->queryShapeItem();
    QList<QVariant>::iterator itL = dataList.begin();
    STRoiParam pRoiItem;
    QPointF pointB;
    QPointF pointE;
    for (int index = 0; itL != dataList.end(); itL++, index++)
    {
        QList<QVariant> dataArray = itL->toList();
        pRoiItem.roiType = (E_RoiType)dataArray.at(0).toInt();
        //获取更新后信息
        if (EnShape_Rect == pRoiItem.roiType)
        {
            pointB = dataArray.at(5).toPointF();
            pointE = dataArray.at(6).toPointF();
        }
    }

    double dSourcePixelRatio = 0.0;
    double dSourceRatio = 0.0;
    double dDestRatio = 0.0;
    double dDestPixelRatio = 0.0;
    double dCircleSize = 0.0;
    int width = 0;
    int height = 0;
    int iThrMin = 0;
    int iThrMax = 0;
    stImageInfo image;

    dCircleSize = ui->doubleSpinBox_size->value();
    dSourceRatio = ui->doubleSpinBox_ratio->value();
    dSourcePixelRatio = ui->doubleSpinBox_pixel_ratio->value();
    iThrMin = ui->spinBox_th_min->value();
    iThrMax = ui->spinBox_th_max->value();
    if (NULL == m_pCalibInstance)
    {
        m_pCalibInstance = (IArithmeticCalibration*)GetSDKInstance("g12arithmetic", IArithmeticCalibration::IDD_ARITHMETICCALIBRATION);
    }
    string strFileName = ui->lineEdit_file->text().toStdString();
    if (NULL == m_pImageData)
    {
        m_pImageData = new stImageInfo;
        m_pCalibInstance->readImage(strFileName, m_pImageData);
    }
    m_pCalibInstance->getMarkSize(m_pImageData, &image, iThrMin, iThrMax, pointB.y(), pointB.x(), pointE.y(), pointE.x(), width, height);

    dDestPixelRatio = dCircleSize / width;
    dDestRatio = height * dSourceRatio / width;

    ui->spinBox_x_pixel->setValue(width);
    ui->spinBox_y_pixel->setValue(height);
    ui->doubleSpinBox_calc_pixel->setValue(dDestPixelRatio);
    ui->doubleSpinBox_calc_ratio->setValue(dDestRatio);
    ui->widget_show->showPictureInLabel(image.pixelR, image.pixelG, image.pixelB, image.width, image.height);
}

void CalibRatio::on_pushButton_save_clicked(bool checked /*= false*/)
{
    double dDestRatio = 0.0;
    IFileVariable *pFile = getCameraSetCfgFileVariable();
    if (NULL == pFile)
    {
        dDestRatio = ui->doubleSpinBox_calc_ratio->value();
        pFile->setValue(CAMERAPARAM_MOVEUNIT, dDestRatio, true);
    }
}

void CalibRatio::on_spinBox_th_min_valueChanged(int value)
{
    if (ui->checkBox_enable->isChecked())
    {
        showImage();
    }
}

void CalibRatio::on_spinBox_th_max_valueChanged(int value)
{
    if (ui->checkBox_enable->isChecked())
    {
        showImage();
    }
}
