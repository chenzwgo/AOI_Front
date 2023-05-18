#ifndef CONTROLSDEF_H
#define CONTROLSDEF_H
#include <QAbstractGraphicsShapeItem>
#include "common_roi.h"


#ifndef PI
#define PI 3.1415926  //pai值
#endif

// 枚举顺序决定了图形的开始结束端点
enum ControlPos { None,
                  LeftTop, Top, RightTop, Right, Bottom, LeftBottom, Left, Center,RightBottom, 
                  LineP1,LineP2,
                  ArcP1 = 100, ArcP2, ArcP3,           // 3点画圆弧
                  ArcCP1=200,ArcCenter,ArcCMid, ArcCRotate, ArcCP2, // 中心加2点画圆弧
                  CircleP1 = 300,CircleP2,CircleP3,    // 3点画圆
                  CircleCenter,CircleP,                // 中心加1点画圆
                  Poly = 1000};

typedef int NControlPos; // 取值取ControlPos中的内容,但可以更多(多线段)

//// 图形类型
//enum DRAW_SHAPE {
//    DS_None=0,       // 无
//    DS_Line2Point,   // 线
//    DS_Arc3Point,DS_ArcCenterPoint,  // 圆弧
//    DS_Circle3Point,DS_CircleCenterPoint,DS_CircleCenterRadius, // 圆
//    DS_PolyLine,   // 多线段
//    DS_Rect,      // 矩形
//    DS_Curve,      // 曲线
//};

// 操作类型
enum OPERATE_TYPE{
    OT_None=0,      // 无
    OT_Draw,        // 绘图
    OT_Move,        // 平移
    OT_Select,      // 选择
    OT_ZoomRect,    // 局部放大
};

// 类型转字符串
struct StShapeType
{
    QMap<int,QString> typeMap;
    StShapeType()
    {
        typeMap[EnShape_Line] = "Line";
        typeMap[EnShape_Rect] = "Rectangle";
        typeMap[EnShape_Arc] = "Arc(p)";
        typeMap[EnShape_Arc_Center] = "Arc(r)";
        typeMap[EnShape_Polygon] = "pLine";
        typeMap[EnShape_Circle] = "Circle(p)";
        typeMap[EnShape_Circle_Center] = "Circle(r)";
    }
};

#define ControlMaxWidth 24   // 控制点正方形最大边长
#define ControlMinWidth 0.1 // 控制点正方形最小边长

#define ARROW_SIZE 3 // 箭头长度

// 线段端点自动吸附距离
#define AdSorbDistance ControlMaxWidth/2

// 按上下左右键移动的距离
#define MoveDistance 12
// 按Ctrl+上下左右键移动的距离
#define MovePix      1

#endif // CONTROLS
