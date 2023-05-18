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


    // �����ļ�����ȡ�ļ���д IFileVariable* ָ��
    RESOURCE_LIBRARY_EXPORT  IFileVariable* getFileVariable(const string& fileName);

    // ��ȡ��ǰ�����·��
    RESOURCE_LIBRARY_EXPORT  const string getCurrentProgramPath();

    // ��ȡ��ǰ��������
    RESOURCE_LIBRARY_EXPORT  const string getCurrentProgramName();

    // ��ȡ���г��������
    RESOURCE_LIBRARY_EXPORT const  string getProgramNameList();
	// �������г��������
	RESOURCE_LIBRARY_EXPORT const  void setProgramNameList(const string& ProgramNameList);

	// ���õ�ǰ���������
	RESOURCE_LIBRARY_EXPORT   const void setCurrentProgramName(const string& programName);

    // ��ȡ��ǰ�����䷽ͨ������
    RESOURCE_LIBRARY_EXPORT const  string getCurrentProgramRecipesList();

    // ��ȡ��ǰ�������
    RESOURCE_LIBRARY_EXPORT const  string getCurrentProgramCamNameList();

	// ��ȡȫ�����þ��
	RESOURCE_LIBRARY_EXPORT IFileVariable *getMotionFileVariable();

	// ��ȡȫ�����þ��
    RESOURCE_LIBRARY_EXPORT IFileVariable *getGlobalFileVariable();
	RESOURCE_LIBRARY_EXPORT IFileVariable *getGlobalSNFileVariable();
	// ��ȡ��ǰprogram ���þ��
    RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentProgramFileVariable(const string &fileName);
    // ��ȡ��ǰprogram ��̬У׼����
    RESOURCE_LIBRARY_EXPORT IFileVariable *getCurrentStaticCorrectVariable(const string& programName ="");

	//��ȡ������������ļ����
	RESOURCE_LIBRARY_EXPORT IFileVariable * getCameraSetCfgFileVariable(const string& programName="");
	//��ȡʱ�������ļ����
	RESOURCE_LIBRARY_EXPORT IFileVariable * getSequenceSetCfgFileVariable_new(const string& programName = "", string strSceneName = "");
	//��ȡʱ����
	RESOURCE_LIBRARY_EXPORT int getSequenceNumber(string strSceneName = "" );

	//��ȡ���������ļ����
	RESOURCE_LIBRARY_EXPORT IFileVariable * getIniLightEleFileVariable_new(const string& programName = "", string strSceneName = "");

    //��ȡʱ���������ʹ��
    RESOURCE_LIBRARY_EXPORT int getCameraOn(int iSequence,int iVal[4]);

    //��ȡ���������ļ����
    RESOURCE_LIBRARY_EXPORT IFileVariable * getDVCfgFileVariable(const string& programName = "");

    //��ȡ��ͼ�����ļ����
    RESOURCE_LIBRARY_EXPORT IFileVariable * getAcquisitionFileVariable(const string& programName = "", string strSceneName="");

    //��ȡInner�����ļ����
    RESOURCE_LIBRARY_EXPORT IFileVariable * getInnerCfgFileVariable(const string& programName = "" );
	//��ȡʱ�������õĹ�Դ�������������
	//����ֵ0Ϊ�ɹ���-1�������ļ�
	//iType 0 ��Ч��1���� ��2 ������ 3 ��͸
	//iCameraIndex 0 ��Ч 1���һ��2�������3�������4�����
    RESOURCE_LIBRARY_EXPORT int getSqueLightTypeAndCameraIndex(const string& sQue, int &iType, int &iCameraIndex, string strSceneName = "");
    RESOURCE_LIBRARY_EXPORT int getCamNameList(int group, string *vecNameList, string strSceneName = "");

    //��ȡȱ�����������ļ����
    RESOURCE_LIBRARY_EXPORT IFileVariable * getFlawgenerationCfgFileVariable(const string& programName  = "");

	RESOURCE_LIBRARY_EXPORT void getEnabelChannle(vector<string> &vec, string strSceneName = "");

    RESOURCE_LIBRARY_EXPORT const string getCamNameWithID(int index);
    RESOURCE_LIBRARY_EXPORT const string getSeqNameWithID(int index);
    RESOURCE_LIBRARY_EXPORT const int getCamIndexWithName(string name);
    RESOURCE_LIBRARY_EXPORT const int getSeqIndexWithName(string name);


	RESOURCE_LIBRARY_EXPORT bool enableCutImage(string name);
	//��ȡȱ�ݶ����ļ����
	RESOURCE_LIBRARY_EXPORT IFileVariable * getDefectFileVariable(const string& programName = "");

	//��ȡblobCluster�ļ�
	RESOURCE_LIBRARY_EXPORT IFileVariable * getQualificationCfgFileVariable(const string& programName = "");

    //��ȡָ�������¶�Ӧ�����ļ����
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
		//��ȡ���ϻ��ļ����
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

