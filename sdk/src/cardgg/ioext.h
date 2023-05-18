#ifndef _GG_IO_EXT_H_
#define _GG_IO_EXT_H_

#include "jmutex.h"
#include "card/iio.h"

// IO������չ��
class CIOExt : public IIO
{
public:
    CIOExt(short cardIndex, short extIndex); //pDllName ��չ�����˶�������һ����ʱ�����˶���������dll����(ע������dllҪ����ͬһĿ¼)
    virtual ~CIOExt();

public:

	bool open();		// �� ���˶���һ��ʹ��ʱ�����ȴ��˶���   
	bool close();		// �ر�
	bool reset();		// ��λ

	bool loadConfig(char *path);		// ��������
	bool saveConfig(char *path);		// ��������
	bool isOpen();						// ��չ���Ƿ��
	bool SetCardIndex(short cardIndex);	// ������չ������

	//daд��
	bool SetExtDaValue(short chn,unsigned short value);
	bool SetExtDaVoltage(short chn,double value);

	virtual bool setDigOut(unsigned short portCount = 16, unsigned char portdata[] = NULL);
	virtual bool setDigOutBit(short doIndex,short value);	//ʵ�֣����ͨ�������ĳһλ
	virtual bool getDigOut(unsigned short portCount = 16, unsigned char* portdata = NULL);					//ʵ��,��ȡȫ��ͨ�����
	virtual bool getDigOutBit(short doIndex,short &value);	//ʵ��,��ȡͨ�������ĳһλ
	virtual bool getDigIn(unsigned short portCount = 16, unsigned char* portdata = NULL);					//ʵ�֣���ȡȫ��ͨ������
	virtual bool getDigInBit(short doIndex,short &value);	//ʵ�֣���ȡͨ������ĳһλ

	virtual  bool getAdcValue(unsigned int index , short &AdcValue);		//ʵ�֣� ��ȡģ��ֵ//
	virtual  bool getAdc( unsigned int index, double &AdcValue );			//ʵ��

	virtual  bool setAdcValue(unsigned int index , short AdcValue);		//ʵ�֣� ����ģ��ֵ//
	virtual  bool setAdc( unsigned int index, double dcValue );			//ʵ��

private:

    static bool m_isOpen;
    short m_cardIndex;
    short m_extIndex;
    long  m_gpiOut;
    static JMutex m_lock_IO;

	void sysEnterCriticalSection() {m_lock_IO.Lock();}
	void sysLeaveCriticalSection() {m_lock_IO.Unlock();}
};

#endif
