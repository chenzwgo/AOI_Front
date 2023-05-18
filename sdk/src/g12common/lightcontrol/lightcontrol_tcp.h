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

	/* 1:获取控制器类型-上下光源或者弧形光
	  发
	  0x55aa(包头)+0x01(指令)
	  回
	  0x55aa(包头)+0x01(指令)+ls_type
	函数返回0成功，-1失败
	*/
	int  get_ctr_type(uint8_t &ls_type);

	/*
	2:控制常亮或频闪模式
		发
		0x55aa(包头)+0x02(指令)+ls_model(8位)--0频闪 1常亮
		回
		0x55aa(包头)+0x02(指令)+操作结果--0失败 1 正常
		函数返回0成功，-1失败
	*/
	int set_flash_mode(uint8_t ls_model);

	/*
	3:常亮模式下灯管开关
	发
	 0x55aa(包头)+0x03(指令)+ls_switch
	回
	0x55aa(包头)+0x03(指令)+操作结果   --0失败 1 正常
	函数返回0成功，-1失败
	*/
	int set_ls_switch(LightSourceSwitch_t  &ls_switch);

	/*
	 4:开启自充电模式
	 发
	  0x55aa(包头)+0x04(指令)+ls_auto_charge(8位)
	 回
	  0x55aa(包头)+0x04(指令)+操作结果   --0失败 1 正常
	 函数返回0成功，-1失败
	*/
	int set_auto_charge(uint8_t ls_auto_charge);

	/*
	5:清除计数指令-即每扫完一次需要清除计数，为下一次做准备
	发
	  0x55aa(包头)+0x05(指令)+ls_config_id(8位)   --从0开始
	回
	  0x55aa(包头)+0x05(指令)+操作结果   --0失败 1 正常
	函数返回0成功，-1失败
	*/
	int clear_counter(uint8_t ls_config_id);

	/*
	6:下发每个通道电流设置
	发
	  0x55aa(包头)+0x06(指令)+ls_current+CRC(16位)
	回
	  0x55aa(包头)+0x06(指令)+操作结果   --0失败 1 正常
	函数返回0成功，-1失败
	*/
	int set_ls_current(uint16_t ls_current[6][4]);

	/*
	7:单个通道下发
	发
	  0x55aa(包头)+0x07(指令)+ls_config_id(8位)+N通道(8位)+MutFlashParams[5]+CRC(16位)
	回
	  0x55aa(包头)+0x07(指令)+操作结果   --0失败 1 正常
	函数返回0成功，-1失败
	*/
	int set_MutFlashParams_by_channel(uint8_t ls_config_id, uint8_t channel, MutFlashParams mfp[5]);

	/*
	 8:单个通道下发
	  发
	  0x55aa(包头)+0x08(指令)+ls_config_id(8位)+MutFlashParams[8][5]+CRC(16位)
	 回
	  0x55aa(包头)+0x08(指令)+操作结果   --0失败 1 正常
	 函数返回0成功，-1失败
	*/
	int set_MutFlashParams(uint8_t ls_config_id, MutFlashParams mfp[8][5]);

	/*
	9:一次下发所有数据
	发
	  0x55aa(包头)+0x09(指令)+MutFlashParams[2][8][5]+CRC(16位)
	回
	  0x55aa(包头)+0x09(指令)+操作结果   --0失败 1 正常
	函数返回0成功，-1失败
	*/
	int set_all_MutFlashParams(MutFlashParams mfp[2][8][5]);

	/*
	10:获取通道扫描次数
	发
	0x55aa(包头)+0x0a(指令)+ls_config_id(8位)   --从0开始
	回
	0x55aa(包头)+0x0a(指令)+uint16_t[8](每个通道实际触发次数)
	函数返回0成功，-1失败
	*/
	int get_counter(uint8_t ls_config_id, uint16_t  trig[8]);

	//
	int set_ControllerParams(ControllerParams stParams);
	//
	int get_Controller_type(uint8_t net_id);

	int set_Order_To_Controller(uint8_t net_id, uint8_t Order, uint8_t setMark);

	int get_Controller_TargetStatus(uint8_t net_id, uint8_t setMark);
	//
	int get_create_tcp_ok(); //0创建成功 - 1 失败
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
	unsigned char recv_buf[LENGTH_TO_READ * 7];  // 空间放多一点儿
	unsigned char send_buf[LENGTH_TO_READ * 7];
	int m_create_tcp;//0创建成功 -1 失败
	JMutex  m_Lock;
};


#endif
