#ifndef  __LIGHTCONTROL_TCP_H__
#define  __LIGHTCONTROL_TCP_H__

#include <winsock2.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
# include <ws2tcpip.h>
#include "lighthearder.h"
#include "jmutex.h"

#define  LENGTH_TO_READ   (sizeof(LightSource_Params)+2)
//

/////////c++  class wrap:///////////////////////////////////////////////////////////////////////////////

class  LightControlTcp {

public:
	LightControlTcp(char *ip, uint16_t port);
	~LightControlTcp();

	/* 1:��ȡ����������-���¹�Դ���߻��ι�
	  ��
	  0x55aa(��ͷ)+0x01(ָ��)
	  ��
	  0x55aa(��ͷ)+0x01(ָ��)+ls_type
	��������0�ɹ���-1ʧ��
	*/
	int  get_ctr_type(uint8_t &ls_type);

	/*
	2:���Ƴ�����Ƶ��ģʽ
		��
		0x55aa(��ͷ)+0x02(ָ��)+ls_model(8λ)--0Ƶ�� 1����
		��
		0x55aa(��ͷ)+0x02(ָ��)+�������--0ʧ�� 1 ����
		��������0�ɹ���-1ʧ��
	*/
	int set_flash_mode(uint8_t ls_model);

	/*
	3:����ģʽ�µƹܿ���
	��
	 0x55aa(��ͷ)+0x03(ָ��)+ls_switch
	��
	0x55aa(��ͷ)+0x03(ָ��)+�������   --0ʧ�� 1 ����
	��������0�ɹ���-1ʧ��
	*/
	int set_ls_switch(LightSourceSwitch_t  &ls_switch);

	/*
	 4:�����Գ��ģʽ
	 ��
	  0x55aa(��ͷ)+0x04(ָ��)+ls_auto_charge(8λ)
	 ��
	  0x55aa(��ͷ)+0x04(ָ��)+�������   --0ʧ�� 1 ����
	 ��������0�ɹ���-1ʧ��
	*/
	int set_auto_charge(uint8_t ls_auto_charge);

	/*
	5:�������ָ��-��ÿɨ��һ����Ҫ���������Ϊ��һ����׼��
	��
	  0x55aa(��ͷ)+0x05(ָ��)+ls_config_id(8λ)   --��0��ʼ
	��
	  0x55aa(��ͷ)+0x05(ָ��)+�������   --0ʧ�� 1 ����
	��������0�ɹ���-1ʧ��
	*/
	int clear_counter(uint8_t ls_config_id);

	/*
	6:�·�ÿ��ͨ����������
	��
	  0x55aa(��ͷ)+0x06(ָ��)+ls_current+CRC(16λ)
	��
	  0x55aa(��ͷ)+0x06(ָ��)+�������   --0ʧ�� 1 ����
	��������0�ɹ���-1ʧ��
	*/
	int set_ls_current(uint16_t ls_current[6][4]);

	/*
	7:����ͨ���·�
	��
	  0x55aa(��ͷ)+0x07(ָ��)+ls_config_id(8λ)+Nͨ��(8λ)+MutFlashParams[5]+CRC(16λ)
	��
	  0x55aa(��ͷ)+0x07(ָ��)+�������   --0ʧ�� 1 ����
	��������0�ɹ���-1ʧ��
	*/
	int set_MutFlashParams_by_channel(uint8_t ls_config_id, uint8_t channel, MutFlashParams mfp[5]);

	/*
	 8:����ͨ���·�
	  ��
	  0x55aa(��ͷ)+0x08(ָ��)+ls_config_id(8λ)+MutFlashParams[8][5]+CRC(16λ)
	 ��
	  0x55aa(��ͷ)+0x08(ָ��)+�������   --0ʧ�� 1 ����
	 ��������0�ɹ���-1ʧ��
	*/
	int set_MutFlashParams(uint8_t ls_config_id, MutFlashParams mfp[8][5]);

	/*
	9:һ���·���������
	��
	  0x55aa(��ͷ)+0x09(ָ��)+MutFlashParams[2][8][5]+CRC(16λ)
	��
	  0x55aa(��ͷ)+0x09(ָ��)+�������   --0ʧ�� 1 ����
	��������0�ɹ���-1ʧ��
	*/
	int set_all_MutFlashParams(MutFlashParams mfp[2][8][5]);

	/*
	10:��ȡͨ��ɨ�����
	��
	0x55aa(��ͷ)+0x0a(ָ��)+ls_config_id(8λ)   --��0��ʼ
	��
	0x55aa(��ͷ)+0x0a(ָ��)+uint16_t[8](ÿ��ͨ��ʵ�ʴ�������)
	��������0�ɹ���-1ʧ��
	*/
	int get_counter(uint8_t ls_config_id, uint16_t  trig[8]);

	//
	int set_ControllerParams(ControllerParams stParams);
	//
	int get_Controller_type(uint8_t net_id);

	int set_Order_To_Controller(uint8_t net_id, uint8_t Order, uint8_t setMark);

	int get_Controller_TargetStatus(uint8_t net_id, uint8_t setMark);
	//
	int get_create_tcp_ok(); //0�����ɹ� - 1 ʧ��
	//
private:
	int create_tcp_client_ctx(char *ip, uint16_t port);
	int tcp_client_send_and_recv(int send_len, int recv_len);
	int tcp_client_destroy();
	uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);

private:
	WORD sockVersion;
	WSADATA data;
	struct sockaddr_in serAddr;
	SOCKET sclient;
	unsigned char recv_buf[LENGTH_TO_READ * 7];  // �ռ�Ŷ�һ���
	unsigned char send_buf[LENGTH_TO_READ * 7];
	int m_create_tcp;//0�����ɹ� -1 ʧ��
	JMutex  m_Lock;
};


#endif
