#ifndef __ERROR_CODE_BUSSINESS_DEFINE_H__
#define __ERROR_CODE_BUSSINESS_DEFINE_H__

#ifndef SYS_ERR_BUSSINESS_START
#define SYS_ERR_BUSSINESS_START    10000					 // ҵ�������
#endif

enum EM_ERR_BUSSINESS										// ҵ�������
{
 
  // ҵ�������
    ERR_BUSSINESS_PARAM = SYS_ERR_BUSSINESS_START,			//�ص�������δ���
	ERR_BUSSINESS_STATION_INSTANCE ,			            //�ص�������ʹ�õĹ�վָ��Ϊ��
	ERR_BUSSINESS_CARD_INSTANCE ,			                 //�ص�������ʹ�õĿ�ָ��Ϊ��
};
#endif
