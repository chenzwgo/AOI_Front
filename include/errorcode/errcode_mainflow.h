#ifndef __ERROR_CODE_MAINFLOW_DEFINE_H__
#define __ERROR_CODE_MAINFLOW_DEFINE_H__


#ifndef SYS_ERR_MAINFLOW_START
#define SYS_ERR_MAINFLOW_START   300					// mainflow��������ʼ 
#endif


enum EM_ERR_MAINFLOW						//mainflow��������ʼ 
{ 
	SYS_ERR_MAINFLOW_NOINSTANCE = SYS_ERR_MAINFLOW_START,				// û�ж���ʵ��
	SYS_ERR_MAINFLOW_FILE,          //���������ļ�����
	SYS_ERR_MAINFLOW_REPEATNAME,    //����
	SYS_ERR_MAINFLOW_NOPOINT,        //����û�д˵�λ
	SYS_ERR_MAINFLOW_NOSTATION,      //����û�д˹�վ
	SYS_ERR_MAINFLOW_NOWORKPROC,             //û�д�����
	SYS_ERR_MAINFLOW_WORK_FLOW,				// WORK_FLOW  �ṹ�� �ڲ���������
	SYS_ERR_MAINFLOW_WORK_STEP,				// WORK_STEP  �ṹ�� �ڲ���������
	SYS_ERR_MAINFLOW_WORK_POINT,				// WORK_POINT �ṹ�� �ڲ���������
	SYS_ERR_MAINFLOW_UNINIT,					//δ��ʼ��
	SYS_ERR_MAINFLOW_PARAM,					//��δ���
	SYS_ERR_MAINFLOW_ACTION_PARAM,           //Action ��������
	SYS_ERR_MAINFLOW_RUNING,           // ������
	SYS_ERR_MAINFLOW_AXIS_NO_INIT,      // ��δ��ʼ��
	SYS_ERR_MAINFLOW_IO_NO_INIT,        // IOδ��ʼ��
	SYS_ERR_MAINFLOW_FLOWNAME,   // ��������Ϊ��
	SYS_ERR_MAINFLOW_STEPNAME,   // ��������Ϊ��
	SYS_ERR_MAINFLOW_ACTIONNAME,   // ��������Ϊ��
	SYS_ERR_MAINFLOW_INTERPLOATE,  // �岹�˶�ʧ��
	SYS_ERR_MAINFLOW_FUNCTION	,  // ��������ִ��ʧ��
	SYS_ERR_MAINFLOW_CHANGE_SPEED	, //���ٶ�ʧ��
	SYS_ERR_MAINFLOW_MOVE_PIONT	, //�ж���λִ��ʧ��
	SYS_ERR_MAINFLOW_STOP	,       //ִ��ֹͣʧ��
	SYS_ERR_MAINFLOW_MISOPERATION,	  // ����������磬û����ͣ����Ӽ���
	SYS_ERR_MAINFLOW_NO_FUNCTION,     // �Ҳ���Ҫִ�еĺ���
	SYS_ERR_MAINFLOW_NO_EVENT,        // �Ҳ������¼�
	SYS_ERR_MAINFLOW_STATUS,		// ״̬�쳣
    SYS_ERR_MAINFLOW_NO_ACTION,       //û�ж�����û�ж�����ѡ
    SYS_ERR_MAINFLOW_FLOW_FUN,         //���̺���ִ��ʧ��
};
#endif
