#include "defectheatmapclassify.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QScatterSeries>
#include <QGraphicsLayout>
#include <QPointF>
#include <QScatterSeries>
#include <QValueAxis>
#include <QSplineSeries>
#include <QCategoryAxis>
#include "commonapi.h"
#include "iniFile.h"
#include <QFileDialog>
QT_CHARTS_USE_NAMESPACE


DefectheatmapclassifyWidget::DefectheatmapclassifyWidget(QWidget * parent)
	: QWidget(parent)
	, m_pChart(NULL)
	, m_bInit(false)
{
	m_pHLayout = new QHBoxLayout(this);
	m_pHLayout->setMargin(0);
	m_pHLayout->setContentsMargins(0,0,0,0);
	setLayout(m_pHLayout);

	setStyleSheet("background - color: rgb(199, 201, 229);");
	createImage();
													
	m_mapDefectDes[heatmap_Smirch]   = "����";      
	m_mapDefectDes[heatmap_hair]     = "ë��";
	m_mapDefectDes[heatmap_dirtpits] = "����";
	m_mapDefectDes[heatmap_Scratches] = "����";

	m_mapDefectDes[heatmap_Chips] = "����";
	m_mapDefectDes[heatmap_Dent] = "dent";
	m_mapDefectDes[heatmap_BrightLine] = "�";
	m_mapDefectDes[heatmap_Impress] = "ѹ��";

	m_mapDefectDes[heatmap_LightLeak] = "©��";
	m_mapDefectDes[heatmap_ExcessLnk] = "��ī";
	m_mapDefectDes[heatmap_Discolouration] = "��ɫ";
	//m_mapDefectDes[heatmap_NarrowEdge] = "խ��"
	//	;
	//m_mapDefectDes[heatmap_DecomeDeformed] = "����";
	//m_mapDefectDes[heatmap_FalseScratches] = "ģ������";
	//m_mapDefectDes[heatmap_FalseSmirch] = "ģ������";
	//m_mapDefectDes[heatmap_FalseChips] = "ģ������";
	//
	//m_mapDefectDes[heatmap_FalseDent] = "ģ��dent";
	//m_mapDefectDes[heatmap_FalseLightLeak] = "ģ��©��";
	//m_mapDefectDes[heatmap_FalseDiscolouration] = "ģ����ɫ";
	//m_mapDefectDes[heatmap_FalseBrightLine] = "ģ���";

	m_mapImage[heatmap_Smirch] = m_vectorImage[0];
	m_mapImage[heatmap_hair] = m_vectorImage[1];
	m_mapImage[heatmap_dirtpits] = m_vectorImage[2];
	m_mapImage[heatmap_Scratches] = m_vectorImage[3];
	
	m_mapImage[heatmap_Chips] = m_vectorImage[4];
	m_mapImage[heatmap_Dent] = m_vectorImage[5];
	m_mapImage[heatmap_BrightLine] = m_vectorImage[6];
	m_mapImage[heatmap_Impress] = m_vectorImage[7];

	m_mapImage[heatmap_LightLeak] = m_vectorImage[8];
	m_mapImage[heatmap_ExcessLnk] = m_vectorImage[9];
	m_mapImage[heatmap_Discolouration] = m_vectorImage[10];
	//m_mapImage[heatmap_NarrowEdge] = m_vectorImage[5];
	//
	//m_mapImage[heatmap_DecomeDeformed] = m_vectorImage[0];
	//m_mapImage[heatmap_FalseScratches] = m_vectorImage[1];
	//m_mapImage[heatmap_FalseSmirch] = m_vectorImage[2];
	//m_mapImage[heatmap_FalseChips] = m_vectorImage[3];
	
	//m_mapImage[heatmap_FalseDent] = m_vectorImage[4];
	//m_mapImage[heatmap_FalseLightLeak] = m_vectorImage[5];
	//m_mapImage[heatmap_FalseDiscolouration] = m_vectorImage[0];
	//m_mapImage[heatmap_FalseBrightLine] = m_vectorImage[1];

	
	

}

DefectheatmapclassifyWidget::~DefectheatmapclassifyWidget()
{

}

void DefectheatmapclassifyWidget::initHeatMap(int RangeX, int RangeY, bool bByPiece)
{
	if (m_bInit)
	{
		return;
	}
	QChartView *pChartView = new QChartView();
	QVBoxLayout *pVLayout = new QVBoxLayout();
	pVLayout->setContentsMargins(0, 0, 0, 0);
	m_pHLayout->addWidget(pChartView);
	m_pHLayout->addLayout(pVLayout);

	QChart *pChart = pChartView->chart();
	m_pChart = pChart;
	pChartView->setRenderHint(QPainter::Antialiasing);
	pChartView->setContentsMargins(0, 0, 0, 0);
	pChartView->setStyleSheet("background-image:url(:/image/heatmapbackground_gray.png);background-repeat: no-repeat;");

	QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0, RangeX);    //���÷�Χ
	axisX->setGridLineVisible(false);   //�����߿ɼ�
	axisX->setLabelsVisible(false);
	axisX->setTickCount(2);
	axisX->setMinorTickCount(0);
	axisX->setLinePenColor(Qt::gray);

	QValueAxis *axisX1 = new QValueAxis;
	axisX1->setRange(0, RangeX);    //���÷�Χ
	axisX1->setGridLineVisible(false);   //�����߿ɼ�
	axisX1->setLabelsVisible(false);
	axisX1->setTickCount(2);
	axisX1->setMinorTickCount(0);
	axisX1->setLinePenColor(Qt::gray);

	QValueAxis *axisY = new QValueAxis;
	axisY->setReverse(true);
	axisY->setRange(0, RangeY);
	axisY->setGridLineVisible(false);
	axisY->setLabelsVisible(false);
	axisY->setTickCount(2);
	axisY->setMinorTickCount(0);
	axisY->setLinePenColor(Qt::gray);

	QValueAxis *axisY1 = new QValueAxis;
	axisY1->setReverse(true);
	axisY1->setRange(0, RangeY);
	axisY1->setGridLineVisible(false);
	axisY1->setLabelsVisible(false);
	axisY1->setTickCount(2);
	axisY1->setMinorTickCount(0);
	axisY1->setLinePenColor(Qt::gray);

	pChart->addAxis(axisX, Qt::AlignTop);
	pChart->addAxis(axisY, Qt::AlignLeft);
	pChart->addAxis(axisX1, Qt::AlignBottom);
	pChart->addAxis(axisY1, Qt::AlignRight);
    axisX->setLineVisible(false);
	axisX1->setLineVisible(false);
	axisY->setLineVisible(false);
	axisY1->setLineVisible(false);

	pVLayout->addStretch();

	QMap<int, string >::const_iterator iter = m_mapDefectDes.begin();
	for (iter; iter != m_mapDefectDes.end(); iter++)
	{ 
		int index = iter.key();
		index = changeIndex(index);
		QScatterSeries *pScatter = new QScatterSeries();
		pScatter->setName(QString::fromLocal8Bit(iter.value().c_str()));
		pScatter->setMarkerSize(10);
		pScatter->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
		pChart->addSeries(pScatter);
		pScatter->attachAxis(axisX);
		pScatter->attachAxis(axisY);
		
		pScatter->setBrush(m_mapImage[index]);
		pScatter->setPen(QColor(Qt::transparent));
		
		QCheckBox *pCheckBox = new QCheckBox();
		pCheckBox->setText(QString("%1 (%2)").arg(QString::fromLocal8Bit(iter.value().c_str())).arg(m_mapPos[index].size()));
		pCheckBox->setIcon(QIcon(QPixmap::fromImage(m_mapImage[index])));
		pCheckBox->setChecked(true);
		connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(slotSelectChanged(int)));
		pVLayout->addWidget(pCheckBox);

		m_mapCheckSeries[pCheckBox] = pScatter;
		m_mapSeries[index] = pScatter;
		m_mapCheckBox[index] = pCheckBox;
	}

	pVLayout->addStretch();

	pChart->setTitle(QString::fromLocal8Bit("��Ƭȱ�ݷֲ�����ͼ"));
	if (!bByPiece)
	{
		pChart->setTitle(QString::fromLocal8Bit("�����ȱ�ݷֲ�����ͼ"));
		readDefectHeatMapFile();//ֻ�е�������ݲŶ�
	}
	pChart->setAnimationOptions(QChart::SeriesAnimations);
	pChart->legend()->setVisible(false);
	pChart->setBackgroundRoundness(0);
	pChart->layout()->setContentsMargins(0, 0, 0, 0);
	pChart->setBackgroundBrush(Qt::transparent);
	pChart->setDropShadowEnabled(false);
	initData();
	m_bInit = true;
}

void DefectheatmapclassifyWidget::initData()
{
	QMap<int, vector<STPoint> >::const_iterator iter = m_mapPos.begin();
	for (iter; iter != m_mapPos.end(); iter++)
	{
		int index = iter.key();
		index = changeIndex(index);
		if (m_mapSeries.contains(index))
		{
			for (int i = 0; i < iter.value().size(); i++)
			{
				m_mapSeries[index]->append(iter.value()[i].x, iter.value()[i].y);  //��ʾ
			}
		}
	}
}

void DefectheatmapclassifyWidget::addData(int index, double dposx, double dposy, bool Write)
{
	if (dposx == 0.0 && dposy == 0.0)
	{
		return;
	}
	int index2 = changeIndex(index);
	if (m_mapSeries.contains(index2))
	{
		m_mapSeries[index2]->append(dposx, dposy);    //��ʾ
		STPoint tmp;
		tmp.x = dposx; tmp.y = dposy;
		m_mapPos[index2].push_back(tmp);  //���Ӽ���
		int isze = m_mapPos[index2].size();
		QCheckBox* pCheckBox = m_mapCheckBox[index2];
		if (pCheckBox != NULL)
		{
			pCheckBox->setText(QString("%1 (%2)").arg(QString::fromLocal8Bit(m_mapDefectDes[index2].c_str())).arg(isze));   //����������ʾ
		}
		if (Write)
		{
			writeDefectHeatMapFile(index, dposx, dposy);
		}
		
	}
}


void DefectheatmapclassifyWidget::clearUI()
{
	for (int i = 0; i < m_pChart->series().size(); i++)
	{
		QScatterSeries *pSeries = qobject_cast<QScatterSeries*>(m_pChart->series()[i]);
		pSeries->clear();
	}
	QMap<int, vector<STPoint> >::iterator iter = m_mapPos.begin();
	for (iter; iter != m_mapPos.end(); iter++)
	{
		int index = iter.key();
		QCheckBox* pCheckBox = m_mapCheckBox[index];
		if (pCheckBox != NULL)
		{
			pCheckBox->setText(QString("%1 (0)").arg(QString::fromLocal8Bit(m_mapDefectDes[index].c_str())));   //����������ʾ
		}
		iter.value().clear();
		for (size_t i = 0; i < iter.value().size(); i++)
		{
			iter.value().pop_back();
		}
	}
}

void DefectheatmapclassifyWidget::createImage()
{
	int w = 10;

	// ͼ��1��������
	QPainterPath starPath1;
	starPath1.moveTo(0, 0);
	starPath1.lineTo(0, w);
	starPath1.lineTo(w, w);
	starPath1.lineTo(w, 0);
	starPath1.closeSubpath();

	QImage rect(w, w, QImage::Format_ARGB32);
	rect.fill(Qt::transparent);

	QPainter painter1(&rect);
	painter1.setRenderHint(QPainter::Antialiasing);
	painter1.setPen(QRgb(0xf6a625));
	painter1.setBrush(painter1.pen().color());
	painter1.drawPath(starPath1);

	// ͼ��2��������
	QPainterPath starPath2;
	starPath2.moveTo(0, 0);
	starPath2.lineTo(w/2, w);
	starPath2.lineTo(w, 0);
	starPath2.closeSubpath();

	QImage triangle(w, w, QImage::Format_ARGB32);
	triangle.fill(Qt::transparent);

	QPainter painter2(&triangle);
	painter2.setRenderHint(QPainter::Antialiasing);
	painter2.setPen(Qt::blue);
	painter2.setBrush(painter2.pen().color());
	painter2.drawPath(starPath2);

	// ͼ��3������
	QPainterPath starPath3;
	starPath3.moveTo(0, w/2);
	starPath3.lineTo(w/2, w);
	starPath3.lineTo(w, w/2);
	starPath3.lineTo(w/2, 0);
	starPath3.closeSubpath();

	QImage rhombus(w, w, QImage::Format_ARGB32);
	rhombus.fill(Qt::transparent);

	QPainter painter3(&rhombus);
	painter3.setRenderHint(QPainter::Antialiasing);
	painter3.setPen(Qt::darkYellow);
	painter3.setBrush(painter3.pen().color());
	painter3.drawPath(starPath3);

	// ͼ��4��Բ��
	QPainterPath starPath4;
	starPath4.addEllipse(0,0,w,w);

	QImage circle(w, w, QImage::Format_ARGB32);
	circle.fill(Qt::transparent);

	QPainter painter4(&circle);
	painter4.setRenderHint(QPainter::Antialiasing);
	painter4.setPen(Qt::darkRed);
	painter4.setBrush(painter4.pen().color());
	painter4.drawPath(starPath4);

	// ͼ��5����ʮ��
	QPainterPath starPath5;
	starPath5.moveTo(0, w/2);
	starPath5.lineTo(w, w/2);
	starPath5.moveTo(w/2, 0);
	starPath5.lineTo(w/2, w);

	QImage cross(w, w, QImage::Format_ARGB32);
	cross.fill(Qt::transparent);

	QPainter painter5(&cross);
	painter5.setRenderHint(QPainter::Antialiasing);
	painter5.setPen(Qt::darkMagenta);
	painter5.setBrush(painter5.pen().color());
	painter5.drawPath(starPath5);

	// ͼ��6��ʮ�ֲ�
	QPainterPath starPath6;
	starPath6.moveTo(0, 0);
	starPath6.lineTo(w, w);
	starPath6.moveTo(0, w);
	starPath6.lineTo(w, 0);

	QImage cross2(w, w, QImage::Format_ARGB32);
	cross2.fill(Qt::transparent);

	QPainter painter6(&cross2);
	painter6.setRenderHint(QPainter::Antialiasing);
	painter6.setPen(Qt::darkGreen);
	painter6.setBrush(painter6.pen().color());
	painter6.drawPath(starPath6);

	// ͼ��7��������2
	QImage rect2(w, w, QImage::Format_ARGB32);
	QPainter painter7(&rect2);
	painter7.setRenderHint(QPainter::Antialiasing);
	painter7.setPen(Qt::green);  //������ɫ
	painter7.setBrush(painter7.pen().color());
	painter7.drawPath(starPath1);

	// ͼ��8��������2
	QImage triangle2(w, w, QImage::Format_ARGB32);
	QPainter painter8(&triangle2);
	painter8.setRenderHint(QPainter::Antialiasing);
	painter8.setPen(Qt::yellow);  //������ɫ
	painter8.setBrush(painter8.pen().color());
	painter8.drawPath(starPath2);

	// ͼ��9������2
	QImage rhombus2(w, w, QImage::Format_ARGB32);
	QPainter painter9(&rhombus2);
	painter9.setRenderHint(QPainter::Antialiasing);
	painter9.setPen(Qt::darkBlue);  //������ɫ
	painter9.setBrush(painter9.pen().color());
	painter9.drawPath(starPath3);

	// ͼ��10��Բ��2
	QImage circle2(w, w, QImage::Format_ARGB32);
	QPainter painter10(&circle2);
	painter10.setRenderHint(QPainter::Antialiasing);
	painter10.setPen(Qt::darkCyan);  //������ɫ
	painter10.setBrush(painter10.pen().color());
	painter10.drawPath(starPath4);

	// ͼ��11����ʮ��2
	QImage cross_2(w, w, QImage::Format_ARGB32);
	QPainter painter11(&cross_2);
	painter11.setRenderHint(QPainter::Antialiasing);
	painter11.setPen(Qt::red);  //������ɫ
	painter11.setBrush(painter11.pen().color());
	painter11.drawPath(starPath5);

	// ͼ��12��ʮ�ֲ�2
	QImage cross2_2(w, w, QImage::Format_ARGB32);
	QPainter painter12(&cross2_2);
	painter12.setRenderHint(QPainter::Antialiasing);
	painter12.setPen(QRgb(0x666666));  //������ɫ
	painter12.setBrush(painter12.pen().color());
	painter12.drawPath(starPath6);

	// ͼ�걣��
	m_vectorImage << rect << triangle<< rhombus << circle << cross << cross2 << rect2 << triangle2 << rhombus2 << circle2 << cross_2 << cross2_2;
}

void DefectheatmapclassifyWidget::slotSelectChanged(int checked)
{
	QCheckBox *pCheckBox = qobject_cast<QCheckBox*>(sender());
	if (m_mapCheckSeries.contains(pCheckBox))
	{
		m_mapCheckSeries[pCheckBox]->setVisible(checked == Qt::Checked);
	}
}

void DefectheatmapclassifyWidget::writeDefectHeatMapFile(int itype, double dposx, double dposy)
{
	if (dposx == 0 || dposy == 0)
	{
		return;
	}
	CIniFile iniFile;

	QString strSavePath = getCurrenTimeDayOrNightPath();
	QString FileName = strSavePath + "/" + getCurrentProducsTimeStrPath() + "_HeatMapPoint.csv";
	QFile file(FileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		return;
	QString msg = QString("posx=%1,poxy=%2,type=%3\n").arg(QString::number(dposx, 'f', 3)).arg(QString::number(dposy, 'f', 3)).arg(itype);
	file.write(msg.toStdString().c_str(), msg.length());
	file.flush();
	file.close();
}

void DefectheatmapclassifyWidget::readDefectHeatMapFile()
{
	QString strSavePath = getCurrenTimeDayOrNightPath();
	QString FileName = strSavePath + "/" + getCurrentProducsTimeStrPath() + "_HeatMapPoint.csv";
	//D:\uilog\ProductResult\2020-08-05\2020-08-05_HeatMapPoint.csv
	if (!QFile::exists(FileName))
	{
		return;
	}
	QFile file(FileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	while (!file.atEnd()) {
		QString line = file.readLine();
 		if (line.isEmpty())
		{
			continue;
		}
		STPoint tmp;
		int iType = 0;
		//sscanf(line.toStdString().c_str(), "posx=%.3f,poxy=%.3f,type=%d", &tmp.x, &tmp.y, &iType);
		QStringList strlist = line.split(",");
		tmp.x = strlist[0].mid(5).toDouble();
		tmp.y = strlist[1].mid(5).toDouble();
		iType = strlist[2].mid(5).toInt();
		m_mapPos[iType].push_back(tmp);
	}
	file.close();
	return;
}

void DefectheatmapclassifyWidget::readDefectHeatMapFile(QString strDate,int worktimes)
{
	QString strDir = getSaveResultFilePath();
	QString strSavePath = strDir + "/" + strDate;
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);
		
	}
	
	strSavePath += "/" + QString::fromLocal8Bit(szDayPath[worktimes]);
	QDir dir3(strSavePath);
	if (!dir3.exists(strSavePath))
	{
		dir3.mkdir(strSavePath);
	}
	QString FileName = strSavePath + "/" + strDate + "_HeatMapPoint.csv";
	//D:\uilog\ProductResult\2020-08-05\2020-08-05_HeatMapPoint.csv
	m_mapPos.clear();
	if (!QFile::exists(FileName))
	{
		return;
	}
	QFile file(FileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	while (!file.atEnd()) {
		QString line = file.readLine();
		if (line.isEmpty())
		{
			continue;
		}
		STPoint tmp;
		int iType = 0;
		//sscanf(line.toStdString().c_str(), "posx=%.3f,poxy=%.3f,type=%d", &tmp.x, &tmp.y, &iType);  //��ȡ���ݸ�ʽ����ȷ
		QStringList strlist = line.split(",");
		if (strlist.size() == 3)
		{
			tmp.x = strlist[0].mid(5).toDouble();
			tmp.y = strlist[1].mid(5).toDouble();
			iType = strlist[2].mid(5).toInt();
			m_mapPos[iType].push_back(tmp);
		}
	}
	file.close();
	return;
}

void DefectheatmapclassifyWidget::slotDateChanged(QString cDate)
{
	//readDefectHeatMapFile(cDate);
	//showHeatMapPoint();
}

void DefectheatmapclassifyWidget::showHeatMapPoint()
{
	//ˢ�½���ǰ�����
	for (int i = 1; i <= m_mapSeries.size(); i++)
	{
		m_mapSeries[i]->clear();
	}
	QMap<int, vector<STPoint> >::const_iterator iter = m_mapPos.begin();
	for (iter; iter != m_mapPos.end(); iter++)
	{
		int index = iter.key();
		index = changeIndex(index);
		if (m_mapSeries.contains(index))
		{
			QCheckBox* pCheckBox = m_mapCheckBox[index];

			for (int i = 0; i < iter.value().size(); i++)
			{
				m_mapSeries[index]->append(iter.value()[i].x, iter.value()[i].y);  //��ʾ
			}
			if (pCheckBox != NULL)
			{
				pCheckBox->setText(QString("%1 (%2)").arg(QString::fromLocal8Bit(m_mapDefectDes[index].c_str())).arg(m_mapSeries[index]->count()));   //����������ʾ
			}
		}
	}
    // update(); // �Ż�ҳ�濨�٣�����update();
}

int DefectheatmapclassifyWidget::changeIndex(int index)
{
	if(index < 12)		//
		return index;
	if (index == 12)  //
		return 7;
	if (index == 16)  //ģ������
		return 1;
	if (index == 17)  //ģ������
		return 4;
	if (index == 18)	//ģ������
		return 5;		//
	if (index == 19)	//ģ��dent
		return 6;		//
	if (index == 20)	//ģ��©��
		return 8;		//
	if (index == 21)	//ģ����ɫ
		return 10;		//
	if (index == 22)	//ģ���
		return 11;
}


void DefectheatmapclassifyWidget::updateUI(QString cdate,int worktimes)
{
	readDefectHeatMapFile(cdate,worktimes);
	showHeatMapPoint();
}
