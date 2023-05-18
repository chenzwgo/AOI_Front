#ifndef WORK_MOTOR_INTERPOLATION_H
#define WORK_MOTOR_INTERPOLATION_H
#include "work_inter_delay.h"
#include "work_inter_ln_point.h"
#include "work_io_output.h"

#include <vector>
#include "../actioncfgdefine.h"
#include "rtdefine.h"

using namespace std;


enum
{
    INTER_TYPE_LN_POINT = 0, // 点位直线插补类型
    INTER_TYPE_IO,           // IO类型
    INTER_TYPE_DELAY	     // 延时类型
};

struct WORK_INTER_UNION
{
    string			description;       //点位功能描述
    int             type;             // 0 INTER_TYPE_LN_POINT ；1 INTER_TYPE_IO 
    union
    {
        WORK_INTER_LN_POINT    *pPoint;
        WORK_IO_OUTPUT             *pIo; 
        WORK_INTER_DELAY	*pDelay;
    };
    WORK_INTER_UNION()
    {
        description = "";
        type = 0;
    }
    bool clear()
    {
        switch(type)
        {
        case INTER_TYPE_LN_POINT:
            SAFE_DELETE(pPoint);
            break;
        case INTER_TYPE_IO:
            SAFE_DELETE(pIo);
            break;
        case INTER_TYPE_DELAY:
            SAFE_DELETE(pDelay);
            break;
        default:
            break;
        }
        return true;
    }

    bool operator==(const WORK_INTER_UNION &other)
    {
        bool bItem = false;

        if (NULL == pPoint && NULL == other.pPoint&& NULL == other.pIo && NULL == other.pDelay) {bItem = true;}
        else if (type == INTER_TYPE_LN_POINT && pPoint && other.pPoint) {bItem = (*pPoint == *other.pPoint);}
        else if (type == INTER_TYPE_IO && pIo && other.pIo) {bItem = (*pIo == *other.pIo);}
        else if (type == INTER_TYPE_DELAY  && pDelay && other.pDelay) {bItem = (*pDelay == *other.pDelay);}


        return description==other.description&&
            type == other.type
            && bItem;
    }
};
typedef std::vector< WORK_INTER_UNION > VecInterUnion;

struct WORK_MOTOR_INTERPOLATION : ACTION_PARAM
{
    char			stationname[ITEM_NAME_LEN];      //工站名称// 这个工站只能在一个卡上的最多4个轴
    unsigned short  crd;                             //坐标系号，取值范围： [1,2]
    unsigned short  evenTime;						 // 平滑时间： ms
    double          synVelMax;						        // 最大合成速度 pulse/ms
    double          synAccMax;						        // 最大加速度： pulse/ms^2
    VecInterUnion   *pVecInterUnion;

    WORK_MOTOR_INTERPOLATION()
    {
        memset(stationname, 0, ITEM_NAME_LEN);  
        crd           = 1;
        synVelMax     = 0;
        synAccMax     = 0;
        evenTime      = 0;
        pVecInterUnion = NULL;
    }

    bool clear()
    {
        if (pVecInterUnion != NULL){
            for (VecInterUnion::iterator iter = pVecInterUnion->begin(); iter != pVecInterUnion->end(); ++iter) {
                iter->clear();
            }
            SAFE_DELETE(pVecInterUnion);
        }
        return true;
    }

    bool operator == (const WORK_MOTOR_INTERPOLATION &other)
    {
        bool res = false;

        res = stationname == other.stationname &&
            crd   == other.crd &&
            synVelMax     == other.synVelMax &&
            synAccMax    == other.synAccMax &&
            evenTime     == other.evenTime ;
        if (res)
        {
            if ( pVecInterUnion != NULL && other.pVecInterUnion !=NULL)
            {
                if ( pVecInterUnion->size() == other.pVecInterUnion->size())// size 一样
                {
                    int isize = pVecInterUnion->size();
                    res = true;
                    for (int i = 0; i < isize; i++)
                    {
                        if (pVecInterUnion->at(i).description != other.pVecInterUnion->at(i).description|| 
                            pVecInterUnion->at(i).type != other.pVecInterUnion->at(i).type)
                        {
                            return false;//说明或类型不一样
                        }
                        switch(pVecInterUnion->at(i).type)
                        {
                        case INTER_TYPE_LN_POINT:
                            if (NULL == pVecInterUnion->at(i).pPoint && NULL == other.pVecInterUnion->at(i).pPoint)
                            {
                                break;
                            }
                            else if (pVecInterUnion->at(i).pPoint && other.pVecInterUnion->at(i).pPoint)
                            {
                                if (pVecInterUnion->at(i).pPoint->index !=  other.pVecInterUnion->at(i).pPoint->index ||
                                    pVecInterUnion->at(i).pPoint->dVel !=  other.pVecInterUnion->at(i).pPoint->dVel ||
                                    pVecInterUnion->at(i).pPoint->dAcc !=  other.pVecInterUnion->at(i).pPoint->dAcc )
                                {
                                    return false;//元素有一个不一样
                                }

                            }
                            else
                            {
                                return false;//pPoint 有一个 不存在
                            }
                            break;
                        case INTER_TYPE_IO:
                            if (NULL == pVecInterUnion->at(i).pIo && NULL == other.pVecInterUnion->at(i).pIo)
                            {
                                break;
                            }
                            else if (pVecInterUnion->at(i).pIo && other.pVecInterUnion->at(i).pIo)
                            {
                                if ((pVecInterUnion->at(i).pIo->cIoName != other.pVecInterUnion->at(i).pIo->cIoName)||
                                    pVecInterUnion->at(i).pIo->bIoOpen !=  other.pVecInterUnion->at(i).pIo->bIoOpen)

                                {
                                    return false;//有一个元素不一样
                                }

                            }
                            else
                            {
                                return false;//pIo 有一个 不存在
                            }
                            break;
                        case INTER_TYPE_DELAY:

                            if (NULL == pVecInterUnion->at(i).pDelay && NULL == other.pVecInterUnion->at(i).pDelay)
                            {
                                break;
                            }
                            else if (pVecInterUnion->at(i).pDelay && other.pVecInterUnion->at(i).pDelay)
                            {
                                if (pVecInterUnion->at(i).pDelay->time !=  other.pVecInterUnion->at(i).pDelay->time)
                                {
                                    return false;//元素不一样
                                }
                            }
                            else
                            {
                                return false;//pDelay 有一个 不存在
                            }
                            break;

                        default:

                            return false;//没有对应的类型


                        }
                    }

                    return true;//size 一样匹配完成
                }
                else   // size 不一样
                {
                    return false;
                }
            }
            else if (pVecInterUnion == NULL && other.pVecInterUnion ==NULL)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        return res;
    }

    void setString(const string &data){}
    string toString(){return "";}
};


#endif // WORKFLOWDEFINE
