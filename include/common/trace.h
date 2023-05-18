#ifndef TRACE_H
#define TRACE_H

//Trace����
enum TraceLevel
{
	TL_NULL = 0,/**< ����¼ */
	TL_CRIT,	/**< ���� */
	TL_ERRO,	/**< ���� */
	TL_WARN,	/**< ���� */
	TL_INFO		/**< ��Ϣ */
};

extern void TraceSrv_Trace(const int iTraceLevel, const char* pszModule, const char* pszFileName, \
                    const char *file, const char *pszFun, int line, const char* pszMsg, ...);

/*��������Ϣ��ͨ�ü��汾,������־����trace.log��*/
/*�����Ҫ�޸ı�����ļ����� ��ɽ�����һ�ο�������ģ��ͷ�ļ��У�����һ��NULL��Ϊģ�������ڶ���NULL��Ϊ�ļ���*/
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

// ָ���ļ���ӡlog
#define LOG_INFO_F(file, msg, ...) TraceSrv_Trace(TL_INFO, NULL, file, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_WARN_F(file, msg, ...) TraceSrv_Trace(TL_WARN, NULL, file, __FILE__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LOG_FUNTION_F(file) CFunEnterTrace CFunEnterTraceTmp(file, __FILE__,__FUNCTION__,__LINE__);
// �򻯰�log(���������ļ�)
#define LOG_INFO_F_SIMP(file, msg, ...) TraceSrv_Trace(TL_INFO, NULL, file, NULL, NULL, -1, msg, ##__VA_ARGS__)

#endif // TRACE_H
