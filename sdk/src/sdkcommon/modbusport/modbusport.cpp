#include "modbusport.h"


typedef eMBErrorCode (*fnModbusMsg2Frame) ( unsigned char *, unsigned int &, CModbusMsg & );
typedef eMBErrorCode (*fnModbusFrame2Msg) ( unsigned char *, unsigned int, CModbusMsg & );


CModbusMsg::CModbusMsg():
	m_SlaveId(255),
	m_FuncCode(255),
	m_pData(0),
	m_cbLength(0)
{
	m_MsgNo[0] = 0;
	m_MsgNo[1] = 0;
	m_MsgId[0] = 0;
	m_MsgId[1] = 0;
}

 CModbusMsg::~CModbusMsg()
 {
	  if ( m_pData != 0 )
		  delete[] m_pData;
 }

CModbusMsg::CModbusMsg(const CModbusMsg &other):
	m_SlaveId(other.m_SlaveId),
	m_FuncCode(other.m_FuncCode),
	m_pData(0),
	m_cbLength(other.m_cbLength)
{
	m_MsgNo[0] = other.m_MsgNo[0];
	m_MsgNo[1] = other.m_MsgNo[1];
	m_MsgId[0] = other.m_MsgId[0];
	m_MsgId[1] = other.m_MsgId[1];

	if ( other.m_cbLength != 0 )
	{
		m_pData = new unsigned char[m_cbLength];
		for (unsigned int i=0; i<m_cbLength; i++)
		{
			m_pData[i] = other.m_pData[i];
		}
	}
}

void CModbusMsg::setSlaveId( unsigned char slaveId ) 
{
	m_SlaveId = slaveId;
}

void CModbusMsg::setFuncCode( unsigned char funcCode )
{
	m_FuncCode = funcCode;
}

void CModbusMsg::setData( unsigned char *pdata, unsigned int cbLength )
{
	if ( m_pData != NULL )
	{
		delete[] m_pData;
		m_pData    = 0;
		m_cbLength = 0;
	}

	if ( pdata != NULL )
	{
		m_pData    = pdata;
		m_cbLength = cbLength;
	}

}

void CModbusMsg::setTcpModeArg( int no, int id )
{
	m_MsgNo[0] = (no >> 8 )& 0x000000FF;
	m_MsgNo[1] = no & 0x000000FF;
	m_MsgId[0] = (id >> 8 )& 0x000000FF;
	m_MsgId[1] = id & 0x000000FF;
}

unsigned char CModbusMsg::slaveId( void )
{
	return m_SlaveId;
}

unsigned char CModbusMsg::funcCode( void )
{
	return m_FuncCode;
}

const unsigned char *CModbusMsg::data( void )
{
	return m_pData;
}

unsigned char *CModbusMsg::popData( void )
{
	unsigned char *temp = m_pData;
	if ( m_pData != NULL )
	{
		delete[] m_pData;
		m_cbLength = 0;
	}
	return temp;
}

unsigned int CModbusMsg::dataLength( void )
{
	return m_cbLength;
}

void CModbusMsg::tcpModeArg( int &no, int &id )
{
	no =  m_MsgNo[0] << 8;
	no += m_MsgNo[1];
	id =  m_MsgId[0] << 8;
	id += m_MsgId[1];
}

eMBErrorCode CModbusMsg::available( bool isTcpMode )
{
	if ( !( MB_SLAVE_ADDRESS( m_SlaveId ) || MB_ADDRESS_BROADCAST == m_SlaveId ) )
	{
		return MB_EINVADDR;
	}
	else if ( 255 == m_FuncCode )
	{
		return MB_EINVFUNCCODE;
	}
	else if ( !MB_MSG_DATA_IN_RANGE( m_cbLength ) )
	{
		return MB_EINVMSGSIZE;
	}

	if ( isTcpMode )
	{
		if ( 255 == m_MsgNo[0] && 255 == m_MsgNo[1] )
		{
			return MB_EINVMSGNO;
		}
		else if ( 255 == m_MsgId[0] && 255 == m_MsgId[1] )
		{
			return MB_EINVMSGID;
		}
	}

	return MB_ENOERR;
}

unsigned int CModbusMsg::frameBufferSize( eMBMode mode )
{
	if ( MB_ASCII == mode )
		return ( m_cbLength*2 + MB_PDU_ASCII_SIZE_MIN );
	else if ( MB_RTU == mode )
		return ( m_cbLength + MB_PDU_RTU_SIZE_MIN );
	else if ( MB_TCP == mode )
		return ( m_cbLength + MB_PDU_TCP_SIZE_MIN );

	return 0;
}

eMBErrorCode CModbusMsg::fromPdu( eMBMode mode, unsigned char *pPdu, unsigned int pduLen)
{
	eMBErrorCode eRes;
	switch ( mode )
	{
	case MB_ASCII:
		eRes = FromAsciiPdu( pPdu, pduLen );
		break;
	case  MB_RTU:
		eRes = FromRtuPdu( pPdu, pduLen );
		break;
	case MB_TCP:
		eRes = FromTcpPdu( pPdu, pduLen );
		break;
	default:
		// 不支持该模式
		eRes = MB_EILLMODE;
		break;
	}

	return eRes;
}

eMBErrorCode CModbusMsg::toPdu( eMBMode mode, unsigned char *pPdu, unsigned int &pduLen)
{
	eMBErrorCode eRes;

	switch ( mode )
	{
	case MB_ASCII:
		eRes = ToAsciiPdu( pPdu, pduLen );
		break;
	case  MB_RTU:
		eRes = ToRtuPdu( pPdu, pduLen );
		break;
	case MB_TCP:
		eRes = ToTcpPdu( pPdu, pduLen );
		break;
	default:
		// 不支持该模式
		eRes = MB_EILLMODE;
		break;
	}

	return eRes;
}

eMBErrorCode CModbusMsg::ToAsciiPdu( unsigned char *pPdu, unsigned int &pduLen )
{
    if ( NULL == pPdu )
        return MB_EINVAL;

	if ( available() != MB_ENOERR )
		return available();

	if ( pduLen < dataLength()*2 + MB_PDU_ASCII_SIZE_MIN )
        return MB_EINVASCIIPDUSIZE;

	pduLen = dataLength()*2 + MB_PDU_ASCII_SIZE_MIN;

    unsigned char msgBuffer[255];                               // 1 byte(地址) + 1 byte(功能码) + MB_PDU_DATA_SIZE_MAX byte(252 byte 数据) + 1 byte(LRC校验码)
    unsigned int  msgSizeCur = dataLength() + 3;                // 1 byte(地址) + 1 byte(功能码) + dataLen byte(数据) + 1 byte(LRC校验码)

    msgBuffer[0] = slaveId();                                   // 填充地址
    msgBuffer[1] = funcCode();                                  // 填充功能号
    for (unsigned int i=0; i<dataLength(); ++i)                 // 填充数据
	{
        msgBuffer[i + 2] = data()[i];
	}
    msgBuffer[dataLength()+2] = LRC( msgBuffer, dataLength()+2 );// LRC校验

    pPdu[0] = ':';                                              // 起始字符
    for (unsigned int i=0; i<msgSizeCur; ++i)                   // 填充数据
    {
        pPdu[i*2+1] = Binary2Char( msgBuffer[i] >> 4);          // 数据高位
        pPdu[i*2+2] = Binary2Char( msgBuffer[i] & 0x0F);        // 数据低位
    }
    pPdu[pduLen-MB_PDU_ASCII_CR_REOFF] = '\r';				    // 结束符 回车
    pPdu[pduLen-MB_PDU_ASCII_LF_REOFF] = '\n';				    // 结束符 换行

    return MB_ENOERR;
}

eMBErrorCode CModbusMsg::ToRtuPdu( unsigned char *pPdu, unsigned int &pduLen )
{
	if ( NULL == pPdu )
		return MB_EINVAL;

	if ( available() != MB_ENOERR )
		return available();

	if ( pduLen < dataLength() + MB_PDU_RTU_SIZE_MIN )
		return MB_EINVRTUPDUSIZE;

	pduLen = dataLength() + MB_PDU_RTU_SIZE_MIN;

    pPdu[0] = slaveId();                           // 填入从机地址
    pPdu[1] = funcCode();                          // 填入功能编码
    for (unsigned int i=0; i<dataLength(); ++i)    // 填入数据
    {
        pPdu[i+2] = data()[i];
    }
    unsigned short usCRC16 = CRC16( pPdu,  pduLen-2 );
    pPdu[pduLen-MB_PDU_RTU_CRC_REOFF  ] = ( unsigned char )( usCRC16 & 0xFF );
    pPdu[pduLen-MB_PDU_RTU_CRC_REOFF+1] = ( unsigned char )( usCRC16 >> 8 );

    return MB_ENOERR;
}

eMBErrorCode CModbusMsg::ToTcpPdu( unsigned char *pPdu, unsigned int &pduLen )
{
	if ( NULL == pPdu )
		return MB_EINVAL;

	if ( available(true) != MB_ENOERR )
		return available();

	if ( pduLen < dataLength() + MB_PDU_TCP_SIZE_MIN )
		return MB_EINVTCPPDUSIZE;

	pduLen = dataLength() + MB_PDU_TCP_SIZE_MIN;

	int tcpModeNo = 0;
	int tcpModeId = 0;
	tcpModeArg( tcpModeNo, tcpModeId );

    pPdu[0] = m_MsgNo[0];
    pPdu[1] = m_MsgNo[1];

    pPdu[2] = m_MsgId[0];
    pPdu[3] = m_MsgId[1];
    
    pPdu[4] = (dataLength()+2) >> 8;
    pPdu[5] = (dataLength()+2) & 0xFF;

    pPdu[6] = slaveId();
    pPdu[7] = funcCode();

    for (unsigned int i=0; i<dataLength(); ++i)        // 填入数据
    {
        pPdu[MB_PDU_TCP_DATA_OFF + i] = data()[i];
    }

    return MB_ENOERR;
}

eMBErrorCode CModbusMsg::FromAsciiPdu( unsigned char *pPdu, unsigned int pduLen )
{
	if ( NULL == pPdu )
		return MB_EINVAL;

	if ( !MB_PDU_ASCII_SIZE_IN_RANGE(pduLen) )
		return MB_EINVASCIIPDUSIZE;

    // 待解析消息格式判断
    if ( pPdu[0] != ':' 
        && pPdu[pduLen-MB_PDU_ASCII_CR_REOFF] != '\r'
        && pPdu[pduLen-MB_PDU_ASCII_LF_REOFF] != '\n' ) 
        return MB_EINVASCIIPDU;

    unsigned char msgBuffer[255];                          // 1 byte(地址) + 1 byte(功能码) + MB_PDU_DATA_SIZE_MAX byte(数据) + 1 byte(LRC校验码)
    unsigned int  msgSizeCur = ( pduLen - 3 ) / 2;         // 实际消息大小 - 头尾标示符(':' '\r' '\n') 由于1个字节被拆成2个字符，故接收时缓冲区要除2
    for (unsigned int i=0; i<msgSizeCur; ++i)
    {
        msgBuffer[i] =  Char2Binary(pPdu[MB_PDU_ASCII_ADDR_OFF + i*2]) << 4;
        msgBuffer[i] |= Char2Binary(pPdu[MB_PDU_ASCII_ADDR_OFF + i*2+1]);
    }

    if ( LRC( msgBuffer, msgSizeCur ) != 0 ) // LRC校验
    {
        return MB_ELRCERR;
    }

    setSlaveId( msgBuffer[0] );
    setFuncCode( msgBuffer[1] );

    unsigned int dataLen = MB_PDU_ASCII_DATA_SIZE(pduLen) / 2;
	unsigned char *pdata = new unsigned char[dataLen];
    for (unsigned int i=0; i<dataLen; ++i)        // 数据的大小 = uiMsgSizeCur - 1 byte(地址) - 1 byte(功能码) - 1 byte(LRC校验码)
    {
        pdata[i] =  msgBuffer[2+i];
    }
    
	setData( pdata, dataLen );
    return MB_ENOERR;
}

eMBErrorCode CModbusMsg::FromRtuPdu( unsigned char *pPdu, unsigned int pduLen )
{
	if ( NULL == pPdu )
		return MB_EINVAL;

	if ( !MB_PDU_RTU_SIZE_IN_RANGE(pduLen) )
		return MB_EINVRTUPDUSIZE;

    unsigned short usCRC  = 0;
    unsigned short usTemp = 0;

    usCRC  = CRC16( pPdu, pduLen-2 );                    // CRC校验
    usTemp =  pPdu[pduLen -MB_PDU_RTU_CRC_REOFF  ];
    usTemp |= pPdu[pduLen -MB_PDU_RTU_CRC_REOFF+1] << 8;

    if ( usCRC != usTemp ) 
    {    // CRC校验 结果不对
        return MB_ECRCERR;
    }

	setSlaveId( pPdu[MB_PDU_RTU_ADDR_OFF] );
	setFuncCode( pPdu[MB_PDU_RTU_FUNC_OFF] );
    
    unsigned int dataLen  = MB_PDU_RTU_DATA_SIZE(pduLen);
	unsigned char *pdata = NULL;
    if ( dataLen != NULL )
    {
        pdata = new unsigned char[dataLen];
        for (unsigned int i=0; i<dataLen; ++i)
        {
            pdata[i] = pPdu[MB_PDU_RTU_DATA_OFF + i];
        }
    }

	setData( pdata, dataLen );
    return MB_ENOERR;
}

eMBErrorCode CModbusMsg::FromTcpPdu( unsigned char *pPdu, unsigned int pduLen )
{
	if ( NULL == pPdu )
		return MB_EINVAL;

	if ( !MB_PDU_TCP_SIZE_IN_RANGE(pduLen) )
		return MB_EINVTCPPDUSIZE;

    unsigned int msgLen = 0;
    msgLen = pPdu[4] << 8;
    msgLen |= pPdu[5];

    if ( msgLen != pduLen -6 )
    {    // 帧的格式错误
        return MB_EINVTCPPDU;
    }

	m_MsgNo[0] = pPdu[0];
    m_MsgNo[1] = pPdu[1];

	m_MsgId[0] = pPdu[2];
	m_MsgId[1] = pPdu[3];
	
	setSlaveId( pPdu[MB_PDU_TCP_ADDR_OFF] );
	setFuncCode( pPdu[MB_PDU_TCP_FUNC_OFF] );

    unsigned int dataLen = msgLen - 2;
	unsigned char *pdata = 0;
    if ( dataLen != 0 )
    {
        pdata = new unsigned char[dataLen];
        for (unsigned int i=0; i<dataLen; ++i)
        {
            pdata[i] = pPdu[MB_PDU_TCP_DATA_OFF + i];
        }
    }

	setData( pdata, dataLen );

    return MB_ENOERR;
}


unsigned char CModbusMsg::Char2Binary( char character )
{
	if( ( character >= '0' ) && ( character <= '9' ) )
	{
		return ( unsigned char )( character - '0' );
	}
	else if( ( character >= 'A' ) && ( character <= 'F' ) )
	{
		return ( unsigned char )( character - 'A' + 0x0A );
	}
	else if( ( character >= 'a' ) && ( character <= 'f' ) )
	{
		return ( unsigned char )( character - 'a' + 0x0A );
	}
	return 0xFF;
}

char CModbusMsg::Binary2Char( unsigned char binary )
{
	if( binary <= 0x09 )
	{
		return '0' + binary;
	}
	else if( ( binary >= 0x0A ) && ( binary <= 0x0F ) )
	{
		return binary - 0x0A + 'A';
	}
	return '?';
}

unsigned char CModbusMsg::LRC( unsigned char * pframe, unsigned short len )
{
    unsigned char ucLRC = 0;  /* LRC char initialized */

    while( len-- )
    {
        ucLRC += *pframe++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( unsigned char ) ( -( ( char ) ucLRC ) );
    return ucLRC;
}

static const unsigned char aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

static const unsigned char aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

unsigned short CModbusMsg::CRC16( unsigned char * pframe, unsigned short len )
{
    unsigned char  ucCRCHi = 0xFF;
    unsigned char  ucCRCLo = 0xFF;
    int            iIndex;

    while( len-- )
    {
        iIndex = ucCRCLo ^ *( pframe++ );
        ucCRCLo = ( unsigned char )( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( unsigned short )( ucCRCHi << 8 | ucCRCLo );
}