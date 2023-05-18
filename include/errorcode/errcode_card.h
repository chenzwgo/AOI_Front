#ifndef __ERROR_CODE_CARD_DEFINE_H__
#define __ERROR_CODE_CARD_DEFINE_H__

#ifndef SYS_ERR_CARD_START
#define SYS_ERR_CARD_START   2500					// card��������ʼ 
#endif

enum EM_ERR_CARD						//card��������ʼ 
{
	//�������� 
    SYS_ERR_CARD_INIT_FAIL = SYS_ERR_CARD_START,				//���ƿ���ʼ��ʧ��
    SYS_ERR_CARD_OPEN_CARD_FALSE,			  //�򿪿�ʧ�� 
    SYS_ERR_CARD_NO_CARDINSTANCE,				  //û�ж�Ӧ��ʵ��
    SYS_ERR_CARD_AXIS_HOME,					  //��ѯ�Զ���ԭ�������״̬ʧ��
    SYS_ERR_CARD_SET_CAPTURE,				  //���ñ���������ʽʧ�ܣ���ԭ��
    SYS_ERR_CARD_CLEAR_STATUS,		          //��״̬ʧ��
    SYS_ERR_CARD_SET_ZERO,                    //λ������ʧ��
    SYS_ERR_CARD_CFG,					      //���ؿ��ƿ������ļ�ʧ��
    SYS_ERR_CARD_AXIS_MOVE_N,			      //��ԭʱ�ƶ�������λʧ��
    SYS_ERR_CARD_AXIS_MOVE_P,			      //��ԭʱ�ƶ�������λʧ��
    SYS_ERR_CARD_AXIS_ON,					   //��ʹ��ʧ��
    SYS_ERR_CARD_AXIS_IN_LIMITN,			  //���ڸ���λ
    SYS_ERR_CARD_AXIS_STOP,			          //ֹͣʧ��
    SYS_ERR_CARD_SPEED_ZERO,                  //�ٶ�Ϊ0
    SYS_ERR_CARD_HOME_MODE,                   //��ԭģʽ����
    SYS_ERR_CARD_COMPARE_POS,                 //λ�ñȽ��������
    SYS_ERR_CARD_GET_ADC,                     //��ȡ��ѹֵʧ��
    SYS_ERR_CARD_CTRL_FAIL,                   // ����ʧ��

    //mcs��������
    SYS_ERR_MOVE_N,					//��ԭʱ�ƶ�����λʧ��
    SYS_ERR_CLEAR_STATUS_SET_ZERO,	//��״̬ʧ�ܻ�λ������ʧ��
    SYS_ERR_AXIS_HOME,				//��ѯ�Զ���ԭ�������״̬ʧ��
    SYS_ERR_HOME					//��ԭʧ��
};

#endif
