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
        en_Error = -1,      // ����
        en_UnSafely = 0,    // �����Ϊ ����ȫ
        en_Safely,          // �����Ϊ ��ȫ
        en_Disable,         // ����ĿΪʹ��
        en_Invalide,        // �������Բ��Ϸ�
        en_NoExist,         // ������
    }enCheckRes;

    /************************************************************************/
    // @brief ����һ��IOʱ�����а�ȫ���
    // @param pIOName					-- [in]IO����
    // @param bool On					-- [in]true ����false �� ����
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getIOCheck(const char* pIOName,bool On) = 0;

    /************************************************************************/
    // @brief �����ƶ�ʱ�����а�ȫ���
    // @param pAxisName					-- [in]������
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getAxisMoveCheck(const char* pAxisName) = 0;

    /************************************************************************/
    // @brief �����ԭʱ�����а�ȫ���
    // @param pAxisName					-- [in]������
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getAxisHomeCheck(const char* pAxisName) = 0;

    /************************************************************************/
    // @brief ��վ�ƶ���ĳ��ʱ�����а�ȫ���
    // @param pStationName					-- [in]��վ����
    // @param pointName					    -- [in]��λ����
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getStationMoveCheck(const char* pStationName,const char* pointName) = 0;

    /************************************************************************/
    // @brief ��վ��ԭʱ�����а�ȫ���
    // @param pStationName					-- [in]��վ����
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getStationHomeCheck(const char* pStationName) = 0;

    /************************************************************************/
    // @brief ִ�б������һ��Actionʱ�����а�ȫ���
    // @param pFlowType					-- [in]�������work,reset,calib
    // @param pFlowType					-- [in]��������
    // @param pFlowType					-- [in]step����
    // @param pFlowType					-- [in]action����
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getFlowAcitonCheck(const char* pFlowType,const char* pWorkName,const char* pStepName,const char* pActionName) = 0;

    /************************************************************************/
    // @brief �Զ��壬��ȫ���
    // @param pCustomID					-- [in]�Զ���ID
    // @return enCheckRes
    /************************************************************************/
    virtual enCheckRes getCustomCheck(const char* pCustomID) = 0;

};

#endif
