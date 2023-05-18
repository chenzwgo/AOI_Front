#ifndef LIGHTCONTROL_NEW_H
#define LIGHTCONTROL_NEW_H
#include<list>
#include "g12globalenv.h"
#include "modbuscom.h"
#include "g12commonexport.h"
#include "lighthearder.h"
#include <map>
#include <string>
using namespace std;
#define  CD_Size			6		//	6 �ſ�
#define  Section_Size		4       //  ��4 �� ; ���� 1��2��4 ���� 1��2��3
#define  Cam_Cnt			4	    //  �������
#define	 CFG_2K			0
#define	 CFG_8K			1
typedef enum {
	LIGHT_0 = 0,  				//���Ϲ�Դ		0
	LIGHT_1, 					//���¹�Դ		1 
	LIGHT_2,					//�ϻ��ι�		2		
	LIGHT_3,					//�����¹�Դ	3
	LIGHT_4,					//��ֱ
	LIGHT_SIZE
}Light_Type;


typedef enum {
	Channel_Index_A = 0,  				// Channel	A 
	Channel_Index_B, 					// Channel	B 	
	Channel_Index_C,					// Channel	C 
	Channel_Index_D,  					// Channel	D 
	Channel_Index_E,					// Channel	E 
	Channel_Index_F,					// Channel	F 
	Channel_Index_G,  					// Channel	H 
	Channel_Index_H,					// Channel	G 
	Channel_Index_Size,
}Channel_Index;

typedef enum {
	Scan_line_1 = 0,  				//ɨ��0��
	Scan_line_2, 					//ɨ��1��
	Scan_line_3,					//ɨ��2��
	Scan_line_4,					//ɨ��3��
	Scan_line_5,					//ɨ��4��
	Scan_line_Size
}Scan_line_num;


typedef uint16_t EleValueArray[6][5];

typedef struct EleCfg
{
	LightSourceModel model;				 // Ƶ��ģʽ0 , ����1
	EleValueArray EleValue[LIGHT_SIZE];  // Ƶ������ֵ
	bool alwaysOn[LIGHT_SIZE][CD_Size];  // ����ʹ��
}stEleCfg;

typedef struct LightEnaleCfg
{
	Light_Type type;					 // ��Դ����
	bool enable[CD_Size][Section_Size];	 // �ƹ�ʹ��
	uint16_t ipulse[6];	
};

typedef struct IllumChannelCfg
{
	LightEnaleCfg LightEnale[LIGHT_SIZE];	 // ʹ��;
	//int ipulse;							 // ����
	//int icarema_delay;					 // �����ʱ
	int iscan_line_count;					 // ɨ������-�����л���һ�ֵƹ�����
	bool CamEnable[Cam_Cnt];				 // ���ʹ��
	bool ChannelEnable;						 // ����ҳ��ѡ���жϣ�ͨ���Ƿ�ʹ��
	int  CamEnableIndex;					 // �ĸ����ʹ��,�������
}stIllumChannelCfg;


class IFileVariable;
class LightControlTcp;
class RESOURCE_LIBRARY_EXPORT lightcontrol_new
{
public:
	//static lightcontrol_new* GetInstance();
	//void   ReleaseInstance();
	lightcontrol_new(int index = 0);
	~lightcontrol_new() {};

	//���Դ������
	bool ClearNumber();
	bool setCurrentInitEleFile(const string& FileName);
	string getCurrentInitEleFileName();

	/******************** �������ýӿ� ***************************/
	// ���ļ���ȡ�����䷽��
	bool getInitEleCfg(EleCfg *lightParam);
	// �ӱ�������䷽���ļ���
	bool setInitEleCfg(EleCfg *lightParam = NULL);
	// �·������䷽����������
	bool writeEleCfgToFlash(EleCfg *lightParam = NULL);
	// ��ȡ�����䷽�ӿ�����
	bool readEleCfgFromFlash(EleCfg *lightParam);

	/******************** ͨ���䷽�ӿ� ***************************/
	bool getllumChannelCfg( Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1,stIllumChannelCfg *lightParam = NULL);
	bool setllumChannelCfg( Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL, string strSence ="");

	bool writellumChannelCfgToFlash(Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL, Light_Type type= LIGHT_SIZE);
	bool readllumChannelCfgToFlash(Channel_Index index = Channel_Index_A, Scan_line_num line_index = Scan_line_1, stIllumChannelCfg *lightParam = NULL);

	/* 1:��ȡ����������-���¹�Դ���߻��ι�,��������0�ɹ���-1ʧ��*/
	int  getCtrType(uint8_t &ls_type, Light_Type type = LIGHT_SIZE);
	
	/*
	*2:���Ƴ�����Ƶ��ģʽ
	* ls_model(8λ)--0Ƶ�� 1����
	* ��������0�ɹ���-1ʧ��
	*/
	int setFlashMode( uint8_t ls_model, Light_Type type = LIGHT_SIZE);

	/*3:����ģʽ�µƹܿ���,��������0�ɹ���-1ʧ��*/
	int setLsSwitch(Light_Type type = LIGHT_SIZE);

	/*
	* 4:�����Գ��ģʽ,
	* uint8_t					ls_auto_charge;					//�����Զ����ģʽ/0�ر�/1����
	*��������0�ɹ���-1ʧ��
	*/
	int setAutoCharge(uint8_t ls_auto_charge, Light_Type type = LIGHT_SIZE);

	/* 5:�������ָ��-��ÿɨ��һ����Ҫ���������Ϊ��һ����׼��,
	*	uint8_t	ls_config_id;						//�䷽ѡ�� 0��1 
	*	uint16_t[8](ÿ��ͨ��ʵ�ʴ�������)
	*   ��������0�ɹ���-1ʧ��
	*/
	int clearCounter(  Light_Type type = LIGHT_SIZE);

	/* 5:��ȡ����ָ��,
	*	uint8_t	ls_config_id;						//�䷽ѡ�� 0��1
	*	uint16_t[8](ÿ��ͨ��ʵ�ʴ�������)
	*   ��������0�ɹ���-1ʧ��
	*/
	int getCounter( uint16_t  trig[8], Light_Type type = LIGHT_SIZE);

	/*6:�·�ÿ��ͨ����������,��������0�ɹ���-1ʧ��*/
	//���ڿ��Ƶ���->6�ſ���ÿ�ſ�4ͨ����������;
	int setLsCurrent(Light_Type type = LIGHT_SIZE);

	/*
	7:����ͨ���·�
	*  uint8_t	ls_config_id;						//�䷽ѡ�� 0��1 
	*  uint8_t channel								ͨ�� 0-7
	*  ��������0�ɹ���-1ʧ��
	*/
	int setMutFlashParamsByChannel( uint8_t channel,  Light_Type type = LIGHT_SIZE);

	/*
	8:����ͨ���·�
	* uint8_t	ls_config_id;						//�䷽ѡ�� 0��1 
	* ��������0�ɹ���-1ʧ��
	*/
	int setMutFlashParams( Light_Type type = LIGHT_SIZE);

	/*
	* 9:һ���·���������
	* ��������0�ɹ���-1ʧ��
	*/
	int setAllMutFlashParams( Light_Type type = LIGHT_SIZE);

	static string  ChannelIndexToName(int index);
	static Channel_Index ChannelNameToIndex(string strChannelName);

private:

    bool readEleConfig();
    bool saveEleConfig();
	bool readAllChannelCfg();
	bool saveAllChannelCfg();
	void initLightTcp();
    string getSenceName();
private:
	string m_strLightEleFileFileName;
	stEleCfg m_currEleCfg;    //���ڿ��Ƶ���->6�ſ���ÿ�ſ�4ͨ����������;
	IFileVariable* m_pCurrentLightEleFile;
	IFileVariable* m_pDefalueLightEleFile;
    LightControlTcp* m_lightControlTcp[LIGHT_SIZE];

	bool m_lightEnabled[LIGHT_SIZE];
	LightSource_Params m_LightSource_Params;
	int m_index;
private:
	stIllumChannelCfg m_stIllumChannelCfg[Channel_Index_Size][Scan_line_Size];
};

class RESOURCE_LIBRARY_EXPORT motioncontrol_new
{
public :

	motioncontrol_new(int index = 0);
	~motioncontrol_new() {};
	////////
	/*
	* 10:һ���·�����������
	* ��������0�ɹ���-1ʧ��
	*/
	int setToController(ControllerParams_t Params);
	/*   NetId  1 2
	* 11:�·����������-��֧��
	Order;//�������
	1->��ȡ������           (CrlNumber ����ֵ������0������
	3->����ģʽ����        ��CrlNumber 0-�ȼ�ഥ��;1-��̬������
	4->ֹͣ�����Ϳ�ʼ����  ��CrlNumber 0-ֹͣ����;1-��ʼ������
	* ��������0�ɹ���-1ʧ��
	*/
	int setOrderToController(uint8_t NetId, Order_num Order, uint8_t CrlNumber = 0);
	//0x01������ID��:0x01=�ͻ���1;0x02=�ͻ���2��+0x05��������ƣ�+0x01(�߼�=0x01;ƽ��=0x02)+0x01 / 0x02 (0x01=��ʼ����; 0x02=��������)
	// ����ֵ  1 ��ʼ����  2 ��������   ���� δ����
	int getControllerTargetStatus(uint8_t NetId, Mark_Type stType);


	bool isEnable();
private:
	LightControlTcp* m_Control;
	ControllerParams_t m_ControllerParams;
	int m_index;
	bool m_bEnalbe;
};

class RESOURCE_LIBRARY_EXPORT lightcontrol_new_manager
{
public:
	static lightcontrol_new_manager* GetInstance();
	void   ReleaseInstance();
	lightcontrol_new*  getLightcontrol(int index);
	void releaseLightcontrol(int index);

private:
	lightcontrol_new_manager();
	~lightcontrol_new_manager();
	static lightcontrol_new_manager* m_pManagerInstance;
	lightcontrol_new* lightcontrols[8];

};

class RESOURCE_LIBRARY_EXPORT motioncontrol_new_manager
{
public:
	static motioncontrol_new_manager* GetInstance();
	void   ReleaseInstance();
	motioncontrol_new*  getMotioncontrol(int index);
	void releaseMotioncontrol(int index);

private:
	motioncontrol_new_manager();
	~motioncontrol_new_manager();
	static motioncontrol_new_manager* m_pManagerInstance;
	motioncontrol_new* motioncontrols[8];

};

#endif // LIGHTCONTROL_NEW_H
