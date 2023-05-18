#ifndef CURVEGRAPH_H
#define CURVEGRAPH_H

#include "OS_DEFINE.h"
#include "dllexport.h"
#include <QWidget>
#include <QRubberBand>
#include <QMouseEvent>

namespace Ui {
class CurveGraph;
}

#define MAX_LINE_COUNT   8

class QCPGraph;
class QCPTextElement;
class RT_API CurveGraph : public QWidget
{
    Q_OBJECT

    // \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(bool showTickLabels READ showTickLabels WRITE setShowTickLabels) // 显示X轴标签
    Q_PROPERTY(bool showSubTicks READ showSubTicks WRITE setShowSubTicks) // 显示子刻度
    Q_PROPERTY(bool autoRangeX READ autoRangeX WRITE setAutoRangeX)       // 范围自适应X
    Q_PROPERTY(bool autoRangeY READ autoRangeY WRITE setAutoRangeY)       // 范围自适应Y
    Q_PROPERTY(CGLineStyle lineStyle READ lineStyle WRITE setLineStyle)          // 线类型
    Q_PROPERTY(CGScatterShape scatterStyle READ scatterStyle WRITE setScatterStyle) // 点类型
    Q_PROPERTY(bool showXGrid READ showXGrid WRITE setShowXGrid)          // X轴网格线显示
    Q_PROPERTY(Qt::PenStyle xGridPen READ xGridPen WRITE setXGridPen)    // X轴网格线画笔样式
    Q_PROPERTY(QColor xGridColor READ xGridColor WRITE setXGridColor)    // X轴网格线画笔颜色
    Q_PROPERTY(bool showYGrid READ showYGrid WRITE setShowYGrid)          // Y轴网格线显示
    Q_PROPERTY(Qt::PenStyle yGridPen READ yGridPen WRITE setYGridPen)    // Y轴网格线画笔样式
    Q_PROPERTY(QColor yGridColor READ yGridColor WRITE setYGridColor)    // Y轴网格线画笔颜色
    Q_PROPERTY(double xAxisLower READ xAxisLower WRITE setXAxisLower)  // y坐标下限(默认0)
    Q_PROPERTY(double xAxisUpper READ xAxisUpper WRITE setXAxisUpper)  // y坐标上限(默认5)
    Q_PROPERTY(double yAxisLower READ yAxisLower WRITE setYAxisLower)  // y坐标下限(默认0)
    Q_PROPERTY(double yAxisUpper READ yAxisUpper WRITE setYAxisUpper)  // y坐标上限(默认5)
    Q_PROPERTY(double rangeLower READ rangeLower WRITE setRangeLower)  // 合格范围下限(默认0)
    Q_PROPERTY(double rangeUpper READ rangeUpper WRITE setRangeUpper)  // 合格范围上限(默认0)
    Q_PROPERTY(bool showTitle READ showTitle WRITE setShowTitle)       // 显示表头
    Q_PROPERTY(QString title READ title WRITE setTitle)                // 表头文字设置
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont)      // 表头字体设置
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel)             // X标签
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel)             // Y标签
    Q_PROPERTY(int tickLabelPadding READ tickLabelPadding WRITE setTickLabelPadding)       //
    Q_PROPERTY(QFont tickLabelFont READ tickLabelFont WRITE setTickLabelFont)
    Q_PROPERTY(QColor tickLabelColor READ tickLabelColor WRITE setTickLabelColor)    // X坐标标签颜色
    Q_PROPERTY(QColor rangeLineColor READ rangeLineColor WRITE setRangeLineColor)    // 范围线颜色
    Q_PROPERTY(double tickLabelRotation READ tickLabelRotation WRITE setTickLabelRotation)
    Q_PROPERTY(QString varName READ varName WRITE setVarName)             // 关联的any变量名
    Q_PROPERTY(QStringList testValue READ testValue WRITE setTestValue)   // 数值测试

    // -------------属性实现----------------
public:
    enum CGScatterShape { ssNone       ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                        ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                        ,ssCross     ///< \enumimage{ssCross.png} a cross
                        ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                        ,ssCircle    ///< \enumimage{ssCircle.png} a circle
                        ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle)
                        ,ssSquare    ///< \enumimage{ssSquare.png} a square
                        ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond
                        ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
                        ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline
                        ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
                        ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square with a cross inside
                        ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square with a plus inside
                        ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle with a cross inside
                        ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle with a plus inside
                        ,ssPeace     ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines
                        ,ssPixmap    ///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates
                        ,ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
                      };
    Q_ENUMS(CGScatterShape)

    enum CGLineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
                                   ///< with symbols according to the scatter style, see \ref setScatterStyle)
                     ,lsLine       ///< data points are connected by a straight line
                     ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
                     ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
                     ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
                     ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
                   };
    Q_ENUMS(CGLineStyle)

    explicit CurveGraph(QWidget *parent = 0);
    ~CurveGraph();

    // 显示下方描述
    bool showTickLabels() const;
    void setShowTickLabels(bool bShow);

    // 范围自适应X
    bool autoRangeX() const { return m_bAutoRangeX; }
    void setAutoRangeX(bool bAuto);

    // 范围自适应Y
    bool autoRangeY() const { return m_bAutoRangeY; }
    void setAutoRangeY(bool bAuto);

    // 刻度描述文字角度
    double tickLabelRotation() const;
    void setTickLabelRotation(double deg);

    // 设置线型
    CGLineStyle lineStyle() const;
    void setLineStyle(CGLineStyle style);

    // 设置点型
    CGScatterShape scatterStyle() const;
    void setScatterStyle(CGScatterShape shape);

    // 显示X轴网格线
    bool showXGrid() const;
    void setShowXGrid(bool bShow);

    // X轴网格线画笔
    Qt::PenStyle xGridPen() const;
    void setXGridPen(Qt::PenStyle penStyle);

    // X轴网格线画笔颜色
    QColor xGridColor() const;
    void setXGridColor(QColor color);

    // 显示Y轴网格线
    bool showYGrid() const;
    void setShowYGrid(bool bShow);

    // Y轴网格线画笔
    Qt::PenStyle yGridPen() const;
    void setYGridPen(Qt::PenStyle penStyle);

    // Y轴网格线画笔颜色
    QColor yGridColor() const;
    void setYGridColor(QColor color);

    // 显示表头
    bool showTitle() const;
    void setShowTitle(bool bShow);

    // 设置表头文字
    QString title() const;
    void setTitle(QString title);

    // 设置表头字体
    QFont titleFont() const;
    void setTitleFont(QFont font);

    // 设置X标签
    QString xLabel() const;
    void setXLabel(QString label);

    // 设置Y标签
    QString yLabel() const;
    void setYLabel(QString label);

    // 设置柱子对应变量名
    QString varName() const { return m_varName; }
    void setVarName(QString varName) {
        m_varName = varName;
    }

    // 设置坐标X范围（上限）
    double xAxisUpper() const;
    void setXAxisUpper(double max);

    // 设置坐标X范围（下限）
    double xAxisLower() const;
    void setXAxisLower(double min);

    // 设置坐标Y范围（上限）
    double yAxisUpper() const;
    void setYAxisUpper(double max);

    // 设置坐标Y范围（下限）
    double yAxisLower() const;
    void setYAxisLower(double min);

    // 设置合格范围（下限）
    double rangeLower() const;
    void setRangeLower(double min);

    // 设置合格范围（上限）
    double rangeUpper() const;
    void setRangeUpper(double max);

    // 设置测试值
    QStringList testValue() const;
    void setTestValue(QStringList strList);

    // 设置描述信息列表
    QStringList tickLabels() const;
    void setTickLabels(QStringList tickVector);

    // 设置标签宽度
    int tickLabelPadding() const;
    void setTickLabelPadding(int pad);

    // 设置标签字体
    QFont tickLabelFont() const;
    void setTickLabelFont(QFont font);

    // 设置标签颜色
    QColor tickLabelColor() const;
    void setTickLabelColor(QColor color);

    // 设置范围线颜色
    QColor rangeLineColor() const;
    void setRangeLineColor(QColor color);

    // 显示子刻度
    bool showSubTicks() const;
    void setShowSubTicks(bool bShow);

public slots:

    void slotBtn();
    // -------------外部调用----------------
public:
    // 设置所有点值,
    void setValues(QVector<double> keyList, QVector<double> valList, int index = 0);

    // 增加点值
    void addValue(double xVal, double yVal, int index = 0);

    // 增加点值
    void addValue(double yVal, int index = 0);

    // 清除数据
    void clearValue();

signals:
    void sigUpdate();

private:
    Ui::CurveGraph *ui;
    QCPTextElement *m_titleElem;       // 标题
    QCPGraph *m_pGrapLine[MAX_LINE_COUNT];           // 数值曲线
    QCPGraph *m_pGrapLower;      // 合格线(下线)
    QCPGraph *m_pGrapUpper;      // 合格线(上线)
    QString m_varName;           // 关联的any变量名称
    double m_rangeLower;         // 合格范围下限值
    double m_rangeUpper;         // 合格范围下限值
    QVector<double> m_keyList;   // key列表
    QVector<double> m_valueList; // 值列表
    bool m_bAutoRangeX;           // 范围自适应X
    bool m_bAutoRangeY;           // 范围自适应Y

    QRubberBand *rubberBand;
    QPoint rubberOrigin;
};

#endif // CURVEGRAPH_H
