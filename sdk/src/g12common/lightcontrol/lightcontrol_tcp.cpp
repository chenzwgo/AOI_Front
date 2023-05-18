
#include<string.h>
#include <assert.h>
#include "lightcontrol_tcp.h"

# define SHUT_RDWR 2

#pragma comment(lib, "ws2_32.lib")



/* Table of CRC values for high-order byte */
static const uint8_t table_crc_hi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* Table of CRC values for low-order byte */
static const uint8_t table_crc_lo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

////////////////c++  class wrap:///////////////////////////////////////////////////////////

LightControlTcp::LightControlTcp(char *ip, uint16_t port)
{
	int ret;
	//����tcp client����ÿ���豸Ҫһ����������ip��ַ���˿ڿ�����Ϊһ����
	m_create_tcp = -1;
	ret = create_tcp_client_ctx(ip, port); // ip�� port
	if (ret < 0) {
		printf("����tcp client����ʧ��\n");
		//exit(-1);
	}
}

LightControlTcp::~LightControlTcp()
{
	//����tcp client����
	//tcpCtx.tcp_client_destroy();
	tcp_client_destroy();
}
int LightControlTcp::get_create_tcp_ok()
{
	return m_create_tcp;
}
int LightControlTcp::create_tcp_client_ctx(char *ip, uint16_t port)
{
	this->sockVersion = MAKEWORD(2, 2);

	if (WSAStartup(this->sockVersion, &this->data) != 0) {
		return -1;
	}

	this->serAddr.sin_family = AF_INET;
	this->serAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &this->serAddr.sin_addr);

	this->sclient = -1;
	//this->recv_len = LENGTH_TO_READ * 7;
	m_create_tcp = 0;
	return 0;
}

int LightControlTcp::tcp_client_send_and_recv(int send_len, int recv_len)
{
	fd_set rset;
	struct timeval tv;
	struct timeval *p_tv;

	// ����socket
	this->sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->sclient == INVALID_SOCKET) {
		printf("invalid socket!");
		return -1;
	}

	// ���ӡ�
#if 0
	if (connect(this->sclient, (struct sockaddr *)&this->serAddr, sizeof(this->serAddr)) == SOCKET_ERROR) {
		//����ʧ�ܡ�TCP Ĭ��75�볬ʱ���˷���ʱ�������δ������UI�Ῠ��
		printf("connect error !");
		closesocket(this->sclient);
		return -1;
	}
#else
	//���ӳ�ʱ100����, �������߲�ͨʱ�ȴ�TCP 75�볬ʱ����UI����
	struct timeval timeout;
	fd_set r;
	unsigned long ul = 1;
	int error = -1;
	bool bret = false;
	int len = sizeof(int);
	ioctlsocket(this->sclient, FIONBIO, &ul);   //����Ϊ������ģʽ

												//�������ʧ�ܣ������ó�ʱ
	if (-1 == connect(this->sclient, (struct sockaddr *)&this->serAddr, sizeof(this->serAddr))) {
		FD_ZERO(&r);
		FD_SET(this->sclient, &r);
		timeout.tv_sec = 0; //���ӳ�ʱ100����
		timeout.tv_usec = 100000;
		if (select(0, NULL, &r, NULL, &timeout) > 0) {
			getsockopt(this->sclient, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
			if (error == 0)
				bret = true;
			else
				bret = false;
		}
		else {
			bret = false;
		}
	}
	if (bret == false) {
		//����ʧ�� 
		printf("connect error !");
		closesocket(this->sclient);
		return -1;
	}
	// ���ӳɹ���continue....
	ul = 0;
	ioctlsocket(this->sclient, FIONBIO, &ul); //����Ϊ����ģʽ ����ʱ��Ϊtimeoutʱ��
#endif

											  // ����
	send(this->sclient, (const char *)this->send_buf, send_len, 0);

	// ����,select����
	FD_ZERO(&rset);
	FD_SET(this->sclient, &rset);

	//timeout = 0.2s     
	tv.tv_sec = 0;
	tv.tv_usec = 200000;  // 0.2S��ʱ���������200ms֮��û�л�Ӧ��Ϣ
	p_tv = &tv;

	int s_rc;
	while ((s_rc = select(this->sclient + 1, &rset, NULL, NULL, p_tv)) == -1) {
		if (errno == EINTR) {
			printf("A non blocked signal was caught\n");
			/* Necessary after an error */
			FD_ZERO(&rset);
			FD_SET(this->sclient, &rset);
		}
		else {
			return -1; // select error
		}
	}

	if (s_rc == 0) {
		errno = ETIMEDOUT;
		return 0; // select  timeout
	}
	// select �з�����Ϣ����ȡ
	//��0׼����������
	memset(this->recv_buf, 0, sizeof(LightSource_Params) + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
	int ret = recv(this->sclient, (char *)this->recv_buf, recv_len, 0);
	if (ret > 0) {
		this->recv_buf[ret] = 0x00;
		//printf(this->recv_buf);
	}
	// �ر�����
	if (this->sclient != -1) {
		shutdown(this->sclient, SHUT_RDWR);
		closesocket(this->sclient);
		this->sclient = -1;
	}

	return ret;   //���ؽ��յ����ֽ���
}

#if 0
// ���͹�Դ���ƽṹ�壬���ؽṹ��
static LightSource_Params* wr_params(struct tcp_client_ctx_s *thiz, LightSource_Params * p_Params)
{
	int ret;
	int data_len = sizeof(LightSource_Params) + 2;  // Ҫ���͵Ľṹ�峤�ȣ�����Ҫ��2�ֽ�CRC

													//��0׼����������
	memset(this->send_buf, 0, sizeof(LightSource_Params) + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
																// ����Ҫ���͵�����
	memcpy(this->send_buf, (unsigned char *)p_Params, sizeof(LightSource_Params));
	// ����CRCУ�飬��λ�ֽ���ǰ
	uint16_t crc = crc16(this->send_buf, sizeof(LightSource_Params));
	this->send_buf[sizeof(LightSource_Params)] = crc >> 8;
	this->send_buf[sizeof(LightSource_Params) + 1] = crc & 0x00FF;

	ret = this->send_and_recv(thiz, data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= sizeof(LightSource_Params)) {
		return (LightSource_Params *)(this->recv_buf);
	}
	return NULL;
}
#endif

int LightControlTcp::tcp_client_destroy()
{
	if (this->sclient != -1) {
		shutdown(this->sclient, SHUT_RDWR);
		closesocket(this->sclient);
		this->sclient = -1;
	}

	WSACleanup();

	return 0;
}


/* 1:��ȡ����������-���¹�Դ���߻��ι�
��
0x55aa(��ͷ)+0x01(ָ��)
��
0x55aa(��ͷ)+0x01(ָ��)+ls_type
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::get_ctr_type(uint8_t &ls_type)
{
	int ret;
	int data_len = 3;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

							//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x01;

	ret = this->tcp_client_send_and_recv(data_len, data_len + 1 + 2);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 4 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x01) {
		ls_type = this->recv_buf[3];    //printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}

/*
2:���Ƴ�����Ƶ��ģʽ
��
0x55aa(��ͷ)+0x02(ָ��)+ls_model(8λ)--0Ƶ�� 1����
��
0x55aa(��ͷ)+0x02(ָ��)+�������--0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_flash_mode(uint8_t ls_model)
{
	int ret;
	int data_len = 4;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

							//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x02;
	this->send_buf[3] = ls_model;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x02) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}

/*
3:����ģʽ�µƹܿ���
��
0x55aa(��ͷ)+0x03(ָ��)+ls_switch
��
0x55aa(��ͷ)+0x03(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_ls_switch(LightSourceSwitch_t &ls_switch)
{
	int ret;
	int data_len = sizeof(LightSourceSwitch_t) + 3;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC
	ls_switch.lss_switch.reserve = 0x00;
	//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x03;
	//this->send_buf[3]= ls_model;
	memcpy(&(this->send_buf[3]), &(ls_switch.ui_switch), sizeof(ls_switch.ui_switch));

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x03 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}

/*
4:�����Գ��ģʽ
��
0x55aa(��ͷ)+0x04(ָ��)+ls_auto_charge(8λ)
��
0x55aa(��ͷ)+0x04(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_auto_charge(uint8_t ls_auto_charge)
{
	int ret;
	int data_len = 4;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

							//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x04;
	this->send_buf[3] = ls_auto_charge;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x04) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}

/*
5:�������ָ��-��ÿɨ��һ����Ҫ���������Ϊ��һ����׼��
��
0x55aa(��ͷ)+0x05(ָ��)+ls_config_id(8λ)   --��0��ʼ
��
0x55aa(��ͷ)+0x05(ָ��)+uint16_t[8](ÿ��ͨ��ʵ�ʴ�������)
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::clear_counter(uint8_t ls_config_id)
{
	CISAPUGuard<JMutex> autoLock(m_Lock);
	int ret;
	int data_len = 4;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC
	//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x05;
	this->send_buf[3] = ls_config_id;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x05 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}

	//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
	return -1;
	
}

/*
6:�·�ÿ��ͨ����������
��
0x55aa(��ͷ)+0x06(ָ��)+ls_current+CRC(16λ)
��
0x55aa(��ͷ)+0x06(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_ls_current(uint16_t ls_current[6][4])
{
	int ret;
	int data_len = sizeof(uint16_t) * 6 * 4 + 3 + 2;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

														   //��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x06;
	memcpy(&(this->send_buf[3]), ls_current, sizeof(uint16_t) * 6 * 4);

	// ����CRCУ�飬��λ�ֽ���ǰ
	uint16_t crc = crc16(this->send_buf, sizeof(uint16_t) * 6 * 4 + 3);
	this->send_buf[sizeof(uint16_t) * 6 * 4 + 3] = crc >> 8;
	this->send_buf[sizeof(uint16_t) * 6 * 4 + 3 + 1] = crc & 0x00FF;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3&& this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x06) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}

/*
7:����ͨ���·�
��
0x55aa(��ͷ)+0x07(ָ��)+ls_config_id(8λ)+Nͨ��(8λ)+MutFlashParams[5]+CRC(16λ)
��
0x55aa(��ͷ)+0x07(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_MutFlashParams_by_channel(uint8_t ls_config_id, uint8_t channel, MutFlashParams mfp[5])
{
	int ret;
	int data_len = 5 + 5 * sizeof(MutFlashParams) + 2;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

															 //��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x07;
	this->send_buf[3] = ls_config_id;
	this->send_buf[4] = channel;
	memcpy(&(this->send_buf[5]), mfp, 5 * sizeof(MutFlashParams));

	// ����CRCУ�飬��λ�ֽ���ǰ
	uint16_t crc = crc16(this->send_buf, 5 + 5 * sizeof(MutFlashParams));
	this->send_buf[5 + 5 * sizeof(MutFlashParams)] = crc >> 8;
	this->send_buf[5 + 5 * sizeof(MutFlashParams) + 1] = crc & 0x00FF;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >=3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x07 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}


/*
8:����ͨ���·�
��
0x55aa(��ͷ)+0x08(ָ��)+ls_config_id(8λ)+MutFlashParams[8][5]+CRC(16λ)
��
0x55aa(��ͷ)+0x08(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_MutFlashParams(uint8_t ls_config_id, MutFlashParams mfp[8][5])
{
	int ret;
	int data_len = 4 + 5 * 8 * sizeof(MutFlashParams) + 2;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

																 //��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x08;
	this->send_buf[3] = ls_config_id;
	memcpy(&(this->send_buf[4]), mfp, 5 * 8 * sizeof(MutFlashParams));

	// ����CRCУ�飬��λ�ֽ���ǰ
	uint16_t crc = crc16(this->send_buf, 4 + 5 * 8 * sizeof(MutFlashParams));
	this->send_buf[4 + 5 * 8 * sizeof(MutFlashParams)] = crc >> 8;
	this->send_buf[4 + 5 * 8 * sizeof(MutFlashParams) + 1] = crc & 0x00FF;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x08 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}

/*
9:һ���·���������
��
0x55aa(��ͷ)+0x09(ָ��)+MutFlashParams[2][8][5]+CRC(16λ)
��
0x55aa(��ͷ)+0x09(ָ��)+�������   --0ʧ�� 1 ����
��������0�ɹ���-1ʧ��
*/
int LightControlTcp::set_all_MutFlashParams(MutFlashParams mfp[2][8][5])
{
	int ret;
	int data_len = 3 + 2 * 5 * 8 * sizeof(MutFlashParams) + 2;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC

																	 //��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x09;
	memcpy(&(this->send_buf[3]), mfp, 2 * 5 * 8 * sizeof(MutFlashParams));

	// ����CRCУ�飬��λ�ֽ���ǰ
	uint16_t crc = crc16(this->send_buf, 3 + 2 * 5 * 8 * sizeof(MutFlashParams));
	this->send_buf[3 + 2 * 5 * 8 * sizeof(MutFlashParams)] = crc >> 8;
	this->send_buf[3 + 2 * 5 * 8 * sizeof(MutFlashParams) + 1] = crc & 0x00FF;

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 3 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x09 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}

int LightControlTcp::get_counter(uint8_t ls_config_id, uint16_t trig[8])
{
	int ret;
	//int data_len = 3;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC
		int data_len = 4;       //+ 2;  // Ҫ�������ݳ��ȣ�����Ҫ��2�ֽ�CRC
	int data_rev = 19;
	//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�����Ҫ��2�ֽ�CRC
											  // ����Ҫ���͵�����
	this->send_buf[0] = 0x55;
	this->send_buf[1] = 0xaa;
	this->send_buf[2] = 0x0a;
	this->send_buf[3] = ls_config_id;

	ret = this->tcp_client_send_and_recv(data_len, data_rev);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 4 && this->recv_buf[0] == 0x55 && this->recv_buf[1] == 0xaa && this->recv_buf[2] == 0x0a) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		memcpy(trig, &(this->recv_buf[3]), sizeof(uint16_t) * 8);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}
}

uint16_t LightControlTcp::crc16(uint8_t *buffer, uint16_t buffer_length)
{
	uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
	uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
	unsigned int i; /* will index into CRC lookup */

					/* pass through message buffer */
	while (buffer_length--) {
		i = crc_hi ^ *buffer++; /* calculate the CRC  */
		crc_hi = crc_lo ^ table_crc_hi[i];
		crc_lo = table_crc_lo[i];
	}

	return (crc_hi << 8 | crc_lo);
}
//add 2020 6.1
/*
����:0x01������ID�ţ�+0x02��������ƣ�+ ControllerParams_t(�ṹ��)
����:0x01
*/
int LightControlTcp::set_ControllerParams(ControllerParams stParams)
{
	int ret;
	int data_len = sizeof(ControllerParams);      
										
	memset(this->send_buf, 0, data_len + 20); // �����һ��									  
	memcpy(&(this->send_buf[0]), &stParams, sizeof(ControllerParams));

	ret = this->tcp_client_send_and_recv(data_len, data_len);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 2 && this->recv_buf[0] == stParams.net_id && this->recv_buf[1] == 0x01 ) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],ret );
		return -1;
	}
}
/*
1->��ȡ������
����:0x01������ID�ţ�+0x01��������ƣ�+0x55+0xaa
����:0x01
*/
int LightControlTcp::get_Controller_type(uint8_t net_id)
{
	int ret;
	int data_len = 4;        // Ҫ�������ݳ���

							//��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�
											  
	this->send_buf[0] = net_id;
	this->send_buf[1] = 0x01;
	this->send_buf[2] = 0x55;
	this->send_buf[3] = 0xaa;

	ret = this->tcp_client_send_and_recv(data_len, data_len + 1 + 2 +3);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 1 && this->recv_buf[0] == net_id && this->recv_buf[1] == 0x01) {
   //printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}
/*

����:0x01������ID�ţ�+0x03��������ƣ�+ 1���ֽڣ�����ģʽ���ã�0-�ȼ�ഥ��;1-��̬��������
����:0x01

����:0x01������ID�ţ�+0x04��������ƣ�+ 1���ֽڣ�����ģʽ���ã�0-ֹͣ����;1-��ʼ�������� -- ��ʼ����ָ��1�����ھ�̬����ģʽʱ���࿪ʼ��̬����Ƶ����2�����ڵȼ�ഥ��ʱ���࿪ʼ��ˮ�ȼ��ܴ�������
����:0x01

*/
int LightControlTcp::set_Order_To_Controller(uint8_t net_id, uint8_t Order, uint8_t setMark)
{
	int ret;
	int data_len = 3;        // Ҫ�������ݳ���

							 //��0׼����������
	memset(this->send_buf, 0, data_len + 20); // �����һ�Σ�

	this->send_buf[0] = net_id;
	this->send_buf[1] = Order;
	this->send_buf[2] = setMark;

	ret = this->tcp_client_send_and_recv(data_len, data_len + 1 + 2 );
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 1 && this->recv_buf[0] == net_id && this->recv_buf[1] == 0x01) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		return 0;
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}
//0x01������ID��:0x01=�ͻ���1;0x02=�ͻ���2��+0x05��������ƣ�+0x01(�߼�=0x01;ƽ��=0x02)+0x01 / 0x02 (0x01=��ʼ����; 0x02=��������)
int LightControlTcp::get_Controller_TargetStatus(uint8_t net_id, uint8_t setMark)
{
	int ret;
	int data_len = 0; 
	ret = this->tcp_client_send_and_recv(data_len, 5);
	//printf("�յ�����˷�����%d�ֽ�\n", ret);
	if (ret >= 4 && this->recv_buf[0] == net_id&& this->recv_buf[1] == 0x05 && this->recv_buf[2] == setMark) {
		//printf("�յ�����˷�����ls_type=%d\n",  this->recv_buf[3]);
		if (0x01 == this->recv_buf[3])
		{
			return 1;
		}
		else if (0x02 == this->recv_buf[3])
		{
			return 2;
		}
		
	}
	else {
		//printf("�յ�����˷���������0x%x,0x%x,0x%x,0x%x,ret=%d\n",this->recv_buf[0],this->recv_buf[1],this->recv_buf[2], this->recv_buf[3],ret );
		return -1;
	}

}