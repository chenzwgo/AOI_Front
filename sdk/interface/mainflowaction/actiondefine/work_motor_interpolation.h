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
    INTER_TYPE_LN_POINT = 0, // ��λֱ�߲岹����
    INTER_TYPE_IO,           // IO����
    INTER_TYPE_DELAY	     // ��ʱ����
};

struct WORK_INTER_UNION
{
    string			description;       //��λ��������
    int             type;             // 0 INTER_TYPE_LN_POINT ��1 INTER_TYPE_IO 
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
    char			stationname[ITEM_NAME_LEN];      //��վ����// �����վֻ����һ�����ϵ����4����
    unsigned short  crd;                             //����ϵ�ţ�ȡֵ��Χ�� [1,2]
    unsigned short  evenTime;						 // ƽ��ʱ�䣺 ms
    double          synVelMax;						        // ���ϳ��ٶ� pulse/ms
    double          synAccMax;						        // �����ٶȣ� pulse/ms^2
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
                if ( pVecInterUnion->size() == other.pVecInterUnion->size())// size һ��
                {
                    int isize = pVecInterUnion->size();
                    res = true;
                    for (int i = 0; i < isize; i++)
                    {
                        if (pVecInterUnion->at(i).description != other.pVecInterUnion->at(i).description|| 
                            pVecInterUnion->at(i).type != other.pVecInterUnion->at(i).type)
                        {
                            return false;//˵�������Ͳ�һ��
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
                                    return false;//Ԫ����һ����һ��
                                }

                            }
                            else
                            {
                                return false;//pPoint ��һ�� ������
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
                                    return false;//��һ��Ԫ�ز�һ��
                                }

                            }
                            else
                            {
                                return false;//pIo ��һ�� ������
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
                                    return false;//Ԫ�ز�һ��
                                }
                            }
                            else
                            {
                                return false;//pDelay ��һ�� ������
                            }
                            break;

                        default:

                            return false;//û�ж�Ӧ������


                        }
                    }

                    return true;//size һ��ƥ�����
                }
                else   // size ��һ��
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
