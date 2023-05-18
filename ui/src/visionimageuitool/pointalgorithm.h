#ifndef POINT_ALGORITHM_H
#define POINT_ALGORITHM_H

#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <qmath.h>

// 坐标点计算类
class PMethod
{
public:
    // 求点到线的垂点(垂线与线的交点)
    static QPointF verticalPointToLine( const QPointF &p, const QLineF &line )
    {
        QPointF p1 = line.p1();
        QPointF p2 = line.p2();
        double cross = (p2.x()- p1.x()) * (p.x() - p1.x()) + (p2.y() - p1.y()) * (p.y() - p1.y());
        if (cross <= 0) return p1;

        double d2 = (p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y());
        if (cross >= d2) return p2;

        double r = cross / d2;
        double px = p1.x() + (p2.x() - p1.x()) * r;
        double py = p1.y() + (p2.y() - p1.y()) * r;
        return QPointF(px,py);
    }

    // 求点到线的最短距离
    static qreal distancePointToLine( const QPointF &p, const QLineF &line )
    {
        QPointF p1 = line.p1();
        QPointF p2 = line.p2();
        double cross = (p2.x()- p1.x()) * (p.x() - p1.x()) + (p2.y() - p1.y()) * (p.y() - p1.y());
        if (cross <= 0) return qSqrt((p.x() - p1.x()) * (p.x() - p1.x()) + (p.y() - p1.y()) * (p.y() - p1.y()));

        double d2 = (p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y());
        if (cross >= d2) return qSqrt((p.x() - p2.x()) * (p.x() - p2.x()) + (p.y() - p2.y()) * (p.y() - p2.y()));

        double r = cross / d2;
        double px = p1.x() + (p2.x() - p1.x()) * r;
        double py = p1.y() + (p2.y() - p1.y()) * r;
        return qSqrt((p.x() - px) * (p.x() - px) + (p1.y() - py) * (p1.y() - py));
    }

    // 两点间的距离
    static qreal distanceP2P( const QPointF& p1, const QPointF& p2 )
    {
        return qSqrt((p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y()));
    }

    // 获取直线的外接矩形,r为内径
    static QPolygonF getOutSideRect( const QLineF &line,qreal r=1 )
    {
        QPointF p1 = line.p1();
        QPointF p2 = line.p2();
        if (p1 == p2)
        {
            QRectF rect(0,0,r,r);
            rect.moveCenter(p1);
            return rect;
        }

        QPointF tp = p2-p1;
        QPointF corner1 = p1+QPointF(-tp.y(), tp.x())*r/line.length();
        QPointF corner2 = p1+QPointF(tp.y(), -tp.x())*r/line.length();
        QPointF corner3 = p2+QPointF(tp.y(), -tp.x())*r/line.length();
        QPointF corner4 = p2+QPointF(-tp.y(), tp.x())*r/line.length();

        QPolygonF poly;
        poly<<corner1<<corner2<<corner3<<corner4<<corner1;
        return poly;
    }

    // 3点取圆
    static QPointF tCircle(QPointF pt1, QPointF pt2, QPointF pt3, double &radius)
    {
        double x1 = pt1.x(), x2 = pt2.x(), x3 = pt3.x();
        double y1 = pt1.y(), y2 = pt2.y(), y3 = pt3.y();
        double a = x1 - x2;
        double b = y1 - y2;
        double c = x1 - x3;
        double d = y1 - y3;
        double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) / 2.0;
        double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) / 2.0;
        double det = b * c - a * d;
        if( fabs(det) < 1e-5)
        {
            radius = -1;
            return QPointF();
        }

        double x0 = -(d * e - b * f) / det;
        double y0 = -(a * f - c * e) / det;
        radius = hypot(x1 - x0, y1 - y0);
        return QPointF(x0, y0);
    }

    // 已知圆心和圆弧两点,求圆弧中点,clockwise表示圆弧是否为顺时针旋转
    static QPointF arcMidPoint(const QPointF &center, const QPointF &p1, const QPointF &p2, bool clockwise = false)
    {
        if (p1==p2 || center==p1 || center==p2)
        {
            return QPointF();
        }

        qreal Y1=p1.y()-center.y();
        qreal Y2=p2.y()-center.y();
        qreal X1=p1.x()-center.x();
        qreal X2=p2.x()-center.x();

        qreal r = (X1*X1+Y1*Y1+X2*X2+Y2*Y2)/((X1+X2)*(X1+X2)+(Y1+Y2)*(Y1+Y2));
        qreal X = (X1+X2)*qSqrt(r/2);
        qreal Y = (Y1+Y2)*qSqrt(r/2);
        int radio = clockwise ? -1 : 1;
        qreal x0 = center.x()+X*radio;
        qreal y0 = center.y()+Y*radio;
        return QPointF(x0,y0);
    }

    // 判断3点是否共线
    static bool is3PointOnLine(const QPointF &p1,const QPointF &p2,const QPointF &p3)
    {
        if (p1.x() == p2.x() && p1.x() == p3.x())
        {
            return true;
        }
        else if (p1.y() == p2.y() && p1.y() == p3.y())
        {
            return true;
        }
        else if(p1.x() != p2.x() && p1.x() != p3.x()
            && ((p2.y()-p1.y())/(p2.x()-p1.x()))==((p3.y()-p1.y())/(p3.x()-p1.x())))
        {
            return true;
        }

        return false;
    }

    static QPolygonF getArrow(const QLineF &line, qreal width)
    {
        double angle = ::acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = (PI * 2) - angle;

        QPointF arrowP1 = line.p2() - QPointF(sin(angle + PI / 3) * width,
            cos(angle + PI / 3) * width);
        QPointF arrowP2 = line.p2() - QPointF(sin(angle + PI - PI / 3) * width,
            cos(angle + PI - PI / 3) * width);

        QPolygonF arrowHead;
        arrowHead  << arrowP1 << line.p2()<< arrowP2 << arrowP1;
        return arrowHead;
    }
};
#endif
