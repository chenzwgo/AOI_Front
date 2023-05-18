#include "imagecalibprocess.h"
#include "ui_imagecalibprocess.h"
#include "g12common.h"
#include "callsdk.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include <QFileDialog>

ImageCalibProcess::ImageCalibProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCalibProcess),
    m_pCalibInstance(NULL),
    m_currentType(IArithmeticCalibration::TYPE_BASE_IMAGE)
{
    ui->setupUi(this);
    init();
}

ImageCalibProcess::~ImageCalibProcess()
{
    delete ui;
}

void ImageCalibProcess::init()
{
    m_pCalibInstance = (IArithmeticCalibration*)GetSDKInstance("g12arithmetic", IArithmeticCalibration::IDD_ARITHMETICCALIBRATION);
    if (NULL != m_pCalibInstance)
    {
        m_pCalibInstance->getCalibParam(&m_stInnerParam);
        ui->doubleSpinBox_scale_x->setValue(m_stInnerParam.dXScale);
        ui->doubleSpinBox_scale_y->setValue(m_stInnerParam.dYScale);
        ui->doubleSpinBox_offset_x->setValue(m_stInnerParam.dXOffset);
        ui->doubleSpinBox_offset_y->setValue(m_stInnerParam.dYOffset);

        ui->lineEdit_base_file->setText(m_stInnerParam.strBaseFile.c_str());
        ui->lineEdit_source_file->setText(m_stInnerParam.strSourceFile.c_str());
        ui->spinBox_base_th_min->setValue(m_stInnerParam.iBaseMinThr);
        ui->spinBox_base_th_max->setValue(m_stInnerParam.iBaseMaxThr);
        ui->spinBox_source_th_min->setValue(m_stInnerParam.iSourceMinThr);
        ui->spinBox_source_th_max->setValue(m_stInnerParam.iSourceMaxThr);
        ui->spinBox_base_area_min->setValue(m_stInnerParam.iBaseMinArea);
        ui->spinBox_source_area_min->setValue(m_stInnerParam.iSourceMinArea);
        ui->checkBox_is_rotation->setChecked(m_stInnerParam.bIsMirror);

        ui->spinBox_th_min->blockSignals(true);
        ui->spinBox_th_min->setValue(m_stInnerParam.iGrayMinThr);
        ui->spinBox_th_min->blockSignals(false);
        ui->spinBox_th_max->blockSignals(true);
        ui->spinBox_th_max->setValue(m_stInnerParam.iGrayMaxThr);
        ui->spinBox_th_max->blockSignals(false);
    }
    ui->widget_show->showStatusBar(XTV_IMAGE_SHOW::VS_STATUS_SHOW_ALL);
    ui->widget_show->showToolBar(XTV_IMAGE_SHOW::VS_TOOLBAR_IMAGEZOOM);
}

void ImageCalibProcess::on_pushButton_base_browse_clicked( bool checked /*= false */ )
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"),
        "E:\\image",
        "Image Files(*.jpg *.png *.bmp)",
        0);
    if (!fileName.isNull())
    {
        ui->lineEdit_base_file->setText(fileName);
    }
}

void ImageCalibProcess::on_pushButton_source_browse_clicked( bool checked /*= false */ )
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Factors File"),
        "E:\\image",
        "Image Files(*.jpg *.png *.bmp)",
        0);
    if (!fileName.isNull())
    {
        ui->lineEdit_source_file->setText(fileName);
    }
}

void ImageCalibProcess::on_pushButton_exe_clicked( bool checked /*= false */ )
{
    QString strBaseFile = ui->lineEdit_base_file->text();
    QString strSourceFile = ui->lineEdit_source_file->text();

    int thBaseMin = 20;
    int thBaseMax = 50;
    int thSourceMin = 20;
    int thSourceMax = 50;

    int iBMinArea = 10;
    int iSMinArea = 10;

    thBaseMin = ui->spinBox_base_th_min->value();
    thBaseMax = ui->spinBox_base_th_max->value();
    thSourceMin = ui->spinBox_source_th_min->value();
    thSourceMax = ui->spinBox_source_th_max->value();

    bool bIsMirror = true;
    bIsMirror = ui->checkBox_is_rotation->isChecked();

    if (strBaseFile.isEmpty() || strSourceFile.isEmpty())
    {
        return;
    }
    
    if (NULL != m_pCalibInstance)
    {
        m_pCalibInstance->startCalibration(&m_stInnerParam);

        BJCalibParam stParam;
        m_pCalibInstance->getCalibParam(&stParam);
        m_stInnerParam.dXOffset = stParam.dXOffset;
        m_stInnerParam.dYOffset = stParam.dYOffset;
        m_stInnerParam.dXScale = stParam.dXScale;
        m_stInnerParam.dYScale = stParam.dYScale;
        ui->doubleSpinBox_scale_x->setValue(m_stInnerParam.dXScale);
        ui->doubleSpinBox_scale_y->setValue(m_stInnerParam.dYScale);
        ui->doubleSpinBox_offset_x->setValue(m_stInnerParam.dXOffset);
        ui->doubleSpinBox_offset_y->setValue(m_stInnerParam.dYOffset);
        showImage(IArithmeticCalibration::TYPE_UNION_IMAGE);
    }
}

void ImageCalibProcess::on_pushButton_save_clicked( bool checked /*= false */ )
{
    if (NULL != m_pCalibInstance)
    {
        m_stInnerParam.dXScale = ui->doubleSpinBox_scale_x->value();
        m_stInnerParam.dYScale = ui->doubleSpinBox_scale_y->value();
        m_stInnerParam.dXOffset = ui->doubleSpinBox_offset_x->value();
        m_stInnerParam.dYOffset = ui->doubleSpinBox_offset_y->value();

        m_stInnerParam.strBaseFile = ui->lineEdit_base_file->text().toLocal8Bit();
        m_stInnerParam.strSourceFile = ui->lineEdit_source_file->text().toLocal8Bit();
        m_stInnerParam.iBaseMinThr = ui->spinBox_base_th_min->value();
        m_stInnerParam.iBaseMaxThr = ui->spinBox_base_th_max->value();
        m_stInnerParam.iSourceMinThr = ui->spinBox_source_th_min->value();
        m_stInnerParam.iSourceMaxThr = ui->spinBox_source_th_max->value();

        m_stInnerParam.iBaseMinArea = ui->spinBox_base_area_min->value();
        m_stInnerParam.iSourceMinArea = ui->spinBox_source_area_min->value();
        m_stInnerParam.bIsMirror = ui->checkBox_is_rotation->isChecked();

        m_pCalibInstance->saveCalibParam(&m_stInnerParam);
    }
}

void ImageCalibProcess::on_pushButton_base_clicked( bool checked /*= false */ )
{
   // showImage(IArithmeticCalibration::TYPE_BASE_IMAGE);
}

void ImageCalibProcess::on_pushButton_source_clicked( bool checked /*= false */ )
{
    //showImage(IArithmeticCalibration::TYPE_SOURCE_IMAGE);
}

void ImageCalibProcess::on_pushButton_correction_clicked( bool checked /*= false */ )
{
    //showImage(IArithmeticCalibration::TYPE_PROCESS_IMAGE);
}

void ImageCalibProcess::on_pushButton_union_clicked( bool checked /*= false */ )
{
    //showImage(IArithmeticCalibration::TYPE_UNION_IMAGE);
}

void ImageCalibProcess::on_spinBox_th_min_valueChanged(int value)
{
    //showImage(m_currentType);
}

void ImageCalibProcess::on_spinBox_th_max_valueChanged(int value)
{
   // showImage(m_currentType);
}

