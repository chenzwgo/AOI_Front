#include "form.h"
#include "ui_form.h"
#include "common_data.h"
#include "./vscameratools/cameramanager/icamerafactorymanage.h"
#include "vscameratools/camera/ilinecamera.h"
#include "uicommon/commonutils.h"
#include "callsdk.h"
#include "rtdefine.h"
#include "g12globalenv.h"
#include "g12common.h"
#include "g12arithmetic/iarithmeticDynamic.h"
#include "g12arithmetic/iarithmeticStatic.h"
#include "g12arithmetic/iarithmeticSignalFilter.h"
#include "g12arithmetic/iarithmeticCutImage.h"
#include "g12arithmetic/iarithmeticInner.h"
#include "g12arithmetic/iarithmeticClassification.h"
#include "filevariable/ifilevariable.h"
#include <QFileDialog>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::openWindows(int width, int height)
{

}

void Form::on_pushButton_get_online_pic_clicked(bool checked)
{
    ILineCameraFactoryManage *pCameraManager = (ILineCameraFactoryManage*)GetSDKInstance("cameramanager", ILineCameraFactoryManage::IID_LINE_CAMERA_FACTORY_MANAGE);
    RETURN_IF(NULL == pCameraManager);

    int index = ui->comboBox_channel->currentIndex();

    m_pCurrentCamera = pCameraManager->getSequenceImageInstance(index);
    RETURN_IF(NULL == m_pCurrentCamera);

    int width = 0;
    int height = 0;
    m_pCurrentCamera->GetImageSize(width, height);
    if (m_imageInfoBuffer->width != width || m_imageInfoBuffer->height != height)
    {
        if (NULL != m_imageInfoBuffer->pixelR)
        {
            delete[]m_imageInfoBuffer->pixelR;
            m_imageInfoBuffer->pixelR = NULL;
        }
    }
    m_imageInfoBuffer->width = width;
    m_imageInfoBuffer->height = height;
    if (NULL == m_imageInfoBuffer->pixelR)
    {
        m_imageInfoBuffer->pixelR = new unsigned char[m_imageInfoBuffer->width*m_imageInfoBuffer->height];
    }
    int iCamOn[CAMERA_NUMBER] = { 0 };

    int iCamIndex = 0; //相机索引
    int iChIndex = pCameraManager->getSequenceInCamIndex(index);  //图像在当前相机中的索引
    RETURN_IF(iChIndex < 0);
    const unsigned char *pszBuffer = m_pCurrentCamera->GetImageBufferAdr(iChIndex);
    if (NULL != pszBuffer)
    {
        memcpy(m_imageInfoBuffer->pixelR, pszBuffer, width*height);
    }
}
