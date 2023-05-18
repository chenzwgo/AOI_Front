#include "clinecameralist.h"
#include "ui_clinecameralist.h"
#include "vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/ilinecamera.h"
#include "proxymanager.h"

CLineCameraList::CLineCameraList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLineCameraList)
{
    ui->setupUi(this);
    m_pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
    init();
}

CLineCameraList::~CLineCameraList()
{
    delete ui;
}

void CLineCameraList::init()
{
    if (NULL != m_pCameraManager)
    {
        m_pCameraManager->RefreshValidCameraList();
        map<string, ILineCamera *> mapCamera;
        m_pCameraManager->GetValidCameraList(mapCamera);
        for (map<string, ILineCamera *>::iterator itC = mapCamera.begin(); itC != mapCamera.end(); ++itC)
        {
            ui->comboBox_Cam_List->addItem(itC->first.c_str());
        }
        vector<string> vecName;
        m_pCameraManager->GetCameraNameList(vecName);
        ui->listWidget_cam_list->blockSignals(true);
        for (vector<string>::iterator itN = vecName.begin(); itN != vecName.end(); ++itN)
        {
            ui->listWidget_cam_list->addItem(itN->c_str());
        }
        ui->listWidget_cam_list->blockSignals(false);

        ui->listWidget_cam_list->setCurrentRow(0);
    }
}

#include <QInputDialog>
#include <QMessageBox>

void CLineCameraList::on_pushButton_add_clicked()
{
    QInputDialog dlg;
    dlg.exec();
    QString camName = dlg.textValue();
    if (camName.isEmpty())
    {
        QMessageBox::information(this, QString::fromLocal8Bit("请输入相机名称"), QString::fromLocal8Bit("请输入相机名称"));
        return;
    }
    if (NULL != m_pCameraManager)
    {
        LINE_CAM_PARAM info;
        info.strCameraName = camName.toLocal8Bit();
        info.bEable = ui->checkBox_Enable->isChecked();
        m_pCameraManager->CreateCameraConfig(camName.toLocal8Bit().data(), info);
    }
    ui->listWidget_cam_list->addItem(camName);
}

void CLineCameraList::on_pushButton_delete_clicked()
{
    QListWidgetItem *pItem = ui->listWidget_cam_list->currentItem();
    QString name = pItem->text();
    if (NULL != m_pCameraManager)
    {
        m_pCameraManager->DeleteCameraConfig(name.toLocal8Bit().data());
    }
}

void CLineCameraList::on_pushButton_update_clicked()
{
    QListWidgetItem *pItem = ui->listWidget_cam_list->currentItem();
    QString name = pItem->text();
    if (NULL != m_pCameraManager)
    {
        LINE_CAM_PARAM info;
        m_pCameraManager->GetCameraInParam(name.toLocal8Bit().data(), info);
        QString strCamName = ui->lineEdit_CamName->text();
        if (!strCamName.isEmpty())
        {
            info.strCameraName = strCamName.toLocal8Bit();
            info.bEable = ui->checkBox_Enable->isChecked();
            if (ui->comboBox_Cam_List->count() > 0)
            {
                info.strCameraID = ui->comboBox_Cam_List->currentText().toLocal8Bit();
            }
            if (name != strCamName)
            {
                m_pCameraManager->UpdateCameraName(info, name.toLocal8Bit().data());
            }
            else
            {
                m_pCameraManager->SetCameraInParam(info);
            }
        }
    }
}

void CLineCameraList::on_pushButton_save_clicked()
{
    m_pCameraManager->SaveCameraInParam();
}

void CLineCameraList::on_listWidget_cam_list_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    if (NULL != m_pCameraManager)
    {
        LINE_CAM_PARAM info;
        if (m_pCameraManager->GetCameraInParam(name.toLocal8Bit().data(), info))
        {
            ui->lineEdit_CamName->setText(info.strCameraName.c_str());
            ui->checkBox_Enable->setChecked(info.bEable);
            for (int index = 0; index < ui->comboBox_Cam_List->count(); index++)
            {
                QString strList = ui->comboBox_Cam_List->itemText(index);
                if (0 == strList.compare(info.strCameraID.c_str()))
                {
                    ui->comboBox_Cam_List->setCurrentIndex(index);
                }
            }
            if (ui->comboBox_Cam_List->count() <= 0)
            {
                ui->comboBox_Cam_List->setCurrentText(info.strCameraID.c_str());
            }
        }
    }
}
