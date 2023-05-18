#ifndef __ERROR_CODE_ACTION_DEFINE_H__
#define __ERROR_CODE_ACTION_DEFINE_H__

#ifndef SYS_ERR_ACTION_START
#define SYS_ERR_ACTION_START   500					// Action���嶯����������ʼ 
#endif

enum EM_ERR_ACTION						//�˶����ƴ������
{
    // ���嶯��������(500~999)
    SYS_ERR_ACTION_MANUAL_STOP = SYS_ERR_ACTION_START,      // ��Ϊ��ֹ
    SYS_ERR_ACTION_VACUUM_OPEN,      // �����ʧ��
    SYS_ERR_ACTION_VACUUM_READ,      // ��ȡ��յ�λ�ź�ʧ��
    SYS_ERR_ACTION_VACUUM_TIMEOUT,   // ��յ�λ��ʱ
    SYS_ERR_ACTION_CYLINDER_OPEN,    // ������ʧ��
    SYS_ERR_ACTION_CYLINDER_READ,    // ��ȡ���׵�λ�ź�ʧ��
    SYS_ERR_ACTION_CYLINDER_TIMEOUT, // �����׳�ʱ
    SYS_ERR_ACTION_IO_WRITE,         // дIOʧ��
    SYS_ERR_ACTION_IO_READ,          // ��IOʧ��
    SYS_ERR_ACTION_IO_TIMEOUT,       // �ȴ�IO��ʱ
    SYS_ERR_ACTION_MOVE_JOG,         // jog�˶�ʧ��
    SYS_ERR_ACTION_MOVE_PTP,         // �ܵ�ʧ��
    SYS_ERR_ACTION_MOVE_AXIS,         // �����˶�ʧ��
    SYS_ERR_ACTION_MOVE_STOP,         // �˶�ֹͣʧ��
    SYS_ERR_ACTION_MOVE_LINE,         // ֱ�߲岹ʧ��
    SYS_ERR_ACTION_MOVE_ARC,          // Բ���岹ʧ��
    SYS_ERR_ACTION_WAITACTION_TIMEOUT, // �ȴ�����������ʱ
    SYS_ERR_ACTION_WAITACTION_NOACTION,// �ȴ��Ķ���������
    SYS_ERR_ACTION_SETEVENT,           // �����¼�ʧ��
    SYS_ERR_ACTION_WAITEVENT_TIMEOUT,   // �ȴ��¼���ʱ
    SYS_ERR_ACTION_CALLFUN_NOFUN,       // �Ҳ����ص�����
    SYS_ERR_ACTION_CALLFUN_EXECUTE,     // ִ�лص�����ʧ��
    SYS_ERR_ACTION_CALLFUN_PARAM,       // �ص�������������
    SYS_ERR_ACTION_TCP_UNEXIST,         // TCP�ͻ��˲�����
    SYS_ERR_ACTION_TCP_SEND,            // TCP����ʧ��
    SYS_ERR_ACTION_TCP_READ,            // TCP��ȡʧ��
    SYS_ERR_ACTION_TCP_CONN,            // TCP����ʧ��
    SYS_ERR_ACTION_UDP_SEND,            // UDP����ʧ��
    SYS_ERR_ACTION_UDP_READ,            // UDP��ȡʧ��
    SYS_ERR_ACTION_UDP_PARAM,               // DUP��������
    SYS_ERR_ACTION_COM_SEND,                // ���ڷ���ʧ��
    SYS_ERR_ACTION_COM_READ,                // ���ڶ�ȡʧ��
    SYS_ERR_ACTION_COM_PARAM,               // ���ڲ�������
    SYS_ERR_ACITON_FILEINI_FILENOTEXIST,   // ini�ļ������ڻ�����Ϊ�ո�
    SYS_ERR_ACITON_FILEINI_VARNOTEXIST,     // variiable not exist
    SYS_ERR_ACITON_FILEINI_KEYNOTEXIST,     // Key name or Group Name not exist
    SYS_ERR_ACITON_FILEINI_SETVALUE,    // Key name or Group Name not exist
    SYS_ERR_ACITON_FILECSV_FILENOTEXIST,    // CSV�ļ�������
    SYS_ERR_ACITON_FILECSV_VARNOTEXIST,     // variiable not exist
    SYS_ERR_ACITON_FILECSV_WIRTE_FAIL,      // CSV�ļ�д��ʧ��
    SYS_ERR_ACITON_FILECSV_READ_FAIL,       // CSV�ļ���ȡʧ��
    SYS_ERR_TRAN_DATA_TIMEOUT,	    // ��������
    SYS_ERR_VAR_TYPE_ERROR         // �������Ͳ�ƥ��	
};
#endif
