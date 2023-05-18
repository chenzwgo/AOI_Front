#ifndef _I_DEFECTRECORD_H
#define _I_DEFECTRECORD_H
#include "maindefine.h"
#include "common_data.h"
#include <QWidget>

class CFeatures;
class DiagramScene;
class CVisionimagetoolDlg;

namespace Ui {
class CDefectRecord;
}

class IDefectRecordInterface : public QWidget
{
public:
    enum
    {
        IID_DEFECT_RECORD_INTERFACE = 0x100
    };
    virtual ~IDefectRecordInterface() {};
public:
    //void setDefectRecordType(int iType);  //0 ������⵽��ȱ�ݣ� 1 ��ɱ����ȱ�ݣ� 2 ©���ȱ��
    virtual void setDefectParam(stDefectFeature *param) = 0;  //����ȱ������

    virtual void clearImagePtr() = 0;  //���ͼ������
    virtual void setImageAddr(stImageInfo *pImage, int index) = 0;
    virtual void setImageAddr(ImageSrcStrut **pImage) = 0;  //����ȱ������

    virtual void saveDefectParam() = 0;

};

#endif // _I_DEFECTRECORD_H
