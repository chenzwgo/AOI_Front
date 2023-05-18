#ifndef CIMAGE_LOGIC_MANAGER_H
#define CIMAGE_LOGIC_MANAGER_H
#include "iimagelogicmanager.h"
#include "jthreadt.h"
#include "jevent.h"
#include "businesscommon.h"
#include "g12globalenv.h"
#include "common_data.h"
#include "cimagelogic.h"
#include <map>
#include <string>
#include <QString>
#include <QMap>
#include <QTime>
using namespace std;

class IFileVariable;
struct stImageInfo;
struct CImageProduct; 
class INotifyService;
class CImageLogic;
class IBufferManager;


#define MAX_IMAGE_COUNT   8
#define CAM_GROUP_COUNT   CAMERA_NUMBER     //相机分组个数
#define CAM_INDEX_COUNT   CAMERA_NUMBER     //每组中相机最大个数

class CImageLogicManager : public IImageLogicManager
{
public:
    CImageLogicManager(int index = 0);
    ~CImageLogicManager();
    void init();
    void uninit();
    bool setImageParam(int iMinLength = 12800);
    bool setProductPtr(CImageProduct *product);
    bool startGetImage();
    bool isAcqFinished();
    bool resetImageProcess();

    bool triggerGrabImage();
    bool abortGrabImage();
private:
    void initConfig();
private:
    CImageLogic    *m_ImageProInstance[SEQUE_NUMBER];
    string          m_strSeqName[SEQUE_NUMBER];
    IBufferManager               *m_pCameraBufferManager;
    bool            m_bEnable[SEQUE_NUMBER];
	CImageProduct  *m_product;

    int    m_imageUnitSize;
    int    m_imageHeight;
    QTime  m_startGrabTime;
	string m_strScene;
	int m_iIndex;

};
#endif // CIMAGE_LOGIC_MANAGER_H
