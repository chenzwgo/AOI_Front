#ifndef _DISPENSERCOMMON_H_
#define _DISPENSERCOMMON_H_
#include "g12commonexport.h"
#include <string>
#include <vector>
#include "list"
using namespace std;

class IFileVariable;

#ifdef __cplusplus
extern "C" {
#endif


    // 根据文件名获取文件读写 IFileVariable* 指针
    RESOURCE_LIBRARY_EXPORT  IFileVariable* getFileVariable(const string& fileName);

    // 获取当前程序的路径
    RESOURCE_LIBRARY_EXPORT  const string getCurrentProgramPath();

    // 获取当前程序名称
    RESOURCE_LIBRARY_EXPORT  const string getCurrentProgramName();

    // 获取所有程序的名字
    RESOURCE_LIBRARY_EXPORT const  string getProgramNameList();
	// 设置所有程序的名字
	RESOURCE_LIBRARY_EXPORT const  void setProgramNameList(const string& ProgramNameList);

	// 设置当前程序的名称
	RESOURCE_LIBRARY_EXPORT   const void setCurrentProgramName(const string& programName);

    // 获取当前程序配方通道名字
    RESOURCE_LIBRARY_EXPORT const  string getCurrentProgramRecipesList();

    // 获取当前相机名字
    RESOURCE_LIBRARY_EXPORT const  string getCurrentProgramCamNameList();

	// 获取全局配置句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable *getMotionFileVariable();

	// 获取全局配置句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable *getGlobalFileVariable();
	RESOURCE_LIBRARY_EXPORT IFileVariable *getGlobalSNFileVariable();
	// 获取当前program 配置句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentProgramFileVariable(const string &fileName);
    // 获取当前program 静态校准配置
    RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentStaticCorrectVariable(const string& programName ="");

	//获取相机参数配置文件句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable * getCameraSetCfgFileVariable(const string& programName="");
	//获取时序配置文件句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable * getSequenceSetCfgFileVariable_new(const string& programName = "", string strSceneName = "");
	//获取时序数
	RESOURCE_LIBRARY_EXPORT int getSequenceNumber(string strSceneName = "" );

	//获取电流配置文件句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable * getIniLightEleFileVariable_new(const string& programName = "", string strSceneName = "");

    //获取时序中相机的使能
    RESOURCE_LIBRARY_EXPORT int getCameraOn(int iSequence,int iVal[4]);

    //获取深视配置文件句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable * getDVCfgFileVariable(const string& programName = "");

    //获取切图配置文件句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable * getAcquisitionFileVariable(const string& programName = "", string strSceneName="");

    //获取Inner配置文件句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable * getInnerCfgFileVariable(const string& programName = "" );
	//获取时序中设置的光源类型与相机索引
	//返回值0为成功，-1无配置文件
	//iType 0 无效，1亮场 ，2 暗场， 3 穿透
	//iCameraIndex 0 无效 1相机一，2相机二，3相机三，4相机四
    RESOURCE_LIBRARY_EXPORT int getSqueLightTypeAndCameraIndex(const string& sQue, int &iType, int &iCameraIndex, string strSceneName = "");
    RESOURCE_LIBRARY_EXPORT int getCamNameList(int group, string *vecNameList, string strSceneName = "");

    //获取缺陷生成配置文件句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable * getFlawgenerationCfgFileVariable(const string& programName  = "");

	RESOURCE_LIBRARY_EXPORT void getEnabelChannle(vector<string> &vec, string strSceneName = "");

    RESOURCE_LIBRARY_EXPORT const string getCamNameWithID(int index);
    RESOURCE_LIBRARY_EXPORT const string getSeqNameWithID(int index);
    RESOURCE_LIBRARY_EXPORT const int getCamIndexWithName(string name);
    RESOURCE_LIBRARY_EXPORT const int getSeqIndexWithName(string name);


	RESOURCE_LIBRARY_EXPORT bool enableCutImage(string name);
	//获取缺陷定义文件句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable * getDefectFileVariable(const string& programName = "");

	//获取blobCluster文件
	RESOURCE_LIBRARY_EXPORT IFileVariable * getQualificationCfgFileVariable(const string& programName = "");

    //获取指定流程下对应配置文件句柄
    RESOURCE_LIBRARY_EXPORT IFileVariable * getProgramFileVariable(const string &fileName, const string& programName = "");	

	RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentRoiVariable(const string& programName = "");

	RESOURCE_LIBRARY_EXPORT bool camIsOnline(int iCamIndex);

    RESOURCE_LIBRARY_EXPORT IFileVariable * getLightComFile();

    RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentThresholdVariable(const string &programName = "");

	RESOURCE_LIBRARY_EXPORT void  setConfigName(string programName);

	RESOURCE_LIBRARY_EXPORT bool  isChannelEnable(string strChannelName = "", string strSceneName = "");

	RESOURCE_LIBRARY_EXPORT const string getCamNameWithChannel(string strChannelName = "", string strSceneName = "");

	RESOURCE_LIBRARY_EXPORT void getChannleByCamIndex(vector<string> &vec, int iCamIndex,string strSceneName = "");

	RESOURCE_LIBRARY_EXPORT bool  isImageStationEnable(int index);

	RESOURCE_LIBRARY_EXPORT  int getImageStationCnt();

	RESOURCE_LIBRARY_EXPORT IFileVariable * getLightCfgVariable(string stationIndex);
	
	RESOURCE_LIBRARY_EXPORT string getStationName(unsigned int iIndex);

	RESOURCE_LIBRARY_EXPORT int getStationType(unsigned int iIndex);

	RESOURCE_LIBRARY_EXPORT IFileVariable * getStationCfgVariable();

    RESOURCE_LIBRARY_EXPORT int  getFirstEnableStation();

	RESOURCE_LIBRARY_EXPORT IFileVariable* getdefectIndex();
		//获取下料机文件句柄
	RESOURCE_LIBRARY_EXPORT IFileVariable * getCurrentRobotVariable(const string& programName = "");

    RESOURCE_LIBRARY_EXPORT bool isEnableClean();
    RESOURCE_LIBRARY_EXPORT bool isEnableShang();
    RESOURCE_LIBRARY_EXPORT bool isEnableXia();
    RESOURCE_LIBRARY_EXPORT bool isEnableClearMode();
	
	RESOURCE_LIBRARY_EXPORT bool isAutoDeleteImage();
	RESOURCE_LIBRARY_EXPORT bool isAutoDeleteData();

	RESOURCE_LIBRARY_EXPORT bool isEnablePenma();

	RESOURCE_LIBRARY_EXPORT bool getEnableFrontSig();

#ifdef __cplusplus
}
#endif

#endif _DISPENSERCOMMON_H_

