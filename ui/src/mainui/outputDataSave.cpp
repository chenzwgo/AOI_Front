#include "outputDataSave.h"
#include "iniFile.h"
#include <string>
#include <QString>
#include <QDir>
#include <jtime.h>
#include "sdkcore.h"
#include "g12globalenv.h"
#include "filevariable\ifilevariable.h"
#include <QDateTime>
#include "g12common.h"
#include "commonapi.h"
#include "trace.h"
#include "callsdk.h"
#include "defect\idefectlist.h"
#include "defect\idefectlistmanage.h"

using namespace std;

outputDataSave* outputDataSave::m_pInstance = NULL;

outputDataSave::outputDataSave()
{
	m_SaveResultFilePath = getSaveResultFilePath();
	IDefectListManage* p = (IDefectListManage*)GetSDKInstance("defectqualification", IDefectListManage::IDD_DEFECT_DEFECTLIST_MANAGE);
	if (NULL != p)
	{
		m_defectNameVec = p->getDefectListInstance(0)->getDefectCnNameList();
	}
}

outputDataSave::~outputDataSave()
{

}

//保存uph以及缺陷分布数据
bool outputDataSave::saveUphData(stUPHCfg m_UPHCfgs[UPHCFG_SIZES])
{
	CIniFile iniFile;
	// 路径不存在，创建路径
	//QString strDir = getSaveResultFilePath();
	QString strDir = getSaveResultFilePathNew();
	//QString strDir = QString::fromLocal8Bit("D:/uilog/productResult");
	QString strSavePath = QString("%1/%2").arg(strDir).arg(getCurrentLocalDateTimeStrPath());
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);
	}
	QString csvFileName = strSavePath + "/" + QString("%1").arg(getCurrentLocalDateTimeStrPath()) + "_UPH.ini";
	iniFile.Initialize(csvFileName.toStdString());
	iniFile.ReadFile();
	string firstKeyName = "UPHINFO";
	//total
	string keyname = firstKeyName;
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[0].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[0].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[0].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[0].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[0].iReClean);
	iniFile.SetValueI(keyname, "DefectAvgCnt", m_UPHCfgs[0].iDefectAvgCnt);
	iniFile.SetValueI(keyname, "SmirchAvgCnt", m_UPHCfgs[0].iSmirchAvgCnt);
	iniFile.SetValueI(keyname, "HairAvgCnt", m_UPHCfgs[0].iHairAvgCnt);
	iniFile.SetValueI(keyname, "DirtpitsAvgCnt", m_UPHCfgs[0].iDirtpitsAvgCnt);
	iniFile.SetValueI(keyname, "SmirchAvgAera", m_UPHCfgs[0].dSmirchAvgAera);
    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectNameVec[i], m_UPHCfgs[0].NgDefectType[i]);
	}
	//day
	keyname = firstKeyName + "_DAY";
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[1].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[1].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[1].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[1].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[1].iReClean);
    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectNameVec[i], m_UPHCfgs[1].NgDefectType[i]);
	}
	//night
	keyname = firstKeyName + "_NIGHT";
	iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[2].iTotalCnt);
	iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[2].iOKCnt);
	iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[2].iNGCnt);
	iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[2].iReCheck);
	iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[2].iReClean);
    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
        iniFile.SetValueI(keyname, m_defectNameVec[i], m_UPHCfgs[2].NgDefectType[i]);
	}
	for (int i = 3; i < UPHCFG_SIZES; i++)
	{
		keyname = firstKeyName + "_" + QString::number(i - 3).toStdString();
		iniFile.SetValueI(keyname, "TotalCnt", m_UPHCfgs[i].iTotalCnt);
		iniFile.SetValueI(keyname, "OKCnt", m_UPHCfgs[i].iOKCnt);
		iniFile.SetValueI(keyname, "NGCnt", m_UPHCfgs[i].iNGCnt);
		iniFile.SetValueI(keyname, "ReCheck", m_UPHCfgs[i].iReCheck);
		iniFile.SetValueI(keyname, "ReClean", m_UPHCfgs[i].iReClean);
        for (int j = 1; j < m_defectNameVec.size(); ++j)
		{
            iniFile.SetValueI(keyname, m_defectNameVec[j], m_UPHCfgs[0].NgDefectType[j]);
		}
	}
	iniFile.WriteFile();
	return true;
}

bool outputDataSave::saveOneProductHeatmapData(int itype, double dposx, double dposy, int sn)
{
	CIniFile iniFile;
	QString strSavePath = getSaveResultFilePathNew() + "/" + getCurrentLocalDateTimeStrPath() + "/heatmap";
	QDateTime cTime = QDateTime::currentDateTime();
	QDir dir;
	if (!dir.exists(strSavePath))
	{
		dir.mkdir(strSavePath);
	}
	//文件名中包含产品序号和当前小时，分钟
	QString FileName = strSavePath + "/" + "heatMapPoint_" + QString::number(sn) + "_" + QString::number(cTime.time().hour()) + "_" + QString::number(cTime.time().minute()) + ".csv";
	QFile file(FileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		return false;
	QString msg = QString("posx=%1,poxy=%2,type=%3\n").arg(QString::number(dposx, 'f', 3)).arg(QString::number(dposy, 'f', 3)).arg(itype);
	file.write(msg.toStdString().c_str(), msg.length());
	file.flush();
	file.close();
	return true;
}

bool outputDataSave::saveAllDayHeatmapData(int itype, double dposx, double dposy)
{
	if (dposx == 0 || dposy == 0)
	{
		return false;
	}
	CIniFile iniFile;
	QString strSavePath = getSaveResultFilePathNew();  //D:uilog/productResult
													//文件名中包含产品序号和当前小时，分钟
	QString FileName = strSavePath + "/" + getCurrentLocalDateTimeStrPath() + "/" + getCurrentLocalDateTimeStrPath() + "_HeatMapPoint.csv";
	QFile file(FileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		return false;
	QString msg = QString("posx=%1,poxy=%2,type=%3\n").arg(QString::number(dposx, 'f', 3)).arg(QString::number(dposy, 'f', 3)).arg(itype);
	file.write(msg.toStdString().c_str(), msg.length());
	file.flush();
	file.close();
	return true;
}
#define CSV_FILE_TIMEOUT_HEADER "时间,sn,算法时间,说明,\n"
void outputDataSave::SaveTimeOutProduct(long timeout, long sn,QString msg)
{
	QString strSavePath = getSaveResultFilePath() + "/" + getCurrentLocalDateTimeStrPath();
	QDir dir;
	if (!dir.exists(strSavePath))
	{
		dir.mkdir(strSavePath);
	}
	QString strTitleValue;
	QString csvFileName = strSavePath + "/" + getCurrentLocalDateTimeStrPath() + "dv_error_msg.csv";
	if (!QFile::exists(csvFileName))
	{
		strTitleValue = QString::fromLocal8Bit(CSV_FILE_TIMEOUT_HEADER);
	}
	QFile csvFile(csvFileName);
	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return ;
	}
	csvFile.write(strTitleValue.toLocal8Bit().data());

	JTime curTime = JTime::CurrentLocalDateTime();
	QString strTime = QString("time %1:%2:%3").arg(curTime.hour()).arg(curTime.minute()).arg(curTime.second());
	QString strValue = QString("%1%2,%3,%4,%5").arg("\n").arg(strTime).arg(sn).arg(timeout).arg(msg);

	csvFile.write(strValue.toLocal8Bit());
	csvFile.close();
}
#define CSV_FILE_SN_DEFECT_HEADER "通道,序列,等级,H等级,形状,Size,类型,区域,对比度,得分,灰度,宽度,高度,X,Y,缺陷来源,绝对位置X,绝对位置Y,区域位置,H类型,\n"

bool outputDataSave::SaveProductResult(stDefectFeature *mark)
{
	QString strSavePath = getSaveResultFilePathNew() + "/" + getCurrentLocalDateTimeStrPath();
	QDir dir;
	if (!dir.exists(strSavePath))
	{
		dir.mkdir(strSavePath);
	}
	QString strTitleValue;
	QString csvFileName = strSavePath + "/" + getCurrentLocalDateTimeStrPath() + "_defectlist.csv";
	if (!QFile::exists(csvFileName))
	{
		strTitleValue = QString::fromLocal8Bit(CSV_FILE_SN_DEFECT_HEADER);
	}
	QFile m_csvFile(csvFileName);
	if (!m_csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return false;
	}
	m_csvFile.write(strTitleValue.toLocal8Bit().data());
	QString strValue = QString("%1%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20").arg("\n") \
		.arg(mark->strChannelName.c_str()) \
		.arg(mark->iIndex + 1) \
		.arg(QString("%1,%2").arg(mark->iDefectLevel).arg(mark->iH_DefectLevel)) \
		.arg(QString::fromLocal8Bit(g_ShapeFlag[mark->iShape < 0 ? 0 : mark->iShape])) \
		.arg(QString("%1/%2(%3/%4/%5/%6/%7/%8/%9)").arg(mark->iSize).arg(mark->dSizeArea, 0, 'f', 3).arg(mark->iSAreaSize[0]).arg(mark->iSAreaSize[1]).arg(mark->iSAreaSize[2]).arg(mark->iSAreaSize[3]).arg(mark->iSAreaSize[4]).arg(mark->iSAreaSize[5]).arg(mark->iSAreaSize[6])) \
		.arg(QString::fromStdString(m_defectNameVec[mark->iDefectType])) \
		.arg(QString::fromLocal8Bit(g_strFlag[mark->iposition])) \
		.arg(QString("%1(%2/%3/%4/%5/%6/%7/%8)").arg(mark->contrast).arg(mark->iScontrast[0]).arg(mark->iScontrast[1]).arg(mark->iScontrast[2]).arg(mark->iScontrast[3]).arg(mark->iScontrast[4]).arg(mark->iScontrast[5]).arg(mark->iScontrast[6])) \
		.arg(mark->fdetect_score) \
		.arg(QString("%1/%2/%3/%4").arg(mark->iSGray[0]).arg(mark->iSGray[1]).arg(mark->iSGray[2]).arg(mark->iSGray[3]))  \
		.arg(mark->dWide) \
		.arg(mark->dHigh) \
		.arg(mark->posx) \
		.arg(mark->posy) \
		.arg(mark->strOriginate.c_str())	\
		.arg(mark->absposx)	\
		.arg(mark->absposy)	\
		.arg(mark->regionPos)\
		.arg(mark->iDvDefectType);
	m_csvFile.write(strValue.toLocal8Bit());
	return true;
}

bool outputDataSave::SaveOneProductResult(stDefectFeature *mark)
{
	//需要一个总表//
	//QString strDir = getSaveResultFilePath();
	JTime curTime = JTime::CurrentLocalDateTime();
	//QString strSavePath = GetSaveResultSavePath(dayOrNight);

	QString strSavePath = getSaveResultFilePathNew() + "/" + getCurrentLocalDateTimeStrPath() + "/" + "defectlist";

	QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_SN_DEFECT_HEADER);
	QDir dir;
	if (!dir.exists(strSavePath))
	{
		dir.mkdir(strSavePath);
	}
	QDateTime dateTime = QDateTime::currentDateTime();
	QTime time = dateTime.time();
	int hour = time.hour();
	int minute = time.minute();
	int proSN = QString::fromStdString(mark->sn).toInt();
	QString csvFileName = strSavePath + "/" + "defectlist_" + QString::number(proSN) + "_" + QString::number(hour) + "_" + QString::number(minute) + ".csv";
	QString addData = "";
	if (!QFile::exists(csvFileName))
	{
		addData = strTitleValue;
	}
	QFile csvFile(csvFileName);

	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return false;
	}
	QString strValue = QString("%1%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20").arg("\n") \
		.arg(mark->strChannelName.c_str()) \
		.arg(mark->iIndex + 1) \
		.arg(QString("%1,%2").arg(mark->iDefectLevel).arg(mark->iH_DefectLevel)) \
		.arg(QString::fromLocal8Bit(g_ShapeFlag[mark->iShape < 0 ? 0 : mark->iShape])) \
		.arg(QString("%1/%2(%3/%4/%5/%6/%7/%8/%9)").arg(mark->iSize).arg(mark->dSizeArea, 0, 'f', 3).arg(mark->iSAreaSize[0]).arg(mark->iSAreaSize[1]).arg(mark->iSAreaSize[2]).arg(mark->iSAreaSize[3]).arg(mark->iSAreaSize[4]).arg(mark->iSAreaSize[5]).arg(mark->iSAreaSize[6])) \
		.arg(QString::fromStdString(m_defectNameVec[mark->iDefectType])) \
		.arg(QString::fromLocal8Bit(g_strFlag[mark->iposition])) \
		.arg(QString("%1(%2/%3/%4/%5/%6/%7/%8)").arg(mark->contrast).arg(mark->iScontrast[0]).arg(mark->iScontrast[1]).arg(mark->iScontrast[2]).arg(mark->iScontrast[3]).arg(mark->iScontrast[4]).arg(mark->iScontrast[5]).arg(mark->iScontrast[6])) \
		.arg(mark->fdetect_score) \
		.arg(QString("%1/%2/%3/%4").arg(mark->iSGray[0]).arg(mark->iSGray[1]).arg(mark->iSGray[2]).arg(mark->iSGray[3]))  \
		.arg(mark->dWide) \
		.arg(mark->dHigh) \
		.arg(mark->posx) \
		.arg(mark->posy) \
		.arg(mark->strOriginate.c_str())	\
		.arg(mark->absposx)	\
		.arg(mark->absposy)	\
		.arg(mark->regionPos)\
		.arg(mark->iDvDefectType);
	csvFile.write(strValue.toLocal8Bit());
	csvFile.close();
	return true;
}

void outputDataSave::SaveProductSummaryResult(unsigned long long  sn, stProductSummary pProductSummary)
{
	CIniFile iniFile;
	// 路径不存在，创建路径
	QString strDir = getSaveResultFilePathNew();

	JTime curTime = JTime::CurrentLocalDateTime();
	QString strSavePath = QString("%1/%2-%3-%4").arg(strDir).arg(curTime.year()).arg(curTime.month()).arg(curTime.day()) + "/" + "summary";
	QDir dir2(strSavePath);
	if (!dir2.exists(strSavePath))
	{
		dir2.mkdir(strSavePath);
	}
	int hour = curTime.hour();
	int minute = curTime.minute();
	QString csvFileName = strSavePath + "/" + "summary" + "_" + QString::number(sn) + "_" + QString::number(hour) + "_" + QString::number(minute) + ".ini";
	QFile file(csvFileName);
	iniFile.Initialize(csvFileName.toStdString());
    for (int i = 1; i < m_defectNameVec.size(); i++)
	{
		QString keyname = "defectSummary";
		QString valuename = "defectCnt" + QString::number(i);
        iniFile.SetValueI(keyname.toStdString(), valuename.toStdString(), pProductSummary.dvdefectSummaryArr[i]);
	}
	iniFile.WriteFile();
	return;
}

outputDataSave* outputDataSave::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new outputDataSave();
	}
	return m_pInstance;
}

void outputDataSave::DestroyInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}


QString outputDataSave::getCurrentLocalDateTimeStrPath()
{
	JTime curTime = JTime::CurrentLocalDateTime();
	QString str = QString("%1-%2-%3").arg(curTime.year()).arg(curTime.month(), 2, 10, QChar('0')).arg(curTime.day(), 2, 10, QChar('0'));
	return str;
}

/***
* 计算前一天日期
* @param y :年，负数表示公元前
* @param m :月
* @param d :日
* @return
*/
QString outputDataSave::CalculatorYesterday(int y, int m, int d)
{
	int am[] = { 12,1,2,3,4,5,6,7,8,9,10,11,12 };          //month am[1] 一月
	int ad[] = { 31,31,28,31,30,31,30,31,31,30,31,30,31 };//day   ad[1] 一月，31天

	if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {              // 判断是否是闰年
		ad[2] = 29;
	}

	if (m < 1 || m>12 || d < 1 || (d > ad[m]))               //判断输入日期是否合法
	{
		return "Input error:Invalid date";
	}

	if (m == 1 && d == 1)
	{
		y = y - 1;    //年
	}

	if (d == 1) {
		d = ad[m - 1];
		m = am[m - 1];//月
	}
	else
	{
		d = d - 1;   //日
	}

	QString str = QString("%1-%2-%3").arg(y).arg(m, 2, 10, QChar('0')).arg(d, 2, 10, QChar('0'));

	return str;
}

//设备id，机器id，二维码，tray盘位置
//热力图的点
//void outputDataSave::saveBatchData(QString deviceId,QString machineId, QString qRCode, int trayPos, vector<pointInfo> pointVec,int productId,QString result)
//{
//	QString default_path = "D:uilog/BatchResult/";
//	QString fileNamePath = default_path + qRCode;
//	//QString fileName = deviceId + "_" + machineId + "_" + trayPos + ".csv";
//	QString fileName = trayPos + ".csv";
//	fileName = fileNamePath + "/" + fileName;
//	QString fileResultInfoFile = fileNamePath + "/" + "resultinfo.ini";
//	saveResultInfoFile(fileResultInfoFile, deviceId, machineId, trayPos, productId,result);
//	saveResultHeatmapFile(fileName, pointVec);
//}

void outputDataSave::saveResultInfoFile(QString fileName, QString deviceId, QString machineId,int trayPos,int productId,QString result)
{
	CIniFile iniFile;
	iniFile.SetPath(fileName.toStdString());
	iniFile.ReadFile();
	string trayNum = QString::number(trayPos).toStdString();
	iniFile.SetValue(trayNum, "name", "p" + trayNum);
	iniFile.SetValue(trayNum, "deviceId", deviceId.toStdString());
	iniFile.SetValue(trayNum, "machineId", machineId.toStdString());
	iniFile.SetValue(trayNum, "productId", QString::number(productId).toStdString());
	iniFile.SetValue(trayNum, "productResult", result.toStdString());
	iniFile.WriteFile();
}

//void outputDataSave::saveResultHeatmapFile(QString fileName, vector<pointInfo> pointVec)
//{
//	QFile file(fileName);
//	if (!file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
//	{
//		return;
//	}
//	for (int i = 0; i < pointVec.size(); ++i)
//	{
//		double x = pointVec.at(i).posx;
//		double y = pointVec.at(i).posy;
//		int type = pointVec.at(i).m_type;
//		QString msg = QString("posx=%1,poxy=%2,type=%3\n").arg(QString::number(x, 'f', 3)).arg(QString::number(y, 'f', 3)).arg(type);
//		file.write(msg.toStdString().c_str(), msg.length());
//	}
//	file.flush();
//	file.close();
//}

QString outputDataSave::createOneProductDir(unsigned int sn,QString result)
{
	QString strDir = getSaveResultFilePathNew();
	QString dateStr = getCurrentLocalDateTimeStrPath();
	QTime time = QTime::currentTime();
	//2020-01-09_14_16_23_sn_OK//年-月-日_时_分_秒_sn_结果
	QString dirName = QString("%1_%2_%3_%4_%5_%6").arg(dateStr).arg(time.hour()).arg(time.minute()).arg(time.second()).arg(sn).arg(result);
	QString productResultPath = strDir + "/" + dateStr + "/" + dirName;
	QDir dir(productResultPath);
	if (!dir.exists())
	{
		if (!dir.mkdir(productResultPath))
		{
			return "";
		}
	}
	return productResultPath;
}

void outputDataSave::saveJpgImage(QString productResultPath)
{
	QString jpgImagePath = productResultPath + "/" + "jpgImage";
}

void outputDataSave::saveDefectImage(QString productResultPath)
{
	QString defectImagePath = productResultPath + "/" + "defectImage";
}

void outputDataSave::saveDefectList(QString productResultPath, stDefectFeature *mark)
{
	QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_SN_DEFECT_HEADER);
	QString strSavePath = productResultPath; 
	QDir dir;
	if (!dir.exists(strSavePath))
	{
		dir.mkdir(strSavePath);
	}
	QString csvFileName = strSavePath + "/" + DEFECTLIST_FILE_NAME;
	QString addData = "";
	if (!QFile::exists(csvFileName))
	{
		addData = strTitleValue;
	}
	QFile csvFile(csvFileName);

	if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		return;
	}
	QString strValue = QString("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19%20")
		.arg(mark->strChannelName.c_str()) \
		.arg(mark->iIndex + 1) \
		.arg(QString("%1,%2").arg(mark->iDefectLevel).arg(mark->iH_DefectLevel)) \
		.arg(QString::fromLocal8Bit(g_ShapeFlag[mark->iShape < 0 ? 0 : mark->iShape])) \
		.arg(QString("%1/%2(%3/%4/%5/%6/%7/%8/%9)").arg(mark->iSize).arg(mark->dSizeArea, 0, 'f', 3).arg(mark->iSAreaSize[0]).arg(mark->iSAreaSize[1]).arg(mark->iSAreaSize[2]).arg(mark->iSAreaSize[3]).arg(mark->iSAreaSize[4]).arg(mark->iSAreaSize[5]).arg(mark->iSAreaSize[6])) \
		.arg(QString::fromStdString(m_defectNameVec[mark->iDefectType])) \
		.arg(QString::fromLocal8Bit(g_strFlag[mark->iposition])) \
		.arg(QString("%1(%2/%3/%4/%5/%6/%7/%8)").arg(mark->contrast).arg(mark->iScontrast[0]).arg(mark->iScontrast[1]).arg(mark->iScontrast[2]).arg(mark->iScontrast[3]).arg(mark->iScontrast[4]).arg(mark->iScontrast[5]).arg(mark->iScontrast[6])) \
		.arg(mark->fdetect_score) \
		.arg(QString("%1/%2/%3/%4").arg(mark->iSGray[0]).arg(mark->iSGray[1]).arg(mark->iSGray[2]).arg(mark->iSGray[3]))  \
		.arg(mark->dWide) \
		.arg(mark->dHigh) \
		.arg(mark->posx) \
		.arg(mark->posy) \
		.arg(mark->strOriginate.c_str())	\
		.arg(mark->absposx)	\
		.arg(mark->absposy)	\
		.arg(mark->regionPos)\
		.arg(mark->iDvDefectType)\
		.arg("\n");
	csvFile.write(strValue.toLocal8Bit());
	csvFile.close();
	return;
}
#define CSV_FILE_ONE_DEFCET_HEADER "时间,sn,缺陷,等級,x,y,区域,通道,\n"
void outputDataSave::SaveOneDefectMsg(stDefectFeature* mark, defectType iDefect)
{
	//if (iDefect != mark->iDefectType)
	//{
	//	return;
	//}
	//
	//QString strSavePath = getSaveResultFilePath() + "/" + getCurrentLocalDateTimeStrPath();
	//QDir dir;
	//if (!dir.exists(strSavePath))
	//{
	//	dir.mkdir(strSavePath);
	//}
	//QString strTitleValue;
    //QString csvFileName = strSavePath + "/" + getCurrentLocalDateTimeStrPath() +"_"+szdefectTypeEn[iDefect]+"_defect.csv";
    //if (!QFile::exists(csvFileName))
	//{
	//	strTitleValue = QString::fromLocal8Bit(CSV_FILE_ONE_DEFCET_HEADER);
	//}
	//QFile csvFile(csvFileName);
	//if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	//{
	//	return;
	//}
	//csvFile.write(strTitleValue.toLocal8Bit().data());
	//JTime curTime = JTime::CurrentLocalDateTime();
	//QString strTime = QString("%1:%2:%3").arg(curTime.hour()).arg(curTime.minute()).arg(curTime.second());
	//QString strChannel = QString::fromStdString(mark->strChannelName);
	//if (strChannel.length()>2)
	//{
	//	strChannel = strChannel.right(2);
	//}
	//QString strValue = QString("%1,%2,%3,%4,%5,%6,%7,%8")\
	//	.arg(strTime) \
	//	.arg(mark->sn.c_str()) \
	//	.arg(QString::fromLocal8Bit(szdefectTypeCn[mark->iDefectType])) \
	//	.arg(mark->iDefectLevel) \
	//	.arg(mark->posx) \
	//	.arg(mark->posy) \
	//	.arg(mark->regionPos) \
	//	.arg(strChannel + "\n");
	//
	//csvFile.write(strValue.toLocal8Bit());
	//csvFile.close();
}
void outputDataSave::saveLog(QString strLogName, QString strLog, bool bTosave)
{
	if (!bTosave)
	{
		return;
	}
	QString strPath = m_SaveResultFilePath;
	strPath += "/" + getCurrentProducsTimeStrPath() + "/" + strLogName;
	LOG_INFO_F_SIMP(strPath.toStdString().data(), strLog.toLatin1());
}
#define CSV_FILE_FOX_DATA_HEADER "时间,sn,二维码,结果,不良类型,\n"
#define CSV_FILE_FOX_RES_HEADER "OK,NG,RI,\n"
#define CSV_FILE_FOX_RES_DATA_HEADER "缺陷类型,个数,百分比,\n"
void outputDataSave::saveFoxTrackPadData(stProductSummary ResData)
{
	//年//一个百份比的数据表-明细
	saveFoxPData(ResData, Data_Year);
	//月//一个百份比的数据表-明细
	saveFoxPData(ResData, Data_Month);
	//日//--OK,NG,RI;一个表加一个百份比的数据表-明细
	saveFoxPData(ResData, Data_Day);
}
void outputDataSave::saveFoxPData(stProductSummary ResData, dataType iType)
{
	QString strDir = "D://TrackPadData";
	QDir dir;
	if (!dir.exists(strDir))
	{
		dir.mkdir(strDir);
	}
	JTime curTime = JTime::CurrentLocalDateTime();
	QString strDirYear = QString("%1//%2").arg(strDir).arg(curTime.year());
	if (!dir.exists(strDirYear))
	{
		dir.mkdir(strDirYear);
	}
	QString strMonth = QString("%1//%2").arg(strDirYear).arg(curTime.month(), 2, 10, QChar('0'));
	if (!dir.exists(strMonth))
	{
		dir.mkdir(strMonth);
	}
	QString strDay = QString("%1//%2").arg(strMonth).arg(curTime.day(), 2, 10, QChar('0'));
	if (!dir.exists(strDay))
	{
		dir.mkdir(strDay);
	}

	QString csvFileName = "";
	switch (iType)
	{
	case Data_Day:
	     {
		     
		     //记录当天第一片数据
		     QString strTime = QString("%1:%2:%3").arg(curTime.hour()).arg(curTime.minute()).arg(curTime.second());
		     QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_FOX_DATA_HEADER);
		     csvFileName = strDay + "/" + getCurrentLocalDateTimeStrPath()+"_DayDetailedData.csv";
		     QString addData = "";
		     if (!QFile::exists(csvFileName))
		     {
		     	addData = strTitleValue;
		     }
			 QFile csvFile(csvFileName);
			 int iNgType = 0;
			 if (!(res_OK==ResData.ResultFlag|| res_OK2 == ResData.ResultFlag))
			 {
				 iNgType = ResData.iProductNgDefectType>0 ? ResData.iProductNgDefectType : ResData.iProductRiDefectType;
			 }

			 if (csvFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
			 {
				 QString strValue = QString("%1,%2,%3,%4,%5,%6") \
					 .arg(strTime).arg(QString::fromStdString(ResData.sn)).arg(QString::fromStdString(ResData.str2Dcode))\
					 .arg(getResByResultFlag(ResData.ResultFlag)).arg(QString::fromStdString(m_defectNameVec[iNgType])).arg("\n");
					 addData += strValue;
				 csvFile.write(addData.toLocal8Bit());
				 csvFile.close();
			 }
			 //记录每天统计数据-->先读今天的之前的数据-->增加新的->写入保存
			 QString csvDayFileName = strDay + "/" + getCurrentLocalDateTimeStrPath() + "_DayStatisticalData.csv";
			 saveDataByName(ResData, csvDayFileName);

	     }

		break;
	case Data_Month:
		csvFileName = strMonth + "/" + QString("%1-%2").arg(curTime.year()).arg(curTime.month(), 2, 10, QChar('0')) + "_MonthStatisticalData.csv";
		saveDataByName(ResData, csvFileName);
		break;
	case Data_Year:
		csvFileName = strDirYear + "/" + QString("%1").arg(curTime.year()) + "_YearStatisticalData.csv";
		saveDataByName(ResData, csvFileName);
		break;
	default:
		break;
	}
}
void outputDataSave::saveDataByName(stProductSummary ResData, QString strFileName)
{
	//记录每天统计数据-->先读今天的之前的数据-->增加新的->写入保存
	//QString csvDayFileName = strDay + "/" + getCurrentLocalDateTimeStrPath() + "_DayStatisticalData.csv";
	QString csvDayFileName = strFileName;
	//
	int iOk = 0;
	int iNg = 0;
	int iRi = 0;
	int iArr[100];
	memset(iArr, 0, sizeof(iArr));
	int iIndex = 0;
	int iSizeVec = m_defectNameVec.size();
	if (QFile::exists(csvDayFileName))
	{
		QFile csvReadFile(csvDayFileName);
		if (csvReadFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QString title = csvReadFile.readLine();//--
			int iLine = 0;
			while (!csvReadFile.atEnd())
			{
				//-----
				QString strLineData = csvReadFile.readLine();
				QList<QString> attrListData = strLineData.split(",");
				if (0 == iLine)//第0行记录OK,NG,RI
				{
					if (attrListData.count() >= 3)
					{
						iOk = attrListData.at(0).toInt();
						iNg = attrListData.at(1).toInt();
						iRi = attrListData.at(2).toInt();
					}
					//
				}
				//第1行记录百比不需要读
				//第2行为缺陷百分比抬头
				//第3行开始为第一种缺陷的个数百分比
				if ((3 + iIndex) == iLine)
				{
					if (attrListData.count() >= 3)
					{
						iArr[iIndex] = attrListData.at(1).toInt();
					}
					iIndex++;
				}
				//
				iLine++;
			}
			csvReadFile.close();
		}
	}
	//写数据
	//
	QString strTitleValue = QString::fromLocal8Bit(CSV_FILE_FOX_RES_HEADER);
	QString addData = "";
	//if (!QFile::exists(csvDayFileName))
	// {
	addData = strTitleValue;
	// }
	QFile csvSaveFile(csvDayFileName);
	//
	if (res_OK == ResData.ResultFlag || res_OK2 == ResData.ResultFlag) {
		iOk++;

	}
	else if (res_Ng == ResData.ResultFlag) {
		iNg++;
		iArr[ResData.iProductNgDefectType]++;
	}
	else {
		iRi++;
		iArr[ResData.iProductRiDefectType]++;
	}
	//
	//int iAllNgCouts = 0;
	//for (size_t i = 0; i < 100; i++)
	//{
	//	iAllNgCouts += iArr[i];
	//}
	//iAllNgCouts = (iAllNgCouts == 0) ? 1 : iAllNgCouts;
	if (csvSaveFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		// 总数记录
		QString strValue = QString("%1,%2,%3,%4").arg(iOk).arg(iNg).arg(iRi).arg("\n");
		addData += strValue;
		int iAll = iOk + iNg + iRi;
		iAll = (iAll == 0) ? 1 : iAll;
		strValue = QString("%1%,%2%,%3%,%4").arg((double)iOk * 100 / (double)iAll, 0, 'f', 2).arg((double)iNg * 100 / (double)iAll, 0, 'f', 2).arg((double)iRi * 100 / (double)iAll, 0, 'f', 2).arg("\n");
		addData += strValue;

		//
		strTitleValue = QString::fromLocal8Bit(CSV_FILE_FOX_RES_DATA_HEADER);
		addData += strTitleValue;
		//不良明细--QString::fromStdString(m_defectNameVec[mark->iDefectType])
		for (size_t i = 0; i < m_defectNameVec.size(); i++)
		{
			QString strData = QString("%1,%2,%3%,%4").arg(QString::fromStdString(m_defectNameVec.at(i)))\
				.arg(iArr[i]).arg((double)iArr[i] * 100 / iAll, 0, 'f', 2).arg("\n");
			addData += strData;

		}
		//
		csvSaveFile.write(addData.toLocal8Bit());
		csvSaveFile.close();
	}
}