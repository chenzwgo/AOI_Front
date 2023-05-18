/**
 * 文件说明: 包含axis的专用接口
 * 作者: 
 * 日期:
 * 修改日志:              
 */ 
#ifndef _I_CONTINTERP_H_
#define _I_CONTINTERP_H_

#include "motor_common.h"
#include "any.h"
#include "card/icardcontinterp.h"

#include <vector>
using namespace std;

struct StBufMoveLine    //缓存直线插补
{
    double dVel;
    double dAcc;
    double dEndVel;
    StPos pos;						//点位信息

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


struct StBufMoveArc    //缓存圆弧插补
{
    double dVel;
    double dAcc;
    double dEndVel;
    StPos  midPos;						//圆弧中间点
    StPos  endPos;						//圆弧终点

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

struct StBufIO         //连续运动中输出IO
{
    int   index;                  //io索引
    bool  bOpen;                  //开或关

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

struct StBufDA      //连续运动中输出电压
{
    int      channel;                //通道
    double   value;                  //电压值

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

struct StBufDelay    //连续运动中延时
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

struct StContInterpolation         //连续插补运动
{
    char			cardName[NAME_LEN];              //卡名称
    unsigned short  crd;                             //坐标系号，取值范围： [1,2]
    unsigned short  evenTime;						 // 平滑时间： ms
    double          velMax;						     // 最大合成速度 pulse/ms
    double          accMax;						     // 最大加速度： pulse/ms^2
    vector<int>     axisList;                        // 连续插补轴列表
    vector<any>     bufList;                         // 插补动作列表(跑点,io,延时等)

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
    // @brief 连续插补运动,设置参数
    // @param name					-- [in]插补名称
    // @param desc					-- [in]插补描述
    // @param param                 -- [in]插补参数
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool SetParam(const string &name, const string &desc, const StContInterpolation &param) = 0;

    /************************************************************************/
    // @brief 连续插补运动,获取插补名称列表
    // @param nameList				-- [in]插补名称列表
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetContInterpList(vector<string> &nameList) = 0;

    /************************************************************************/
    // @brief 连续插补运动,获取插补名称列表
    // @param param				-- [in]插补参数
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetParam(const string &name, StContInterpolation & param) = 0;

    /************************************************************************/
    // @brief 连续插补运动,获取插补名称列表
    // @param desc				-- [in]插补描述
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual bool GetDesc(const string &name, string & desc) = 0;

    /************************************************************************/
    // @brief 连续插补运动,获取插补操作接口
    // @param cardName				-- [in]卡名称
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ICardContInterp* GetContInterpInterface(const string &cardName) = 0;
};

#endif