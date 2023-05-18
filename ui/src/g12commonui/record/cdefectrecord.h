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
    //void setDefectRecordType(int iType);  //0 ������⵽��ȱ�ݣ� 1 ��ɱ����ȱ�ݣ� 2 ©���ȱ��
    virtual void setDefectParam(stDefectFeature *param);  //����ȱ������

    virtual void clearImagePtr();  //���ͼ������
    virtual void setImageAddr(stImageInfo *pImage, int index);  //����ͼ������
    virtual void setImageAddr(ImageSrcStrut **pImage);  //����ȱ������

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
    *��ȡ����Rect��Ӧ����������� ��Ӧ��Rect
    *��� �� source ת��ǰλ��
    *        bIsReversal �Ƿ�Ϊ�������õ�����ת�� �籣��Ϊ A��B��ӳ�䣬 ����ǰ��֪B��Ӧ��Rect�� ��A��Ӧ��Rect
    *        indexGroup  ��������Ӧ��������¼�ڼ������
    *        indexItem   ͬ������ж�Ӧ���������
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
