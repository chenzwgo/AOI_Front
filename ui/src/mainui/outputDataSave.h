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
	bool saveUphData(stUPHCfg m_UPHCfgs[UPHCFG_SIZES]);  //uph��Ϣ�Լ�ȱ�ݷֲ�������ʱ����Ϣ
	bool saveOneProductHeatmapData(int itype, double dposx, double dposy, int sn); //������Ʒ����ͼ������ʱ����Ϣ
	bool saveAllDayHeatmapData(int itype, double dposx, double dposy);  //��������ͼ��Ϣ																	//bool SaveOneProductResult(unsigned long long proSN, QString &data);  //һ����Ʒ��ȱ���б�
	bool SaveOneProductResult(stDefectFeature *mark);  //һ����Ʒ��ȱ���б�
	bool SaveProductResult(stDefectFeature* mark);  //���в�Ʒ��ȱ���б�
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
	void saveFoxPData(stProductSummary ResData, dataType iType= Data_Day);//0 �գ�1 �£�2����
	void saveDataByName(stProductSummary ResData,QString strFileName);
private:
	static outputDataSave* m_pInstance;
	vector<string> m_defectNameVec;
	QString m_SaveResultFilePath;
};

