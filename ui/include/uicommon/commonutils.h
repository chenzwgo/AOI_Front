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
     * \brief getLayoutService  ��Layoutģ��Ľӿ�
     * \return
     */
    static ILayoutService* getLayoutService();

    /*!
     * \brief getPluginContainer  ��ȡϵͳ�������ӿ�
     * \return
     */
    static iuicontainer *getPluginContainer();

    /*!
     * \brief getPluginContainer  ��ȡֻ�ܲ����IPluginָ��
     * \return
     */
    static IUIPlugin* getIUIPlugin(QString strCLSID);

    /*!
     * \brief getNotifyService     ��ȡnotify �Ľӿ�
     * \return
     */
    static INotifyService *getNotifyService();

    static IStatusBar* getStatusBarServer();

public:

    /*!
     * \brief getCentrePoint    ��ȡUI����ܵ����ĵ�����
     * \return
     */
    static QPoint getCentrePoint();

    /*!
     * \brief moveCentre   ��һ���Ի����ƶ�������λ��
     * \param pDialog
     */
    static void moveCentre (QDialog *pDialog);

    /*!
     * \brief moveRelatedCentre   ��һ���Ի����ƶ�������λ��
     * \param pDialog
     */
    static void moveRelatedCentre (QDialog *pDialog);

    /*!
    * \brief getFileCodec  ��ȡ�ļ��ı��뷽ʽ
    * \param filePath      �ļ���·��
    * \return "Unicode" ,"Unicode big endian"  "UTF-8" "ANSI"
    */
    static QString getFileCodec(const QString& filePath);
    /*!
     * \brief getMainWindow  ��ȡ��UI��ָ��
     * \return
     */
    static QWidget* getMainWindow();

    static bool ChangeMainFrameSize();
    static void threadCountInc (QString file, QString func);
    static int threadCountDec (QString file, QString func);
    static void waitThreadsStop();

    /*!
     * \brief getPluginLibraryPath  ��ȡ�����·��
     * \return
     */
    static QString getPluginLibraryPath();

    /*!
     * \brief isPluginAvailable  �жϲ���Ƿ����
     * \param stringPluginClsid  ���GUID
     * \return
     */
    static bool isPluginAvailable(QString stringPluginClsid);


    static void waitForThreadToStop(QThread* pThread, unsigned int unMaxWait = 5);	//stop thread. Timeout: unMaxWait seconds.


    /*!
     * \brief GetObjectByName   ���ݶ�����������õ����ǵ�UI����
     * \param strName
     * \return
     */
    static QWidget* GetObjectByName(QString strName);

    /*!
     * \brief ShowWidget  ����Wedget�����֣�����ʾһ��Widget
     * \param strWidgetName
     * \return
     */
    static bool ShowWidget(QString strWidgetName);

    /*!
     * \brief ShowPlugin       ����һ�����������������ʾһ�����
     * \param strPluginName
     * \return
     */
    static bool ShowPlugin(QString strPluginName);

    /*!
     * \brief ShowPlugin       ����һ�����������������ʾһ�����
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
     * \brief GetPluginChilrdWidget ��ȡһ�������UI�е�һ��С����
     * \param strPluginName         �������
     * \param strWidgetName         С��������
     * \return
     */
    static QWidget* GetPluginChilrdWidget(QString strPluginName,QString strWidgetName);

    /*!
     * \brief GetPluginMainWidget ��ȡһ��������������widget
     * \param strPluginName ���������
     * \return ���ظò����������
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
    static iuicontainer *m_pPluginContainer;    //!< ���������ָ��
    static ILayoutService *m_pLayoutService;    //!< layout����ָ��
    static INotifyService *m_pNotifyService;    //!< Notify ����ָ��
};

#endif // COMMONUTILS_H


