#ifndef _GG_IO_H_
#define _GG_IO_H_

#include "card/iio.h"
// ͨ��IO������
class CIO : public IIO
{
public:
	CIO(unsigned short cardIndex);
	virtual ~CIO();

	virtual bool setDigOut(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool setDigOutBit(short doIndex,short value);
	virtual bool getDigOut(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool getDigOutBit(short doIndex,short &value);
	virtual bool getDigIn(unsigned short portCount = 16, unsigned char* portdata = 0);
	virtual bool getDigInBit(short doIndex,short &value);

	// ��������ӿ�
	virtual  bool getLmtsP(long &value);                                  // ��ȡ����λ����
	virtual  bool getLmtsN(long &value);                                  // ��ȡ����λ����
	virtual  bool getDrvAlm(long &value);                                 // ��ȡ������������
	virtual  bool getHome(long &value);                                   // ��ȡԭ������


	virtual  bool getLmtsPBit(short bitnum,short &value);                  // ��ȡ����λ����ĳһλ
	virtual  bool getLmtsNBit(short bitnum,short &value);                  // ��ȡ����λ����ĳһλ
	virtual  bool getDrvAlmBit(short bitnum,short &value);                 // ��ȡ������������ĳһλ
	virtual  bool getHomeBit(short bitnum,short &value);                   // ��ȡ����λ����ĳһλ

	// ��������ӿ�
	virtual  bool setDrvEnable(long value);                               // ����������ʹ��
	virtual  bool setClrAlarm(long value);                                // ���ñ������
	virtual  bool getDrvEnable(long &value);                              // ��ȡ������ʹ��
	virtual  bool getClrAlarm(long &value);                               // ��ȡ�������

	virtual  bool setDrvEnableBit(short bitnum,short value);               // ����������ʹ��ĳһλ
	virtual  bool setClrAlarmBit(short bitnum,short value);                // ���ñ������ĳһλ
	virtual  bool getDrvEnableBit(short bitnum,short &value);              // ��ȡ������ʹ��ĳһλ
	virtual  bool getClrAlarmBit(short bitnum,short &value);               // ��ȡ�������ĳһλ

	virtual  bool getAdcValue(unsigned int index , short &AdcValue);		// ��ȡģ��ֵ
	virtual  bool getAdc( unsigned int index, double &AdcValue );

private:
	unsigned short m_cardIndex;
};
#endif
