#ifndef CONTROLSDEF_H
#define CONTROLSDEF_H
#include <QAbstractGraphicsShapeItem>
#include "common_roi.h"


#ifndef PI
#define PI 3.1415926  //paiֵ
#endif

// ö��˳�������ͼ�εĿ�ʼ�����˵�
enum ControlPos { None,
                  LeftTop, Top, RightTop, Right, Bottom, LeftBottom, Left, Center,RightBottom, 
                  LineP1,LineP2,
                  ArcP1 = 100, ArcP2, ArcP3,           // 3�㻭Բ��
                  ArcCP1=200,ArcCenter,ArcCMid, ArcCRotate, ArcCP2, // ���ļ�2�㻭Բ��
                  CircleP1 = 300,CircleP2,CircleP3,    // 3�㻭Բ
                  CircleCenter,CircleP,                // ���ļ�1�㻭Բ
                  Poly = 1000};

typedef int NControlPos; // ȡֵȡControlPos�е�����,�����Ը���(���߶�)

//// ͼ������
//enum DRAW_SHAPE {
//    DS_None=0,       // ��
//    DS_Line2Point,   // ��
//    DS_Arc3Point,DS_ArcCenterPoint,  // Բ��
//    DS_Circle3Point,DS_CircleCenterPoint,DS_CircleCenterRadius, // Բ
//    DS_PolyLine,   // ���߶�
//    DS_Rect,      // ����
//    DS_Curve,      // ����
//};

// ��������
enum OPERATE_TYPE{
    OT_None=0,      // ��
    OT_Draw,        // ��ͼ
    OT_Move,        // ƽ��
    OT_Select,      // ѡ��
    OT_ZoomRect,    // �ֲ��Ŵ�
};

// ����ת�ַ���
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

#define ControlMaxWidth 24   // ���Ƶ����������߳�
#define ControlMinWidth 0.1 // ���Ƶ���������С�߳�

#define ARROW_SIZE 3 // ��ͷ����

// �߶ζ˵��Զ���������
#define AdSorbDistance ControlMaxWidth/2

// ���������Ҽ��ƶ��ľ���
#define MoveDistance 12
// ��Ctrl+�������Ҽ��ƶ��ľ���
#define MovePix      1

#endif // CONTROLS
