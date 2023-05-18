#ifndef SNED_MESSAGE_H
#define SNED_MESSAGE_H

#include <QObject>
#include <QString>
#include "uidefine.h"

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

signals:
	void signalMessage(const QString &, int);
	void signalLoginModelChange(int);

private:
	CSendMessage(QObject *parent);
	~CSendMessage();

private:
	static CSendMessage *m_pInstance;
};

#endif
