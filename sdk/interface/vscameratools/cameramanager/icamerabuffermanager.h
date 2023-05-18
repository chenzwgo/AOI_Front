#ifndef ICAMERA_BUFFER_MANAGER_H
#define ICAMERA_BUFFER_MANAGER_H

#define MAX_CHANNEL_COUNT    8

typedef void (*FnDataRecvFunction)(void *, unsigned char *, int , int);

class IBufferManager
{
public:
    enum
    {
        IID_CAMERA_BUFFER_MANAGER = 0x110
    };
    virtual ~IBufferManager() {};

    virtual bool init() = 0;
    virtual bool startProcess() = 0;
    virtual void getBufferSize(int &width, int &height) = 0;
    virtual const unsigned char *getBuffer(int iChannel, int lineOffset = 0, int iLineNumb = -1) = 0;
    virtual const unsigned char *getBufferAdress(int iChannel) = 0;

    virtual bool continuations(int channel, FnDataRecvFunction fnTriggerFunction, void *pInParam) = 0; //block == 1, 取完就清
    virtual bool stopContinuations(int channel) = 0;

    virtual bool reset() = 0;
    virtual bool abort() =0;

    virtual bool isBusying() = 0;
    virtual bool isInitFinished() = 0;
    virtual bool updateImageBuffer() = 0;
    virtual bool stopInit() = 0;   //停止初始化接口， 防止还在初始化时，外部调用被阻塞导致无法关闭软件问题

    virtual int getCamStatus(int iGroupIndex) = 0;
};


class IBufferManagerFactory
{
public:
	enum
	{
		IID_CAMERA_BUFFER_MANAGER_FACTORY = 0x111
	};
	virtual ~IBufferManagerFactory() {};

	virtual IBufferManager* createBufferManager(string SceneName) = 0;
	virtual void relealseBufferManager(IBufferManager* pBufferManager) = 0;
	virtual void relealseBufferManager(string SceneName) = 0;
};
#endif // ICAMERA_BUFFER_MANAGER_H