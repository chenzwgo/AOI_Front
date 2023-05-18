#ifndef __ERROR_CODE_MOTION_DEFINE_H__
#define __ERROR_CODE_MOTION_DEFINE_H__

#ifndef SYS_ERR_MOTION_START
#define SYS_ERR_MOTION_START   1000					// motion��������ʼ 
#endif

enum EM_ERR_MOTION						//�˶����ƴ������
{
	//�����ļ�����
	SYS_ERR_STATION_POINT_CFG_OPEN = SYS_ERR_MOTION_START,    //���ļ��򿪴���
	SYS_ERR_STATION_CFG,				//��վ���ô���
	SYS_ERR_STATION_LOAD_AXIS_CFG,      // ���������ļ�ʧ��
	SYS_ERR_MOTION_GET_INSTANCE,       //��ȡmotion����ʧ��
	SYS_ERR_MOTION_GET_CFG,				//���������ļ�����ʧ��
	SYS_ERR_MOTION_CFG_INIT,					//��ʼ�������ļ�ʧ��
	SYS_ERR_MOTION_STATION_LOAD,		//���ع�վ������Ϣʧ��
	SYS_ERR_MOTION_LOAD_CARD_CFG,				//���ؿ�����ʧ��
	SYS_ERR_MOTION_LOAD_IO_CFG,				//����IO����ʧ��
	SYS_ERR_MOTION_SAVE_CARD_CFG,				//���濨����ʧ��
	SYS_ERR_MOTION_SAVE_AXIS_CFG,				//����������ʧ��
	SYS_ERR_MOTION_SAVE_IO_CFG,				//����IO����ʧ��
};


#endif // __ERROR_CODE_MOTION_DEFINE_H__
