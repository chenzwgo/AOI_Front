#include "g12common.h"
#include "sdkcore.h"
#include "filevariable/ifilevariable.h"
#include "motion/imotion.h"
#include "card/iaxis.h"
#include "g12globalenv.h"
#include "stringcommon.h"
static string g_ConfigName = "0";//TRACK_LONG_SIDE;

IFileVariable *getFileVariable(const string &fileName)
{

	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	return  pFileManager->getFileVariable(fileName);
}

const string getCurrentProgramPath()
{
	string strPaht = "./programs/";
	string strName =  getCurrentProgramName();
	if (strName.length() == 0)
	{
		return "";
	}
	return strPaht.append(strName);
}

const string getCurrentProgramName()
{
	if (getFileVariable("./programs/globalenv.ini") == NULL)
	{
		return "";
	}
	return getFileVariable("./programs/globalenv.ini")->getValue("products","current_program","productA").toCString();
}

const string getProgramNameList()
{
    if (getFileVariable("./programs/globalenv.ini") == NULL)
    {
        return "";
    }
    return getFileVariable("./programs/globalenv.ini")->getValue("products","product_list").toCString();
}
const void setProgramNameList(const string& ProgramNameList)
{
	if (getFileVariable("./programs/globalenv.ini") == NULL)
	{
		return ;
	}
	getFileVariable("./programs/globalenv.ini")->setValue("products", "product_list", ProgramNameList, true);
}
const void setCurrentProgramName(const string& programName)
{
	if (getFileVariable("./programs/program.ini") == NULL)
	{
		return;
	}
	getFileVariable("./programs/globalenv.ini")->setValue("products","current_program",programName,true);
}

const string getCurrentProgramRecipesList()
{
    string path = getCurrentProgramPath() + "/productinfo.ini";

    if (getFileVariable(path) == NULL)
    {
        return "";
    }
    return getFileVariable(path)->getValue("channels","name").toCString();
}

const string getCurrentProgramCamNameList()
{
    string path = getCurrentProgramPath() + "/productinfo.ini";

    if (getFileVariable(path) == NULL)
    {
        return "";
    }
    return getFileVariable(path)->getValue("camera","name", "camU1,camU2,camD1,camD2,camU3").toCString();
}

IFileVariable *getGlobalFileVariable()
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	return  pFileManager->getFileVariable("./programs/globalenv.ini");
}
IFileVariable *getGlobalSNFileVariable()
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	return  pFileManager->getFileVariable("./programs/globalenvSn.ini");
}

IFileVariable *getCurrentProgramFileVariable(const string &fileName)
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	string path = getCurrentProgramPath() + "/" + fileName;
	return  pFileManager->getFileVariable(path);
}


IFileVariable * getMotionFileVariable()
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	return  pFileManager->getFileVariable("./programs/motion.ini");
}

IFileVariable * getCurrentStaticCorrectVariable(const string& programName)
{
    IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
    if (NULL == pFileManager)
    {
        return NULL;
    }
    string path = getCurrentProgramPath() + "/" + STATICCORRECT_FILE;
    if (programName.length() != 0)
    {
        string strPaht = "./programs/";
        path = strPaht + programName +"/" + STATICCORRECT_FILE;
    }

    return  pFileManager->getFileVariable(path);
}

 IFileVariable * getCurrentDynamicCorrectVariable(const string& programName)
{
    IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
    if (NULL == pFileManager)
    {
        return NULL;
    }
    string path = getCurrentProgramPath() + "/" + DYNAMICCORRECT_FILE;
    if (programName.length() != 0)
    {
        string strPaht = "./programs/";
        path = strPaht + programName +"/" + DYNAMICCORRECT_FILE;
    }
    return  pFileManager->getFileVariable(path);
}


 IFileVariable * getCurrentSignalFilterVariable( const string& programName /*= ""*/ )
 {
     IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
     if (NULL == pFileManager)
     {
         return NULL;
     }
     string path = getCurrentProgramPath() + "/" + SIGNALFILTER_FILE;
     if (programName.length() != 0)
     {
         string strPaht = "./programs/";
         path = strPaht + programName +"/" + SIGNALFILTER_FILE;
     }
     return  pFileManager->getFileVariable(path);
 }

 IFileVariable * getCameraSetCfgFileVariable(const string& programName )
{
	////
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	string path = getCurrentProgramPath() + "/" + USERSETTING_CAMERA_CFG_FILE;
	if (programName.length() != 0)
	{
		string strPaht = "./programs/";
		path = strPaht + programName + "/" + USERSETTING_CAMERA_CFG_FILE;
	}

	string strPaht;
	if (programName.length() != 0)
	{
		strPaht = "./programs/";
		strPaht = strPaht + programName + "/";
	}
	else
	{
		strPaht = getCurrentProgramPath() + "/";
	}
	//path = strPaht + g_ConfigName + "_" + USERSETTING_CAMERA_CFG_FILE;
	path = strPaht + USERSETTING_CAMERA_CFG_FILE;
	return  pFileManager->getFileVariable(path);

}

IFileVariable * getSequenceSetCfgFileVariable_new(const string& programName, string strSceneName/* = ""*/)
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	string path = getCurrentProgramPath() + "/" + ILLUMINATIONCONTROL_FILE_NEW;
	if (programName.length() != 0)
	{
		string strPaht = "./programs/";
		path = strPaht + programName + "/" + ILLUMINATIONCONTROL_FILE_NEW;
	}

	if (strSceneName.length() != 0)
	{
		string strPaht;
		if (programName.length() != 0)
		{
			strPaht = "./programs/";
			strPaht = strPaht + programName + "/";
		}
		else
		{
			strPaht = getCurrentProgramPath() + "/";
		}
		path = strPaht + strSceneName + "_" + ILLUMINATIONCONTROL_FILE_NEW;
	}
	else
	{
		string strPaht;
		if (programName.length() != 0)
		{
			strPaht = "./programs/";
			strPaht = strPaht + programName + "/";
		}
		else
		{
			strPaht = getCurrentProgramPath() + "/";
		}
		path = strPaht + g_ConfigName + "_" + ILLUMINATIONCONTROL_FILE_NEW;
	}

	return  pFileManager->getFileVariable(path);
}

RESOURCE_LIBRARY_EXPORT int getSequenceNumber(string strSceneName)
{
	IFileVariable *pFile = getSequenceSetCfgFileVariable_new( getCurrentProgramName(),strSceneName);
	int iSqu = 1;
	if (pFile)
	{
		iSqu = 0;

        string programs = getCurrentProgramRecipesList();
        vector<string> vect = SString::split(programs,",");
        for (int index = 0;index <vect.size();index++)
        {
            iSqu+=(pFile->getValue(vect.at(index), ENABELCHANNLE).toInt()>0 ) ?1:0;
        }

	}
	return iSqu;
}

RESOURCE_LIBRARY_EXPORT IFileVariable * getIniLightEleFileVariable_new(const string& programName, string strSceneName/* = ""*/)
{
	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return NULL;
	}
	string path = getCurrentProgramPath() + "/" + INITLINGHTELE_FILE_NEW;
	if (programName.length() != 0)
	{
		string strPaht = "./programs/";
		path = strPaht + programName + "/" + INITLINGHTELE_FILE_NEW;
	}
	if (strSceneName.length() != 0)
	{
		string strPaht;
		if (programName.length() != 0)
		{
			strPaht = "./programs/";
			strPaht = strPaht + programName + "/";
		}
		else
		{
			strPaht = getCurrentProgramPath() + "/";
		}
		path = strPaht + strSceneName + "_" + INITLINGHTELE_FILE_NEW;
	}

	return  pFileManager->getFileVariable(path);
}


 int getCameraOn( int iSequence,int iVal[CAMERA_NUMBER] )
{
    IFileVariable *pFile = getSequenceSetCfgFileVariable_new();
    string programs = getCurrentProgramRecipesList();
    vector<string> vect = SString::split(programs,",");
	if (iSequence >= vect.size())
	{
		return -1;
	}
    if (pFile)
    {
        bool benable =  pFile->getValue(vect[iSequence], ENABELCHANNLE,0).toInt(0);
        if(!benable)
        {
            for (int i = 0; i < 4;i++)
            {
                iVal[i] = 0;
            }
            return 0;
        }

        int Val = 0;
        Val =  pFile->getValue(vect[iSequence], PRO_COMERAINDEX,-1).toInt(-1);
		for (int i = 0; i < 4;i++)
		{
			iVal[i] = 0;
		}
        if (Val>=0)
		{
            iVal[Val] = 1;
		}
        return 0;
    }
    return -1;
}


 RESOURCE_LIBRARY_EXPORT IFileVariable * getDVCfgFileVariable(const string& programName/* = ""*/)
 {
     IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
     if (NULL == pFileManager)
     {
         return NULL;
     }
     string path = getCurrentProgramPath() + "/" + "dvconfig.ini";
     if (programName.length() != 0)
     {
         string strPaht = "./programs/";
         path = strPaht + programName + "/" + "dvconfig.ini";
     }
     return  pFileManager->getFileVariable(path);
 }

 RESOURCE_LIBRARY_EXPORT IFileVariable * getAcquisitionFileVariable(const string& programName, string strSceneName /*= ""*/ )
 {

     IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
     if (NULL == pFileManager)
     {
         return NULL;
     }
     string path = getCurrentProgramPath() + "/" + ACQUISITION_FILE;
     if (programName.length() != 0)
     {
         string strPaht = "./programs/";
         path = strPaht + programName + "/" + ACQUISITION_FILE;
     }

     if (strSceneName.length() != 0)
     {
         string strPaht;
         if (programName.length() != 0)
         {
             strPaht = "./programs/";
             strPaht = strPaht + programName + "/";
         }
         else
         {
             strPaht = getCurrentProgramPath() + "/";
         }
         path = strPaht + strSceneName + "_" + ACQUISITION_FILE;
     }
     else
     {
         string strPaht;
         if (programName.length() != 0)
         {
             strPaht = "./programs/";
             strPaht = strPaht + programName + "/";
         }
         else
         {
             strPaht = getCurrentProgramPath() + "/";
         }
         path = strPaht + g_ConfigName + "_" + ACQUISITION_FILE;
     }

     return  pFileManager->getFileVariable(path);
 }


 RESOURCE_LIBRARY_EXPORT IFileVariable * getInnerCfgFileVariable(const string& programName )
 {

     IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
     if (NULL == pFileManager)
     {
         return NULL;
     }
     string path = getCurrentProgramPath() + "/" + INNER_FILE;
     if (programName.length() != 0)
     {
         string strPaht = "./programs/";
         path = strPaht + programName +"/" + INNER_FILE;
     }
     return  pFileManager->getFileVariable(path);
 }
  RESOURCE_LIBRARY_EXPORT int getSqueLightTypeAndCameraIndex(const string& sQue,int &iType, int &iCameraIndex, string strSceneName)
 {
	 IFileVariable *pFile = getSequenceSetCfgFileVariable_new(getCurrentProgramName(),strSceneName);
	 if (pFile)
	 {
		 //
		 iType = pFile->getValue(sQue, PRO_TYPE).toInt();

         bool benable =  pFile->getValue(sQue, ENABELCHANNLE,0).toInt(0);
         if(!benable)
         {
             iCameraIndex = -1;
			 return 0;
         }
         iCameraIndex = pFile->getValue(sQue, PRO_COMERAINDEX,-1).toInt(-1)+1;

		 /*
		 int iRFCFD = iLightOn_F / 1000;
		 int iRFDD5 = (iLightOn_F / 100) % 2;
		 iType = (iRFCFD + iRFDD5) > 0 ? 2 : 0; //暗场 2

		 int iRFH   = (iLightOn_F / 10) % 2;
		 int iRFL   = iLightOn_F % 2;

		 iType = (iRFH + iRFL) > 0 ? 1 : iType;//亮场 1

		 int iLightOn_R = pFile->getValue(sQue, LIGHTON_R).toInt();

		 int iRRDD5 = (iLightOn_R / 100) % 2;
		 iType = iRRDD5 > 0 ? 2 : iType;   //暗场 2

		 int iRRH   = (iLightOn_R / 10) % 2;
		 int iRRL   = iLightOn_R % 2;
		 iType = (iRRH + iRRL) > 0 ? 3 : iType;//穿透 3


		 int iCameraOn_F = pFile->getValue(sQue, CAMERAON_F).toInt();

		 int  iCameraFisrt  = (iCameraOn_F / 10) % 2;
		 iCameraIndex = iCameraFisrt > 0 ? 1 : 0;

		 int  iCameraSecond = iCameraOn_F % 2;
		 iCameraIndex = iCameraSecond > 0 ? 2 : iCameraIndex;

		 int   iCameraOn_R = pFile->getValue(sQue, CAMERAON_R).toInt();

		 int  iCameraThree = (iCameraOn_R / 10) % 2;
		 iCameraIndex = iCameraThree > 0 ? 3 : iCameraIndex;

		 int  iCameraFour  = iCameraOn_R % 2;
		 iCameraIndex = iCameraFour > 0 ? 4 : iCameraIndex;


		 int iLightVal = iRFCFD + iRFDD5 + iRFH + iRFL + iRRDD5 + iRRH + iRRL;
		 if ( 2 < iLightVal || 0 == iLightVal)
		 {
			 iType = 0;
		 }

		 int iCameraVal = iCameraFisrt + iCameraSecond + iCameraThree + iCameraFour;
		 if (1 != iCameraVal)
		 {
			 iCameraIndex = 0;
		 }*/
		 return 0;
	 }
	 return -1;
  }

  IFileVariable *getFlawgenerationCfgFileVariable(const string &programName)
  {
      IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
      if (NULL == pFileManager)
      {
          return NULL;
      }
      string path = getCurrentProgramPath() + "/" + FLAWGENERATION_FILE;
      if (programName.length() != 0)
      {
          string strPaht = "./programs/";
          path = strPaht + programName +"/" + FLAWGENERATION_FILE;
      }
      return  pFileManager->getFileVariable(path);
  }

  void getEnabelChannle(vector<string> &vec, string strSceneName)
  {
	  vec.clear();
	  IFileVariable *pFile = getSequenceSetCfgFileVariable_new(getCurrentProgramName(),strSceneName);
	  if (pFile)
	  {
		  string strChannelNames = getCurrentProgramRecipesList().c_str();
		  std::vector<std::string> ChannelNameList = SString::split(strChannelNames, ",");
		  for (int f = 0;  f < ChannelNameList.size(); f++)//时序
		  {
			  int iChannleVal = pFile->getValue(ChannelNameList.at(f),ENABELCHANNLE).toInt();
			  if (iChannleVal>0)
			  {
				  vec.push_back(ChannelNameList.at(f));
			  }
		  }
	  }

  }

  RESOURCE_LIBRARY_EXPORT const string getCamNameWithID(int index)
  {
      int iSize = 0;
      int id = 0;
      string nameList;
      string name = "";
      size_t posBegin = 0;
      size_t pos = 0;

      nameList = getCurrentProgramCamNameList();
      iSize = count(nameList.begin(), nameList.end(), ',') + 1;
      for (int i = 0; i < iSize && i < CAMERA_NUMBER; i++)
      {
          pos = nameList.find(',', posBegin);
          if (pos == string::npos)
          {
              pos = nameList.size();
          }
          if (i == index)
          {
              name = nameList.substr(posBegin, pos - posBegin);
              break;
          }
          posBegin = pos + 1;
      }
      return name;
  }

  RESOURCE_LIBRARY_EXPORT const string getSeqNameWithID(int index)
  {
      int iSize = 0;
      int chid = 0;
      string seqList;
      string lchName = "";
      size_t posBegin = 0;
      size_t pos = 0;

      seqList = getCurrentProgramRecipesList();
      iSize = count(seqList.begin(), seqList.end(), ',') + 1;
      for (int i = 0; i < iSize && i < SEQUE_NUMBER; i++)
      {
          pos = seqList.find(',', posBegin);
          if (pos == string::npos)
          {
              pos = seqList.size();
          }
          if (i == index)
          {
              lchName = seqList.substr(posBegin, pos - posBegin);
              break;
          }
          posBegin = pos + 1;
      }
      return lchName;
  }

  RESOURCE_LIBRARY_EXPORT const int getCamIndexWithName(string name)
  {
      int iSize = 0;
      int id = 0;
      string nameList;
      string camName;
      size_t posBegin = 0;
      size_t pos = 0;

      nameList = getCurrentProgramCamNameList();
      iSize = count(nameList.begin(), nameList.end(), ',')+1;
      for (int i = 0; i < iSize && i < CAMERA_NUMBER; i++)
      {
          pos = nameList.find(',', posBegin);
          if (pos == string::npos)
          {
              pos = nameList.size();
          }
          camName = nameList.substr(posBegin, pos - posBegin);
          if (camName == name)
          {
              id = i;
              break;
          }
          posBegin = pos + 1;
      }
      return id;
  }

  RESOURCE_LIBRARY_EXPORT const int getSeqIndexWithName(string name)
  {
      int iSize = 0;
      int id = 0;
      string nameList;
      string seqName;
      size_t posBegin = 0;
      size_t pos = 0;

      nameList = getCurrentProgramRecipesList();
      iSize = count(nameList.begin(), nameList.end(), ',') + 1;
      for (int i = 0; i < iSize && i < SEQUE_NUMBER; i++)
      {
          pos = nameList.find(',', posBegin);
          if (pos == string::npos)
          {
              pos = nameList.size();
          }
          seqName = nameList.substr(posBegin, pos - posBegin);
          if (seqName == name)
          {
              id = i;
              break;
          }
          posBegin = pos + 1;
      }
      return id;
  }


  RESOURCE_LIBRARY_EXPORT IFileVariable * getDefectFileVariable(const string& programName /*= ""*/)
  {
	  IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	  if (NULL == pFileManager)
	  {
		  return NULL;
	  }
	  string path = getCurrentProgramPath() + "/" + DEFECT_CFG_FILE;
	  if (programName.length() != 0)
	  {
		  string strPaht = "./programs/";
		  path = strPaht + programName + "/" + DEFECT_CFG_FILE;
	  }
	  return  pFileManager->getFileVariable(path);
  }

  RESOURCE_LIBRARY_EXPORT IFileVariable * getProgramFileVariable(const string &fileName, const string& programName /*= ""*/)
  {
      IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
      if (NULL == pFileManager)
      {
          return NULL;
      }
      string strProName = programName;
      if (strProName.length() == 0)
      {
          strProName = getCurrentProgramName();
      }
      string path = "./programs/" + strProName + "/" + fileName;
      return  pFileManager->getFileVariable(path);
  }

  RESOURCE_LIBRARY_EXPORT IFileVariable * getLightComFile()
  {
      IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
      if (NULL == pFileManager)
      {
          return NULL;
      }
      return  pFileManager->getFileVariable("./plugins/customsettingplugin/usersetting.ini");
  }


   IFileVariable * getQualificationCfgFileVariable(const string& programName /*= ""*/)
  {
	  IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	  if (NULL == pFileManager)
	  {
		  return NULL;
	  }
	  string path = getCurrentProgramPath() + "/" + DEFECT_QUALIFICATION_FILE;
	  if (programName.length() != 0)
	  {
		  string strPaht = "./programs/";
		  path = strPaht + programName + "/" + DEFECT_QUALIFICATION_FILE;
	  }
	  return  pFileManager->getFileVariable(path);
  }

   RESOURCE_LIBRARY_EXPORT int getCamNameList(int group, string *vecNameList,string strSceneName)
   {
	   // for fix
       if (NULL == vecNameList)
       {
           return -1;
       }
       IFileVariable* pFileVariable = getAcquisitionFileVariable(getCurrentProgramName(), strSceneName);
       if (NULL != pFileVariable)
       {
		   string KeyName = "ImageConfig_" + SString::IntToStr(group);
		   int iCamCnt = pFileVariable->getValue(KeyName, ACQUISITION_CAM_CNT).toInt();
		   bool bCam1Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM1_ENABLE).toBool();
		   bool bCam2Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM2_ENABLE).toBool();
		   bool bCam3Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM3_ENABLE).toBool();
		   bool bCam4Enable = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM4_ENABLE).toBool();
		   string strCam1Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM1_SN).toCString("");
		   string strCam2Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM2_SN).toCString("");
		   string strCam3Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM3_SN).toCString("");
		   string strCam4Sn = pFileVariable->getValue(KeyName, ACQUISITION_UPCAM4_SN).toCString("");
		   if (bCam1Enable && iCamCnt > 0)
		   {
			   vecNameList[0] = strCam1Sn;
		   }
		   if (bCam2Enable && iCamCnt > 1)
		   {
			   vecNameList[1] = strCam2Sn;
		   }
		   if (bCam3Enable && iCamCnt > 2)
		   {
			   vecNameList[2] = strCam3Sn;
		   }
		   if (bCam4Enable && iCamCnt > 3)
		   {
			   vecNameList[3] = strCam4Sn;
		   }   
           return 0;
       }
       return -1;
   }



   IFileVariable *getCurrentThresholdVariable(const string &programName)
   {
       IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
       if (NULL == pFileManager)
       {
           return NULL;
       }
       string path = getCurrentProgramPath() + "/" + THRESHOLD_FILE;
       if (programName.length() != 0)
       {
           string strPaht = "./programs/";
           path = strPaht + programName +"/" + THRESHOLD_FILE;
       }
       return  pFileManager->getFileVariable(path);
   }

   RESOURCE_LIBRARY_EXPORT IFileVariable * getCurrentCompoundVariable(const string& programName /*= ""*/)
   {
	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager)
	   {
		   return NULL;
	   }
	   string path = getCurrentProgramPath() + "/" + COMPOUND_FILE;
	   if (programName.length() != 0)
	   {
		   string strPaht = "./programs/";
		   path = strPaht + programName + "/" + COMPOUND_FILE;
	   }
	   return  pFileManager->getFileVariable(path);
   }

   RESOURCE_LIBRARY_EXPORT IFileVariable * getCurrentRoiVariable(const string& programName /*= ""*/)
   {
	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager)
	   {
		   return NULL;
	   }
	   string path = getCurrentProgramPath() + "/" + ROI_FILE;
	   if (programName.length() != 0)
	   {
		   string strPaht = "./programs/";
		   path = strPaht + programName + "/" + ROI_FILE;
	   }
	   return  pFileManager->getFileVariable(path);
   }
   RESOURCE_LIBRARY_EXPORT bool camIsOnline(int iCamIndex)
   {
	   // 2.5Dplus不需要
	   return true;

	   string CanOnline[4] = { "Cam1_Online","Cam2_Online","Cam3_Online","Cam4_Online" };
	   if (iCamIndex >= 4 || iCamIndex < 0)
	   {
		   return "";
	   }

	   IFileVariable *pFile = getAcquisitionFileVariable();
	   if (pFile != NULL)
	   {
		   return pFile->getValue("share",CanOnline[iCamIndex],false).toBool(0);
	   }
	   return false;
   }

   RESOURCE_LIBRARY_EXPORT void setConfigName(string programName)
   {
	   g_ConfigName = programName;
   }
   RESOURCE_LIBRARY_EXPORT int  get2Kor8KFromSceneName( string strSceneName, const string& programName)
   {
	   int iType = -1;

	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager || 0 == strSceneName.length())
	   {
		   return iType;
	   }
	   string srProgramPath = getCurrentProgramName();
	   if (programName.length() != 0)
	   {
		   srProgramPath = programName;
	   }
	   IFileVariable* pFileVariable = getAcquisitionFileVariable(srProgramPath, strSceneName);
	   if (pFileVariable != NULL)
	   {
		   int iresolutionX = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CAM_RESOLUTIONX, 0).toInt();
		   if (iresolutionX == 2048)
		   {
			   iType = 0;
		   }
		   else if (iresolutionX >= 8192)
		   {
			   iType = 1;
		   }

	   }
	   return iType;
   }

   RESOURCE_LIBRARY_EXPORT bool isChannelEnable(string strChannelName /*= ""*/, string strSceneName /*= ""*/)
   {
	   bool bRet = false;
	   IFileVariable *pFile = getSequenceSetCfgFileVariable_new(getCurrentProgramName(), strSceneName);
	   if (pFile)
	   {
		   bRet = pFile->getValue(strChannelName, ENABELCHANNLE).toInt();
	   }
	   return bRet;
   }

   RESOURCE_LIBRARY_EXPORT const string getCamNameWithChannel(string strChannelName /*= ""*/, string strSceneName /*= ""*/)
   {
	   int iCamIndex = 0;
	   IFileVariable *pFile = getSequenceSetCfgFileVariable_new(getCurrentProgramName(), strSceneName);
	   if (pFile)
	   {
		   iCamIndex = pFile->getValue(strChannelName, PRO_COMERAINDEX).toInt();
	   }
	   return getCamNameWithID(iCamIndex);
   }

   RESOURCE_LIBRARY_EXPORT void getChannleByCamIndex(vector<string> &vec, int iCamIndex,string strSceneName /*= ""*/)
   {
	   vec.clear();
	   string strChannel[8] = { "SEQUENCE_A","SEQUENCE_B","SEQUENCE_C","SEQUENCE_D","SEQUENCE_E","SEQUENCE_F","SEQUENCE_G","SEQUENCE_H" };
	   IFileVariable *pFile = getSequenceSetCfgFileVariable_new(getCurrentProgramName(), strSceneName);
	   if (pFile)
	   {
		   for (int i = 0; i < 8; i++)
		   {
			   int iIndex = pFile->getValue(strChannel[i], PRO_COMERAINDEX).toInt();
			   if (iCamIndex == iIndex)
			   {
				   vec.push_back(strChannel[i]);
			   }
		   }		  
	   }
   }
   RESOURCE_LIBRARY_EXPORT bool isImageStationEnable(int index)
   {
	   string sense = SString::IntToStr(index);
	   bool benable = false;

	   IFileVariable* pAcquisitionFile = getAcquisitionFileVariable(getCurrentProgramName(), sense);
	   if (pAcquisitionFile != NULL)
	   {
		   benable = pAcquisitionFile->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_GRAP_ENABLE, true).toBool(true);
	   }
	   return benable;
   }

   RESOURCE_LIBRARY_EXPORT int getImageStationCnt()
   {
	   int iCnt = getFileVariable("./programs/stationcfg.ini")->getValue("ImageStation", "cnt", 1).toInt(1);
	   return iCnt;
   }

   RESOURCE_LIBRARY_EXPORT string getStationName(unsigned int iIndex)
   {
	   char key[100] = { 0 };
	   sprintf(key, "ImageStation_%d", iIndex);
	   string stationName = getFileVariable("./programs/stationcfg.ini")->getValue(key, "name","").toCString();
	   return stationName;
   }

   RESOURCE_LIBRARY_EXPORT int getStationType(unsigned int iIndex)
   {
	   char key[100] = { 0 };
	   sprintf(key, "ImageStation_%d", iIndex);
	   int type = getFileVariable("./programs/stationcfg.ini")->getValue(key, "type", "").toInt();
	   return type;
   }

   RESOURCE_LIBRARY_EXPORT IFileVariable * getLightCfgVariable(string stationIndex)
   {
	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager)
	   {
		   return NULL;
	   }
	   string strPath = "./programs/";
	   strPath = strPath + LIGHT_CFG + "_" + stationIndex + ".ini";
	   return  pFileManager->getFileVariable(strPath);
   }

   RESOURCE_LIBRARY_EXPORT IFileVariable * getStationCfgVariable()
   {
	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager)
	   {
		   return NULL;
	   }
       return  pFileManager->getFileVariable("./programs/stationcfg.ini");
   }

   RESOURCE_LIBRARY_EXPORT int getFirstEnableStation()
   {
       int index = -1;
       for(int i = 0;i <getImageStationCnt();i++)
       {
           if(isImageStationEnable(i))
           {
               return i;
           }
       }
       return index;
   }
   
   IFileVariable * getCurrentRobotVariable(const string& programName /*= ""*/)
{
   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
   if (NULL == pFileManager)
   {
	   return NULL;
   }
   string path = getCurrentProgramPath() + "/" + ROBOT_SET_FILE;
   if (programName.length() != 0)
   {
	   string strPaht = "./programs/";
	   path = strPaht + programName + "/" + ROBOT_SET_FILE;
   }
   return  pFileManager->getFileVariable(path);
   }

   bool isEnableClean()
   {
        IFileVariable *pFile = getCameraSetCfgFileVariable();
        if (pFile != NULL)
        {
           return !pFile->getValue(IMAGE_PARAM_DISABLE_CLEAN, true).toBool();
        }
        return false;
   }

   bool isEnableShang()
   {
       IFileVariable *pFile = getCameraSetCfgFileVariable();
       if (pFile != NULL)
       {
          return !pFile->getValue(IMAGE_PARAM_DISABLE_SHANG, true).toBool();
       }
       return false;
   }

   bool isEnableXia()
   {
       IFileVariable *pFile = getCameraSetCfgFileVariable();
       if (pFile != NULL)
       {
          return !pFile->getValue(IMAGE_PARAM_DISABLE_XIA, true).toBool();
       }
       return false;
   }

   bool isEnableClearMode()
   {
       IFileVariable *pFile = getCameraSetCfgFileVariable();
       if (pFile != NULL)
       {
          return !pFile->getValue(IMAGE_PARAM_DISABLE_CLEAR_MODE, true).toBool();
       }
       return false;
   }
   
   RESOURCE_LIBRARY_EXPORT bool isAutoDeleteImage()
   {
	   IFileVariable *pFile = getCameraSetCfgFileVariable();
	   if (pFile)
	   {
		   bool isAutoDeleteImage = pFile->getValue(AUTO_DELETE_IMAGE_ENABLE, false).toBool();
		   return isAutoDeleteImage;
	   }
	   else
	   {
		   return false;
	   }
   }

   RESOURCE_LIBRARY_EXPORT bool isAutoDeleteData()
   {
	   IFileVariable *pFile = getCameraSetCfgFileVariable();
	   if (pFile)
	   {
		   bool isAutoDeleteImage = pFile->getValue(AUTO_DELETE_DATA_ENABLE, false).toBool();
		   return isAutoDeleteImage;
	   }
	   else
	   {
		   return false;
	   }
	   return false;
   }

   RESOURCE_LIBRARY_EXPORT bool isEnablePenma()
   {
	   IFileVariable* pFileRobot = getCurrentRobotVariable();
	   if (NULL == pFileRobot)
	   {
		   return false;
	   }
	   return  pFileRobot->getValue(PENMA_ENABLE, false).toBool();
   }
   RESOURCE_LIBRARY_EXPORT bool getEnableFrontSig()	//获取使能前后端通讯状态
   {
	   IFileVariable *pFile = getCameraSetCfgFileVariable();
	   if (pFile != NULL)
	   {
		   return pFile->getValue(BACKPC_ENABLE, false).toBool();
	   }
	   return false;
 }

   RESOURCE_LIBRARY_EXPORT IFileVariable* getdefectIndex()
   {
	   IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	   if (NULL == pFileManager)
	   {
		   return NULL;
	   }
	   string path = getCurrentProgramPath() + "/" + "defectIndex.ini";
	   
	   return  pFileManager->getFileVariable(path);
   }

