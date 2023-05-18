#include "sendmessage.h"

CSendMessage *CSendMessage::m_pInstance = NULL;

CSendMessage::CSendMessage(QObject *parent)
	: QObject(parent)
{
}

CSendMessage::~CSendMessage()
{

}

void CSendMessage::Send( const QString &message, UILogLevel type )
{	
	emit signalMessage(message, type);
}

CSendMessage * CSendMessage::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CSendMessage(NULL);		
	}

	return m_pInstance;
}

void CSendMessage::ReleaseInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void CSendMessage::SendLoginModelChange( int iModel )
{
	emit signalLoginModelChange(iModel);
}

void CSendMessage::sendPressUI( int index )
{
    emit signalPressUi(index);
}

void CSendMessage::sendPressTabView( int index )
{
    emit signalPressTabView(index);
}

void CSendMessage::sendNewDefet( double width,double height )
{
    emit signalNewDefet(width,height);
}

void CSendMessage::sendAddsetdecet( MarkStrut *mark )
{
    emit signalAddsetdecet(mark);
}

void CSendMessage::sendAddVecsetdecet( QVector<MarkStrut*>* vecmark )
{
    emit signalAddVecsetdecet(vecmark);
}

void CSendMessage::sendUpdateUiStus( QString str,int iType )
{
    emit signalUpdateUiStus(str,iType);
}

void CSendMessage::sendAddMapVecsetdecet( QMap<QString, QVector<MarkStrut *> *> * mapMark ,int iScene)
{
    emit signalAddMapVecsetdecet(mapMark,iScene);
}

void CSendMessage::sendAddMapVecsetUiTabledecet(QMap<QString, QVector<MarkStrut *> *> * mapMark)
{
	emit signalAddMapVecsetUiTabledecet(mapMark);
}

void CSendMessage::sendProductInfo(CImageProduct* pProduct)
{
	emit signalProductInfo(pProduct);
}

void CSendMessage::sendOneDefecTable(stDefectFeature Defect)
{
	emit signalOneDefecTable(Defect);
}

void CSendMessage::sendImageDefecTable(ImageSrcStrut* Image, int istation)
{
	emit signalImageDefecTable(Image, istation);
}

void CSendMessage::sendOneDefecUI(stDefectFeature Defect, int iScene)
{
	emit signalOneDefecUI(Defect, iScene);
}

void CSendMessage::sendAddCutROIUI(QRect Defect, int iScene)
{
    emit signalAddCutROIUI(Defect, iScene);
}


void CSendMessage::sendChangeProduct()
{
	emit signalChangeProduct();
}

void CSendMessage::sendShowOkNGEnd(unsigned long long sn, enResFlag ResultFlag, string  str2dcode)
{
	emit signalShowOkNGEnd(sn, ResultFlag,str2dcode);
}

void CSendMessage::sendShowChoosePic(int iChoose)
{
	emit signalShowChoosePic(iChoose);
}

void CSendMessage::sendChooseShow( int *icheck ,int iScene)
{
    emit signalChooseShow(icheck,iScene);
}

void CSendMessage::sendResUpdateUi(bool bOk /*= true*/)
{
	emit  signalResUpdateUi(bOk);
}
void CSendMessage::sendClearTable()
{
	emit  signalClearTable();
}
void CSendMessage::sendShowDefectChart(gHistogram* pHistogram, int iSize,int ial)
{
	emit  signalShowDefectChart( pHistogram,iSize,ial);
}

void CSendMessage::sendCtToUi(double ctTimes)
{
	emit  signalCtToUi(ctTimes);
}
void CSendMessage::sendRobotLog(const QString &message)
{
	emit  signalRobotLog(message);
}
void CSendMessage::sendInitOtherUi()
{
	emit  signalInitOtherUi();
}
