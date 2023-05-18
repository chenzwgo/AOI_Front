#ifndef __ERROR_CODE_SAFECHECK_DEFINE_H__
#define __ERROR_CODE_SAFECHECK_DEFINE_H__

#ifndef SYS_ERR_SAFECHECK_START
#define SYS_ERR_SAFECHECK_START   1500					// safecheck��������ʼ 
#endif

enum EM_ERR_SAFECHECK						// safecheck��������ʼ 
{
	//�����ļ�����
	SYS_ERR_SAFE_INIT = SYS_ERR_SAFECHECK_START,				// ��ȫ����ʼ��ʧ��
	SYS_ERR_SAFE_CFG_INIT,				// ��ȫ������ó�ʼ��ʧ��
	SYS_ERR_SAFE_LOAD_ITEM_CFG,			// ���ذ�ȫ�����Ŀ�����ļ�ʧ��
	SYS_ERR_SAFE_LOAD_PARAM_CFG,			// ���ذ�ȫ�����������ļ�ʧ��
	SYS_ERR_SAFE_SAVE_ITEM_CFG,			// ���氲ȫ�����Ŀ�����ļ�ʧ��
	SYS_ERR_SAFE_SAVE_PARAM_CFG,			// ���氲ȫ�����������ļ�ʧ��
	SYS_ERR_SAFE_PARAM,					// ��������
	SYS_ERR_SAFE_ALARM,					// ��ⱨ��
	SYS_ERR_SAFE_NO_PROJECT,				// û�м������
	SYS_ERR_SAFE_NO_GROUP,				// û�м������
	SYS_ERR_SAFE_NO_ITEM,					// û�м����
	SYS_ERR_SAFE_IO,						// IO����ȫ
	SYS_ERR_SAFE_PT,						// ��λ����ȫ
	SYS_ERR_SAFE_POS,						// λ�ò���ȫ
	SYS_ERR_SAFE_AXIS,					// �᲻�ڰ�ȫ��Χ
	SYS_ERR_SAFE_NETWORK,					// �����쳣
	SYS_ERR_SAFE_COM,						// COM״̬�쳣
	SYS_ERR_SAFE_CARD,					// Card״̬�쳣
	SYS_ERR_SAFE_EXIST_PROJECT_NAME,		// project ���ֳ�ͻ
	SYS_ERR_SAFE_EXIST_PROJECT_DESC,		// project ������ͻ
	SYS_ERR_SAFE_EXIST_GROUP_NAME,			// group ���ֳ�ͻ
	SYS_ERR_SAFE_EXIST_GROUP_DESC,			// group ������ͻ
	SYS_ERR_SAFE_POINT_INFO,				// point ��Ϣ����
	SYS_ERR_SAFE_IO_INFO					// io ��Ϣ����
};
#endif  //__ERROR_CODE_SAFECHECK_DEFINE_H__
