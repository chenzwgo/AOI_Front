#ifndef SNED_MESSAGE_H
#define SNED_MESSAGE_H

#include <QObject>
#include <QString>
#include "maindefine.h"
#include "uidefine.h"
#include "ProductUnion.h"

#define SENDMSG_INSTANCE CSendMessage::GetInstance()
class CSendMessage : public QObject
{
	Q_OBJECT
public:
	static CSendMessage * GetInstance();
	static void ReleaseInstance();

	// 发送log,供外部调用
    void Send( const QString &message, UILogLevel type = UILOG_INFO  );
    void SendLoginModelChange( int iModel );
    void sendPressUI(int index);
    void sendPressTabView(int index);
    void sendNewDefet( double width,double height );
    void sendAddsetdecet(MarkStrut *mark);
    void sendAddVecsetdecet( QVector<MarkStrut*>* vecmark );
    void sendUpdateUiStus( QString str,int iType );
	//清界面显示的缺陷列表//在更新缺陷前必须掉用一次//
	void sendClearTable();
	//iScene 从0开始，0为上相机，1，下相机
    void sendAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> * mapMark ,int iScene );
	//更到界面//
	void sendAddMapVecsetUiTabledecet(QMap<QString, QVector<MarkStrut *> *> * mapMark);
    void sendChooseShow(int *icheck ,int iScene);
	void sendResUpdateUi(bool bOk = true);
	//发送缺陷所有数据到界面
	void sendProductInfo(CImageProduct* pProduct);
	//发送单个数据到列表界面显示
	void sendOneDefecTable(stDefectFeature Defect);
	//发送单个通道图像信息到列表界面
	void sendImageDefecTable(ImageSrcStrut* Image,int istation=0);
	//发送单个数据到界面显示  //iScene 0  为上相机。1 为下相机
    void sendOneDefecUI(stDefectFeature Defect, int iScene);
    void sendAddCutROIUI(QRect Defect, int iScene);

	//切换产品
	void sendChangeProduct();
	//出料显示OKNG
	void sendShowOkNGEnd(unsigned long long sn, enResFlag ResultFlag,string  str2dcode);

	void sendCtToUi(double ctTimes);
	void sendRobotLog(const QString &message);
	void sendShowChoosePic(int iChoose);
	void sendShowDefectChart(gHistogram* pHistogram, int iSize, int ial);
	void sendInitOtherUi();
signals:
	void signalMessage(const QString &, int);
	void signalLoginModelChange(int);
    void signalPressUi(int index);
    void signalPressTabView(int index);
    void signalNewDefet( double width,double height );
    void signalAddsetdecet(MarkStrut *mark);
    void signalAddVecsetdecet( QVector<MarkStrut*>* vecmark );
    void signalUpdateUiStus( QString str,int iType );
    void signalAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> * mapMark ,int iScene);
    void signalChooseShow(int *icheck ,int iScene);
	void signalResUpdateUi(bool bOk);
	void signalClearTable();
	void signalAddMapVecsetUiTabledecet(QMap<QString, QVector<MarkStrut *> *> * mapMark);
	void signalProductInfo(CImageProduct* pProduct);
	void signalOneDefecTable(stDefectFeature Defect);
	void signalImageDefecTable(ImageSrcStrut* Image, int istation);
	void signalOneDefecUI(stDefectFeature Defect, int iScene);
    void signalAddCutROIUI(QRect Defect, int iScene);
	void signalUpdateDefectPara();
	void signalChangeProduct();
	void signalShowOkNGEnd(unsigned long long sn, enResFlag ResultFlag, string  str2dcode);
	void signalShowChoosePic(int iChoose);
	void signalShowDefectChart(gHistogram* pHistogram, int iSize, int ial);
	void signalRobotLog(const QString &message);
	void signalCtToUi(double ctTimes);
	void signalInitOtherUi();
private:
	CSendMessage(QObject *parent);
	~CSendMessage();

private:
	static CSendMessage *m_pInstance;
};

#endif
