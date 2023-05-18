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
    // @brief �����¼�
    // @param eventName					-- [in ]�¼�����
    // @param desc					    -- [in ]�¼�����
    // @return bool
    /************************************************************************/ 
	virtual bool addEvent(const string &eventName, const string &desc = "") = 0;
    
    /************************************************************************/
    // @brief �޸��¼�����
    // @param eventName					-- [in ]�¼�����
    // @param desc					    -- [in ]�¼�����
    // @return bool
    /************************************************************************/
    virtual bool modifyDesc(const string &eventName, const string &desc) = 0;

    /************************************************************************/
    // @brief ɾ���¼�
    // @param eventName					-- [in ]�¼�����
    // @return bool
    /************************************************************************/
	virtual bool delEvent(const string &eventName) = 0;

    /************************************************************************/
    // @brief �õ��¼��б�
    // @return vector< pair<string, string> >   -- �¼��б�
    /************************************************************************/
	virtual vector< pair<string, string> > getEventList() = 0;

    /************************************************************************/
    // @brief �õ��¼�״̬
    // @param eventName					-- [in ]�¼�����
    // @return bool                     --falseΪδ������trueΪ�Ѵ���
    /************************************************************************/
	virtual bool getEventStatus(const string &eventName) = 0;

    /************************************************************************/
    // @brief ��λ�����¼�
    // @return void
    /************************************************************************/
    virtual void ResetAllEvent() = 0;

    /************************************************************************/
    // @brief ��λ�¼�
    // @param eventName					-- [in ]�¼�����
    // @return void
    /************************************************************************/
    virtual bool resetEvent(const string &eventName) = 0;
};

#endif // IWORKFLOW

