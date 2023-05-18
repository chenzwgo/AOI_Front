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
    //void setDefectRecordType(int iType);  //0 正常检测到的缺陷， 1 过杀掉的缺陷， 2 漏检的缺陷
    virtual void setDefectParam(stDefectFeature *param) = 0;  //设置缺陷属性

    virtual void clearImagePtr() = 0;  //清除图像数据
    virtual void setImageAddr(stImageInfo *pImage, int index) = 0;
    virtual void setImageAddr(ImageSrcStrut **pImage) = 0;  //设置缺陷属性

    virtual void saveDefectParam() = 0;

};

#endif // _I_DEFECTRECORD_H
