#ifndef LIGHT_HEARDER_H
#define LIGHT_HEARDER_H
#include "OS_DEFINE.h"

typedef unsigned int				uint32_t;			/*!<[CH]32 bit unsigned <BR>[EN]32 bit unsigned */
typedef unsigned char				uint8_t;
typedef unsigned short				uint16_t;

typedef enum {         
   LS_TYPE_UP = 0,  			//�Ϲ�Դ		0
   LS_TYPE_DOWN, 				//�¹�Դ		1 
   LS_TYPE_LOW,				//�ͽǶȹ�Դ	2
   LS_TYPE_UP_ARC,				//�ϻ��ι�	3		
   LS_TYPE_UP_FRONT_ARC,		//��ǰ���ι�	4
   LS_TYPE_UP_REAR_ARC,		//�Ϻ��ι�	5
   LS_TYPE_UP_FLANK_ARC,		//�ϲ໡�ι�	6	
   LS_TYPE_DN_ARC,				//�»��ι�	7	
   LS_TYPE_DN_FRONT_ARC,		//��ǰ���ι�	8
   LS_TYPE_DN_REAR_ARC,		//�º��ι�	9
   LS_TYPE_DN_FLANK_ARC,		//�²໡�ι�	10		
}LightSource_Type;

typedef enum {         
   LS_MODEL_FLASH = 0,			//Ƶ��ģʽ	0  
   LS_MODEL_NORMAL 			//����		1
}LightSourceModel;

//���� 1��2��4 ���� 1��2��3
typedef struct
{
	uint32_t  ls_switch_cd1_1:1;		//����ģʽ����1�ſ���1ͨ����Դ����   ����
	uint32_t  ls_switch_cd1_2:1;
	uint32_t  ls_switch_cd1_3:1;
	uint32_t  ls_switch_cd1_4:1;
	
	uint32_t  ls_switch_cd2_1:1;		//����ģʽ����2�ſ���1ͨ����Դ����   ����
	uint32_t  ls_switch_cd2_2:1;
	uint32_t  ls_switch_cd2_3:1;
	uint32_t  ls_switch_cd2_4:1;
	
	uint32_t  ls_switch_cd3_1:1;		//����ģʽ����3�ſ���1ͨ����Դ����   ����5��
	uint32_t  ls_switch_cd3_2:1;
	uint32_t  ls_switch_cd3_3:1;
	uint32_t  ls_switch_cd3_4:1;
	
	uint32_t  ls_switch_cd4_1:1;		//����ģʽ����4�ſ���1ͨ����Դ����  ����10�� 
	uint32_t  ls_switch_cd4_2:1;
	uint32_t  ls_switch_cd4_3:1;
	uint32_t  ls_switch_cd4_4:1;
	
	uint32_t  ls_switch_cd5_1:1;		//����ģʽ����5�ſ���1ͨ����Դ����  ���Դ
	uint32_t  ls_switch_cd5_2:1;
	uint32_t  ls_switch_cd5_3:1;
	uint32_t  ls_switch_cd5_4:1;
	
	uint32_t  ls_switch_cd6_1:1;		//����ģʽ����6�ſ���1ͨ����Դ����   
	uint32_t  ls_switch_cd6_2:1;
	uint32_t  ls_switch_cd6_3:1;
	uint32_t  ls_switch_cd6_4:1;
	
	uint32_t  reserve:8;
}LightSourceSwitch , *pLightSourceSwitch;


typedef union 
{
	uint32_t  			ui_switch;
	LightSourceSwitch	lss_switch;
}LightSourceSwitch_t , *pLightSourceSwitch_t;

typedef struct
{
	uint8_t		ct_carema_1:1;		
	uint8_t		ct_carema_2:1;
	uint8_t		ct_carema_3:1;
	uint8_t		ct_carema_4:1;
	uint8_t		ct_reserve:4;
}CaremaConfig , *pCaremaConfig;

typedef union 
{
	uint8_t 		us_carema_config;
	CaremaConfig  	carema_config;
}CameraConfig_t , *pCameraConfig_t;

typedef struct
{
	LightSourceSwitch_t			ls_switch_cofnig;		//���õƹܿ���
	CameraConfig_t				ls_carema_config;		//�������	
	uint16_t					ls_pulse[6];			//����
	//uint16_t					ls_carema_delay;		//�����ʱ
	uint16_t					ls_scan_line_count;		//ɨ������-�����л���һ�ֵƹ����� 0-5 ��
}MutFlashParams , *pMutFlashParams;

typedef struct 
{
	uint8_t  				ls_type;							//��鿴LightSource_Type-�����������¹�Դ���߻��ι�Դ��

	uint8_t					ls_model;							//��Դ����������ģʽ->Ƶ��=LS_MODEL_FLASH������=LS_MODEL_NORMAL-Ĭ��Ƶ��;

	LightSourceSwitch_t		ls_switch;							//LightSourceSwitch-����ģʽ�¿��ƹ�Դ���������͹�
	
	uint8_t					ls_auto_charge;					//�����Զ����ģʽ/0�ر�/1����
	
	uint16_t  				ls_current[6][4];					//���ڿ��Ƶ���->6�ſ���ÿ�ſ�4ͨ����������;

	uint8_t					ls_config_id;						//�䷽ѡ�� 0��1
	MutFlashParams			ls_mut_params_config[2][8][5];	//MutFlashParams-2���䷽8ͨ��ÿͨ��5�β�������	
}LightSource_Params , *pLightSource_Params;

/******************************************************************************************************
ͨѶЭ��
��ͷ+ָ��+����+CRC(16λ)
*******************************************************************************************************
1:��ȡ����������-���¹�Դ���߻��ι�
��
0x55aa(��ͷ)+0x01(ָ��)
��
0x55aa(��ͷ)+0x01(ָ��)+ls_type

2:���Ƴ�����Ƶ��ģʽ
��
0x55aa(��ͷ)+0x02(ָ��)+ls_model(8λ)	--0Ƶ�� 1����
��
0x55aa(��ͷ)+0x02(ָ��)+�������   --0ʧ�� 1 ����

3:����ģʽ�µƹܿ���
��
0x55aa(��ͷ)+0x03(ָ��)+ls_switch
��
0x55aa(��ͷ)+0x03(ָ��)+�������   --0ʧ�� 1 ����

4:�����Գ��ģʽ
��
0x55aa(��ͷ)+0x04(ָ��)+ls_auto_charge(8λ)
��
0x55aa(��ͷ)+0x04(ָ��)+�������   --0ʧ�� 1 ����

5:�������ָ��-��ÿɨ��һ����Ҫ���������Ϊ��һ����׼��
��
0x55aa(��ͷ)+0x05(ָ��)+ls_config_id(8λ)   --��0��ʼ
��
0x55aa(��ͷ)+0x05(ָ��)+uint16_t[8](ÿ��ͨ��ʵ�ʴ�������)

6:�·�ÿ��ͨ����������
��
0x55aa(��ͷ)+0x06(ָ��)+ls_current+CRC(16λ)
��
0x55aa(��ͷ)+0x06(ָ��)+�������   --0ʧ�� 1 ����

7:����ͨ���·�
��
0x55aa(��ͷ)+0x07(ָ��)+ls_config_id(8λ)+Nͨ��(8λ)+MutFlashParams[5]+CRC(16λ)
��
0x55aa(��ͷ)+0x07(ָ��)+�������   --0ʧ�� 1 ����

8:����ͨ���·�
��
0x55aa(��ͷ)+0x08(ָ��)+ls_config_id(8λ)+MutFlashParams[8][5]+CRC(16λ)
��
0x55aa(��ͷ)+0x08(ָ��)+�������   --0ʧ�� 1 ����

9:һ���·���������
��
0x55aa(��ͷ)+0x09(ָ��)+MutFlashParams[2][8][5]+CRC(16λ)
��
0x55aa(��ͷ)+0x09(ָ��)+�������   --0ʧ�� 1 ����




*******************************************************************************************************/

/********************************************add 2020 / 06 / 01

ControllerParams

1:net_id;					//����ID��
�ͻ��˱�ţ�1��2

2:order;					//�������
1->��ȡ������
2->���ò�������ʱ�������塢�ȼ�����塢��̬����Ƶ�ʣ�
3->����ģʽ���ã�0-�ȼ�ഥ��;1-��̬������
4->ֹͣ�����Ϳ�ʼ������0-ֹͣ����;1-��ʼ������

���磺
����:0x01������ID�ţ�+0x01��������ƣ�+0x55+0xaa
����:0x01��������ƣ�+0x55+0xaa

����:0x01������ID�ţ�+0x02��������ƣ�+ ControllerParams_t(�ṹ��)
����:0x01��������ƣ�

����:0x01������ID�ţ�+0x03��������ƣ�+ 1���ֽڣ�����ģʽ���ã�0-�ȼ�ഥ��;1-��̬��������
����:0x01��������ƣ�

����:0x01������ID�ţ�+0x04��������ƣ�+ 1���ֽڣ�����ģʽ���ã�0-ֹͣ����;1-��ʼ�������� -- ��ʼ����ָ��1�����ھ�̬����ģʽʱ���࿪ʼ��̬����Ƶ����2�����ڵȼ�ഥ��ʱ���࿪ʼ��ˮ�ȼ��ܴ�������
����:0x01��������ƣ�

��⵽�����������أ�

0x01������ID��:0x01=�ͻ���1;0x02=�ͻ���2��+0x05��������ƣ�+0x01(�߼�=0x01;ƽ��=0x02)+0x01 / 0x02 (0x01=��ʼ����; 0x02=��������)
***********************************************************/

typedef enum {
	Order_GetCrl = 1, //1->��ȡ������
	Order_SetParms,     // 2->���ò�������ʱ�������塢�ȼ�����塢��̬����Ƶ�ʣ�
	Order_WorkType,     //3->����ģʽ���ã�0 - �ȼ�ഥ��; 1 - ��̬������
	Order_WorkStus,     //4->ֹͣ�����Ϳ�ʼ������0 - ֹͣ����; 1 - ��ʼ������
	Order_Size
}Order_num;
typedef enum {
	Mark_Edgee  = 1,//�߼�=0x01
    Mark_Surface    //ƽ��=0x02
}Mark_Type;
//��ʱ�������
typedef struct DelayPulse
{
	uint32_t	delay_pulse_1;		//ͨ��1
	uint32_t	delay_pulse_2;		//ͨ��2
	DelayPulse()
	{
		delay_pulse_1 = 0;
		delay_pulse_2 = 0;
	}
}DelayPulse_t, *pDelayPulse_t;
//�ȼ�ഥ������
typedef struct EqualSpace
{
	uint16_t	pulse_number_1;		//ͨ��1�ȼ����������
	uint16_t	pulse_number_2;		//ͨ��2�ȼ����������
	EqualSpace()
	{
		pulse_number_1 = 0;
		pulse_number_2 = 0;
	}
}EqualSpace_t, *pEqualSpace_t;

//��������
typedef struct AllPulse
{
	uint32_t	all_pulse_1;		//ͨ��1�ȼ�ഥ������
	uint32_t	all_pulse_2;		//ͨ��2�ȼ�ഥ������
	AllPulse()
	{
		all_pulse_1 = 0;
		all_pulse_2 = 0;
	}
}AllPulse_t, *pAllPulse_t;

//��������������
typedef struct ControllerParams
{
	uint8_t		net_id;			    //����ID��
	uint8_t		order;			    //�������
	DelayPulse_t	delay_pulse;    //��ʱ������������
	EqualSpace_t	equal_space;    //�ȼ�ഥ��
	AllPulse_t	all_pulse;		    //�ȼ�ഥ����������
	uint16_t	trigger_frequence;	//��̬����Ƶ��
	ControllerParams()
	{
		net_id = 1;
		order  = 1;
		trigger_frequence = 0;
		memset(&delay_pulse, 0, sizeof(DelayPulse_t));
		memset(&equal_space, 0, sizeof(EqualSpace_t));
		memset(&all_pulse, 0, sizeof(AllPulse_t));
	}
}ControllerParams_t, *pControllerParams_t;
#endif 