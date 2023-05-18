#include <QtGui>
#include "diagramscene.h"
#include "sendmessage.h"
#include "g12common.h"
#include "stringcommon.h"
#include <QGraphicsSceneMoveEvent>

//! [0]

DiagramScene::DiagramScene(enScene iScene ,QObject *parent)
    : QGraphicsScene(parent)
{
    m_operateType = OT_None;
    textItem = 0;
    m_iScne = iScene;
    connect(CSendMessage::GetInstance(),SIGNAL(signalNewDefet(double,double)),this,SLOT(slotSetNewDefet(double,double)));
    connect(CSendMessage::GetInstance(),SIGNAL(signalAddsetdecet(MarkStrut *)),this,SLOT(slotAddsetdecet(MarkStrut *)));
    //connect(CSendMessage::GetInstance(),SIGNAL(signalAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> *,int)),this,SLOT(slotAddMapVecsetdecet(QMap<QString, QVector<MarkStrut *> *> *,int)));
    connect(CSendMessage::GetInstance(),SIGNAL(signalChooseShow(int *,int)),this,SLOT(slotChooseShow(int *,int)));
    connect(CSendMessage::GetInstance(), SIGNAL(signalOneDefecUI(stDefectFeature, int)), this, SLOT(slotAddDefectRes(stDefectFeature, int)));//add 20180513
    connect(CSendMessage::GetInstance(), SIGNAL(signalAddCutROIUI(QRect, int)), this, SLOT(slotAddRoiInfo(QRect, int)));//add 20180513
	connect(CSendMessage::GetInstance(), SIGNAL(signalClearTable()), this, SLOT(slotClearDefect())/*, Qt::BlockingQueuedConnection*/);
    string programs = getCurrentProgramRecipesList();
    m_strSequence = SString::split(programs,",");
	for (int i = 0; i < 8;i++)
	{
		QList<stDefectFeature> p;
		m_vListDefec.push_back(p);
	}
    memset(&m_iCheckShow,1,sizeof(int)*8);
    M_qMapMark= NULL;
	m_bUnion = false;
}
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]

//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //if (mouseEvent->button() != Qt::MidButton)
    //{
    //    QGraphicsScene::mousePressEvent(mouseEvent);
    //    return;
    //}
    m_operateType = OT_None;
    if (mouseEvent->button() == Qt::MiddleButton)
    {
        m_operateType = OT_Move;
        emit sigOperatypeChang(m_operateType); 
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        m_operateType = OT_None;
        emit sigOperatypeChang(m_operateType); 
        emit sigRightButtonClicked(mouseEvent->scenePos());
    }
   //else if (mouseEvent->button() == Qt::LeftButton)
   //{
   //    QRectF rect(mouseEvent->scenePos(), mouseEvent->scenePos());
   //    emit sigLeftButtonClicked(rect);
   //}
       

    QGraphicsScene::mousePressEvent(mouseEvent);
    return;
}
//! [9]

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if (!sceneRect().contains(mouseEvent->scenePos()))
    {
        return;
    }
	QPointF scenePos = mouseEvent->scenePos();
    emit sigCoordinate(scenePos);
    //emit sigUpdateGray(mouseEvent->scenePos().toPoint());
    //update(); // 优化页面卡顿，屏蔽update();

    //// 绘图模式屏蔽item鼠标事件
    //if (!m_curItem && m_operateType == OT_Draw)
    //{
    //    return;
    //}
      QGraphicsScene::mouseMoveEvent(mouseEvent);

      //m_statusBarCoordinate.setText(str);
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::Addsetdecet( MarkStrut mark )
{
   if (mark.bRect)
   {
        addRect(mark.posx,mark.posy,mark.dWide,mark.dHigh,QPen(Qt::red, 5));
        return;
   }
   //mChip = new Chip(mark);
  // QPointF pt(mark.posx,mark.posy);
  // mChip->setPos(pt);
  // addItem(mChip);
  // m_ListchipItem.append(mChip);
    
}


void DiagramScene::setNewDefet( double width,double height )
{
    foreach(Chip* item,m_ListchipItem)
    {
        removeItem(item);
    }
    m_ListchipItem.clear();
    clear();
    m_iW = width;
    m_iH = height;
    //setSceneRect(QRectF(0, 0, width, height));
    //setForegroundBrush(QColor(199,201,229,127));
    //update(QRectF(0, 0, width, height));
    //emit sigUpdteSize(QRectF(0, 0, width, height));
}

void DiagramScene::getNewDefet( double& width,double& height )
{
    width = m_iW;
    height = m_iH;
}


void DiagramScene::slotSetNewDefet( double width,double height )
{
    setNewDefet( width, height);
}

void DiagramScene::slotAddsetdecet( MarkStrut *mark )
{
    if (NULL == mark)
    {
        return;
    }
    Addsetdecet( *mark);
    delete mark;
}

void DiagramScene::slotAddVecsetdecet( QVector<MarkStrut*>* vecmark )
{
    //
    foreach(Chip* item,m_ListchipItem)
    {
        removeItem(item);
    }
    m_ListchipItem.clear();
    clear();

    if (NULL == vecmark)
    {
        return;
    }
    for (int i=0; i<vecmark->size(); i++)
    {
        Addsetdecet(*vecmark->at(i));
    }
    delete vecmark;
}

void DiagramScene::setCtrlWidth( int iWidth )
{
    foreach (QGraphicsItem *pItem, items())
    {
         //setControlWidth(iWidth);
        //  updateControls();
    }
}

void DiagramScene::AddPolygon(QPolygon polygon)
{
    addPolygon(polygon, QPen(Qt::green, 1));
}

OPERATE_TYPE DiagramScene::getOperateType()
{
    return m_operateType;
}

void DiagramScene::AddsetdecetBySequence(stDefectFeature Defect)
{
    if (0 == Defect.strChannelName.compare("SEQUENCE_ABC"))
    {
        Defect.strChannelName = "SEQUENCE_A";
		m_bUnion = true;
    }
	string stStation = "";
	int ilen = 1;
	//if (m_iScne>0)
	{
		stStation = std::to_string(m_iScne);
	}
	//
	for (int i = 0; i < m_strSequence.size();i++)
	{
		if (0 == Defect.strChannelName.compare(m_strSequence.at(i) + stStation))
		{
			//mListA_Defec.append(Defect);
			m_vListDefec.at(i).append(Defect);
			if (m_iCheckShow[i])
			{
				setDefect(Defect);
				
			}
			return;
		}
	}

	/*
	if (m_strSequence.size()>0 && 0 == Defect.strChannelName.compare(m_strSequence.at(0)+ stStation))
	{
		mListA_Defec.append(Defect);
		if (m_iCheckShow[0])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>1 && 0 == Defect.strChannelName.compare(m_strSequence.at(1) + stStation))
	{
		mListB_Defec.append(Defect);
		if (m_iCheckShow[1])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>2 && 0 == Defect.strChannelName.compare(m_strSequence.at(2) + stStation))
	{
		mListC_Defec.append(Defect);
		if (m_iCheckShow[2])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>3 && 0 == Defect.strChannelName.compare(m_strSequence.at(3) + stStation))//4
	{
		mListD_Defec.append(Defect);
		if (m_iCheckShow[3])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>4 && 0 == Defect.strChannelName.compare(m_strSequence.at(4) + stStation))
	{
		mListE_Defec.append(Defect);
		if (m_iCheckShow[4])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>5 && 0 == Defect.strChannelName.compare(m_strSequence.at(5) + stStation))
	{
		mListF_Defec.append(Defect);
		if (m_iCheckShow[5])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>6 && 0 == Defect.strChannelName.compare(m_strSequence.at(6) + stStation))
	{
		mListG_Defec.append(Defect);
		if (m_iCheckShow[6])
		{
			setDefect(Defect);
		}
	}
	else if (m_strSequence.size()>7 && 0 == Defect.strChannelName.compare(m_strSequence.at(7) + stStation))
	{
		mListH_Defec.append(Defect);
		if (m_iCheckShow[7])
		{
			setDefect(Defect);
		}
	}*/
}
void DiagramScene::setDefect(stDefectFeature defect)
{
    int x = defect.posx - defect.dWide / 2;
    x = x > 0 ? x : 0;
    int y = defect.posy - defect.dHigh / 2;
    y = y > 0 ? y : 0;
	//addRect(defect.posx- defect.dWide/2, defect.posy- defect.dHigh/2, defect.dWide, defect.dHigh, QPen(Qt::red, 1));//add 20180619
    addRect(x, y, defect.dWide, defect.dHigh, QPen(Qt::red, 1));//add 20180619
	mChip = new Chip(defect);
	QPointF pt(x, y);
	mChip->setPos(pt);
    mChip->setOpacity(0.2);
	addItem(mChip);
	m_ListchipItem.append(mChip);
}
void DiagramScene::addROIRect(QRect roi)
{
    addRect(roi.x(), roi.y(), roi.width(), roi.height(), QPen(Qt::green, 1));
}

void DiagramScene::ClearDefect()
{
	foreach(Chip* item, m_ListchipItem)
	{
		removeItem(item);
        delete item;
	}
	m_ListchipItem.clear();
	clear();
	for (size_t i = 0; i < m_vListDefec.size(); i++)
	{
		m_vListDefec.at(i).clear();
	}
	//reach(QList<stDefectFeature> List_Defec, m_vListDefec)
	//
	////
	//List_Defec.clear();
	////
	//
	/*
	mListA_Defec.clear();
	mListB_Defec.clear();
	mListC_Defec.clear();
	mListD_Defec.clear();

	mListE_Defec.clear();
	mListF_Defec.clear();
	mListG_Defec.clear();
	mListH_Defec.clear();*/
	
}

void DiagramScene::ShowChoose()
{
	for (int index = 0; index < m_strSequence.size(); index++)
	{
		if (m_iCheckShow[index])
		{
			//
			foreach(stDefectFeature Defect, m_vListDefec.at(index))
			{
				setDefect(Defect);
			}
			return;
			/*
			switch (index)
			{
			case 0:
				foreach (stDefectFeature Defect , mListA_Defec)
				{
					setDefect(Defect);
				}
				break;
			case  1:
				foreach(stDefectFeature Defect, mListB_Defec)
				{
					setDefect(Defect);
				}
				break;
			case 2:
				foreach(stDefectFeature Defect, mListC_Defec)
				{
					setDefect(Defect);
				}
				break;
			case 3:
				foreach(stDefectFeature Defect, mListD_Defec)
				{
					setDefect(Defect);
				}
				break;
			case 4:
				foreach(stDefectFeature Defect, mListE_Defec)
				{
					setDefect(Defect);
				}
				break;
			case 5:
				foreach(stDefectFeature Defect, mListF_Defec)
				{
					setDefect(Defect);
				}
				break; 
			case 6:
				foreach(stDefectFeature Defect, mListG_Defec)
				{
					setDefect(Defect);
				}
				break;
			case 7:
				foreach(stDefectFeature Defect, mListH_Defec)
				{
					setDefect(Defect);
				}
				break;
			default:
				break;
			}
			*/
		}
	}
}

void DiagramScene::slotAddMapVecsetdecet( QMap<QString, QVector<MarkStrut *> *> * mapMark  ,int iScne )
{
    if (NULL == mapMark || m_iScne !=  (enScene)iScne)
    {
        return;
    }
    // 保存最新的点位数据
    if(M_qMapMark != NULL)
    {
        QMap<QString, QVector<MarkStrut *> *>::iterator iKeyVal = M_qMapMark->begin();
        while (iKeyVal != M_qMapMark->end())
        {
            QVector<MarkStrut*>* veMark = iKeyVal.value();
            for (int i=0; i<veMark->size(); i++)
            {
                delete veMark->at(i);
            } 
            iKeyVal++;
        }
    }
    M_qMapMark = mapMark;
    ShowChooseMark(mapMark);
}

void DiagramScene::slotChooseShow( int *showChoose ,int iScene)
{
    if (m_iScne != (enScene)iScene || NULL == showChoose)
    {
        return;
    }
    for (int index = 0;index < m_strSequence.size();index++)
    {
        m_iCheckShow[index] = showChoose[index];
    }
	//先清原有的
	foreach(Chip* item, m_ListchipItem)
	{
		removeItem(item);
       
	}
	m_ListchipItem.clear();
	clear();
	//
	//if (m_bUnion)
	//{
	//	foreach(stDefectFeature Defect, mListA_Defec)
	//	{
	//		setDefect(Defect);
	//	}
	//	return;
	//}
	//
	//显示选择的通道
	ShowChoose();
    //ShowChooseMark(M_qMapMark);
}

void DiagramScene::slotAddDefectRes(stDefectFeature Defect, int iScne)
{
	if (m_iScne != (enScene)iScne )
	{
		return;
	}
	AddsetdecetBySequence(Defect);
}
void DiagramScene::slotAddRoiInfo(QRect Defect, int iScne)
{
	if (m_iScne != (enScene)iScne )
	{
		return;
	}
    addROIRect(Defect);
}

void DiagramScene::slotClearDefect()
{
	ClearDefect();
}

bool DiagramScene::ShowChooseMark(QMap<QString, QVector<MarkStrut *> *>* pMap)
{
    if (pMap == NULL)
    {
        return false;
    }
   // CSendMessage::GetInstance()->Send("scene-update-start");
    //删去旧的mark
    foreach(Chip* item,m_ListchipItem)
    {
        removeItem(item);
    }
    m_ListchipItem.clear();
    clear();
    //CSendMessage::GetInstance()->Send("scene-update-dele-finish");
    QStringList EnableList;
    for (int index = 0;index < m_strSequence.size();index++)
    {
        if (!m_iCheckShow[index])
        {
            continue;
        }

        QString strKey = QString::fromStdString(m_strSequence.at(index));
        EnableList.append(strKey);
    }
    QMap<QString, QVector<MarkStrut *> *>::iterator iKeyVal = pMap->begin();
    while (iKeyVal != pMap->end())
    {
        QString mapkey = iKeyVal.key();
        foreach(QString strName,EnableList)
        {
            if (mapkey.contains(strName))
            {
                QVector<MarkStrut*>* veMark = pMap->value(mapkey);
                for (int i=0; i<veMark->size(); i++)
                {
                    Addsetdecet(*veMark->at(i));
                }
            }    
        }
        iKeyVal++;
    }
   // CSendMessage::GetInstance()->Send("scene-update-end");
    return true;
}



//! [14]
