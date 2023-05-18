#include "defectheatmapper.h"
#include <QPainter>
#include <QString>
#include <QRadialGradient>
#include <QImage>
#include <QMouseEvent>
#include <QDebug>
#include <QToolTip>
#include "heatmapper.h"
#include "gradientpalette.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "stringcommon.h"
#include "sendmessage.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "callsdk.h"

DefectheatmapperWidget::DefectheatmapperWidget(QWidget *parent)
	: QWidget(parent)
	, m_bInit(false)
	, m_xRadio(1)
	, m_yRadio(1)
{
	setMouseTracking(true);
	palette_ = new GradientPalette(DEFAULT_WIDTH);

	palette_->setColorAt(0.45, Qt::blue);
	palette_->setColorAt(0.55, Qt::cyan);
	palette_->setColorAt(0.65, Qt::green);
	palette_->setColorAt(0.85, Qt::yellow);
	palette_->setColorAt(1.0, Qt::red);

	canvas_ = new QImage(CANVAS_WIDTH, CANVAS_HEIGHT, QImage::Format_ARGB32);
	canvas_->fill(QColor(0, 0, 0, 0));
	mapper_ = new HeatMapper(canvas_, palette_, DEFAULT_RADIUS, DEFAULT_OPACITY);
	this->setFixedSize(CANVAS_WIDTH, CANVAS_HEIGHT);
	m_stRODCfg.m_totalData = NULL;
	readRODCfg();

	IDefectListManage* p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);

}

DefectheatmapperWidget::~DefectheatmapperWidget()
{
	if (NULL != mapper_)
	{
		delete mapper_;
	}

	if (NULL != palette_)
	{
		delete palette_;
	}
}

/*
* 绘图方法，当窗口刷新时该方法会被调用
*/
void DefectheatmapperWidget::paintEvent(QPaintEvent *)
{
 	QPainter painter(this);
	painter.drawImage(0, 0, *canvas_);
}

void DefectheatmapperWidget::mouseMoveEvent(QMouseEvent *event)
{
	QString strPos = QString("%1, %2").arg(QString::number((int)(event->pos().x() / m_xRadio))).arg(QString::number((int)(event->pos().y() / m_yRadio)));
	QToolTip::showText(mapToGlobal(event->pos()), strPos);
}

/*
* 该方法用于向绘图对象添加一个数据点
*/
//void DefectheatmapperWidget::mouseReleaseEvent(QMouseEvent *e)
//{
//	if (NULL == mapper_)
//		return;
//
//	mapper_->addPoint(e->x(), e->y());
//	this->update();
//}

void DefectheatmapperWidget::initHeatmap(int RangeX, int RangeY, vector<STPoint>& vecPos)
{
	if (m_bInit|| 0>= RangeX|| 0 >= RangeY)
	{
		return;
	}
	
	m_xRadio = (double)CANVAS_WIDTH / RangeX;
	m_yRadio = (double)CANVAS_HEIGHT / RangeY;

	QVector<int> data;
	data.resize(CANVAS_WIDTH*CANVAS_HEIGHT);
	for (int i = 0; i < vecPos.size(); i++)
	{
		int index = ((int)(vecPos.at(i).y*m_yRadio))*CANVAS_WIDTH + ((int)(vecPos.at(i).x))*m_xRadio;
		if (index>=CANVAS_WIDTH*CANVAS_HEIGHT)
		{
			index = CANVAS_WIDTH*CANVAS_HEIGHT - 1;
		}
		data[index]++;
	}

	mapper_->initData(data);
    //this->update(); // 优化页面卡顿，屏蔽update();
	m_bInit = true;
}

void DefectheatmapperWidget::clearUI()
{
	mapper_->clearData();
    //this->update(); // 优化页面卡顿，屏蔽update();
}

void DefectheatmapperWidget::updateUI()
{
    //this->update();// 优化页面卡顿，屏蔽update();
}


void DefectheatmapperWidget::addPoint(double dposx, double dposy)
{
	if (dposx == 0.0 && dposy == 0.0)
	{
		return;
	}
	mapper_->addPoint(dposx*m_xRadio, dposy*m_yRadio);
}

void DefectheatmapperWidget::addRegionOfDefectPoint(CImageProduct* pProduct)
{
	
	if (!m_stRODCfg.bEnableAll)
	{
		return;
	}
	readRODCfg();
	//testROD(pProduct);
	// 图片尺寸大小变化，重新初始化内存块
	if (m_stRODCfg.m_totalData == NULL)
	{
		m_stRODCfg.m_totalData = new stRODData(pProduct->getProductHeight() / m_stRODCfg.ibatchSize, pProduct->getProductWidth() / m_stRODCfg.ibatchSize);
	}
	else if (m_stRODCfg.m_totalData->ibatchColumnCnt != pProduct->getProductWidth() / m_stRODCfg.ibatchSize || m_stRODCfg.m_totalData->ibatchRowCnt != pProduct->getProductHeight() / m_stRODCfg.ibatchSize)
	{
		clearRegionOfDefectPoint(); //  图片尺寸大小变化 ，之前的数据统计无意义，进行清除
		m_stRODCfg.m_totalData->release();
		delete m_stRODCfg.m_totalData;
		m_stRODCfg.m_totalData = new stRODData(pProduct->getProductHeight() / m_stRODCfg.ibatchSize, pProduct->getProductWidth() / m_stRODCfg.ibatchSize);
	}

	// 删除超出的数据,
	stRODData* pstRODDatafirst = NULL;
	if (m_stRODCfg.m_stRODDataStack.size() >= m_stRODCfg.iContiueCnt)
	{
		pstRODDatafirst = m_stRODCfg.m_stRODDataStack.top();
		m_stRODCfg.m_stRODDataStack.pop();
		if (pstRODDatafirst != NULL)
		{
			if (m_stRODCfg.m_totalData->ibatchColumnCnt == pstRODDatafirst->ibatchColumnCnt 
				&& m_stRODCfg.m_totalData->ibatchRowCnt == pstRODDatafirst->ibatchRowCnt)
			{
				*m_stRODCfg.m_totalData = *m_stRODCfg.m_totalData - *pstRODDatafirst;
			}

			pstRODDatafirst->release();
			delete pstRODDatafirst;
		}
	}

	// 增加新一片的数据

	stRODData* pstRODData = new stRODData(pProduct->getProductHeight() / m_stRODCfg.ibatchSize, pProduct->getProductWidth() / m_stRODCfg.ibatchSize);
	m_stRODCfg.m_stRODDataStack.push(pstRODData);

	// 增加缺陷计数
	for (int i = 0; i < pProduct->pDefectStrutVec->size(); i++)
	{
		stDefectFeature *pDefectRes = pProduct->pDefectStrutVec->at(i);
		int iDefectType = pDefectRes->iDefectType;
		int PosX = pDefectRes->posx;
		int PosY = pDefectRes->posy;
		// 计算缺陷在第几块上	
		int iCol = PosX / m_stRODCfg.ibatchSize;
		int iRow = PosY / m_stRODCfg.ibatchSize;
		if (iRow >= pstRODData->ibatchRowCnt || iCol >= pstRODData->ibatchColumnCnt)
		{
			continue;
		}
        if (iDefectType < pstRODData->pDefectCnt[iRow][iCol].dvdefectVec.size() && iDefectType >0)
		{
            pstRODData->pDefectCnt[iRow][iCol].dvdefectVec[iDefectType]++;
            m_stRODCfg.m_totalData->pDefectCnt[iRow][iCol].dvdefectVec[iDefectType]++;
		}
	}

	// 扫描每一块的缺陷是否超标
    for (int iDefectType = 0; iDefectType < m_stRODCfg.dvdefectWarningVec.size() ; iDefectType++)
	{
        if (m_stRODCfg.dvdefectWarningVec[iDefectType] <=0)
		{
			continue;
		}
		for (int iRow = 0; iRow < m_stRODCfg.m_totalData->ibatchRowCnt;iRow++)
		{
			for (int iCol = 0; iCol < m_stRODCfg.m_totalData->ibatchColumnCnt;iCol++)
			{
                if (m_stRODCfg.m_totalData->pDefectCnt[iRow][iCol].dvdefectVec[iDefectType] >= m_stRODCfg.dvdefectWarningVec[iDefectType] )
				{
					QString strWarningMsg = QString::fromLocal8Bit("  连续 %1片在同一区域(%2-%3,%4-%5)检测检测到缺陷 %6 !")
						.arg(m_stRODCfg.iContiueCnt)
						.arg(iCol*m_stRODCfg.ibatchSize).arg(iCol*m_stRODCfg.ibatchSize + m_stRODCfg.ibatchSize)
						.arg(iRow*m_stRODCfg.ibatchSize).arg(iRow*m_stRODCfg.ibatchSize + m_stRODCfg.ibatchSize)
                        .arg(QString::fromStdString(m_defectNameVec[iDefectType]) + QString::number(m_stRODCfg.m_totalData->pDefectCnt[iRow][iCol].dvdefectVec[iDefectType]));
					// 超过规定值，提出警告
					CSendMessage::GetInstance()->Send(strWarningMsg, UILOG_WARN);
					emit this->sigdefectWarning(strWarningMsg);
				}
			}
		}
	}

}


void DefectheatmapperWidget::readRODCfg()
{
	IFileVariable* pFileVariable = getQualificationCfgFileVariable();
	if (pFileVariable == NULL)
	{
		return;
	}

	bool bRecount = pFileVariable->getValue(AREA_WARNING_RECOUNT, false).toBool();
	if (bRecount)
	{
		clearRegionOfDefectPoint();
		pFileVariable->setValue(AREA_WARNING_RECOUNT, false, true);
	}

	m_stRODCfg.bEnableAll = pFileVariable->getValue(AREA_WARNING_ENABLE_ALL, true).toBool();
	m_stRODCfg.iSearchWidth = pFileVariable->getValue(AREA_WARNING_WIDTH, true).toInt();
	m_stRODCfg.iSearchHeight = pFileVariable->getValue(AREA_WARNING_HEIGHT, true).toInt();
	m_stRODCfg.iContiueCnt = pFileVariable->getValue(AREA_WARNING_CNT, true).toInt();
	m_stRODCfg.ibatchSize = 512;
    //memset(m_stRODCfg.dvdefectWarningCnt, 0, sizeof(m_stRODCfg.dvdefectWarningCnt));
	pFileVariable->bindValue(AREA_WARNING_ENABLE_ALL, &m_stRODCfg.bEnableAll);
	//pFileVariable->bindValue(AREA_WARNING_WIDTH, &m_stRODCfg.iSearchWidth);
	//pFileVariable->bindValue(AREA_WARNING_HEIGHT, &m_stRODCfg.iSearchHeight);
	//pFileVariable->bindValue(AREA_WARNING_CNT, &m_stRODCfg.iContiueCnt);

	for (int i = 0; i < AREA_WARNING_ARRAY_CNT-1; i++)
	{
		m_stRODCfg.SearchEnable[i].bdefectenable = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTENABLE + SString::IntToStr(i), true).toBool();
		m_stRODCfg.SearchEnable[i].idefectype = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTYPE + SString::IntToStr(i), 0).toInt();
		m_stRODCfg.SearchEnable[i].idefectcnt = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTCNT + SString::IntToStr(i), 0).toInt();

        if (m_stRODCfg.SearchEnable[i].bdefectenable &&  m_stRODCfg.SearchEnable[i].idefectype < m_stRODCfg.dvdefectWarningVec.size())
		{
            m_stRODCfg.dvdefectWarningVec[m_stRODCfg.SearchEnable[i].idefectype] = m_stRODCfg.SearchEnable[i].idefectcnt;
		}
		//pFileVariable->bindValue(AREA_WARNING, AREA_WARNING_DEFECTENABLE + SString::IntToStr(i), &m_stRODCfg.SearchEnable[i].bdefectenable);
		//pFileVariable->bindValue(AREA_WARNING, AREA_WARNING_DEFECTYPE + SString::IntToStr(i), &m_stRODCfg.SearchEnable[i].idefectype);
		//pFileVariable->bindValue(AREA_WARNING, AREA_WARNING_DEFECTCNT + SString::IntToStr(i), &m_stRODCfg.SearchEnable[i].idefectcnt);
	}

}


void DefectheatmapperWidget::clearRegionOfDefectPoint()
{
	if (m_stRODCfg.m_totalData != NULL)
	{
		m_stRODCfg.m_totalData->clear();
	}

	while (m_stRODCfg.m_stRODDataStack.size() > 0)
	{
		stRODData* pstRODDatafirst = m_stRODCfg.m_stRODDataStack.top();
		m_stRODCfg.m_stRODDataStack.pop();
		if (pstRODDatafirst != NULL)
		{
			pstRODDatafirst->release();
			delete pstRODDatafirst;
		}
	}
}

void DefectheatmapperWidget::testROD(CImageProduct* pProduct)
{
	IFileVariable* pFileVariable = getQualificationCfgFileVariable();
	if (pFileVariable == NULL)
	{
		return;
	}
	bool bdefectenable = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTENABLE + SString::IntToStr(5), true).toBool();
	int idefectype = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTYPE + SString::IntToStr(5), 0).toInt();
	int idefectcnt = pFileVariable->getValue(AREA_WARNING, AREA_WARNING_DEFECTCNT + SString::IntToStr(5), 0).toInt();
	m_stRODCfg.iSearchWidth = pFileVariable->getValue(AREA_WARNING_WIDTH, true).toInt();
	m_stRODCfg.iSearchHeight = pFileVariable->getValue(AREA_WARNING_HEIGHT, true).toInt();

	for (int i = 0; i < idefectcnt;i++)
	{
		stDefectFeature *pDefectRes = new stDefectFeature();;
		pDefectRes->iDefectType = idefectype;
		pDefectRes->posx = m_stRODCfg.iSearchWidth;
		pDefectRes->posy = m_stRODCfg.iSearchHeight;
		pProduct->pDefectStrutVec->push_back(pDefectRes);
	}

}
