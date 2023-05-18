#ifndef IACTION_H
#define IACTION_H

class IAction
{
public:
    virtual ~IAction(){}

    /************************************************************************/
    // @brief 设置参数
    // @param param					-- [in ]动作参数
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual int setParam(void *param) = 0;

    /************************************************************************/
    // @brief 启动动作(实现时必须非阻塞)
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual int start() = 0;

    /************************************************************************/
    // @brief 暂停动作
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual int pause() = 0;

    /************************************************************************/
    // @brief 停止动作
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual int stop() = 0;

    /************************************************************************/
    // @brief 继续动作
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual int continues() = 0;

    /************************************************************************/
    // @brief 动作是否结束
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual bool isFinished() = 0;

    /************************************************************************/
    // @brief 动作执行结果
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
    virtual bool result() = 0;

    /************************************************************************/
    // @brief 获取错误码
    // @return EM_ERR_CONTROL
    /************************************************************************/ 
	virtual int GetLastError() = 0;
};

#endif // IACTION

