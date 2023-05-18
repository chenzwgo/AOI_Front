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
void CSendMessage::SendLoginModelChange(int iModel)
{
	emit signalLoginModelChange(iModel);
}
