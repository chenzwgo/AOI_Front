#ifndef CDEFECTRECORD_H
#define CDEFECTRECORD_H
#include "maindefine.h"
#include "common_data.h"
#include "g12commonui/idefectrecord.h"
#include <QWidget>
#include <QFile>

class CFeatures;
class CVisionimagetoolDlg;
class IFileVariable;

namespace Ui {
class CDefectRecord;
}

class CDefectRecord : public IDefectRecordInterface
{
    Q_OBJECT

public:
    CDefectRecord(QWidget *parent = 0);
    ~CDefectRecord();
public:
    static CDefectRecord *getInstance();
    static void releaseInstance();
public:
    //void setDefectRecordType(int iType);  //0 正常检测到的缺陷， 1 过杀掉的缺陷， 2 漏检的缺陷
    virtual void setDefectParam(stDefectFeature *param);  //设置缺陷属性

    virtual void clearImagePtr();  //清除图像数据
    virtual void setImageAddr(stImageInfo *pImage, int index);  //设置图像数据
    virtual void setImageAddr(ImageSrcStrut **pImage);  //设置缺陷属性

    virtual void saveDefectParam();
public slots:
    void on_pushButton_save_clicked(bool clicked);
public:
    void init();
    void deinit();
private:
    void showEvent(QShowEvent *event);

    void updateUI();
    void saveImage(unsigned char *pImage, QString strFile, const QRect &rect);
    QRect getImageRoi(int imageWidth, int imageHeight, QPoint center, int resultWidth = 512, int resultHeigth = 512);
    /***************************************************************************************
    *获取传入Rect对应到其他相机后， 对应的Rect
    *入参 ： source 转换前位置
    *        bIsReversal 是否为保存配置的逆向转换 如保存为 A到B的映射， 而当前已知B对应的Rect， 求A对应的Rect
    *        indexGroup  相机分组对应索引，记录第几组相机
    *        indexItem   同组相机中对应的相机索引
    ***************************************************************************************/
    QRect getMatrixRect(QRect source, bool bIsReversal = false, int indexGroup = 0, int indexItem = 0);
    QRect getValidRect(QRect source, int width, int height);
    void saveDefectInfo(QString strImagePath);
private:
    Ui::CDefectRecord *ui;

    stImageInfo     m_pImage[SEQUE_NUMBER];
    stDefectFeature m_param;
    IFileVariable  *m_paramFile;
    CFeatures     * m_pCFeatures;
    CVisionimagetoolDlg    *m_view[SEQUE_NUMBER];
    stImageInfo     m_defectImageBuffer[SEQUE_NUMBER];
    QRect           m_rect[CAMERA_NUMBER];  
    int             m_defectImageWidth;
    int             m_defectImageHeight;
    QFile           m_csvRecordFile;

    static CDefectRecord *m_pInstance;
};

#endif // CDEFECTRECORD_H
