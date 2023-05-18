/**
 * �ļ�˵��: ����axis��ר�ýӿ�
 * ����: 
 * ����:
 * �޸���־:              
 */ 
#ifndef _I_CONTINTERP_H_
#define _I_CONTINTERP_H_

#include "motor_common.h"
#include "any.h"
#include "card/icardcontinterp.h"

#include <vector>
using namespace std;

struct StBufMoveLine    //����ֱ�߲岹
{
    double dVel;
    double dAcc;
    double dEndVel;
    StPos pos;						//��λ��Ϣ

    StBufMoveLine()
    {
        dVel = 0.0;
        dAcc = 0.0;
        dEndVel = 0.0;
    }

    StBufMoveLine& operator=(const StBufMoveLine &other)
    {
        if (this == &other)
        {
            return *this;
        }
        pos = other.pos;
        dVel = other.dVel;
        dAcc = other.dAcc;
        dEndVel = other.dEndVel;
        return *this;
    }

    bool operator==(const StBufMoveLine &other)
    {
        return pos == other.pos &&
            dVel  == other.dVel &&
            dAcc  == other.dAcc &&
            dEndVel == other.dEndVel;
    }
};


struct StBufMoveArc    //����Բ���岹
{
    double dVel;
    double dAcc;
    double dEndVel;
    StPos  midPos;						//Բ���м��
    StPos  endPos;						//Բ���յ�

    StBufMoveArc()
    {
        dVel = 0.0;
        dAcc = 0.0;
        dEndVel = 0.0;
    }

    StBufMoveArc& operator=(const StBufMoveArc &other)
    {
        if (this == &other)
        {
            return *this;
        }

        dVel = 0.0;
        dAcc = 0.0;
        dEndVel = 0.0;
        return *this;
    }

    bool operator==(const StBufMoveArc &other)
    {
        return midPos == other.midPos &&
            endPos == other.endPos &&
            dVel  == other.dVel &&
            dAcc  == other.dAcc &&
            dEndVel == other.dEndVel;
    }
};

struct StBufIO         //�����˶������IO
{
    int   index;                  //io����
    bool  bOpen;                  //�����

    StBufIO()
    {
        index = -1;
        bOpen  = false;
    }
    StBufIO& operator=(const StBufIO &other)
    {
        if (this == &other)
        {
            return *this;
        }
        index = other.index;
        bOpen = other.bOpen;
        return *this;
    }

    bool operator==(const StBufIO &other)
    {
        return index == other.index &&
            bOpen == other.bOpen;
    }
};

struct StBufDA      //�����˶��������ѹ
{
    int      channel;                //ͨ��
    double   value;                  //��ѹֵ

    StBufDA()
    {
        channel = -1;
        value  = 0;
    }

    StBufDA& operator=(const StBufDA &other)
    {
        if (this == &other)
        {
            return *this;
        }
        channel = other.channel;
        value = other.value;
        return *this;
    }

    bool operator==(const StBufDA &other)
    {
        return channel == other.channel &&
            value == other.value;
    }
};

struct StBufDelay    //�����˶�����ʱ
{
    int time;           // ms
    StBufDelay()
    {
        time = 0;
    }

    StBufDelay& operator=(const StBufDelay &other)
    {
        if (this == &other)
        {
            return *this;
        }
        time = other.time;
        return *this;
    }
    bool operator==(const StBufDelay &other)
    {
        return time == other.time;
    }
};

struct StContInterpolation         //�����岹�˶�
{
    char			cardName[NAME_LEN];              //������
    unsigned short  crd;                             //����ϵ�ţ�ȡֵ��Χ�� [1,2]
    unsigned short  evenTime;						 // ƽ��ʱ�䣺 ms
    double          velMax;						     // ���ϳ��ٶ� pulse/ms
    double          accMax;						     // �����ٶȣ� pulse/ms^2
    vector<int>     axisList;                        // �����岹���б�
    vector<any>     bufList;                         // �岹�����б�(�ܵ�,io,��ʱ��)

    StContInterpolation()
    {
        memset(cardName, 0, NAME_LEN);
        crd           = 1;
        velMax        = 10;
        accMax        = 10;
        evenTime      = 10;
    }

    bool clear()
    {
        memset(cardName, 0, NAME_LEN);
        crd           = 1;
        velMax        = 10;
        accMax        = 10;
        evenTime      = 10;
        axisList.clear();
        bufList.clear();
        return true;
    }

    StContInterpolation& operator=(const StContInterpolation &other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        strcpy(cardName, other.cardName);
        crd = other.crd;
        velMax = other.velMax;
        accMax = other.accMax;
        evenTime = other.evenTime;

        axisList.clear();
        for (int i=0; i<other.axisList.size(); i++)
        {
            axisList.push_back(other.axisList[i]);
        }

        bufList.clear();
        for (int i=0; i<other.bufList.size(); i++)
        {
            bufList.push_back(other.bufList[i]);
        }
        return *this;
    }
};

class IContinterp
{
public:
    enum
    {
        IDD_CONT_INTERP = 0x24
    };

    virtual ~IContinterp(){}

    /************************************************************************/
    // @brief �����岹�˶�,���ò���
    // @param name					-- [in]�岹����
    // @param desc					-- [in]�岹����
    // @param param                 -- [in]�岹����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool SetParam(const string &name, const string &desc, const StContInterpolation &param) = 0;

    /************************************************************************/
    // @brief �����岹�˶�,��ȡ�岹�����б�
    // @param nameList				-- [in]�岹�����б�
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetContInterpList(vector<string> &nameList) = 0;

    /************************************************************************/
    // @brief �����岹�˶�,��ȡ�岹�����б�
    // @param param				-- [in]�岹����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetParam(const string &name, StContInterpolation & param) = 0;

    /************************************************************************/
    // @brief �����岹�˶�,��ȡ�岹�����б�
    // @param desc				-- [in]�岹����
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetDesc(const string &name, string & desc) = 0;

    /************************************************************************/
    // @brief �����岹�˶�,��ȡ�岹�����ӿ�
    // @param cardName				-- [in]������
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ICardContInterp* GetContInterpInterface(const string &cardName) = 0;
};

#endif