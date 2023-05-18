#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "g12globalenv.h"
#include <QWidget>
#include "jthreadt.h"
#include "jevent.h"
#include <string>
using namespace std;

#include <QMessageBox>

namespace Ui {
class ImageViewer;
}

struct stImageInfo;
class IArithmeticStatic;
class IArithmeticStaticMgr;
class IFileVariable;
class IDefectRecordInterface;
class IBufferManager;

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();

private:
    void init();
    void ImageProcess();

    void clearImageBuffer(stImageInfo *pImageBuffer);
    void UpdateImageBuffer(int width, int height, QImage::Format format = QImage::Format_Mono, int index = -1);
    void SetUIEnable(bool bEnable);
    QRect getMatrixRect(QRect source, bool bIsReversal = false, int indexGroup = 0, int indexItem = 0);
    void SaveStaticImage();
private slots:
    void on_pushButton_load_pic_clicked(bool checked);
    void on_pushButton_get_online_pic_clicked(bool checked);
    void on_comboBox_channel_currentIndexChanged(int index);
    void on_comboBox_pro_type_currentIndexChanged(int index);
    void on_pushButton_static_clicked(bool checked);

    void slotWaitCancel();
    void slotWaitCancel(int iR);
    void slotWaitFinished();
signals:
    void signalCancelWait();
    void signalFinished();

private:
    void ThreadTrigger(void* param);
	void GetCurrentChannel();
private:
    Ui::ImageViewer *ui;
    stImageInfo                  *m_imageInfoBuffer[SEQUE_NUMBER];
    IArithmeticStaticMgr         *m_pIArithmeticStaticMgr;
    IArithmeticStatic            *m_pIArithmeticStatic;
    IFileVariable                *m_pCfgFile;
    IBufferManager               *m_pCameraBufferManager;
    int           m_iUnionWidth;
    int           m_iUnionHeight;
    int           m_iImageCount;

    int           m_iCurrentSelect;
    int           m_iCurrentProType;
    string        m_currentChName;
    QString       m_strImageName;
    QImage::Format m_imageFormat;
    IDefectRecordInterface *m_pDefectRecord;

    JThreadTP<ImageViewer, void*, void>*    m_pTriggerThread;
    bool   m_bRunning;

    QMessageBox  m_waitDlg;
};

#endif // IMAGEVIEWER_H
