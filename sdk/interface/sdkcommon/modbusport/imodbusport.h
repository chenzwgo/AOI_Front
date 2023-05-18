#ifndef _IMODBUSPORT_
#define _IMODBUSPORT_

/*
    *  <------ RTU模式每个字节（11位）格式 有奇偶校验-------->
    *  +------+---+---+---+---+---+---+---+---+------+-------+
    *  | 起始 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 校验 |  停止 | 
    *  +------+---+---+---+---+---+---+---+---+------+-------+

    *  <------ RTU模式每个字节（11位）格式 无奇偶校验-------->
    *  +------+---+---+---+---+---+---+---+---+------+-------+
    *  | 起始 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 停止 |  停止 | 
    *  +------+---+---+---+---+---+---+---+---+------+-------+

    RTU报文帧开头和结尾要包含3.5个字符的静止时间，两个字符间隔应小于1.5个字符时间


    *  <--- ASCII模式每个字节（10位）格式 有奇偶校验----->
    *  +------+---+---+---+---+---+---+---+------+-------+
    *  | 起始 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 校验 |  停止 | 
    *  +------+---+---+---+---+---+---+---+------+-------+

    *  <---- ASCII模式每个字节（10位）格式 无奇偶校验---->
    *  +------+---+---+---+---+---+---+---+------+-------+
    *  | 起始 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 停止 |  停止 | 
    *  +------+---+---+---+---+---+---+---+------+-------+

    ASCII报文帧字符间隔小于1s即可

    *  <----------------------------------- MODBUS SERIAL RTU PDU -------------------------------->
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
    *  | T1 | T2 | T3 | T4 |   0   |    1    | N Byte (0<=N<=252) | N+2 | N+3 | T1 | T2 | T3 | T4 |
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
    *  |     等待时间      |Address| FunCode |       Data         |    CRC    |     等待时间      |
    *  +----+----+----+----+-------+---------+--------------------+-----+-----+----+----+----+----+
*/

#define MB_PDU_RTU_SIZE_MAX     ( 256 ) /*!< 一帧最多字节数 */
#define MB_PDU_RTU_SIZE_MIN     ( 4 )   /*!< 一帧最少字节数 */
#define MB_PDU_RTU_ADDR_OFF     ( 0 )   /*!< 地址偏移量     */
#define MB_PDU_RTU_FUNC_OFF     ( 1 )   /*!< 功能码偏移量   */
#define MB_PDU_RTU_DATA_OFF     ( 2 )   /*!< 数据偏移量     */

/* 相对帧尾部的偏移量*/
#define MB_PDU_RTU_CRC_REOFF    ( 2 )   /*!< CRC值偏移量    */ 

#define MB_PDU_RTU_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_RTU_SIZE_MIN )                /*!< 数据字节数     */
#define MB_PDU_RTU_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_RTU_SIZE_MIN <= ( PDU_SIZE ) && ( PDU_SIZE ) <= MB_PDU_RTU_SIZE_MAX )


/*
    *  <-------------------- MODBUS SERIAL ASCII PDU ------------------->
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
    *  | 0 | 1 | 2 | 3 | 4 | N Byte (0<=N<=504) | N+5 | N+6 | N+7 | N+8 |
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
    *  | : |Address|FunCode|        Data        |    LRC    | \r  | \n  |
    *  +---+---+---+-------+--------------------+-----+-----+-----+-----+
*/

#define MB_PDU_ASCII_SIZE_MAX     ( 513 ) /*!< 一帧最少字节数 */
#define MB_PDU_ASCII_SIZE_MIN     ( 9 )   /*!< 一帧最少字节数 */
#define MB_PDU_ASCII_ADDR_OFF     ( 1 )   /*!< 地址偏移量     */
#define MB_PDU_ASCII_FUNC_OFF     ( 3 )   /*!< 功能码偏移量   */
#define MB_PDU_ASCII_DATA_OFF     ( 5 )   /*!< 数据偏移量     */

/* 相对帧尾部的偏移量*/
#define MB_PDU_ASCII_LRC_REOFF    ( 4 )   /*!< LRC的偏移量    */
#define MB_PDU_ASCII_CR_REOFF     ( 2 )   /*!< \r 的偏移量    */
#define MB_PDU_ASCII_LF_REOFF     ( 1 )   /*!< \n 的偏移量    */


#define MB_PDU_ASCII_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_ASCII_SIZE_MIN )            /*!< 数据字节数     */
#define MB_PDU_ASCII_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_ASCII_SIZE_MIN <= ( PDU_SIZE )                \
												&& ( PDU_SIZE ) <= MB_PDU_ASCII_SIZE_MAX            \
												&& MB_PDU_ASCII_DATA_SIZE( PDU_SIZE ) % 2 == 0)

/*
    *  <------------------------ MODBUS SERIAL LINE TCP ----------------------->
    *  +---------+----------+---------+---------+---------+--------------------+
    *  | 0  | 1  | 2  |  3  | 4  | 5  |    6    |    7    | N Byte (0<=N<=252) |
    *  +---------+----------+---------+---------+---------+--------------------+
    *  | Msg编号 | 识别号码 | Msg长度 | Address | FunCode |        Data        |
    *  +---------+----------+---------+---------+---------+--------------------+
*/

#define MB_PDU_TCP_SIZE_MAX     ( 260 ) /*!< 一帧最多字节数 */
#define MB_PDU_TCP_SIZE_MIN     ( 8 )   /*!< 一帧最少字节数 */
#define MB_PDU_TCP_ADDR_OFF     ( 6 )   /*!< 地址偏移量     */
#define MB_PDU_TCP_FUNC_OFF     ( 7 )   /*!< 功能码偏移量   */
#define MB_PDU_TCP_DATA_OFF     ( 8 )   /*!< 数据偏移量     */

#define MB_PDU_TCP_DATA_SIZE(PDU_SIZE)     ( ( PDU_SIZE ) - MB_PDU_TCP_SIZE_MIN )            /*!< 数据字节数     */
#define MB_PDU_TCP_SIZE_IN_RANGE(PDU_SIZE) ( MB_PDU_TCP_SIZE_MIN <= ( PDU_SIZE ) && ( PDU_SIZE ) <= MB_PDU_TCP_SIZE_MAX )


#define MB_MSG_DATA_SIZE_MAX   ( 252 )
#define MB_MSG_DATA_SIZE_MIN   ( 0 )
#define MB_MSG_DATA_IN_RANGE(x)   ( MB_MSG_DATA_SIZE_MIN <= (x) && (x) <= MB_MSG_DATA_SIZE_MAX )

#define MB_ADDRESS_BROADCAST    ( 0 )   /*! 广播地址     */
#define MB_ADDRESS_MIN          ( 1 )   /*! 设备最小地址 */
#define MB_ADDRESS_MAX          ( 247 ) /*! 设备最大地址 */

#define MB_SLAVE_ADDRESS(x) ( MB_ADDRESS_MIN <= (x) && (x) <= MB_ADDRESS_MAX )

// ASCII 模式下串口发送每个字节的超时时间为 1 秒 = 1000 毫秒
#define MB_ASCII_TIMEOUT_MS                    ( 1000 )


enum eMBMode/*: int*/				/*!< Modbus 通信的3种格式 */
{
	MB_RTU,                     /*!< RTU 格式       */
	MB_ASCII,                   /*!< ASCII 格式     */
	MB_TCP                      /*!< TCP 格式       */
} ;

enum eMBErrorCode/* : int*/
{
	MB_ENOERR = 0,              /*!< no error.		*/
	MB_EINVAL,                  /*!< 无效参数		*/
	MB_EINVASCIIPDUSIZE,		/*!< 帧长度不在范围 */
	MB_EINVRTUPDUSIZE,			/*!< 帧长度不在范围 */
	MB_EINVTCPPDUSIZE,			/*!< 帧长度不在范围 */
	MB_EINVMSGSIZE,				/*!< 帧长度不在范围 */
	MB_EILLMODE,                /*!< 无效协议模式.	*/
	MB_EINVADDR,                /*!< 无效从机地址.	*/
	MB_EINVFUNCCODE,            /*!< 无效功能码.	*/
	MB_EINVMSGNO,               /*!< 无效消息编码.	*/
	MB_EINVMSGID,               /*!< 无效协议识别码.*/
	MB_EINVTCPPDU,              /*!< TCP帧格式出错.	*/
	MB_EINVASCIIPDU,            /*!< ASCII帧格式出错.*/
	MB_ELRCERR,                 /*!< LRC校验出错.	*/
	MB_ECRCERR                  /*!< CRC校验出错.	*/
} ;


enum eMBParity					/*!< 数值与Windows DCB.Parity 参数一样 */
{
	MB_PAR_NONE = 0,            /*!< 不需要校验 */
	MB_PAR_ODD  = 1,            /*!< 奇校验     */
	MB_PAR_EVEN = 2             /*!< 偶校验     */
};


class IModbusMsg
{
public:
	IModbusMsg(){}
	virtual ~IModbusMsg(){}

	// 设置从机ID
	virtual void setSlaveId( unsigned char slaveId ) = 0;

	// 设置功能码
	virtual void setFuncCode( unsigned char funcCode ) = 0;

	// 填入数据 pdata 指向的内容在之后由 IModbusMsg 来释放，除非调用了 popData()
	virtual void setData( unsigned char *pdata, unsigned int cbLength ) = 0;

	// 设置TCP模式时的消息编号(no)和协议识别码(id)
	virtual void setTcpModeArg( int no, int id=0 ) = 0;

	// 返回从机ID
	virtual unsigned char slaveId( void ) = 0;

	// 返回功能码
	virtual unsigned char funcCode( void ) = 0;

	// 返回数据
	virtual const unsigned char *data( void ) = 0;

	// 弹出数据,之后数据由接收方释放
	virtual unsigned char *popData( void ) = 0;

	// 返回数据的长度
	virtual unsigned int dataLength( void ) = 0;

	// 返回TCP模式时的消息编号(no)和协议识别码(id)
	virtual void tcpModeArg( int &no, int &id ) = 0;

	// 检查 IModbusMsg 里面的数据是否有效
	virtual eMBErrorCode available( bool isTcpMode = false ) = 0;

	// 返回 IModbusMsg 数据转换成某种模式需要的缓冲区大小
	virtual unsigned int frameBufferSize( eMBMode mode ) = 0;

	// 函数调用成功则返回 MB_ENOERR
	// mode   只能是 MB_RTU 、 MB_ASCII 或 MB_TCP 其中一个
	// pPdu   输出 协议单元指针		 
	// pduLen 输出 协议单元大小		   
	virtual eMBErrorCode fromPdu( eMBMode mode, unsigned char *pPdu, unsigned int pduLen) = 0;
	virtual eMBErrorCode toPdu( eMBMode mode, unsigned char *pPdu, unsigned int &pduLen) = 0;
};
#endif