#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QObject>
#include <QPoint>
#include <QDialog>
#include <QMessageBox>
#include <QMutex>
#include <QString>
#include <qdebug.h>
#include "dllexport.h"
#include "uidefine.h"

#define MAINTOOLBAR_EMBED "veiw"

class iuicontainer;
class ILayoutService;
class INotifyService;
class IStatusBar;
class IUIPlugin;

class RT_API CommonUtils : public QObject
{
public:
    /*!
     * \brief getLayoutService  获Layout模块的接口
     * \return
     */
    static ILayoutService* getLayoutService();

    /*!
     * \brief getPluginContainer  获取系统的容器接口
     * \return
     */
    static iuicontainer *getPluginContainer();

    /*!
     * \brief getPluginContainer  获取只能插件的IPlugin指针
     * \return
     */
    static IUIPlugin* getIUIPlugin(QString strCLSID);

    /*!
     * \brief getNotifyService     获取notify 的接口
     * \return
     */
    static INotifyService *getNotifyService();

    static IStatusBar* getStatusBarServer();

public:

    /*!
     * \brief getCentrePoint    获取UI主框架的中心点坐标
     * \return
     */
    static QPoint getCentrePoint();

    /*!
     * \brief moveCentre   将一个对话框移动到中心位置
     * \param pDialog
     */
    static void moveCentre (QDialog *pDialog);

    /*!
     * \brief moveRelatedCentre   将一个对话框移动到中心位置
     * \param pDialog
     */
    static void moveRelatedCentre (QDialog *pDialog);

    /*!
    * \brief getFileCodec  获取文件的编码方式
    * \param filePath      文件的路径
    * \return "Unicode" ,"Unicode big endian"  "UTF-8" "ANSI"
    */
    static QString getFileCodec(const QString& filePath);
    /*!
     * \brief getMainWindow  获取主UI的指针
     * \return
     */
    static QWidget* getMainWindow();

    static bool ChangeMainFrameSize();
    static void threadCountInc (QString file, QString func);
    static int threadCountDec (QString file, QString func);
    static void waitThreadsStop();

    /*!
     * \brief getPluginLibraryPath  获取插件的路径
     * \return
     */
    static QString getPluginLibraryPath();

    /*!
     * \brief isPluginAvailable  判断插件是否可用
     * \param stringPluginClsid  插件GUID
     * \return
     */
    static bool isPluginAvailable(QString stringPluginClsid);


    static void waitForThreadToStop(QThread* pThread, unsigned int unMaxWait = 5);	//stop thread. Timeout: unMaxWait seconds.


    /*!
     * \brief GetObjectByName   根据对象的名字来得到他们的UI对象
     * \param strName
     * \return
     */
    static QWidget* GetObjectByName(QString strName);

    /*!
     * \brief ShowWidget  根据Wedget的名字，来显示一个Widget
     * \param strWidgetName
     * \return
     */
    static bool ShowWidget(QString strWidgetName);

    /*!
     * \brief ShowPlugin       根据一个插件的名字来，显示一个插件
     * \param strPluginName
     * \return
     */
    static bool ShowPlugin(QString strPluginName);

    /*!
     * \brief ShowPlugin       根据一个插件的名字来，显示一个插件
     * \param strPluginName
     * \return
     */
    static bool ShowPluginInCustomFrm(QString strPluginName,QWidget* pParent,QRect rect);

    static QString GetCurrentTimeString();
    static void millisleep(int ms);
    static bool CreateDir(const QString& path);
    static bool IsDirExist(const QString& path);
    static bool RemoveDir(const QString& path);
	static bool CopyToFile (const QString & srcfileName, const QString & desfileName );

    /*!
     * \brief GetPluginChilrdWidget 获取一个插件主UI中的一个小部件
     * \param strPluginName         插件名字
     * \param strWidgetName         小部件名字
     * \return
     */
    static QWidget* GetPluginChilrdWidget(QString strPluginName,QString strWidgetName);

    /*!
     * \brief GetPluginMainWidget 获取一个插件的主窗体的widget
     * \param strPluginName 插件的名字
     * \return 返回该插件的主窗体
     */
    static QWidget* GetPluginMainWidget(QString strPluginName);


    template <typename T_CLASS>
    static void setElidedTextForWidgetText(T_CLASS * pChildWidget, int iMaxShowTextWidth, Qt::TextElideMode ElideMode = Qt::ElideRight)
    {
        qDebug("%s: %s: %s: %d: Enter", _TIME_, __FILE__, __FUNCTION__, __LINE__);

        if(NULL == pChildWidget)
        {
            qDebug("%s: %s: %s: %d: Exit NULL == pChildWidget", _TIME_, __FILE__, __FUNCTION__, __LINE__);
            return;
        }

        QFontMetrics fmFontMetrics(pChildWidget->font());
        if (fmFontMetrics.boundingRect(pChildWidget->text()).width() > iMaxShowTextWidth)
        {
            QString strTemp = fmFontMetrics.elidedText(pChildWidget->text(), ElideMode, iMaxShowTextWidth);
            pChildWidget->setToolTip(pChildWidget->text());
            pChildWidget->setText(strTemp);
        }
        else
        {
            pChildWidget->setToolTip(QString(""));
        }

        qDebug("%s: %s: %s: %d: Exit", _TIME_, __FILE__, __FUNCTION__, __LINE__);
    }


    template <typename T_CLASS>
    static void setElidedTextForWidgetTitle(T_CLASS * pChildWidget, int iMaxShowTextWidth, Qt::TextElideMode ElideMode = Qt::ElideRight)
    {
        qDebug("%s: %s: %s: %d: Enter", _TIME_, __FILE__, __FUNCTION__, __LINE__);

        if(NULL == pChildWidget)
        {
            qDebug("%s: %s: %s: %d: Exit NULL == pChildWidget", _TIME_, __FILE__, __FUNCTION__, __LINE__);
            return;
        }

        QFontMetrics fmFontMetrics(pChildWidget->font());
        if (fmFontMetrics.boundingRect(pChildWidget->title()).width() > iMaxShowTextWidth)
        {
            QString strTemp = fmFontMetrics.elidedText(pChildWidget->title(), ElideMode, iMaxShowTextWidth);
            pChildWidget->setToolTip(pChildWidget->title());
            pChildWidget->setTitle(strTemp);
        }
        else
        {
            pChildWidget->setToolTip(QString(""));
        }

        qDebug("%s: %s: %s: %d: Exit", _TIME_, __FILE__, __FUNCTION__, __LINE__);
    }

private:
    static iuicontainer *m_pPluginContainer;    //!< 插件容器的指针
    static ILayoutService *m_pLayoutService;    //!< layout服务指针
    static INotifyService *m_pNotifyService;    //!< Notify 服务指针
};

#endif // COMMONUTILS_H


