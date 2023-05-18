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
	unsigned char m_SlaveId;		// 从机ID
	unsigned char m_FuncCode;	   	// 功能编码
	unsigned char m_MsgNo[2];		// TCP模式下消息编码		 m_MsgNo[0] 高位  m_MsgNo[1] 低位
	unsigned char m_MsgId[2];		// TCP模式下协议识别码
	unsigned char *m_pData;	   		// 附带的数据  
	unsigned int  m_cbLength;	   	// 附带数据的大小，按字节算


private:	// 相关的辅助函数

	// 要发送的数据转换为MODBUS协议的ASCII格式
	eMBErrorCode ToAsciiPdu( unsigned char *pPdu, unsigned int &pduLen );

	// 要发送的数据转换为MODBUS协议的RTU格式
	eMBErrorCode ToRtuPdu( unsigned char *pPdu, unsigned int &pduLen );

	// 要发送的数据转换为MODBUS协议的TCP格式
	eMBErrorCode ToTcpPdu( unsigned char *pPdu, unsigned int &pduLen );


	// 解析接收的MODBUS协议的ASCII格式数据转换
	eMBErrorCode FromAsciiPdu( unsigned char *pPdu, unsigned int pduLen );

	// 解析接收的MODBUS协议的RTU格式数据转换
	eMBErrorCode FromRtuPdu( unsigned char *pPdu, unsigned int pduLen );

	// 解析接收的MODBUS协议的TCP格式数据转换
	eMBErrorCode FromTcpPdu( unsigned char *pPdu, unsigned int pduLen );

	// 字符转二进制
	unsigned char Char2Binary( char character );

	// 二进制转字符
	char Binary2Char( unsigned char binary );

	// LRC校验
	static unsigned char LRC( unsigned char *pPdu, unsigned short len );

	// CRC16冗余校验
	static unsigned short CRC16( unsigned char *pPdu, unsigned short len );
};

#endif
