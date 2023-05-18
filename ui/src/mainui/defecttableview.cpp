#include "defecttableview.h"
#include "ui_defecttableview.h"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "sendmessage.h"
#include "cimageprocess.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "./g12commonui/idefectrecord.h"
#include "../../sdk/interface/filevariable/ifilevariable.h"
#include "common_data.h"
#include "uicommon/commonutils.h";
#include "iuiplugin.h"
#include "commonapi.h"
#include <QMenu>
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"
#include "callsdk.h"

#define DEFECT_INDEX_COLUMN   1      //索引所在行数

enum TABLE_COLUMN_INFO
{
	TABLE_COLUMN_INFO = 0,                    // 通道信息
	TABLE_DEFECT_INDEX_COLUMN,                // 索引
	TABLE_DEFECT_LEVEL,                       // 等级
	//TABLE_H_DEFECT_LEVEL,                       // 等级
	TABLE_SVM_LEVEL,                          // SVM等级
	TABLE_DEFECT_AREA,                       // 面积
	TABLE_DEFECT_TYPE,                        // 类型
	TABLE_DEFECT_POS,						  // 区域
	TABLE_DEFECT_Contrast,					  // 对比度
	TABLE_DEFECT_REGIONPOS,						  // 位置(9,12等等宫格)
	TABLE_DEFECT_GRAY,						  // 灰度
	TABLE_DEFECT_SCORE,						  // 分数
	TABLE_DEFECT_WIDTH,						  // 宽度
	TABLE_DEFECT_Height,					  // 高度
	//TABLE_DEFECT_X,							  // X
	//TABLE_DEFECT_Y,							  // Y
	//TABLE_DEFECT_ORIGINATE,					  // 缺陷来源
};

JMutex g_Mutex;
defectTableView::defectTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::defectWidget),
    m_pDefectRecord(NULL),
    m_sortIndex(0),
    m_bIsAscending(true)
{
	m_iRow = 0;
    ui->setupUi(this);
    m_model = new QStandardItemModel(this);

    IUIPlugin *pUiPligin = CommonUtils::getIUIPlugin("g12commonui");
    if (NULL != pUiPligin)
    {
        pUiPligin->queryInterface(IUIPlugin::GET_MAIN_WIDGET_OBJECT + 1, (void **)&m_pDefectRecord);
    }

	IDefectListManage* p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (p != NULL)
	{
		m_defectNameVec = p->getDefectListInstance(0)->getDefectCnNameList();
	}
    m_pContextMenu = new QMenu(this);
    m_pActInsert = new QAction(QString::fromLocal8Bit("导入缺陷库"), this);
    connect(m_pActInsert, SIGNAL(triggered()), this, SLOT(slotInsert()));

    m_pContextMenu->addAction(m_pActInsert);

    ui->defectTabView->setModel(m_model);
    //ui->defectTabView->setSortingEnabled(true);   //设置可排序
    m_selectionModel = new QItemSelectionModel(m_model);
    ui->defectTabView->setSelectionModel(m_selectionModel);
    ui->defectTabView->horizontalHeader()->setResizeContentsPrecision(QHeaderView::ResizeToContents);
    ui->defectTabView->horizontalHeader()->setStretchLastSection(true);
    ui->defectTabView->verticalHeader()->hide();
    ui->defectTabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->defectTabView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->defectTabView->setSelectionMode ( QAbstractItemView::SingleSelection);
    //m_model->setHorizontalHeaderLabels(QStringList()<< QString::fromLocal8Bit("缺陷")<<QString::fromLocal8Bit("X")<<QString::fromLocal8Bit("Y")<<QString::fromLocal8Bit("Width")<<QString::fromLocal8Bit("Height"));
    QString strListTitle = QString::fromLocal8Bit(g_strTableTitle);
    QStringList listTitle = strListTitle.split(",");
    /*新ui修改*/
	ui->defectTabView->setStyleSheet("background-color: rgba(0, 0, 0, 0); gridline-color: rgb(152, 152, 152);border: 1px solid #989898;selection-background-color: rgb(85, 170, 255);");
	QPalette pl;
	pl.setColor(QPalette::ButtonText, QColor(102, 102, 102));
	ui->defectTabView->horizontalHeader()->setPalette(pl);
	
    m_model->setHorizontalHeaderLabels(listTitle);
    //m_model->setHorizontalHeaderLabels(QStringList()<< QString::fromLocal8Bit("通道") << QString::fromLocal8Bit("序列")<<"X"<<"Y" << "Size" << QString::fromLocal8Bit("缺陷类型") << QString::fromLocal8Bit("缺陷来源"));
    QHeaderView *headerView = ui->defectTabView->horizontalHeader();
    headerView->setStretchLastSection(true);    
    initDefectTable();
    ui->defectTabView->setContextMenuPolicy(Qt::CustomContextMenu);
   // connect(CSendMessage::GetInstance(),SIGNAL(signalAddMapVecsetUiTabledecet(QMap<QString, QVector<MarkStrut *> *> *)),this,SLOT(slotsinitTable(QMap<QString, QVector<MarkStrut *> *> *)));
    connect(CSendMessage::GetInstance(),SIGNAL(signalPressUi(int)),this,SLOT(slotSelectDefect(int)));
	connect(CSendMessage::GetInstance(), SIGNAL(signalClearTable()), this, SLOT(slotClearTable())/*,Qt::BlockingQueuedConnection*/);
    connect(ui->defectTabView->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),this,SLOT(selePresTabelViw(const QModelIndex& ,const QModelIndex&)));
    connect(ui->defectTabView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotcustomContextMenuRequested(const QPoint &)));
	connect(SENDMSG_INSTANCE, SIGNAL(signalOneDefecUI(stDefectFeature,int)), this, SLOT(slotAddMarkStrut(stDefectFeature,int)));
	connect(SENDMSG_INSTANCE, SIGNAL(signalImageDefecTable(ImageSrcStrut*,int)), this, SLOT(slotAddImageSrc(ImageSrcStrut*,int)));
    connect(SENDMSG_INSTANCE, SIGNAL(signalShowChoosePic(int)), this, SLOT(slotUiShow(int)));
    connect(ui->defectTabView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotSelectDefect(const QModelIndex&)));
	connect(ui->defectTabView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(seleIamge(const QModelIndex&)));
    connect(ui->defectTabView->horizontalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(slotHeaderPress(int)));
	m_iCurChooseRow = 0;
	m_iShow = 0;
}

defectTableView::~defectTableView()
{
    delete ui;
}

void defectTableView::initDefectTable()
{
    m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex());  
}

void defectTableView::dataSort()
{
    Qt::SortOrder order = Qt::DescendingOrder;
    if (m_bIsAscending)
    {
        order = Qt::AscendingOrder;
    }
    m_model->sort(m_sortIndex, order);
}
/*新ui修改*/
void defectTableView::updateTextColor(const QColor& textColor)
{
	if (m_textColor != textColor)
	{
		m_textColor = textColor;
	}
	for (int i = 0; i < m_model->rowCount(); ++i)
	{
		for (int j = 0; j < m_model->columnCount(); ++j)
		{
			m_model->item(i, j)->setForeground(QBrush(textColor));
		}
	}
}

void defectTableView::selePresTabelViw(const QModelIndex& intdex, const QModelIndex& index2)
{
    //int iInchoose = intdex.model()->data(intdex, Qt::DisplayRole).toInt() - 1;
    QModelIndex indexM = m_model->index(intdex.row(), DEFECT_INDEX_COLUMN);
    int iInchoose = m_model->data(indexM, Qt::EditRole).toInt() - 1;
	iInchoose = iInchoose >= 0 ? iInchoose : 0;
    CSendMessage::GetInstance()->sendPressTabView(iInchoose);
    if (m_showimg.isVisible())
    {
        seleIamge(index2);
    }
}
void defectTableView::slotClearTable()
{
	m_model->removeRows(0, m_model->rowCount(QModelIndex()), QModelIndex());
	m_iRow = 0;
	//QMap<QString, ImageSrcStrut *>::iterator iKeyVal = m_MapImageData.begin();
	//while (iKeyVal != m_MapImageData.end())
	//{
	//	delete iKeyVal.value()->imageAddr;
	//	iKeyVal.value()->imageAddr = NULL;
	//	iKeyVal++;
	//}
	m_MapImageData.clear();
    mListQata.clear();
}
QString strColor[8] = { "#99e600","#99cc00","#99b300","#9f991a","#a9664d","#ae4d66","#b33380","#bb3380" };
void defectTableView::slotAddMarkStrut(stDefectFeature Defect,int iSene)
{
    int showType = 5;
    if (Defect.iDefectType < m_defectNameVec.size())
    {
        showType = Defect.iDefectType;
    }
    int iLevelIndex = 0;
	int iHLevelIndex = 0;
	m_model->insertRows(m_iRow, 1, QModelIndex());
    QString strChannelName = QString::fromStdString(Defect.strChannelName);
    strChannelName =  strChannelName.right(2);
    m_model->setData(m_model->index(m_iRow, TABLE_COLUMN_INFO, QModelIndex()), strChannelName);
    m_model->setData(m_model->index(m_iRow, TABLE_COLUMN_INFO, QModelIndex()), Defect.sn.c_str(), Qt::UserRole);
    m_model->setData(m_model->index(m_iRow, TABLE_COLUMN_INFO, QModelIndex()), Defect.iCameraIndex, Qt::UserRole+1);
	m_model->setData(m_model->index(m_iRow, TABLE_COLUMN_INFO, QModelIndex()), QColor(strColor[0]), Qt::DecorationRole);
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_INDEX_COLUMN, QModelIndex()), Defect.iIndex+1);
    iLevelIndex = TABLE_DEFECT_LEVEL;
	//iHLevelIndex = TABLE_H_DEFECT_LEVEL;
    m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_LEVEL, QModelIndex()), Defect.iDefectLevel);
	//m_model->setData(m_model->index(m_iRow, TABLE_H_DEFECT_LEVEL, QModelIndex()), Defect.iH_DefectLevel);
	//m_model->setData(m_model->index(m_iRow, TABLE_SVM_LEVEL, QModelIndex()), Defect.idetect_flag);
	int ishpe = Defect.iShape >= 0 ? Defect.iShape : 0;
	if (Defect.iShape >= DEFECT_SHAPE_SIZE)
	{
		ishpe = 0;
	}
	m_model->setData(m_model->index(m_iRow, TABLE_SVM_LEVEL, QModelIndex()), QString::fromLocal8Bit(g_ShapeFlag[ishpe]));

	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_AREA, QModelIndex()), QString("%1/%2").arg(Defect.iSize).arg(Defect.dSizeArea, 0, 'f', 3));
	
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_TYPE, QModelIndex()),QString::fromStdString(Defect.strDefectName)+ "("+QString::number(Defect.iDvDefectType)+","+QString::fromStdString(Defect.strOriginate)+")");
    m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_POS, QModelIndex()), QString::fromLocal8Bit(g_strFlag[Defect.iposition>0?1:0]));

	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_Contrast, QModelIndex()), QString("%1").arg(Defect.contrast));
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_REGIONPOS, QModelIndex()), QString("%1").arg(Defect.regionPos));
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_GRAY, QModelIndex()), QString("%1").arg(Defect.DefectBAvgGray));
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_SCORE, QModelIndex()), QString("%1").arg(Defect.fdetect_score));
	
    m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_WIDTH, QModelIndex()), Defect.defectWidth);
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_WIDTH, QModelIndex()), Defect.dWide, Qt::UserRole + 1);
    m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_Height, QModelIndex()), Defect.defectHeight);
	m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_Height, QModelIndex()), Defect.dHigh, Qt::UserRole + 1);
	//m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_X, QModelIndex()), Defect.posx);
    //m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_Y, QModelIndex()), Defect.posy);
    //m_model->setData(m_model->index(m_iRow, TABLE_DEFECT_ORIGINATE, QModelIndex()), QString::fromStdString(Defect.strOriginate));
	setTableBackground(Defect.iDefectLevel, iLevelIndex);
	//setTableBackground(Defect.iH_DefectLevel, iHLevelIndex);
	/*新ui修改*/
	//改变文本颜色
	for (int i = 0; i < m_model->columnCount(); ++i)
	{
		m_model->item(m_iRow, i)->setForeground(QBrush(m_textColor));
	}
	
	mListQata.append(Defect);
	m_iRow++;
	//saveDefectImage(&Defect);
}

void defectTableView::slotAddImageSrc(ImageSrcStrut* Image,int istation)
{
	if (NULL !=Image)
	{
		m_MapImageData.insert(QString::fromStdString(Image->strChannel)+QString::number(istation) , Image);
	}
}

void defectTableView::seleIamge(const QModelIndex& iIntdex)
{
	//int irow = iIntdex.row();
    QModelIndex indexM = m_model->index(iIntdex.row(), DEFECT_INDEX_COLUMN);
    int irow = m_model->data(indexM, Qt::EditRole).toInt() - 1;
	irow = irow >= 0 ? irow : 0;
	if (irow >= mListQata.count())
	{
		return;
	}
	ImageSrcStrut *srcImage = NULL;
	QMap<QString, ImageSrcStrut* >::iterator iKeyVal = m_MapImageData.begin();

	while (iKeyVal != m_MapImageData.end())
	{
		QString key = iKeyVal.key();
		if (QString::fromStdString(mListQata.at(irow).strChannelName) == key)
		{
			srcImage = iKeyVal.value();
			break;
		}
		iKeyVal++;
	}

	///////////////////////////深视接口为空/////////////////
	if (NULL == srcImage)
	{
		vector<string> vecName;

		getEnabelChannle(vecName); //取通道数
		QString strName = "";
		if (vecName.size()<=0 || vecName.size() < m_iShow)
		{
			return;
		}
        //确认选择的通道 
		strName = QString::fromStdString(vecName.at(m_iShow));

		if (strName.isEmpty())
		{
			return;
		}

		iKeyVal = m_MapImageData.begin();
		while (iKeyVal != m_MapImageData.end())//找到对应通道的原图
		{
			QString key = iKeyVal.key();
			if (strName == key)
			{
				srcImage = iKeyVal.value();
				break;
			}
			iKeyVal++;
		}

	}
	///////////////////////////////////
	if (NULL == srcImage)
	{
		return;
	}

	double dwx = 1;
	double dWid = mListQata.at(irow).dWide;
	double dhy = 1;
	double dh = mListQata.at(irow).dHigh;
	if (dWid>100)
	{
		dwx = 1.2;
	}
	else if (dWid>50)
	{
		dwx = 1.3;
	}
	else if (dWid>20)
	{
		dwx = 1.4;
	}
	else {
		dwx = 2;
	}
	/////
	if (dh>100)
	{
		dhy = 1.2;
	}
	else if (dh>50)
	{
		dhy = 1.3;
	}
	else if (dh > 20)
	{
		dhy = 1.4;
	}
	else {
		dhy = 2;
	}
	int iwide = dWid*dwx;
    int iheight = dh*dhy;
    int ix = mListQata.at(irow).posx - iwide / 2;
    int iy = mListQata.at(irow).posy - iheight / 2;

	m_showimg.setExpandSize(iwide - dWid, iheight - dh);
	m_showimg.setImageBuffer(srcImage->imageAddr, srcImage->iImageWide, srcImage->iImageHight, ix, iy, iwide, iheight);
	m_showimg.setParent(this);
	m_showimg.setWindowFlag(Qt::Dialog);
    m_showimg.show();
}

void defectTableView::saveDefectImage(const stDefectFeature  *Defect)
{
	//QString strChannalName;
	////step 1. 获取该缺陷在哪个通道 : 如果是多通道一起检测的， 则将每个通道的都保存
	//QMap<QString, ImageSrcStrut* >::iterator iKeyVal = m_MapImageData.find(Defect->strChannelName.c_str());
	//if (iKeyVal == m_MapImageData.end())
	//{
	//	return;
	//}
	if (0 == Defect->strChannelName.compare("SEQUENCE_ABC"))
	{
		saveChannelDefectImage(Defect, "SEQUENCE_A");
		saveChannelDefectImage(Defect, "SEQUENCE_B");
		saveChannelDefectImage(Defect, "SEQUENCE_C");
	}
	else if (0 == Defect->strChannelName.compare("MutiChannel"))
	{
		saveChannelDefectImage(Defect, "SEQUENCE_B");
		saveChannelDefectImage(Defect, "SEQUENCE_C");
	}
	else
	{
		saveChannelDefectImage(Defect, Defect->strChannelName);
	}

}
void defectTableView::saveChannelDefectImage(const stDefectFeature  *Defect, string strChannelName)
{
	unsigned char *szBuf = NULL;
	ImageSrcStrut *srcImage = NULL;
	//获取原始图像
	QMap<QString, ImageSrcStrut* >::iterator iKeyVal = m_MapImageData.find(strChannelName.c_str());
	if (iKeyVal == m_MapImageData.end())
	{
		return;
	}
	srcImage = iKeyVal.value();
	//step 2. 计算切图位置及大小
	int width = 0;
	int height = 0;
	szBuf = getCutImageRect(Defect, srcImage, width, height);
	if (NULL != szBuf)
	{
		//step 3. 保存图片， 命名方式 每个产品一个文件夹，文件夹名为sn号， 图片名 索引_缺陷编号_通道名
		string strFileName;
		QString  strPath = "D:\\IMAGE";

		IFileVariable *m_fileVariable = getCameraSetCfgFileVariable();
		strPath = m_fileVariable->getValue(IMAGE_SAVE_PATH, "D:\\IMAGE").toCString().c_str();
		//JTime curTime = JTime::CurrentLocalDateTime();
		QString datePth = getCurrentProducsTimeStrPath();
		QString strSavePath = QString("%1\\%2").arg(strPath).arg(datePth);
		QDir dir(strSavePath);
		if (!dir.exists(strSavePath))
		{
			dir.mkdir(strSavePath);
		}
        strSavePath += QString("\\cutimage");
        if (!dir.exists(strSavePath))
        {
            dir.mkdir(strSavePath);
        }
		strSavePath += QString("\\%1").arg(Defect->sn.c_str());
		if (!dir.exists(strSavePath))
		{
			dir.mkdir(strSavePath);
		}
		strSavePath += QString("\\%1_%2_%3.jpg").arg(Defect->iIndex).arg(Defect->iDefectType).arg(strChannelName.c_str());
		CImageProcess::SaveImageToFile(szBuf, width, height, strSavePath);
		delete[]szBuf;
		szBuf = NULL;
	}
}

unsigned char * defectTableView::getCutImageRect(const stDefectFeature  *Defect, const ImageSrcStrut *pImageSrc, int &out_width, int &out_height)
{
	if (NULL == Defect || NULL == pImageSrc)
	{
		return NULL;
	}
	double dwx = 1;
	double dWid = Defect->dWide;
	double dhy = 1;
	double dh = Defect->dHigh;
	if (dWid>100)
	{
		dwx = 1.2;
	}
	else if (dWid>50)
	{
		dwx = 1.3;
	}
	else if (dWid>20)
	{
		dwx = 1.4;
	}
	else {
		dwx = 2;
	}
	/////
	if (dh>100)
	{
		dhy = 1.2;
	}
	else if (dh>50)
	{
		dhy = 1.3;
	}
	else if (dh > 20)
	{
		dhy = 1.4;
	}
	else {
		dhy = 2;
	}
	int swidth = dWid*dwx;;
	int sheight = dh*dhy;;
	int destWidth = dWid*dwx;
	int destHeight = dh*dhy;
	int xBegin = Defect->posx - swidth / 2;
	int yBegin = Defect->posy - sheight / 2;

	int expandWidth = swidth - dWid;
	int expandHeight = sheight - dh;

	unsigned char *szBuf = NULL;
	int width = pImageSrc->iImageWide;
	int height = pImageSrc->iImageHight;
	int destXBegin = xBegin;
	int destYBegin = yBegin;
	//左侧扩展
	if (destXBegin - expandWidth < 0)
	{
		destWidth += destXBegin;
		destXBegin = 0;
	}
	else
	{
		destWidth += expandWidth;
		destXBegin -= expandWidth;
	}
	//顶部扩展
	if (destYBegin - expandHeight < 0)
	{
		destHeight += destYBegin;
		destYBegin = 0;
	}
	else
	{
		destHeight += expandHeight;
		destYBegin -= expandHeight;
	}
	//右侧扩展
	if (width - xBegin - swidth - expandWidth < 0)
	{
		destWidth = width - destXBegin;
	}
	else
	{
		destWidth += expandWidth;
	}
	//顶部扩展
	if (height - yBegin - sheight - expandHeight < 0)
	{
		destHeight = height - destYBegin;
	}
	else
	{
		destHeight += expandHeight;
	}
	if (NULL != pImageSrc->imageAddr && width > 0 && height > 0 && destXBegin + destWidth <= width && destYBegin + destHeight <= height)
	{
		szBuf = new unsigned char[destWidth*destHeight];
		out_width = destWidth;
		out_height = destHeight;
		memset(szBuf, 0, destWidth*destHeight);
		for (int i = 0; i < destHeight; i++)
		{
			memcpy(szBuf + i*destWidth, pImageSrc->imageAddr + ((destYBegin + i)*width + destXBegin), destWidth);
		}
	}

	return szBuf;
}
void defectTableView::slotUiShow(int ichoose)
{
	m_iShow = ichoose;
}

void defectTableView::slotSelectDefect(const QModelIndex& iIntdex)
{
    //int irow = iIntdex.row();
    //int irow = iIntdex.model()->data(iIntdex, Qt::DisplayRole).toInt() - 1;
    QModelIndex indexM = m_model->index(iIntdex.row(), DEFECT_INDEX_COLUMN);
    int irow = m_model->data(indexM, Qt::EditRole).toInt() - 1;
	irow = irow >= 0 ? irow : 0;
    if (irow >= mListQata.count())
    {
        return;
    }
    QRectF rect;
    double width = mListQata.at(irow).dWide;
    double height = mListQata.at(irow).dHigh;
    int iSence = mListQata.at(irow).iStation;
	string strName = mListQata.at(irow).strChannelName;
    rect.setX(mListQata.at(irow).posx - width / 2);
    rect.setY(mListQata.at(irow).posy - height / 2 );
    rect.setWidth(width);
    rect.setHeight(height);
    emit sigSelectCamera(QString::fromStdString(strName),iSence);
    emit sigSelectDefect(rect,iSence);

}
void defectTableView::slotSelectDefect(int iIntdex)
{
    for (int i = 0; i < m_model->rowCount(); i++)
    {
        QModelIndex indexM = m_model->index(i, DEFECT_INDEX_COLUMN);
        int id = m_model->data(indexM, Qt::EditRole).toInt() - 1;
        if (id == iIntdex)
        {
            ui->defectTabView->selectRow(i);
            break;
        }
    }
}

void defectTableView::slotcustomContextMenuRequested(const QPoint &pos)
{
    QModelIndex curIndex = ui->defectTabView->indexAt(pos);
    ui->defectTabView->setCurrentIndex(curIndex);

    if (curIndex.isValid())
    {
        int irow = curIndex.row();
        int iItemSize = m_model->columnCount();
        stDefectFeature Defect;
        if (iItemSize > TABLE_COLUMN_INFO)
        {
            Defect.strChannelName = m_model->item(irow, TABLE_COLUMN_INFO)->data(Qt::EditRole).toString().toLocal8Bit();
            Defect.sn = m_model->item(irow, TABLE_COLUMN_INFO)->data(Qt::UserRole).toString().toLocal8Bit();
            Defect.iCameraIndex = m_model->item(irow, TABLE_COLUMN_INFO)->data(Qt::UserRole + 1).toInt() - 1;
        }

        if (iItemSize > TABLE_DEFECT_LEVEL)
        {
            Defect.iDefectLevel = m_model->item(irow, TABLE_DEFECT_LEVEL)->data(Qt::EditRole).toInt();
        }
        if (iItemSize > TABLE_DEFECT_AREA)
        {
            QStringList strItemValue = m_model->item(irow, TABLE_DEFECT_AREA)->data(Qt::EditRole).toString().split("/");
            if (strItemValue.size() >= 2)
            {
                Defect.iSize = strItemValue[0].toInt();
                Defect.dSizeArea = strItemValue[1].toDouble();
            }
        }
        if (iItemSize > TABLE_DEFECT_TYPE)
        {
            Defect.strDefectName = m_model->item(irow, TABLE_DEFECT_TYPE)->data(Qt::EditRole).toString().toLocal8Bit();
        }
        if (iItemSize > TABLE_DEFECT_Contrast)
        {
            Defect.contrast = m_model->item(irow, TABLE_DEFECT_Contrast)->data(Qt::EditRole).toDouble();
        }
       if (iItemSize > TABLE_DEFECT_SCORE)
       {
           Defect.fdetect_score = m_model->item(irow, TABLE_DEFECT_SCORE)->data(Qt::EditRole).toDouble();
			Defect.fscore = Defect.fdetect_score;
       }
        //if (iItemSize > TABLE_DEFECT_GRAY)
        //{
        //    Defect.fscore = m_model->item(irow, TABLE_DEFECT_GRAY)->data(Qt::EditRole).toDouble();
        //}
        if (iItemSize > TABLE_DEFECT_WIDTH)
        {
            Defect.dWide = m_model->item(irow, TABLE_DEFECT_WIDTH)->data(Qt::UserRole + 1).toDouble();
        }
        if (iItemSize > TABLE_DEFECT_Height)
        {
            Defect.dHigh = m_model->item(irow, TABLE_DEFECT_Height)->data(Qt::UserRole + 1).toDouble();
        }
       //if (iItemSize > TABLE_DEFECT_X)
       //{
       //    Defect.posx = m_model->item(irow, TABLE_DEFECT_X)->data(Qt::EditRole).toDouble();
       //}
       //if (iItemSize > TABLE_DEFECT_Y)
       //{
       //    Defect.posy = m_model->item(irow, TABLE_DEFECT_Y)->data(Qt::EditRole).toDouble();
       //}
       //if (iItemSize > TABLE_DEFECT_ORIGINATE)
       //{
       //    Defect.strOriginate = m_model->item(irow, TABLE_DEFECT_ORIGINATE)->data(Qt::EditRole).toString().toLocal8Bit();
       //}
        if (NULL != m_pDefectRecord)
        {
			//屏蔽 崩溃 20201102
            //m_pDefectRecord->setDefectParam(&Defect);
        }
        
        QPoint globalPos = ui->defectTabView->viewport()->mapToGlobal(pos);
        m_pContextMenu->exec(globalPos);
    }
}

void defectTableView::slotInsert()
{
    if (NULL != m_pDefectRecord)
    {
        m_pDefectRecord->show();
    }
}

void defectTableView::slotHeaderPress(int index)
{
    m_sortIndex = index;
    Qt::SortOrder order = Qt::AscendingOrder;
    if (m_bIsAscending)
    {
        m_bIsAscending = false;
        order = Qt::DescendingOrder;
    }
    else
    {
        m_bIsAscending = true;
        order = Qt::AscendingOrder;
    }
    m_model->sort(index, order);
    return;
}

void defectTableView::slotsinitTable(QMap<QString, QVector<MarkStrut *> *> *vecmark)
{
	return;// dele 20180530
}


void defectTableView::setTableBackground(int DefectLevel,int iLevelIndex)
{
	if (NULL == m_model)
	{
		return;
	}
	switch (DefectLevel)
	{
	case en_level_7:
	case en_level_6:
		m_model->item(m_iRow, iLevelIndex)->setBackground(QBrush(QColor(230, 39, 14)));
		break;
	case en_level_5:
		m_model->item(m_iRow, iLevelIndex)->setBackground(QBrush(QColor(255, 85, 127)));
		break;
	case en_level_4:
		m_model->item(m_iRow, iLevelIndex)->setBackground(QBrush(QColor(255, 128, 16)));//
		break;
	case en_level_3:
		m_model->item(m_iRow, iLevelIndex)->setBackground(QBrush(QColor(255, 218, 83)));
		break;
	case en_level_2:
	case en_level_1:
		m_model->item(m_iRow, iLevelIndex)->setBackground(QBrush(QColor(85, 170, 255)));
		break;
	default:

		break;
	}
}
