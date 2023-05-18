#ifndef _MODBUS_PORT_
#define _MODBUS_PORT_

#include "sdkcommon/modbusport/imodbusport.h"


#ifndef NULL
#define NULL 0
#endif


class CModbusMsg:public IModbusMsg
{

public:
	CModbusMsg();
	CModbusMsg(const CModbusMsg &other);
	virtual ~CModbusMsg();

	void setSlaveId( unsigned char slaveId );

	void setFuncCode( unsigned char funcCode );

	void setData( unsigned char *pdata, unsigned int cbLength );

	void setTcpModeArg( int no, int id=0 );

	unsigned char slaveId( void );

	unsigned char funcCode( void );

	const unsigned char *data( void );

	unsigned char *popData( void );

	unsigned int dataLength( void );

	void tcpModeArg( int &no, int &id );

	eMBErrorCode available( bool isTcpMode = false );

	unsigned int frameBufferSize( eMBMode mode );

	eMBErrorCode fromPdu( eMBMode mode, unsigned char *pPdu, unsigned int pduLen);

	eMBErrorCode toPdu( eMBMode mode, unsigned char *pPdu, unsigned int &pduLen);

private:
	unsigned char m_SlaveId;		// �ӻ�ID
	unsigned char m_FuncCode;	   	// ���ܱ���
	unsigned char m_MsgNo[2];		// TCPģʽ����Ϣ����		 m_MsgNo[0] ��λ  m_MsgNo[1] ��λ
	unsigned char m_MsgId[2];		// TCPģʽ��Э��ʶ����
	unsigned char *m_pData;	   		// ����������  
	unsigned int  m_cbLength;	   	// �������ݵĴ�С�����ֽ���


private:	// ��صĸ�������

	// Ҫ���͵�����ת��ΪMODBUSЭ���ASCII��ʽ
	eMBErrorCode ToAsciiPdu( unsigned char *pPdu, unsigned int &pduLen );

	// Ҫ���͵�����ת��ΪMODBUSЭ���RTU��ʽ
	eMBErrorCode ToRtuPdu( unsigned char *pPdu, unsigned int &pduLen );

	// Ҫ���͵�����ת��ΪMODBUSЭ���TCP��ʽ
	eMBErrorCode ToTcpPdu( unsigned char *pPdu, unsigned int &pduLen );


	// �������յ�MODBUSЭ���ASCII��ʽ����ת��
	eMBErrorCode FromAsciiPdu( unsigned char *pPdu, unsigned int pduLen );

	// �������յ�MODBUSЭ���RTU��ʽ����ת��
	eMBErrorCode FromRtuPdu( unsigned char *pPdu, unsigned int pduLen );

	// �������յ�MODBUSЭ���TCP��ʽ����ת��
	eMBErrorCode FromTcpPdu( unsigned char *pPdu, unsigned int pduLen );

	// �ַ�ת������
	unsigned char Char2Binary( char character );

	// ������ת�ַ�
	char Binary2Char( unsigned char binary );

	// LRCУ��
	static unsigned char LRC( unsigned char *pPdu, unsigned short len );

	// CRC16����У��
	static unsigned short CRC16( unsigned char *pPdu, unsigned short len );
};

#endif
