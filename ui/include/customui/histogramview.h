#ifndef HISTOGRAMVIEW_H
#define HISTOGRAMVIEW_H

#include <QWidget>
#include <QPen>

namespace Ui {
class HistogramView;
}

class QCPBars;
class QCPItemText;
class QCPGraph;
class QCPTextElement;
class HistogramView : public QWidget
{
    Q_OBJECT

    // \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(bool showValue READ showValue WRITE setShowValue)                // 显示柱子数值
    Q_PROPERTY(bool showTickLabels READ showTickLabels WRITE setShowTickLabels) // 显示X轴标签
    Q_PROPERTY(bool showYGrid READ showYGrid WRITE setShowYGrid)          // Y轴网格线显示
    Q_PROPERTY(Qt::PenStyle yGridPen READ yGridPen WRITE setYGridPen)    // Y轴网格线画笔样式
    Q_PROPERTY(QColor yGridColor READ yGridColor WRITE setYGridColor)    // Y轴网格线画笔颜色
    Q_PROPERTY(bool autoRange READ autoRange WRITE setAutoRange)       // 范围自适应
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
    Q_PROPERTY(int numberPrecision READ numberPrecision WRITE setNumberPrecision)
    Q_PROPERTY(QStringList tickLabels READ tickLabels WRITE setTickLabels)
    Q_PROPERTY(QStringList varList READ varList WRITE setVarList)             // 关联的any变量名
    Q_PROPERTY(QStringList testValue READ testValue WRITE setTestValue)       // 数值测试

    // -------------属性实现----------------
public:
    explicit HistogramView(QWidget *parent = 0);

    // 显示柱子数值
    bool showValue() const { return m_bShowValue; }
    void setShowValue(bool bShow);

    // 显示下方描述
    bool showTickLabels() const;
    void setShowTickLabels(bool bShow);

    // 显示Y轴网格线
    bool showYGrid() const;
    void setShowYGrid(bool bShow);

    // Y轴网格线画笔
    Qt::PenStyle yGridPen() const;
    void setYGridPen(Qt::PenStyle penStyle);

    // Y轴网格线画笔颜色
    QColor yGridColor() const;
    void setYGridColor(QColor color);

    // 范围自适应
    bool autoRange() const { return m_bAutoRange; }
    void setAutoRange(bool bAuto);

    // 刻度描述文字角度
    double tickLabelRotation() const;
    void setTickLabelRotation(double deg);

    // 设置柱子对应变量名
    QStringList varList() const { return m_varList; }
    void setVarList(QStringList strList) {
        m_varList = strList;
    }

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

    // 设置数值精度
    int numberPrecision() const;
    void setNumberPrecision(int precision);

    // -------------外部调用----------------
public:
    // 设置所有柱子值
    void setValues(QVector<double> valList);

    // 设置单个柱子值
    void setValue(int index, double val);

signals:
    void sigUpdate();

private:
    void resizeEvent(QResizeEvent *);

    // 显示刻度
    bool showTicks() const;
    void setShowTicks(bool bShow);

    // 显示子刻度
    bool showSubTicks() const;
    void setShowSubTicks(bool bShow);

    QCPItemText *addBarValueText();

    // 更新柱子数值显示位置
    void updateBarValueTextPos();

    // 自动调整范围
    void autoYAxisRange();

private:
    Ui::HistogramView *ui;
    QCPBars *m_pBar;
    QCPTextElement *m_titleElem;       // 标题
    QStringList m_varList;
    QVector<double> m_keys;      // 柱子标号列表
    QVector<double> m_valueList; // 柱子值列表
    QVector<QCPItemText*> m_barValueTextList; // 柱子值显示框
    QCPGraph *m_pGrapLower;      // 合格线(下线)
    QCPGraph *m_pGrapUpper;      // 合格线(上线)

    double m_rangeLower;         // 合格范围下限值
    double m_rangeUpper;         // 合格范围下限值
    int m_precision;             // 精度
    bool m_bShowValue;           // 显示数值
    bool m_bAutoRange;           // 范围自适应
};

#endif // HISTOGRAMVIEW_H
