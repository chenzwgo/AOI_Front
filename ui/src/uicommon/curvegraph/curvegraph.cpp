#include "curvegraphwidget.h"
#include "ui_curvegraph.h"

Qt::GlobalColor g_penColor[MAX_LINE_COUNT] = {Qt::blue, Qt::black, Qt::green, Qt::cyan, Qt::magenta, Qt::yellow, Qt::lightGray, Qt::darkGray};
CurveGraph::CurveGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveGraph),
    m_bAutoRangeX(false),
    m_bAutoRangeY(false),
    m_titleElem(NULL)
{
    ui->setupUi(this);
    for (int i = 0; i < MAX_LINE_COUNT; i++)
    {
        m_pGrapLine[i] =  new QCPGraph(ui->customPlot->xAxis,ui->customPlot->yAxis);
        m_pGrapLine[i]->setPen(QPen(g_penColor[i], 0));
    }
    m_pGrapLower = new QCPGraph(ui->customPlot->xAxis,ui->customPlot->yAxis);
    m_pGrapUpper = new QCPGraph(ui->customPlot->xAxis,ui->customPlot->yAxis);
    m_pGrapLower->setPen(QPen(Qt::red, 0));
    m_pGrapUpper->setPen(QPen(Qt::red, 0));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->setAttribute(Qt::WA_TransparentForMouseEvents,true);

    // 增加标题
    m_titleElem = new QCPTextElement(ui->customPlot, "title", QFont("sans", 17, QFont::Bold));
    m_titleElem->setVisible(false);

    // 最小尺寸
    setMinimumSize(260,200);

    // 初始化图表
    m_keyList<<1<<2<<3<<4;
    m_valueList<<0<<1<<3<<2;
    setValues(m_keyList, m_valueList);

    //实现恢复1:1功能
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(slotBtn()));
}

CurveGraph::~CurveGraph()
{
    delete ui;
}

bool CurveGraph::showTickLabels() const
{
    return ui->customPlot->xAxis->tickLabels();
}

void CurveGraph::setShowTickLabels(bool bShow)
{
    ui->customPlot->xAxis->setTickLabels(bShow);
    ui->customPlot->replot();
}

void CurveGraph::setValues(QVector<double> keyList, QVector<double> valList, int index)
{
    m_keyList = keyList;
    m_valueList = valList;
    m_pGrapLine[index]->setData(keyList, valList);

    if (m_bAutoRangeX)
    {
        m_pGrapLine[index]->rescaleKeyAxis();
    }

    if (m_bAutoRangeY)
    {
        m_pGrapLine[index]->rescaleValueAxis();
    }

    ui->customPlot->replot();
}

void CurveGraph::addValue(double key, double val, int index)
{
    m_keyList.push_back(key);
    m_valueList.push_back(val);
    m_pGrapLine[index]->setData(m_keyList, m_valueList);

    if (m_bAutoRangeX)
    {
        m_pGrapLine[index]->rescaleKeyAxis();
    }

    if (m_bAutoRangeY)
    {
        m_pGrapLine[index]->rescaleValueAxis();
    }


    ui->customPlot->replot();
}

void CurveGraph::addValue(double val, int index)
{
    m_keyList.push_back(m_keyList.size());
    m_valueList.push_back(val);
    m_pGrapLine[index]->setData(m_keyList, m_valueList);

    if (m_bAutoRangeX)
    {
        m_pGrapLine[index]->rescaleKeyAxis();
    }

    if (m_bAutoRangeY)
    {
        m_pGrapLine[index]->rescaleValueAxis();
    }


    ui->customPlot->replot();
}

QStringList CurveGraph::testValue() const
{
    QStringList valueList;
    foreach (double val, m_valueList) {
        valueList.push_back(QString::number(val,'f'));
    }
    return valueList;
}

void CurveGraph::setTestValue(QStringList strList)
{
    m_keyList.clear();
    m_valueList.clear();

    foreach (QString val, strList) {
        m_valueList.push_back(val.toDouble());
    }

    for (int i=0; i<m_valueList.size(); i++)
    {
        m_keyList.push_back(i);
    }
    m_pGrapLine[0]->setData(m_keyList, m_valueList);

    if (m_bAutoRangeX)
    {
        m_pGrapLine[0]->rescaleKeyAxis();
    }

    if (m_bAutoRangeY)
    {
        m_pGrapLine[0]->rescaleValueAxis();
    }

    ui->customPlot->replot();
}

double CurveGraph::xAxisLower() const
{
    return ui->customPlot->xAxis->range().lower;
}

void CurveGraph::setXAxisLower(double dRange)
{
    ui->customPlot->xAxis->setRangeLower(dRange);
    ui->customPlot->replot();
}

double CurveGraph::xAxisUpper() const
{
    return ui->customPlot->xAxis->range().upper;
}

void CurveGraph::setXAxisUpper(double dRange)
{
    ui->customPlot->xAxis->setRangeUpper(dRange);
    ui->customPlot->replot();
}

double CurveGraph::yAxisLower() const
{
    return ui->customPlot->yAxis->range().lower;
}

void CurveGraph::setYAxisLower(double dRange)
{
    ui->customPlot->yAxis->setRangeLower(dRange);
    ui->customPlot->replot();
}

double CurveGraph::yAxisUpper() const
{
    return ui->customPlot->yAxis->range().upper;
}

void CurveGraph::setYAxisUpper(double dRange)
{
    ui->customPlot->yAxis->setRangeUpper(dRange);
    ui->customPlot->replot();
}

double CurveGraph::rangeLower() const
{
    return m_rangeLower;
}

void CurveGraph::setRangeLower(double dRange)
{
    m_rangeLower = dRange;
    QVector<double> keys;
    keys << ui->customPlot->xAxis->range().lower << ui->customPlot->xAxis->range().upper;
    QVector<double> values;
    values << m_rangeLower << m_rangeLower;
    m_pGrapLower->setData(keys, values);
    ui->customPlot->replot();
}

double CurveGraph::rangeUpper() const
{
    return m_rangeUpper;
}

void CurveGraph::setRangeUpper(double dRange)
{
    m_rangeUpper = dRange;
    QVector<double> keys;
    keys << ui->customPlot->xAxis->range().lower << ui->customPlot->xAxis->range().upper;
    QVector<double> values;
    values << m_rangeUpper << m_rangeUpper;
    m_pGrapUpper->setData(keys, values);
    ui->customPlot->replot();
}

double CurveGraph::tickLabelRotation() const
{
    return ui->customPlot->xAxis->tickLabelRotation();
}

void CurveGraph::setTickLabelRotation(double deg)
{
    ui->customPlot->xAxis->setTickLabelRotation(deg);
    ui->customPlot->replot();
}

// 设置线型
CurveGraph::CGLineStyle CurveGraph::lineStyle() const
{
    
    return (CGLineStyle)m_pGrapLine[0]->lineStyle();
}

void CurveGraph::setLineStyle(CGLineStyle style)
{
    for (int i = 0; i < MAX_LINE_COUNT; i++)
    {
        m_pGrapLine[i]->setLineStyle((QCPGraph::LineStyle)style);
    }
    ui->customPlot->replot();
}

// 设置点型
CurveGraph::CGScatterShape CurveGraph::scatterStyle() const
{
    return (CGScatterShape)m_pGrapLine[0]->scatterStyle().shape();
}

void CurveGraph::setScatterStyle(CGScatterShape shape)
{
    for (int i = 0; i < MAX_LINE_COUNT; i++)
    {
        QCPScatterStyle style = m_pGrapLine[i]->scatterStyle();
        style.setShape((QCPScatterStyle::ScatterShape)shape);
        m_pGrapLine[i]->setScatterStyle(style);
    }
    ui->customPlot->replot();
}

// X网格线
bool CurveGraph::showXGrid() const
{
    return ui->customPlot->xAxis->grid()->visible();
}

void CurveGraph::setShowXGrid(bool bShow)
{
    ui->customPlot->xAxis->grid()->setVisible(bShow);
    ui->customPlot->replot();
}

Qt::PenStyle CurveGraph::xGridPen() const
{
    return ui->customPlot->xAxis->grid()->pen().style();
}

void CurveGraph::setXGridPen(Qt::PenStyle penStyle)
{
    QPen pen = ui->customPlot->xAxis->grid()->pen();
    pen.setStyle(penStyle);
    ui->customPlot->xAxis->grid()->setPen(pen);
    ui->customPlot->replot();
}

QColor CurveGraph::xGridColor() const
{
    return ui->customPlot->xAxis->grid()->pen().color();
}

void CurveGraph::setXGridColor(QColor color)
{
    QPen pen = ui->customPlot->xAxis->grid()->pen();
    pen.setColor(color);
    ui->customPlot->xAxis->grid()->setPen(pen);
    ui->customPlot->replot();
}

// Y网格线
bool CurveGraph::showYGrid() const
{
    return ui->customPlot->yAxis->grid()->visible();
}

void CurveGraph::setShowYGrid(bool bShow)
{
    ui->customPlot->yAxis->grid()->setVisible(bShow);
    ui->customPlot->replot();
}

Qt::PenStyle CurveGraph::yGridPen() const
{
    return ui->customPlot->yAxis->grid()->pen().style();
}

void CurveGraph::setYGridPen(Qt::PenStyle penStyle)
{
    QPen pen = ui->customPlot->yAxis->grid()->pen();
    pen.setStyle(penStyle);
    ui->customPlot->yAxis->grid()->setPen(pen);
    ui->customPlot->replot();
}

QColor CurveGraph::yGridColor() const
{
    return ui->customPlot->yAxis->grid()->pen().color();
}

void CurveGraph::setYGridColor(QColor color)
{
    QPen pen = ui->customPlot->yAxis->grid()->pen();
    pen.setColor(color);
    ui->customPlot->yAxis->grid()->setPen(pen);
    ui->customPlot->replot();
}

// 显示表头
bool CurveGraph::showTitle() const
{
    if(m_titleElem)
    {
        return m_titleElem->visible();
    }

    return false;
}

void CurveGraph::setShowTitle(bool bShow)
{
    if(m_titleElem)
    {
        m_titleElem->setVisible(bShow);
        if (bShow)
        {
            ui->customPlot->plotLayout()->insertRow(0);
            ui->customPlot->plotLayout()->addElement(0, 0, m_titleElem);
        }
        else
        {
            ui->customPlot->plotLayout()->take(m_titleElem);
            ui->customPlot->plotLayout()->simplify();
        }

        ui->customPlot->replot();
    }

}

// 设置表头
QString CurveGraph::title() const
{
    if(!m_titleElem) return "";
    return m_titleElem->text();
}

void CurveGraph::setTitle(QString title)
{
    if(!m_titleElem) return;
    m_titleElem->setText(title);
    ui->customPlot->replot();
}

// 设置表头字体
QFont CurveGraph::titleFont() const
{
    if(!m_titleElem) return QFont();
    return m_titleElem->font();
}
void CurveGraph::setTitleFont(QFont font)
{
    if(!m_titleElem) return;
    m_titleElem->setFont(font);
    ui->customPlot->replot();
}

// 设置标签宽度
int CurveGraph::tickLabelPadding() const
{
    return ui->customPlot->xAxis->tickLabelPadding();
}

void CurveGraph::setTickLabelPadding(int pad)
{
    ui->customPlot->xAxis->setTickLabelPadding(pad);
    ui->customPlot->replot();
}

// 设置标签字体
QFont CurveGraph::tickLabelFont() const
{
    return ui->customPlot->xAxis->tickLabelFont();
}
void CurveGraph::setTickLabelFont(QFont font)
{
    ui->customPlot->xAxis->setTickLabelFont(font);
    ui->customPlot->replot();
}

// 设置标签颜色
QColor CurveGraph::tickLabelColor() const
{
    return ui->customPlot->xAxis->tickLabelColor();
}

void CurveGraph::setTickLabelColor(QColor color)
{
    ui->customPlot->xAxis->setTickLabelColor(color);
    ui->customPlot->replot();
}

// 设置范围线颜色
QColor CurveGraph::rangeLineColor() const
{
    return m_pGrapLower->pen().color();
}

void CurveGraph::setRangeLineColor(QColor color)
{
    m_pGrapLower->setPen(QPen(color, 0));
    m_pGrapUpper->setPen(QPen(color, 0));
    ui->customPlot->replot();
}

void CurveGraph::setAutoRangeX(bool bAuto)
{
    m_bAutoRangeX = bAuto;
    if (bAuto)
    {
        m_pGrapLine[0]->rescaleKeyAxis();
    }
}

void CurveGraph::setAutoRangeY(bool bAuto)
{
    m_bAutoRangeY = bAuto;
    if (bAuto)
    {
        m_pGrapLine[0]->rescaleValueAxis();
    }
}

// 设置X标签
QString CurveGraph::xLabel() const
{
    return ui->customPlot->xAxis->label();
}

void CurveGraph::setXLabel(QString label)
{
    ui->customPlot->xAxis->setLabel(label);
    ui->customPlot->replot();
}

// 设置Y标签
QString CurveGraph::yLabel() const
{
    return ui->customPlot->yAxis->label();
}

void CurveGraph::setYLabel(QString label)
{
    ui->customPlot->yAxis->setLabel(label);
    ui->customPlot->replot();
}

bool CurveGraph::showSubTicks() const
{
    return ui->customPlot->xAxis->subTicks();
}

void CurveGraph::setShowSubTicks(bool bShow)
{
    ui->customPlot->xAxis->setSubTicks(bShow);
    ui->customPlot->replot();
}

void CurveGraph::slotBtn()
{
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}

// 清除数据
void CurveGraph::clearValue()
{
    m_keyList.clear();
    m_valueList.clear();
    for (int i= 0; i < MAX_LINE_COUNT; i++)
    {
        m_pGrapLine[i]->setData(QVector<double>(), QVector<double>());
    }
    ui->customPlot->replot();
}
