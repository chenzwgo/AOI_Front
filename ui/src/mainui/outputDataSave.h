#pragma once
#include "maindefine.h"
#include <QString>
#include "filevariable\ifilevariable.h"
#include "iniFile.h"
typedef enum dataType
{
	Data_Day=0,
	Data_Month,
	Data_Year
};
class outputDataSave
{
public:
	~outputDataSave();

	static outputDataSave* GetInstance();
	static void DestroyInstance();
	bool saveData();
	bool saveUphData(stUPHCfg m_UPHCfgs[UPHCFG_SIZES]);  //uph信息以及缺陷分布，包含时间信息
	bool saveOneProductHeatmapData(int itype, double dposx, double dposy, int sn); //单个产品热力图，包含时间信息
	bool saveAllDayHeatmapData(int itype, double dposx, double dposy);  //整天热力图信息																	//bool SaveOneProductResult(unsigned long long proSN, QString &data);  //一个产品的缺陷列表
	bool SaveOneProductResult(stDefectFeature *mark);  //一个产品的缺陷列表
	bool SaveProductResult(stDefectFeature* mark);  //所有产品的缺陷列表
	void SaveProductSummaryResult(unsigned long long sn, stProductSummary pProductSummary);															   //bool saveDefectDistributedData();   
//	void saveBatchData(QString devieceId,QString lineId,QString qRCode,int trayPos, vector<pointInfo> pointVec,int productId,QString result);
	void SaveTimeOutProduct(long timeout,long sn, QString msg);  //
	void SaveOneDefectMsg(stDefectFeature* mark, defectType iDefect);
	void saveLog(QString strLogName, QString strLog, bool bTosave = true);
	//
	void saveFoxTrackPadData(stProductSummary ResData);
	//
private:
	explicit outputDataSave();
	QString getCurrentLocalDateTimeStrPath();
	QString CalculatorYesterday(int y, int m, int d);
	void saveResultInfoFile(QString fileName, QString deviceId, QString machineId, int trayPos, int productId,QString result);
	QString createOneProductDir(unsigned int sn, QString result);
	void saveJpgImage(QString );
	void saveDefectImage(QString );
	void saveDefectList(QString , stDefectFeature* );
//	void saveResultHeatmapFile(QString fileName, vector<pointInfo> pointVec);
	void saveFoxPData(stProductSummary ResData, dataType iType= Data_Day);//0 日，1 月，2，年
	void saveDataByName(stProductSummary ResData,QString strFileName);
private:
	static outputDataSave* m_pInstance;
	vector<string> m_defectNameVec;
	QString m_SaveResultFilePath;
};

