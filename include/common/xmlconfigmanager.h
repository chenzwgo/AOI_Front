#ifndef XMLCONFIGMANAGER_H
#define XMLCONFIGMANAGER_H
#include "tinyxml.h"

#ifndef MAX_CONFIG_SIZE
#define MAX_CONFIG_SIZE 256
#endif
/**
 * @brief xml�����ļ�������
 */
class XmlConfigManager
{
public:
    XmlConfigManager();
    XmlConfigManager(const char *filename);

    /**
     * @brief ���������ļ�...
     * @brief ���������ļ�, ����֧����·����ͬ�����ã���ͬ��ͬ�����ڵ������ļ��Ĵ���
     * @param [in]filename �ļ�ȫ·��
     * @return ���ؼ��ؽ��
     */
    bool LoadConfig(const char *filename);

    /**
     * @brief ��ȡָ��������
     * @param [in]key      -- �ؼ���, ����������㣬 ÿһ���ԡ�.���ָ�
     * @param [in]attr     -- ���Թؼ��֣� ΪNULL��ȡ����ֵ
     * @param [out]szValue -- ��ȡ�������ļ�buf
     * @param [in]iLenght  -- ����buffer������ڴ�ռ�
     * @param [in]defValue -- Ĭ��ֵ�� ����key��Ч��Ҫȡ��Ϊ��ʱ��Ĭ��ֵ
     * @return ��ȡ���
     */
    bool GetConfigValue(const char *key, const char *attr, char *szValue, int iLenght, const char *defValue = NULL);

    /**
     * @brief ����ָ��������
     * @param [in]key       -- �ؼ���
     * @param [in]attr      -- ���Թؼ��֣� Ϊ���������ֵ
     * @param [in]szValue   -- Ҫ���õ�ֵ
     * @param [in]bSaveNow  -- �Ƿ��������棬 ���Ϊ�� ����뱣֤����SaveConfig, ��ͬһ���ļ�Ҫ���ö���ʱ��Ҫ����Ϊfalse
     * @return ���ý��
     */
    bool SetConfigValue(const char *key, const char *attr, const char *szValue, bool bSaveNow = true);

    /**
     * @brief ��������...
     * @brief ��������SetConfigValue, �����bSaveNowΪfalse��(���ö����)���������õ��ļ��������
     * @param [in]filename  -- Ҫ�����·���� Ϊ��ʱ����Ϊ��ʱ���õ�·��
     * @return ������
     */
    bool SaveConfig(const char *filename = NULL);
private:
    TiXmlDocument m_doc;
    TiXmlNode *m_pRoot;
};

#endif // XMLCONFIGMANAGER_H
