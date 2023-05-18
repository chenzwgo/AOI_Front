/*! \file uicontainer.h
 * \brief UI 容器的实现类，主要是用于实现所有UI资源的加载，管理，查询
 * Details.*/

#ifndef PLUGINCONTAINER_H
#define PLUGINCONTAINER_H
#include "iuicontainer.h"
#include "dllexport.h"

#include <QString>
#include <QHash>
#include <QList>
#include <QtGui>
#include <QDir>
/*!
 * \brief The uicontainer class
 *
 *
 */
class RT_API uicontainer: public iuicontainer
{
public :
    uicontainer(QString strPluginsFile);
private:    
    virtual ~uicontainer();
public:
    /*!
     * \brief getPlugin  获取一个插件
     * \param strCLSID   插件的GUID
     * \return           返回一个插件的对象
     */
    QObject *getPlugin(QString strCLSID);

    /*!
     * \brief getPluginName  获取插件的名字
     * \param strCLSID       插件的GUID
     * \return               插件的名字
     */
    QString    getPluginName(QString strCLSID);

    /*!
     * \brief getPluginVersion 获取插件的版本信息
     * \param strCLSID         插件的GUID
     * \return                 插件的版本号
     */
    QString    getPluginVersion(QString strCLSID);

    /*!
     * \brief getPluginType    获取插件的类别
     * \param strCLSID         插件的GUID
     * \return                 插件的类别
     */
    QString    getPluginType(QString strCLSID);

    /*!
     * \brief getPluginLicense  获取插件的License
     * \param strCLSID          插件的GUID
     * \return                  返回License
     */
	QString    getPluginLicense(QString strCLSID);

    /*!
     * \brief getPluginInfo     获取插件的基本信息
     * \param strCLSID          插件的GUID
     * \return                  返回插件的基本信息
     */
    PluginInfo* getPluginInfo(QString strCLSID);


    /*!
     * \brief isPluginLoad      判断一个插件是否加载
     * \param strPluginClsid    插件的GUID
     * \return
     */
    bool isPluginLoad (QString strPluginClsid);

    /*!
     * \brief isPluginInit      判断一个插件是否初始化
     * \param strPluginClsid    插件的GUID
     * \return
     */
	bool isPluginInit (QString strPluginClsid);

    /*!
     * \brief getPluginClsidsByType     返回该类型插件的GUID列表
     * \param type                      插接的类别
     * \return                          返回插件GUID类别
     */
    QList<QString> getPluginClsidsByType(QString type);

    /*!
     * \brief getLoadedPluginCLSID  获取已经加载插件的GUID列表
     * \return                      返回已经加载插件的GUID列表
     */
	QList<QString> getLoadedPluginCLSID();

    /*!
     * \brief init  初始化插件
     * \brief 读取配置ui插件配置， 加载ui插件， 并初始化
     * \return
     */
    bool init();

private:
    bool readAllPluginInfo(QString strPluginsFile);
	PluginInfo* readPluginElement (QXmlStreamReader &reader);

    /*!
     * \brief loadAllStaticPlugin  加载所有静态插件
     * \return
     */
    bool loadAllStaticPlugin();

    /*!
     * \brief loadPlugin    按照插件的信息加载一个插件
     * \param PluginInfo
     * \return
     */
    bool loadPlugin(PluginInfo *PluginInfo);

    /*!
     * \brief initPlugin    初始化一个插件
     * \param PluginInfo
     * \return
     */
    bool initPlugin(PluginInfo *PluginInfo);

    /*!
     * \brief GetClsidByName   通过插件的名字获取插件
     * \param strName
     * \return
     */
    QString GetClsidByName(QString strName);

    /*!
     * \brief UnloadPlugin      卸载一个插件
     * \param strPluginClsid
     * \return
     */
    bool UnloadPlugin (QString strPluginClsid);
    bool UnloadPlugin (PluginInfo *pPluginInfo);
    bool UnloadAllPlugins ();


    QString GetPluginLibraryPath();
private:
    QHash<QString, PluginInfo* > m_HashPlugins; //!< 保存插件所有信息的hash
    QList<PluginInfo *> m_ListPlugins;
    QDir             m_dirPlugins;
    QString			m_strPluginConfig;          //!< 插件配置文件信息
    bool            m_binit;


};



#endif // PLUGINCONTAINER_H
