#ifndef _CAMERAMANAGERPLUGIN_SCRIPT_H_
#define _CAMERAMANAGERPLUGIN_SCRIPT_H_
#include <QObject>
#include "cameramanagerplugin.h"

/**
 * @brief 璧勬簮鎿嶄綔鐨勫璞
 * @brief ISDKResource鐨勭户鎵跨被灏嗚绫诲璞℃敞鍐屽埌璧勬簮绠＄悊瀵硅薄涓紝浣垮叾鍙互閫氳繃
 *      鍑芥暟鍚嶏紝杩斿洖鍊硷紝鍙傛暟绛変俊鎭兘鎵惧埌瀵瑰簲鎺ュ彛锛屽苟鎵ц銆 */
class CCameraManagerPluginScript:public QObject
{
    Q_OBJECT
public:
    CCameraManagerPluginScript();
    ~CCameraManagerPluginScript();
public slots:
    CCameraManagerPluginPlugin* new_CCameraManagerPluginPlugin()
    {
        if (_pSelf == NULL)
        {
            _pSelf = new CCameraManagerPluginPlugin();
        }
        return _pSelf;
    }
    //Demo
    //bool motoron(CCameraManagerPluginPlugin *pob, int i1,int i2 )
    //{
    //    return pob->motoron(i1,i2);
    //}
private:
    CCameraManagerPluginPlugin* _pSelf;
};

#endif // _CAMERAMANAGERPLUGIN_SCRIPT_H_

