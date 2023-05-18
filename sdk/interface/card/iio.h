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
		
         // 输入操作接口(通用输入输出)
		virtual bool setDigOutBit(short doIndex,short value) = 0;
        virtual bool getDigOutBit(short doIndex,short &value) = 0;
		virtual bool getDigInBit(short doIndex,short &value) = 0;       
		
		// 获取所有通道的的值，portCount 为通道数，portdata 为存储通道数据的值的数组。
		virtual bool setDigOut(unsigned short portCount, unsigned char* portdata){return false;}       
		virtual bool getDigOut(unsigned short portCount, unsigned char* portdata){return false;}
		virtual bool getDigIn(unsigned short portCount, unsigned char* portdata){return false;}

        // 专用输入操作接口
		virtual  bool getLmtsP(long &value){return false;}                                  // 获取正限位输入
        virtual  bool getLmtsN(long &value){return false;}                                  // 获取负限位输入
        virtual  bool getDrvAlm(long &value){return false;}                                // 获取驱动报警输入
        virtual  bool getHome(long &value){return false;}                                   // 获取原点输入

        virtual  bool getLmtsPBit(short bitnum,short &value){return false;}                  // 获取正限位输入某一位
        virtual  bool getLmtsNBit(short bitnum,short &value){return false;}                  // 获取负限位输入某一位
        virtual  bool getDrvAlmBit(short bitnum,short &value){return false;}                 // 获取驱动报警输入某一位
        virtual  bool getHomeBit(short bitnum,short &value){return false;}                   // 获取正限位输入某一位

        // 专用输出操作接口
        virtual  bool setDrvEnable(long value){return false;}                               // 设置驱动器使能
        virtual  bool setClrAlarm(long value){return false;}                                // 设置报警清除
        virtual  bool getDrvEnable(long &value){return false;}                              // 获取驱动器使能
        virtual  bool getClrAlarm(long &value){return false;}                               // 获取报警清除

        virtual  bool setDrvEnableBit(short bitnum,short value){return false;}               // 设置驱动器使能某一位
        virtual  bool setClrAlarmBit(short bitnum,short value){return false;}                // 设置报警清除某一位
        virtual  bool getDrvEnableBit(short bitnum,short &value){return false;}              // 获取驱动器使能某一位
        virtual  bool getClrAlarmBit(short bitnum,short &value){return false;}               // 获取报警清除某一位

        virtual  bool getAdcValue(unsigned int index , short &AdcValue){return false;}		// 获取模拟值
        virtual  bool getAdc( unsigned int index, double &AdcValue ){return false;}

		virtual  bool setAdcValue(unsigned int index , short AdcValue){return false;}		// 设置模拟值
		virtual  bool setAdc( unsigned int index, double AdcValue ){return false;}
};

#endif // IIO