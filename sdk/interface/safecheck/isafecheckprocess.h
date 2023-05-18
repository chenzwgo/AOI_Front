#ifndef I_ISAFECHECKPROCESS_H
#define I_ISAFECHECKPROCESS_H

class ISafeCheckUion
{
public:
    enum
    {
        I_ISAFECHECKPROCESS_IID = 0x11
    };

    typedef enum CheckRes
    {
        en_Error = -1,      // 错误
        en_UnSafely = 0,    // 检测结果为 不安全
        en_Safely,          // 检测结果为 安全
        en_Disable,         // 改条目为使能
        en_Invalide,        // 输入属性不合法
        en_NoExist,         // 不存在
    }enCheckRes;

    /************************************************************************/
    // @brief 操作一个IO时，进行安全检查
    // @param pIOName					-- [in]IO名字
    // @param bool On					-- [in]true 开，false 关 操作
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getIOCheck(const char* pIOName,bool On) = 0;

    /************************************************************************/
    // @brief 单轴移动时，进行安全检查
    // @param pAxisName					-- [in]轴名字
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getAxisMoveCheck(const char* pAxisName) = 0;

    /************************************************************************/
    // @brief 单轴回原时，进行安全检查
    // @param pAxisName					-- [in]轴名字
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getAxisHomeCheck(const char* pAxisName) = 0;

    /************************************************************************/
    // @brief 工站移动到某点时，进行安全检查
    // @param pStationName					-- [in]工站名字
    // @param pointName					    -- [in]点位名字
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getStationMoveCheck(const char* pStationName,const char* pointName) = 0;

    /************************************************************************/
    // @brief 工站回原时，进行安全检查
    // @param pStationName					-- [in]工站名字
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getStationHomeCheck(const char* pStationName) = 0;

    /************************************************************************/
    // @brief 执行编程器中一个Action时，进行安全检查
    // @param pFlowType					-- [in]流程类别，work,reset,calib
    // @param pFlowType					-- [in]流程名字
    // @param pFlowType					-- [in]step名字
    // @param pFlowType					-- [in]action名字
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getFlowAcitonCheck(const char* pFlowType,const char* pWorkName,const char* pStepName,const char* pActionName) = 0;

    /************************************************************************/
    // @brief 自定义，安全检查
    // @param pCustomID					-- [in]自定义ID
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getCustomCheck(const char* pCustomID) = 0;

};

#endif
