#ifndef _VISION_COMMON_ROI_H_
#define _VISION_COMMON_ROI_H_

#include <math.h>
#include <list>
#include "any.h"
#include "common_data.h"

// 图形类型
enum ShapeType{
    EnShape_None = 65537,
    EnShape_Line,
    EnShape_Rect,
    EnShape_Rect1,    //不可旋转矩形
    EnShape_Arc,
    EnShape_Arc_Center,
    EnShape_Polygon,
    EnShape_Circle,
    EnShape_Circle_Center,
    EnShape_Erase_Path,
};

// Roi类型
typedef ShapeType E_RoiType;

// Roi类型
enum E_ROI_COMP_TYPE
{
    VISION_ROI_NONE = 0,		// 空
    VISION_ROI_UNION,			// 并集 
    VISION_ROI_INTERSECTION,    // 交集
    VISION_ROI_DIFFERENCE,      // 差集
    VISION_ROI_COMPLEMENT,      // 补集
    VISION_ROI_USER,            // 其他
};

enum E_USER_ROI_TYPE
{
    VISION_UROI_CLOSE_CONTOUR = 0,   //封闭的轮廓
    VISION_UROI_ERASE_PATH,          //手动图画的路径
    VISION_UROI_UNKNOW,
};

struct ST_MODEL_POINTS_INFO
{
    int iWidth;
    E_USER_ROI_TYPE roiType;
    std::list<STPoint> points; //点列表， 用于拟合轮廓或根据轨迹外膨胀得到部分区域

    ST_MODEL_POINTS_INFO()
    {
        iWidth = 24;
        roiType = VISION_UROI_UNKNOW;
    }
    ST_MODEL_POINTS_INFO(const ST_MODEL_POINTS_INFO &other)
    {
        iWidth = other.iWidth;
        roiType = other.roiType;
        points = other.points;
    }
    ST_MODEL_POINTS_INFO & operator= (const ST_MODEL_POINTS_INFO &other)
    {
        iWidth = other.iWidth;
        roiType = other.roiType;
        points = other.points;
        return *this;
    }

    void reset()
    {
        iWidth = 24;
        roiType = VISION_UROI_UNKNOW;
        points.clear();
    }
};
struct STRoiParam
{
    E_ROI_COMP_TYPE combType;     //组合类型
    E_RoiType       roiType;      //Roi 类型
    any             stRoiParam;   //Roi 属性参数, 新增类型时需修改赋值处理， 否则将UI层生成的any对象传入sdk层， 会出现退出时崩溃问题
    STRoiParam()
    {
        combType = VISION_ROI_NONE;
        roiType  = EnShape_None;
    }
    STRoiParam(const STRoiParam &other)
    {
        combType    = other.combType;
        roiType     = other.roiType;
        if (!other.stRoiParam.empty())
        {
            if (is_any_type<STRectangle2>(other.stRoiParam))
            {
                stRoiParam = any_cast<STRectangle2>(other.stRoiParam);
            }
            else if (is_any_type<STCircle>(other.stRoiParam))
            {
                stRoiParam = any_cast<STCircle>(other.stRoiParam);
            }
            else if (is_any_type<ST_MODEL_POINTS_INFO>(other.stRoiParam))
            {
                stRoiParam = any_cast<ST_MODEL_POINTS_INFO>(other.stRoiParam);
            }
            else if (is_any_type<STPoint>(other.stRoiParam))
            {
                stRoiParam = any_cast<STPoint>(other.stRoiParam);
            }
            else if (is_any_type<STLine>(other.stRoiParam))
            {
                stRoiParam = any_cast<STLine>(other.stRoiParam);
            }
            else if (is_any_type<STPosition>(other.stRoiParam))
            {
                stRoiParam = any_cast<STPosition>(other.stRoiParam);
            }
            else if (is_any_type<STRatioPoint>(other.stRoiParam))
            {
                stRoiParam = any_cast<STRatioPoint>(other.stRoiParam);
            }
            else
            {
                stRoiParam.clone(other.stRoiParam);
            }
        }
    }
    STRoiParam & operator= (const STRoiParam &other)
    {
        if (this != &other)
        {
            combType    = other.combType;
            roiType     = other.roiType;
            if (!other.stRoiParam.empty())
            {
                if (is_any_type<STRectangle2>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STRectangle2>(other.stRoiParam);
                }
                else if (is_any_type<STCircle>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STCircle>(other.stRoiParam);
                }
                else if (is_any_type<ST_MODEL_POINTS_INFO>(other.stRoiParam))
                {
                    stRoiParam = any_cast<ST_MODEL_POINTS_INFO>(other.stRoiParam);
                }
                else if (is_any_type<STPoint>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STPoint>(other.stRoiParam);
                }
                else if (is_any_type<STLine>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STLine>(other.stRoiParam);
                }
                else if (is_any_type<STPosition>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STPosition>(other.stRoiParam);
                }
                else if (is_any_type<STRatioPoint>(other.stRoiParam))
                {
                    stRoiParam = any_cast<STRatioPoint>(other.stRoiParam);
                }
                else
                {
                    stRoiParam.clone(other.stRoiParam);
                }
            }
        }
        return *this;
    }
};

#define ACTIVE_ROI_COLOR	"green"
#define SELECT_ROI_COLOR	"red"
#define UNACTIVE_ROI_COLOR	"red"

#ifndef LPROIVOID
#define LPROIVOID  STRoiParam *
#endif

static void GetRectanglePoint(const STPoint pCentre, const double nWith, const double nHight, const double dArc, STPoint &p1, STPoint &p2, STPoint &p3, STPoint &p4)
{
    double dPhi = 3.141592654;

    double dHight1 = sin(dArc) * nWith * 0.5;
    double dWith1 = cos(dArc) * nWith * 0.5;
    double dHight2 = sin(0.5 * dPhi - dArc) * nHight * 0.5;
    double dWith2 = cos(0.5 * dPhi - dArc) * nHight * 0.5;

    p1.x = (pCentre.x - dWith1) + dWith2;
    p1.y = (pCentre.y - dHight1) - dHight2;
    p2.x = (pCentre.x + dWith1) + dWith2;
    p2.y = (pCentre.y + dHight1) - dHight2;
    p3.x = (pCentre.x + dWith1) - dWith2;
    p3.y = (pCentre.y + dHight1) + dHight2;
    p4.x = (pCentre.x - dWith1) - dWith2;
    p4.y = (pCentre.y - dHight1) + dHight2;

    //p1.y = (pCentre.y - dWith1) + dWith2;
    //p1.x = (pCentre.x - dHight1) - dHight2;
    //p2.y = (pCentre.y + dWith1) + dWith2;
    //p2.x = (pCentre.x + dHight1) - dHight2;
    //p3.y = (pCentre.y + dWith1) - dWith2;
    //p3.x = (pCentre.x + dHight1) + dHight2;
    //p4.y = (pCentre.y - dWith1) - dWith2;
    //p4.x = (pCentre.x - dHight1) + dHight2;
}

#endif