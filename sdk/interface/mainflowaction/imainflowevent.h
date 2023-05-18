#ifndef IMAINFLOW_EVENT_H
#define IMAINFLOW_EVENT_H

#include <string>
#include <vector>
using namespace std;

class IMainFlowEvent
{
public:
    enum
    {
        IDD_MAIN_FLOW_EVENT = 0x004
    };

    virtual ~IMainFlowEvent(){}

    /************************************************************************/
    // @brief 增加事件
    // @param eventName					-- [in ]事件名称
    // @param desc					    -- [in ]事件描述
    // @return bool
    /************************************************************************/ 
	virtual bool addEvent(const string &eventName, const string &desc = "") = 0;
    
    /************************************************************************/
    // @brief 修改事件描述
    // @param eventName					-- [in ]事件名称
    // @param desc					    -- [in ]事件描述
    // @return bool
    /************************************************************************/
    virtual bool modifyDesc(const string &eventName, const string &desc) = 0;

    /************************************************************************/
    // @brief 删除事件
    // @param eventName					-- [in ]事件名称
    // @return bool
    /************************************************************************/
	virtual bool delEvent(const string &eventName) = 0;

    /************************************************************************/
    // @brief 得到事件列表
    // @return vector< pair<string, string> >   -- 事件列表
    /************************************************************************/
	virtual vector< pair<string, string> > getEventList() = 0;

    /************************************************************************/
    // @brief 得到事件状态
    // @param eventName					-- [in ]事件名称
    // @return bool                     --false为未触发，true为已触发
    /************************************************************************/
	virtual bool getEventStatus(const string &eventName) = 0;

    /************************************************************************/
    // @brief 复位所有事件
    // @return void
    /************************************************************************/
    virtual void ResetAllEvent() = 0;

    /************************************************************************/
    // @brief 复位事件
    // @param eventName					-- [in ]事件名称
    // @return void
    /************************************************************************/
    virtual bool resetEvent(const string &eventName) = 0;
};

#endif // IWORKFLOW

