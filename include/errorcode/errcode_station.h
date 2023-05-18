#ifndef __ERROR_CODE_STATION_DEFINE_H__
#define __ERROR_CODE_STATION_DEFINE_H__


#ifndef SYS_ERR_STATION_START
#define SYS_ERR_STATION_START   2500					// station������ʼ 
#endif

enum EM_ERR_STATION								// station������ʼ 
{
    //��վ������
    SYS_ERR_STATION_INIT_FAIL = SYS_ERR_STATION_START,			//��վ��ʼ��ʧ��
    SYS_ERR_STATION_NO_ID,						//��Ч�Ĺ�վID
    SYS_ERR_STATION_NO_CARD,					//�޿��ÿ�
    SYS_ERR_STATION_NO_EXTMODULE,				//����չ��
    SYS_ERR_STATION_NO_VALIDAXIS,				//�޿�����
    SYS_ERR_STATION_ROBOT_CONTRL_FAIL,			//�����˲���ʧ��
    SYS_ERR_STATION_CARD_CONTRL_FAIL,			//����վ��������
    SYS_ERR_STATION_ROBOT_EXEC,				//robot����ʧ��
    SYS_ERR_STATION_NO_CONNET,					//��վ����ʧ��
    SYS_ERR_STATION_GET_INSTANCE,				//��ȡ����ʧ��
    SYS_ERR_STATION_GET_PARAM,					//��ȡ����ʧ��
    SYS_ERR_STATION_QUERY_INTEFACE,			//��ѯ�ӿ�ʧ��
    SYS_ERR_STATION_CREAT_PT_FILE,				//�������ļ�ʧ��
    SYS_ERR_STATION_READ_INTEFACE,				//��ȡ���ļ�ʧ��
    SYS_ERR_STATION_SAVE_INTEFACE,				//������ļ�ʧ��
    SYS_ERR_STATION_NO_POINT,					//û���ҵ���Ӧ�ĵ���Ϣ
    SYS_ERR_STATION_NO_FUN,					//û�нӿ�ʵ��
    SYS_ERR_STATION_MOVING,            //�·��˶�ָ��ʱ,��վ�����˶���
    SYS_ERR_STATION_NO_IO,						//û�ж�Ӧioʵ��
    SYS_ERR_STATION_NO_AXIS,					//û�ж�Ӧ��ʵ��
    SYS_ERR_STATION_NO_AXIS_PARA_MSG,			//û�ж�Ӧ��Ĳ���
    SYS_ERR_STATION_AXIS_PARA_MSG,				//���������
    SYS_ERR_STATION_NO_INIT,					//ģ��û�г�ʼ��
    SYS_ERR_STATION_NO_READY,                   //��վδ׼����
    SYS_ERR_STATION_STOP,						//ֹͣ���˶�ʧ��
    SYS_ERR_STATION_ALWAYS_MOVE,				//�����˶�ʧ��
    SYS_ERR_STATION_MOVE_POS,					//��λ�ƶ�ʧ��	
    SYS_ERR_STATION_IO,						   //IO��ȡ�����ʧ��
    SYS_ERR_STATION_GET_POS,					//��ȡ��ǰ��λ������ʧ��
    SYS_ERR_STATION_SET_CRDPRM,				//��������ϵʧ��
    SYS_ERR_STATION_SET_BUFF_CMD,				//���ò岹����������ʧ��
    SYS_ERR_STATION_MOVE_CRDPRM,				//�岹�˶�ʧ��
    SYS_ERR_STATION_STAR_CRTDPRM,				//���˲岹�˶�ʧ��
    SYS_ERR_STATION_GET_CRDPRM_STATUS,			//��ȡ�岹�˶�״̬ʧ��
    SYS_ERR_STATION_ALCEAR_CRD,				//����岹�������ڵĲ岹����ʧ��
    SYS_ERR_STATION_SET_CRD_IO,				//��������������IO�������ָ��ʧ��
    SYS_ERR_STATION_SET_CRD_DELAY,				//����������ʱ����ָ��ʧ��
    SYS_ERR_STATION_GET_CRD_SPACE,				//��ѯ�岹������ʣ��ռ�ʧ��
    SYS_ERR_STATION_GET_CRD_INDEX,				//��ȡδ��ɵĲ岹����ʧ��
    SYS_ERR_STATION_MM_TO_PULSE,				   //����ת��Ϊ����ʧ��
    SYS_ERR_STATION_RESET,						   //����ʧ��
    SYS_ERR_STATION_INIT_INTERP,			   //��ʼ���岹ʧ��
    SYS_ERR_STATION_PARAM,							//�����������
    SYS_ERR_STATION_HOME_FAILED,		         //��ԭʧ��
    SYS_ERR_STATION_DEVIATE_POINT,               //ƫ���λ���������趨�Ĳ�ֵ��
    SYS_ERR_STATION_SET_INTERP_SPEED,           // ���ò岹�ٶ�ʧ��
    SYS_ERR_STATION_PAUSE,							// ��ͣʧ��
    SYS_ERR_STATION_HAS_ALARM,					// ���ᱨ��
};
#endif
