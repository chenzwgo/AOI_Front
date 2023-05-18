#ifndef _ICOM_H_
#define _ICOM_H_


class ICom
{
public:

	virtual ~ICom(){}
public:
    
    virtual bool OpenCom() = 0;
    virtual bool CloseCom() = 0;
    virtual void SetComName(const char* pName) = 0;
    virtual int WriteCom(void* pmsg ,unsigned int msglen) = 0;
    virtual int ReadCom(void* buffer ,unsigned int buffersize) = 0;
    virtual bool IsOpen() = 0;
    virtual bool FlushCom() =0;
    

   /* Parity  0-4=None,Odd,Even,Mark,Space    */
    /* ByteSize Number of bits/byte, 4-8        */
   /* StopBits 1,3,2 = 1, 1.5, 2               */
    virtual bool SetComState(unsigned long BaudRate,unsigned long Parity,unsigned long ByteSize,unsigned long StopBits) = 0;
	virtual bool GetComState(unsigned long &BaudRate,unsigned long &Parity,unsigned long &ByteSize,unsigned long &StopBits) = 0;

	virtual bool SetTimeOut( unsigned long ReadIntervalTimeout20us,
		unsigned long ReadTotalTimeoutMultiplier20us,
		unsigned long ReadTotalTimeoutConstant20us,
		unsigned long WriteTotalTimeoutMultiplier20us,
		unsigned long WriteTotalTimeoutConstant20us ) = 0;
	virtual bool GetTimeOut( unsigned long &ReadIntervalTimeout20us,
		unsigned long &ReadTotalTimeoutMultiplier20us,
		unsigned long &ReadTotalTimeoutConstant20us,
		unsigned long &WriteTotalTimeoutMultiplier20us,
		unsigned long &WriteTotalTimeoutConstant20us ) = 0;
};


#endif //_ICOM_H_
