#ifndef _IMODBUSPORT_
#define _IMODBUSPORT_

/*
    *  <------ RTUģʽÿ���ֽڣ�11λ����ʽ ����żУ��-------->
    *  +------+---+---+---+---+---+---+---+---+------+-------+
    *  | ��ʼ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | У�� |  ֹͣ | 
    *  +------+---+---+---+---+---+---+---+---+------+-------+

    *  <------ RTUģʽÿ���ֽڣ�11λ����ʽ ����żУ��-------->
    *  +------+---+---+---+---+---+---+---+---+------+-------+
    *  | ��ʼ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | ֹͣ |  ֹͣ | 
    *  +------+---+---+---+---+---+---+---+---+------+-------+

    RTU����֡��ͷ�ͽ�βҪ����3.5���ַ��ľ�ֹʱ�䣬�����ַ����ӦС��1.5���ַ�ʱ��


    *  <--- ASCIIģʽÿ���ֽڣ�10λ����ʽ ����żУ��----->
    *  +------+---+---+---+---+---+---+---+------+-------+
    *  | ��ʼ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | У�� |  ֹͣ | 
    *  +------+---+---+---+---+---+---+---+------+-------+

    *  <---- ASCIIģʽÿ���ֽڣ�10λ����ʽ ����żУ��---->
    *  +------+---+---+---+---+---+---+---+------+-------+
    *  | ��ʼ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | ֹͣ |  ֹͣ | 
    *  +------+---+---+---+---+---+---+---+------+-------+

    ASCII����֡�ַ����С��1s����

    *  <----------------------------------- MODBUS SERIAL RTU PDU -------------------------------->
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
    *  | T1 | T2 | T3 | T4 |   0   |    1    | N Byte (0<=N<=252) | N+2 | N+3 | T1 | T2 | T3 | T4 |
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
    *  |     �ȴ�ʱ��      |Address| FunCode |       Data         |    CRC    |     �ȴ�ʱ��      |
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
*/

#define MB_PDU_RTU_SIZE_MAX     ( 256 ) /*!< һ֡����ֽ��� */
#define MB_PDU_RTU_SIZE_MIN     ( 4 )   /*!< һ֡�����ֽ��� */
#define MB_PDU_RTU_ADDR_OFF     ( 0 )   /*!< ��ַƫ����     */
#define MB_PDU_RTU_FUNC_OFF     ( 1 )   /*!< ������ƫ����   */
#define MB_PDU_RTU_DATA_OFF     ( 2 )   /*!< ����ƫ����     */

/* ���֡β����ƫ����*/
#define MB_PDU_RTU_CRC_REOFF    ( 2 )   /*!< CRCֵƫ����    */ 

#define MB_PDU_RTU_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_RTU_SIZE_MIN )                /*!< �����ֽ���     */
#define MB_PDU_RTU_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_RTU_SIZE_MIN <= ( PDU_SIZE ) && ( PDU_SIZE ) <= MB_PDU_RTU_SIZE_MAX )


/*
    *  <-------------------- MODBUS SERIAL ASCII PDU ------------------->
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
    *  | 0 | 1 | 2 | 3 | 4 | N Byte (0<=N<=504) | N+5 | N+6 | N+7 | N+8 |
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
    *  | : |Address|FunCode|        Data        |    LRC    | \r  | \n  |
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
*/

#define MB_PDU_ASCII_SIZE_MAX     ( 513 ) /*!< һ֡�����ֽ��� */
#define MB_PDU_ASCII_SIZE_MIN     ( 9 )   /*!< һ֡�����ֽ��� */
#define MB_PDU_ASCII_ADDR_OFF     ( 1 )   /*!< ��ַƫ����     */
#define MB_PDU_ASCII_FUNC_OFF     ( 3 )   /*!< ������ƫ����   */
#define MB_PDU_ASCII_DATA_OFF     ( 5 )   /*!< ����ƫ����     */

/* ���֡β����ƫ����*/
#define MB_PDU_ASCII_LRC_REOFF    ( 4 )   /*!< LRC��ƫ����    */
#define MB_PDU_ASCII_CR_REOFF     ( 2 )   /*!< \r ��ƫ����    */
#define MB_PDU_ASCII_LF_REOFF     ( 1 )   /*!< \n ��ƫ����    */


#define MB_PDU_ASCII_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_ASCII_SIZE_MIN )            /*!< �����ֽ���     */
#define MB_PDU_ASCII_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_ASCII_SIZE_MIN <= ( PDU_SIZE )                \
												&& ( PDU_SIZE ) <= MB_PDU_ASCII_SIZE_MAX            \
												&& MB_PDU_ASCII_DATA_SIZE( PDU_SIZE ) % 2 == 0)

/*
    *  <------------------------ MODBUS SERIAL LINE TCP ----------------------->
    *  +---------+----------+---------+---------+---------+--------------------+
    *  | 0  | 1  | 2  |  3  | 4  | 5  |    6    |    7    | N Byte (0<=N<=252) |
    *  +---------+----------+---------+---------+---------+--------------------+
    *  | Msg��� | ʶ����� | Msg���� | Address | FunCode |        Data        |
    *  +---------+----------+---------+---------+---------+--------------------+
*/

#define MB_PDU_TCP_SIZE_MAX     ( 260 ) /*!< һ֡����ֽ��� */
#define MB_PDU_TCP_SIZE_MIN     ( 8 )   /*!< һ֡�����ֽ��� */
#define MB_PDU_TCP_ADDR_OFF     ( 6 )   /*!< ��ַƫ����     */
#define MB_PDU_TCP_FUNC_OFF     ( 7 )   /*!< ������ƫ����   */
#define MB_PDU_TCP_DATA_OFF     ( 8 )   /*!< ����ƫ����     */

#define MB_PDU_TCP_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_TCP_SIZE_MIN )            /*!< �����ֽ���     */
#define MB_PDU_TCP_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_TCP_SIZE_MIN <= ( PDU_SIZE ) && ( PDU_SIZE ) <= MB_PDU_TCP_SIZE_MAX )


#define MB_MSG_DATA_SIZE_MAX   ( 252 )
#define MB_MSG_DATA_SIZE_MIN   ( 0 )
#define MB_MSG_DATA_IN_RANGE(x)   ( MB_MSG_DATA_SIZE_MIN <= (x) && (x) <= MB_MSG_DATA_SIZE_MAX )

#define MB_ADDRESS_BROADCAST    ( 0 )   /*! �㲥��ַ     */
#define MB_ADDRESS_MIN          ( 1 )   /*! �豸��С��ַ */
#define MB_ADDRESS_MAX          ( 247 ) /*! �豸����ַ */

#define MB_SLAVE_ADDRESS(x) ( MB_ADDRESS_MIN <= (x) && (x) <= MB_ADDRESS_MAX )

// ASCII ģʽ�´��ڷ���ÿ���ֽڵĳ�ʱʱ��Ϊ 1 �� = 1000 ����
#define MB_ASCII_TIMEOUT_MS                    ( 1000 )


enum eMBMode/*: int*/				/*!< Modbus ͨ�ŵ�3�ָ�ʽ */
{
	MB_RTU,                     /*!< RTU ��ʽ       */
	MB_ASCII,                   /*!< ASCII ��ʽ     */
	MB_TCP                      /*!< TCP ��ʽ       */
} ;

enum eMBErrorCode/* : int*/
{
	MB_ENOERR = 0,              /*!< no error.		*/
	MB_EINVAL,                  /*!< ��Ч����		*/
	MB_EINVASCIIPDUSIZE,		/*!< ֡���Ȳ��ڷ�Χ */
	MB_EINVRTUPDUSIZE,			/*!< ֡���Ȳ��ڷ�Χ */
	MB_EINVTCPPDUSIZE,			/*!< ֡���Ȳ��ڷ�Χ */
	MB_EINVMSGSIZE,				/*!< ֡���Ȳ��ڷ�Χ */
	MB_EILLMODE,                /*!< ��ЧЭ��ģʽ.	*/
	MB_EINVADDR,                /*!< ��Ч�ӻ���ַ.	*/
	MB_EINVFUNCCODE,            /*!< ��Ч������.	*/
	MB_EINVMSGNO,               /*!< ��Ч��Ϣ����.	*/
	MB_EINVMSGID,               /*!< ��ЧЭ��ʶ����.*/
	MB_EINVTCPPDU,              /*!< TCP֡��ʽ����.	*/
	MB_EINVASCIIPDU,            /*!< ASCII֡��ʽ����.*/
	MB_ELRCERR,                 /*!< LRCУ�����.	*/
	MB_ECRCERR                  /*!< CRCУ�����.	*/
} ;


enum eMBParity					/*!< ��ֵ��Windows DCB.Parity ����һ�� */
{
	MB_PAR_NONE = 0,            /*!< ����ҪУ�� */
	MB_PAR_ODD  = 1,            /*!< ��У��     */
	MB_PAR_EVEN = 2             /*!< żУ��     */
};


class IModbusMsg
{
public:
	IModbusMsg(){}
	virtual ~IModbusMsg(){}

	// ���ôӻ�ID
	virtual void setSlaveId( unsigned char slaveId ) = 0;

	// ���ù�����
	virtual void setFuncCode( unsigned char funcCode ) = 0;

	// �������� pdata ָ���������֮���� IModbusMsg ���ͷţ����ǵ����� popData()
	virtual void setData( unsigned char *pdata, unsigned int cbLength ) = 0;

	// ����TCPģʽʱ����Ϣ���(no)��Э��ʶ����(id)
	virtual void setTcpModeArg( int no, int id=0 ) = 0;

	// ���شӻ�ID
	virtual unsigned char slaveId( void ) = 0;

	// ���ع�����
	virtual unsigned char funcCode( void ) = 0;

	// ��������
	virtual const unsigned char *data( void ) = 0;

	// ��������,֮�������ɽ��շ��ͷ�
	virtual unsigned char *popData( void ) = 0;

	// �������ݵĳ���
	virtual unsigned int dataLength( void ) = 0;

	// ����TCPģʽʱ����Ϣ���(no)��Э��ʶ����(id)
	virtual void tcpModeArg( int &no, int &id ) = 0;

	// ��� IModbusMsg ����������Ƿ���Ч
	virtual eMBErrorCode available( bool isTcpMode = false ) = 0;

	// ���� IModbusMsg ����ת����ĳ��ģʽ��Ҫ�Ļ�������С
	virtual unsigned int frameBufferSize( eMBMode mode ) = 0;

	// �������óɹ��򷵻� MB_ENOERR
	// mode   ֻ���� MB_RTU �� MB_ASCII �� MB_TCP ����һ��
	// pPdu   ��� Э�鵥Ԫָ��		 
	// pduLen ��� Э�鵥Ԫ��С		   
	virtual eMBErrorCode fromPdu( eMBMode mode, unsigned char *pPdu, unsigned int pduLen) = 0;
	virtual eMBErrorCode toPdu( eMBMode mode, unsigned char *pPdu, unsigned int &pduLen) = 0;
};
#endif