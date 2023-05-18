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

	// ����log,���ⲿ����
    void Send( const QString &message, UILogLevel type = UILOG_INFO  );
    void SendLoginModelChange( int iModel );
    void sendPressUI(int index);
    void sendPressTabView(int index);
    void sendNewDefet( double width,double height );
    void sendAddsetdecet(MarkStrut *mark);
    void sendAddVecsetdecet( QVector<MarkStrut*>* vecmark );
    void sendUpdateUiStus( QString str,int iType );
	//�������ʾ��ȱ���б�//�ڸ���ȱ��ǰ�������һ��//
	void sendClearTable();
	//iScene ��0��ʼ��0Ϊ�������1�������
    void sendAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> * mapMark ,int iScene );
	//��������//
	void sendAddMapVecsetUiTabledecet(QMap<QString, QVector<MarkStrut *> *> * mapMark);
    void sendChooseShow(int *icheck ,int iScene);
	void sendResUpdateUi(bool bOk = true);
	//����ȱ���������ݵ�����
	void sendProductInfo(CImageProduct* pProduct);
	//���͵������ݵ��б������ʾ
	void sendOneDefecTable(stDefectFeature Defect);
	//���͵���ͨ��ͼ����Ϣ���б����
	void sendImageDefecTable(ImageSrcStrut* Image,int istation=0);
	//���͵������ݵ�������ʾ  //iScene 0  Ϊ�������1 Ϊ�����
    void sendOneDefecUI(stDefectFeature Defect, int iScene);
    void sendAddCutROIUI(QRect Defect, int iScene);

	//�л���Ʒ
	void sendChangeProduct();
	//������ʾOKNG
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
