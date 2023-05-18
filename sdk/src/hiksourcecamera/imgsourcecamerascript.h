#ifndef _IMAGINGSOURCECAMERA_SCRIPT_H_
#define _IMAGINGSOURCECAMERA_SCRIPT_H_
#include <QObject>
#include "imagingsourcecamera.h"

/**
 * @brief 璧勬簮鎿嶄綔鐨勫璞
 * @brief ISDKResource鐨勭户鎵跨被灏嗚绫诲璞℃敞鍐屽埌璧勬簮绠＄悊瀵硅薄涓紝浣垮叾鍙互閫氳繃
 *      鍑芥暟鍚嶏紝杩斿洖鍊硷紝鍙傛暟绛変俊鎭兘鎵惧埌瀵瑰簲鎺ュ彛锛屽苟鎵ц銆 */
class CImagingSourceCameraScript:public QObject
{
    Q_OBJECT
public:
    CImagingSourceCameraScript();
    ~CImagingSourceCameraScript();
public slots:
    CImagingSourceCameraPlugin* new_CImagingSourceCameraPlugin()
    {
        if (_pSelf == NULL)
        {
            _pSelf = new CImagingSourceCameraPlugin();
        }
        return _pSelf;
    }
    //Demo
    //bool motoron(CImagingSourceCameraPlugin *pob, int i1,int i2 )
    //{
    //    return pob->motoron(i1,i2);
    //}
private:
    CImagingSourceCameraPlugin* _pSelf;
};

#endif // _IMAGINGSOURCECAMERA_SCRIPT_H_

