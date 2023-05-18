/*! \file uicontainer.h
 * \brief UI ������ʵ���࣬��Ҫ������ʵ������UI��Դ�ļ��أ�������ѯ
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
     * \brief getPlugin  ��ȡһ�����
     * \param strCLSID   �����GUID
     * \return           ����һ������Ķ���
     */
    QObject *getPlugin(QString strCLSID);

    /*!
     * \brief getPluginName  ��ȡ���������
     * \param strCLSID       �����GUID
     * \return               ���������
     */
    QString    getPluginName(QString strCLSID);

    /*!
     * \brief getPluginVersion ��ȡ����İ汾��Ϣ
     * \param strCLSID         �����GUID
     * \return                 ����İ汾��
     */
    QString    getPluginVersion(QString strCLSID);

    /*!
     * \brief getPluginType    ��ȡ��������
     * \param strCLSID         �����GUID
     * \return                 ��������
     */
    QString    getPluginType(QString strCLSID);

    /*!
     * \brief getPluginLicense  ��ȡ�����License
     * \param strCLSID          �����GUID
     * \return                  ����License
     */
	QString    getPluginLicense(QString strCLSID);

    /*!
     * \brief getPluginInfo     ��ȡ����Ļ�����Ϣ
     * \param strCLSID          �����GUID
     * \return                  ���ز���Ļ�����Ϣ
     */
    PluginInfo* getPluginInfo(QString strCLSID);


    /*!
     * \brief isPluginLoad      �ж�һ������Ƿ����
     * \param strPluginClsid    �����GUID
     * \return
     */
    bool isPluginLoad (QString strPluginClsid);

    /*!
     * \brief isPluginInit      �ж�һ������Ƿ��ʼ��
     * \param strPluginClsid    �����GUID
     * \return
     */
	bool isPluginInit (QString strPluginClsid);

    /*!
     * \brief getPluginClsidsByType     ���ظ����Ͳ����GUID�б�
     * \param type                      ��ӵ����
     * \return                          ���ز��GUID���
     */
    QList<QString> getPluginClsidsByType(QString type);

    /*!
     * \brief getLoadedPluginCLSID  ��ȡ�Ѿ����ز����GUID�б�
     * \return                      �����Ѿ����ز����GUID�б�
     */
	QList<QString> getLoadedPluginCLSID();

    /*!
     * \brief init  ��ʼ�����
     * \brief ��ȡ����ui������ã� ����ui����� ����ʼ��
     * \return
     */
    bool init();

private:
    bool readAllPluginInfo(QString strPluginsFile);
	PluginInfo* readPluginElement (QXmlStreamReader &reader);

    /*!
     * \brief loadAllStaticPlugin  �������о�̬���
     * \return
     */
    bool loadAllStaticPlugin();

    /*!
     * \brief loadPlugin    ���ղ������Ϣ����һ�����
     * \param PluginInfo
     * \return
     */
    bool loadPlugin(PluginInfo *PluginInfo);

    /*!
     * \brief initPlugin    ��ʼ��һ�����
     * \param PluginInfo
     * \return
     */
    bool initPlugin(PluginInfo *PluginInfo);

    /*!
     * \brief GetClsidByName   ͨ����������ֻ�ȡ���
     * \param strName
     * \return
     */
    QString GetClsidByName(QString strName);

    /*!
     * \brief UnloadPlugin      ж��һ�����
     * \param strPluginClsid
     * \return
     */
    bool UnloadPlugin (QString strPluginClsid);
    bool UnloadPlugin (PluginInfo *pPluginInfo);
    bool UnloadAllPlugins ();


    QString GetPluginLibraryPath();
private:
    QHash<QString, PluginInfo* > m_HashPlugins; //!< ������������Ϣ��hash
    QList<PluginInfo *> m_ListPlugins;
    QDir             m_dirPlugins;
    QString			m_strPluginConfig;          //!< ��������ļ���Ϣ
    bool            m_binit;


};



#endif // PLUGINCONTAINER_H
