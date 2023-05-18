#ifndef _IIO_H
#define _IIO_H


class IIO
{
public:
        enum
        {
                IID_IO = 0x301
        };
        virtual ~IIO(){}
		
         // ��������ӿ�(ͨ���������)
		virtual bool setDigOutBit(short doIndex,short value) = 0;
        virtual bool getDigOutBit(short doIndex,short &value) = 0;
		virtual bool getDigInBit(short doIndex,short &value) = 0;       
		
		// ��ȡ����ͨ���ĵ�ֵ��portCount Ϊͨ������portdata Ϊ�洢ͨ�����ݵ�ֵ�����顣
		virtual bool setDigOut(unsigned short portCount, unsigned char* portdata){return false;}       
		virtual bool getDigOut(unsigned short portCount, unsigned char* portdata){return false;}
		virtual bool getDigIn(unsigned short portCount, unsigned char* portdata){return false;}

        // ר����������ӿ�
		virtual  bool getLmtsP(long &value){return false;}                                  // ��ȡ����λ����
        virtual  bool getLmtsN(long &value){return false;}                                  // ��ȡ����λ����
        virtual  bool getDrvAlm(long &value){return false;}                                // ��ȡ������������
        virtual  bool getHome(long &value){return false;}                                   // ��ȡԭ������

        virtual  bool getLmtsPBit(short bitnum,short &value){return false;}                  // ��ȡ����λ����ĳһλ
        virtual  bool getLmtsNBit(short bitnum,short &value){return false;}                  // ��ȡ����λ����ĳһλ
        virtual  bool getDrvAlmBit(short bitnum,short &value){return false;}                 // ��ȡ������������ĳһλ
        virtual  bool getHomeBit(short bitnum,short &value){return false;}                   // ��ȡ����λ����ĳһλ

        // ר����������ӿ�
        virtual  bool setDrvEnable(long value){return false;}                               // ����������ʹ��
        virtual  bool setClrAlarm(long value){return false;}                                // ���ñ������
        virtual  bool getDrvEnable(long &value){return false;}                              // ��ȡ������ʹ��
        virtual  bool getClrAlarm(long &value){return false;}                               // ��ȡ�������

        virtual  bool setDrvEnableBit(short bitnum,short value){return false;}               // ����������ʹ��ĳһλ
        virtual  bool setClrAlarmBit(short bitnum,short value){return false;}                // ���ñ������ĳһλ
        virtual  bool getDrvEnableBit(short bitnum,short &value){return false;}              // ��ȡ������ʹ��ĳһλ
        virtual  bool getClrAlarmBit(short bitnum,short &value){return false;}               // ��ȡ�������ĳһλ

        virtual  bool getAdcValue(unsigned int index , short &AdcValue){return false;}		// ��ȡģ��ֵ
        virtual  bool getAdc( unsigned int index, double &AdcValue ){return false;}

		virtual  bool setAdcValue(unsigned int index , short AdcValue){return false;}		// ����ģ��ֵ
		virtual  bool setAdc( unsigned int index, double AdcValue ){return false;}
};

#endif // IIO