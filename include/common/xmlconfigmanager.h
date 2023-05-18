#ifndef XMLCONFIGMANAGER_H
#define XMLCONFIGMANAGER_H
#include "tinyxml.h"

#ifndef MAX_CONFIG_SIZE
#define MAX_CONFIG_SIZE 256
#endif
/**
 * @brief xml配置文件操作类
 */
class XmlConfigManager
{
public:
    XmlConfigManager();
    XmlConfigManager(const char *filename);

    /**
     * @brief 加载配置文件...
     * @brief 加载配置文件, 但不支持有路径相同的配置，即同级同名存在的配置文件的处理
     * @param [in]filename 文件全路径
     * @return 返回加载结果
     */
    bool LoadConfig(const char *filename);

    /**
     * @brief 获取指定项配置
     * @param [in]key      -- 关键字, 不包含根结点， 每一项以‘.’分割
     * @param [in]attr     -- 属性关键字， 为NULL则不取属性值
     * @param [out]szValue -- 读取的配置文件buf
     * @param [in]iLenght  -- 传出buffer的最大内存空间
     * @param [in]defValue -- 默认值， 传入key无效或要取处为空时赋默认值
     * @return 读取结果
     */
    bool GetConfigValue(const char *key, const char *attr, char *szValue, int iLenght, const char *defValue = NULL);

    /**
     * @brief 设置指定项配置
     * @param [in]key       -- 关键字
     * @param [in]attr      -- 属性关键字， 为空则非属性值
     * @param [in]szValue   -- 要设置的值
     * @param [in]bSaveNow  -- 是否立即保存， 如果为否， 则必须保证调用SaveConfig, 如同一个文件要设置多项时需要设置为false
     * @return 设置结果
     */
    bool SetConfigValue(const char *key, const char *attr, const char *szValue, bool bSaveNow = true);

    /**
     * @brief 保存配置...
     * @brief 用于配套SetConfigValue, 其参数bSaveNow为false后(设置多项后)来保存配置到文件，或另存
     * @param [in]filename  -- 要保存的路径， 为空时保存为打开时设置的路径
     * @return 保存结果
     */
    bool SaveConfig(const char *filename = NULL);
private:
    TiXmlDocument m_doc;
    TiXmlNode *m_pRoot;
};

#endif // XMLCONFIGMANAGER_H
