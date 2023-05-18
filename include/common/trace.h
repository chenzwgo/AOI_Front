#ifndef TRACE_H
#define TRACE_H

//Trace级别
enum TraceLevel
{
	TL_NULL = 0,/**< 不记录 */
	TL_CRIT,	/**< 致命 */
	TL_ERRO,	/**< 错误 */
	TL_WARN,	/**< 警告 */
	TL_INFO		/**< 信息 */
};

extern void TraceSrv_Trace(const int iTraceLevel, const char* pszModule, const char* pszFileName, \
                    const char *file, const char *pszFun, int line, const char* pszMsg, ...);

/*带代码信息的通用简洁版本,所有日志都在trace.log中*/
/*如果需要修改保存的文件名， 则可将下面一段拷贝到本模块头文件中，将第一个NULL改为模块名，第二个NULL改为文件名*/
#define LOG_CRIT(msg, ...) TraceSrv_Trace(TL_CRIT, NULL, NULL, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_ERRO(msg, ...) TraceSrv_Trace(TL_ERRO, NULL, NULL, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) TraceSrv_Trace(TL_WARN, NULL, NULL, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...) TraceSrv_Trace(TL_INFO, NULL, NULL, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)

class CFunEnterTrace
{
public:
    CFunEnterTrace(const char *pszFileName, const char *pszFile, const char *pszFun, int line)
    {
        memset(m_msgFile, 0, sizeof(m_msgFile));
        memset(m_msgFun, 0, sizeof(m_msgFun));
        memset(m_pszFileName, 0, sizeof(m_pszFileName));
 
        if (NULL != pszFile)
        {
            strncpy(m_msgFile, pszFile, sizeof(m_msgFile));
        }
        
        if (NULL != pszFun)
        {
            strncpy(m_msgFun, pszFun, sizeof(m_msgFun));
        }

        if (NULL != pszFileName)
        {
            strncpy(m_pszFileName, pszFileName, sizeof(m_pszFileName));
        }
        
        m_msgLine = line;

        if (0 == strlen(m_pszFileName))
        {
            TraceSrv_Trace(TL_INFO, NULL, NULL, m_msgFile, m_msgFun, m_msgLine, "Enter");
        }
        else
        {
            TraceSrv_Trace(TL_INFO, NULL, m_pszFileName, m_msgFile, m_msgFun, m_msgLine, "Enter");
        }

        
    }
    ~CFunEnterTrace()
    {
        if (0 == strlen(m_pszFileName))
        {
            TraceSrv_Trace(TL_INFO, NULL, NULL, m_msgFile, m_msgFun, m_msgLine, "Exit");
        }
        else
        {
            TraceSrv_Trace(TL_INFO, NULL, m_pszFileName, m_msgFile, m_msgFun, m_msgLine, "Exit");
        }
        
    }
private:
    char m_msgFile[128];
    char m_msgFun[128];
    char m_pszFileName[128];
    int m_msgLine;    
};

#define LOG_FUNTION  CFunEnterTrace CFunEnterTraceTmp(NULL,__FILE__,__FUNCTION__,__LINE__)

// 指定文件打印log
#define LOG_INFO_F(file, msg, ...) TraceSrv_Trace(TL_INFO, NULL, file, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_WARN_F(file, msg, ...) TraceSrv_Trace(TL_WARN, NULL, file, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_FUNTION_F(file) CFunEnterTrace CFunEnterTraceTmp(file, __FILE__,__FUNCTION__,__LINE__);
// 简化版log(不打函数和文件)
#define LOG_INFO_F_SIMP(file, msg, ...) TraceSrv_Trace(TL_INFO, NULL, file, NULL, NULL, -1, msg, ##__VA_ARGS__)

#endif // TRACE_H
