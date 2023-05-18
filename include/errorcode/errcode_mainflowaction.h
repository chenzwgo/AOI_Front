#ifndef __ERROR_CODE_ACTION_DEFINE_H__
#define __ERROR_CODE_ACTION_DEFINE_H__

#ifndef SYS_ERR_ACTION_START
#define SYS_ERR_ACTION_START   500					// Action具体动作错误码起始 
#endif

enum EM_ERR_ACTION						//运动控制错误代码
{
    // 具体动作错误码(500~999)
    SYS_ERR_ACTION_MANUAL_STOP = SYS_ERR_ACTION_START,      // 人为终止
    SYS_ERR_ACTION_VACUUM_OPEN,      // 打开真空失败
    SYS_ERR_ACTION_VACUUM_READ,      // 读取真空到位信号失败
    SYS_ERR_ACTION_VACUUM_TIMEOUT,   // 真空到位超时
    SYS_ERR_ACTION_CYLINDER_OPEN,    // 打开气缸失败
    SYS_ERR_ACTION_CYLINDER_READ,    // 读取气缸到位信号失败
    SYS_ERR_ACTION_CYLINDER_TIMEOUT, // 打开气缸超时
    SYS_ERR_ACTION_IO_WRITE,         // 写IO失败
    SYS_ERR_ACTION_IO_READ,          // 读IO失败
    SYS_ERR_ACTION_IO_TIMEOUT,       // 等待IO超时
    SYS_ERR_ACTION_MOVE_JOG,         // jog运动失败
    SYS_ERR_ACTION_MOVE_PTP,         // 跑点失败
    SYS_ERR_ACTION_MOVE_AXIS,         // 单轴运动失败
    SYS_ERR_ACTION_MOVE_STOP,         // 运动停止失败
    SYS_ERR_ACTION_MOVE_LINE,         // 直线插补失败
    SYS_ERR_ACTION_MOVE_ARC,          // 圆弧插补失败
    SYS_ERR_ACTION_WAITACTION_TIMEOUT, // 等待动作结束超时
    SYS_ERR_ACTION_WAITACTION_NOACTION,// 等待的动作不存在
    SYS_ERR_ACTION_SETEVENT,           // 设置事件失败
    SYS_ERR_ACTION_WAITEVENT_TIMEOUT,   // 等待事件超时
    SYS_ERR_ACTION_CALLFUN_NOFUN,       // 找不到回调函数
    SYS_ERR_ACTION_CALLFUN_EXECUTE,     // 执行回调函数失败
    SYS_ERR_ACTION_CALLFUN_PARAM,       // 回调函数参数错误
    SYS_ERR_ACTION_TCP_UNEXIST,         // TCP客户端不存在
    SYS_ERR_ACTION_TCP_SEND,            // TCP发送失败
    SYS_ERR_ACTION_TCP_READ,            // TCP读取失败
    SYS_ERR_ACTION_TCP_CONN,            // TCP连接失败
    SYS_ERR_ACTION_UDP_SEND,            // UDP发送失败
    SYS_ERR_ACTION_UDP_READ,            // UDP读取失败
    SYS_ERR_ACTION_UDP_PARAM,               // DUP参数错误
    SYS_ERR_ACTION_COM_SEND,                // 串口发送失败
    SYS_ERR_ACTION_COM_READ,                // 串口读取失败
    SYS_ERR_ACTION_COM_PARAM,               // 串口参数错误
    SYS_ERR_ACITON_FILEINI_FILENOTEXIST,   // ini文件不存在或内容为空格
    SYS_ERR_ACITON_FILEINI_VARNOTEXIST,     // variiable not exist
    SYS_ERR_ACITON_FILEINI_KEYNOTEXIST,     // Key name or Group Name not exist
    SYS_ERR_ACITON_FILEINI_SETVALUE,    // Key name or Group Name not exist
    SYS_ERR_ACITON_FILECSV_FILENOTEXIST,    // CSV文件不存在
    SYS_ERR_ACITON_FILECSV_VARNOTEXIST,     // variiable not exist
    SYS_ERR_ACITON_FILECSV_WIRTE_FAIL,      // CSV文件写入失败
    SYS_ERR_ACITON_FILECSV_READ_FAIL,       // CSV文件读取失败
    SYS_ERR_TRAN_DATA_TIMEOUT,	    // 变量错误
    SYS_ERR_VAR_TYPE_ERROR         // 变量类型不匹配	
};
#endif
