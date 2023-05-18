#include "lightcontrol_tcp.h"
#include "lightcontrol_new.h"
#include "g12common.h"
#include "filevariable/ifilevariable.h"
#include "rtdefine.h"
#include "sdkcore.h"
#include "jfile.h"
#include <string>
#include "stringcommon.h"

using namespace std;

#define POWER_VAL     0.01

#define SOURCE_LIGHT_TYPE 0


lightcontrol_new::lightcontrol_new(int index)
{
	m_index = index;
	m_strLightEleFileFileName = INITLINGHTELE_FILE_NEW;
	m_pCurrentLightEleFile = NULL;
	m_pDefalueLightEleFile = NULL;
	memset(m_stIllumChannelCfg, 0, sizeof(m_stIllumChannelCfg));
    memset(&m_currEleCfg,0,sizeof(m_currEleCfg));
	initLightTcp();
    readEleConfig();
	readAllChannelCfg();
}

bool lightcontrol_new::readEleConfig()
{
	IFileVariable * pFile = getIniLightEleFileVariable_new(getCurrentProgramName(), SString::IntToStr(m_index));
	if (pFile == NULL)
	{
		return false;
    }

    int iLightEnableCnt  =  pFile->getValue(LIGHT_ENABLE_CNT).toInt(LIGHT_SIZE);
    for (int i = 0; i < iLightEnableCnt; i++)
	{
		for (int cd = 0; cd < 6; cd++)
		{
			for (int section = 0; section < 5; section++)
			{
				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
				sprintf(keyName, "ele_cd%d_value_%d", icdIndex, iSectionIndex);
				m_currEleCfg.EleValue[i][cd][section]=pFile->getValue(SString::IntToStr(i), keyName).toInt(0);
			}
		}
	}
    return true;
}

bool lightcontrol_new::saveEleConfig()
{
	IFileVariable * pFile = getIniLightEleFileVariable_new(getCurrentProgramName(), SString::IntToStr(m_index));
	if (pFile == NULL)
	{
		return false;
	}

	for (int i = 0; i < LIGHT_SIZE;i++)
	{
		for (int cd = 0; cd < 6;cd ++)
		{
			for (int  section = 0; section < 5; section++)
			{
				char keyName[128] = { 0 };	
				int icdIndex = cd + 1;
				int iSectionIndex = section + 1;
				sprintf(keyName, "ele_cd%d_value_%d", icdIndex, iSectionIndex);
				int iValue = m_currEleCfg.EleValue[i][cd][section];
				pFile->setValue(SString::IntToStr(i), keyName, iValue, true);
			}
		}
	}
	return true;
}

bool lightcontrol_new::readAllChannelCfg()
{
	string programName = getCurrentProgramName();

	"section1_up_cd1_1";
	"section1_douw_cd1_1";
	"section1_arc_cd1_1";
	"section1_c_cd1_1";
	"section1_pulse";
	"section1_carema_delay";
	"section1_scan_line";
	"section1_camera1_enable";

	IFileVariable *pFile = getSequenceSetCfgFileVariable_new(programName, SString::IntToStr(m_index));
    IFileVariable* pFileVariable = getAcquisitionFileVariable(programName, SString::IntToStr(m_index));

    int channelCnt = Channel_Index_Size;
    if (pFileVariable != NULL)
    {
        channelCnt = pFileVariable->getValue(ACQUISITION_IMAGECONFIG, ACQUISITION_CHANNEL_CNT).toInt();
    }

    for (int iChannel = 0; iChannel < Channel_Index_Size;iChannel++)
	{
		int CamEnableIndex=-1;
		bool ChannelEnable = false;

		for (int iScan_line = 0; iScan_line < Scan_line_Size;iScan_line++)
		{
			char sectionName[128] = { 0 };
			sprintf(sectionName, "section%d", iScan_line+1);
			m_stIllumChannelCfg[iChannel][iScan_line].iscan_line_count = pFile->getValue(ChannelIndexToName(iChannel), string(sectionName) + "_scan_line").toInt(0);
			for (int iCam = 0; iCam < Cam_Cnt;iCam++)
			{
				char CamName[128] = { 0 };
				sprintf(CamName, "section%d_camera%d_enable", iScan_line+1,iCam+1);
				m_stIllumChannelCfg[iChannel][iScan_line].CamEnable[iCam] = pFile->getValue(ChannelIndexToName(iChannel), CamName).toBool(0);
				if (pFile->getValue(ChannelIndexToName(iChannel), CamName).toBool(0))
				{
					m_stIllumChannelCfg[iChannel][iScan_line].CamEnableIndex = iCam;
					m_stIllumChannelCfg[iChannel][iScan_line].ChannelEnable = true;
					CamEnableIndex = iCam;
					ChannelEnable = true;
				}
				else
				{
					m_stIllumChannelCfg[iChannel][iScan_line].CamEnableIndex = iCam;
					m_stIllumChannelCfg[iChannel][iScan_line].ChannelEnable = false;
				}
			}

			for (int iLight = 0; iLight < LIGHT_SIZE;iLight++)
			{
				for (int cd = 0; cd < CD_Size;cd ++)
				{
					for (int iSection = 0; iSection < Section_Size;iSection++)
					{
						char keyName[128] = { 0 };
						int icdIndex = cd + 1;
						int iSectionIndex = iSection + 1;
						sprintf(keyName, "section%d_%d_cd%d_%d", iScan_line+1, iLight, icdIndex, iSectionIndex);
						m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].type = (Light_Type)iLight;
						m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].enable[cd][iSection] = pFile->getValue(ChannelIndexToName(iChannel), keyName).toBool(false);
					}

					// 脉宽改成各通道中的每种光类型各一个
					char keyNamePluse[128] = { 0 };
					sprintf(keyNamePluse, "section%d_%d_cd%d_pulse", iScan_line + 1, iLight, cd + 1);
					m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].ipulse[cd] = pFile->getValue(ChannelIndexToName(iChannel), keyNamePluse).toInt();
				}
			}
		}
        
		pFile->setValue(ChannelIndexToName(iChannel), PRO_COMERAINDEX, CamEnableIndex, true);
		pFile->setValue(ChannelIndexToName(iChannel), ENABELCHANNLE, ChannelEnable, true);
	}

	return true;
}

bool lightcontrol_new::saveAllChannelCfg()
{
	string programName = getCurrentProgramName();

	"section1_0_cd1_1";
	"section1_1_cd1_1";
	"section1_2_cd1_1";
	"section1_3_cd1_1";
	"section1_pulse";
	"section1_carema_delay";
	"section1_scan_line";
	"section1_camera1_enable";

	IFileVariable *pFile = getSequenceSetCfgFileVariable_new(programName, SString::IntToStr(m_index));
	for (int iChannel = 0; iChannel < Channel_Index_Size; iChannel++)
	{
		for (int iScan_line = 0; iScan_line < Scan_line_Size; iScan_line++)
		{
			char sectionName[128] = { 0 };
			sprintf(sectionName, "section%d", iScan_line + 1);
			
			pFile->setValue(ChannelIndexToName(iChannel), string(sectionName) + "_scan_line", m_stIllumChannelCfg[iChannel][iScan_line].iscan_line_count, true);
			for (int iCam = 0; iCam < Cam_Cnt; iCam++)
			{
				char CamName[128] = { 0 };
				sprintf(CamName, "section%d_camera%d_enable", iScan_line + 1, iCam + 1);
				pFile->setValue(ChannelIndexToName(iChannel), CamName, m_stIllumChannelCfg[iChannel][iScan_line].CamEnable[iCam],true);
			}

			for (int iLight = 0; iLight < LIGHT_SIZE; iLight++)
			{
				for (int cd = 0; cd < CD_Size; cd++)
				{
					for (int iSection = 0; iSection < Section_Size; iSection++)
					{
						char keyName[128] = { 0 };
						int icdIndex = cd + 1;
						int iSectionIndex = iSection + 1;
						sprintf(keyName, "section%d_%d_cd%d_%d", iScan_line+1, iLight, icdIndex, iSectionIndex);
						m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].type = (Light_Type)iLight;
						pFile->setValue(ChannelIndexToName(iChannel), keyName, m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].enable[cd][iSection],true);
					}

					// 脉宽改成各通道中的每种光类型各一个
					char keyNamePluse[128] = { 0 };
					sprintf(keyNamePluse, "section%d_%d_cd%d_pulse", iScan_line + 1, iLight, cd + 1);
					pFile->setValue(ChannelIndexToName(iChannel), keyNamePluse, m_stIllumChannelCfg[iChannel][iScan_line].LightEnale[iLight].ipulse[cd], true);
				}
			}
		}
	}
	return true;
}

void lightcontrol_new::initLightTcp()
{
	IFileVariable *  pfile = getLightComFile();
	char lightStation[128] = { 0 };
	sprintf(lightStation, "LightIp_%d", m_index);
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		m_lightControlTcp[i] = NULL;		
		char lightName[128] = { 0 };
		char lightPort[128] = { 0 };
		char lightEnable[128] = { 0 }; 
		char enable = { 0 };
		
		sprintf(lightName, "light_%d", i);
		sprintf(lightPort, "port_%d", i);
		sprintf(lightEnable, "enable_%d", i);
		string strIP = pfile->getValue(lightStation, lightName).toCString();
		char* ip = (char*)strIP.c_str();
		uint16_t uPort = pfile->getValue(lightStation, lightPort).toUInt();
		bool bEnabled = pfile->getValue(lightStation, lightEnable).toBool();

		m_lightEnabled[i] = bEnabled;
		if (bEnabled)
		{
			LightControlTcp* pTcp = new LightControlTcp(ip, uPort);
			if (0 == pTcp->get_create_tcp_ok())  //连接成功才使用
			{
				m_lightControlTcp[i] = pTcp;
			}
			else {
				delete pTcp;
				pTcp = NULL;
			}
		}
	}
}


bool lightcontrol_new::ClearNumber()
{
	bool bres = true;
	for (int i = 0; i < LIGHT_SIZE; i++)
	{
		if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
		{
			uint8_t ls_config_id = 0;
			uint16_t  trig[8];
			bool res =  m_lightControlTcp[i]->clear_counter(0)==0?true:false;
			bres = res&&bres;
		}
	}
	return bres;
}


string lightcontrol_new::getSenceName()
{
    string strSceneName = "";
    IFileVariable *pFile = getIniLightEleFileVariable_new();
    if (NULL != pFile)
    {
        strSceneName = pFile->getValue(LIGHT_SCENE_NAME).toCString();
    }
    return strSceneName;
}


string lightcontrol_new::ChannelIndexToName(int index)
{
	string strChannel[Channel_Index_Size] = { "SEQUENCE_A","SEQUENCE_B","SEQUENCE_C","SEQUENCE_D","SEQUENCE_E","SEQUENCE_F","SEQUENCE_G","SEQUENCE_H" };
	if (index >= Channel_Index_Size || index <0)
	{
		return "";
	}
	return strChannel[index];
}

Channel_Index lightcontrol_new::ChannelNameToIndex(string strChannelName)
{
	string strChannel[Channel_Index_Size] = { "SEQUENCE_A","SEQUENCE_B","SEQUENCE_C","SEQUENCE_D","SEQUENCE_E","SEQUENCE_F","SEQUENCE_G","SEQUENCE_H" };
	for (int i = 0; i < 8; i++)
	{
		if (strChannelName == strChannel[i])
		{
			return (Channel_Index)i;
		}
	}
	return Channel_Index_A;
}

bool lightcontrol_new::setCurrentInitEleFile(const string& FileName)
{

	IFileVariableManager *pFileManager = (IFileVariableManager *)sdkcore::queryPriInterface("filevariable", IFileVariableManager::IID_FILEVARIABLEMANAGER);
	if (NULL == pFileManager)
	{
		return false;
	}
	m_strLightEleFileFileName = FileName;
	string path = getCurrentProgramPath() + "/" + m_strLightEleFileFileName;;
	m_pCurrentLightEleFile = pFileManager->getFileVariable(path);

	return true;
}

string lightcontrol_new::getCurrentInitEleFileName()
{
	return m_strLightEleFileFileName;
}


bool lightcontrol_new::getInitEleCfg(EleCfg *lightParam)
{
	if (lightParam != NULL)
	{
		*lightParam = m_currEleCfg;
	}	
	return true;
}


bool lightcontrol_new::setInitEleCfg(EleCfg *lightParam)
{
	if (lightParam != NULL)
	{
		m_currEleCfg = *lightParam;
	}
	saveEleConfig();
	return true;
}

bool lightcontrol_new::writeEleCfgToFlash(EleCfg *lightParam)
{
	if (lightParam != NULL)
	{
		m_currEleCfg = *lightParam;
	}
	
	return setLsCurrent() == 0;
}

bool lightcontrol_new::readEleCfgFromFlash(EleCfg *lightParam)
{
	if (lightParam != NULL)
	{

	}
	return true;
}

bool lightcontrol_new::getllumChannelCfg( Channel_Index index /*= Channel_Index_A*/, Scan_line_num line_index /*= Scan_line_1*/, stIllumChannelCfg *lightParam /*= NULL*/)
{
	if (index >= Channel_Index_Size || line_index >= Scan_line_Size)
	{
		return false;
	}
	*lightParam = m_stIllumChannelCfg[index][line_index];
	return true;
}

bool lightcontrol_new::setllumChannelCfg( Channel_Index index /*= Channel_Index_A*/, Scan_line_num line_index /*= Scan_line_1*/, stIllumChannelCfg *lightParam /*= NULL*/, string strSence)
{
	if ( index >= Channel_Index_Size || line_index >= Scan_line_Size)
	{
		return false;
	}
	m_stIllumChannelCfg[index][line_index] = *lightParam;

	string programName = getCurrentProgramName();

	"section1_0_cd1_1";
	"section1_1_cd1_1";
	"section1_2_cd1_1";
	"section1_3_cd1_1";
	"section1_pulse";
	"section1_carema_delay";
	"section1_scan_line";
	"section1_camera1_enable";

	IFileVariable *pFile = getSequenceSetCfgFileVariable_new(programName, SString::IntToStr(m_index));
	char sectionName[128] = { 0 };
	sprintf(sectionName, "section%d", line_index + 1);
	pFile->setValue(ChannelIndexToName(index), string(sectionName) + "_scan_line", m_stIllumChannelCfg[index][line_index].iscan_line_count, true);


	m_stIllumChannelCfg[index][line_index].ChannelEnable = false;
	m_stIllumChannelCfg[index][line_index].CamEnableIndex = -1;	
	for (int iCam = 0; iCam < Cam_Cnt; iCam++)
	{
		char CamName[128] = { 0 };
		sprintf(CamName, "section%d_camera%d_enable", line_index + 1, iCam + 1);
		pFile->setValue(ChannelIndexToName(index), CamName, m_stIllumChannelCfg[index][line_index].CamEnable[iCam], true);
		if (m_stIllumChannelCfg[index][line_index].CamEnable[iCam])
		{
			m_stIllumChannelCfg[index][line_index].ChannelEnable = true;
			m_stIllumChannelCfg[index][line_index].CamEnableIndex = iCam;
		}
	}

	for (int iLight = 0; iLight < LIGHT_SIZE; iLight++)
	{
		for (int cd = 0; cd < CD_Size; cd++)
		{
			for (int iSection = 0; iSection < Section_Size; iSection++)
			{
				char keyName[128] = { 0 };
				int icdIndex = cd + 1;
				int iSectionIndex = iSection + 1;
				sprintf(keyName, "section%d_%d_cd%d_%d", line_index + 1, iLight, icdIndex, iSectionIndex);
				m_stIllumChannelCfg[index][line_index].LightEnale[iLight].type = (Light_Type)iLight;
				pFile->setValue(ChannelIndexToName(index), keyName, m_stIllumChannelCfg[index][line_index].LightEnale[iLight].enable[cd][iSection], true);
			}

			// 脉宽改成各通道中的每种光类型各一个
			char keyNamePluse[128] = { 0 };
			sprintf(keyNamePluse, "section%d_%d_cd%d_pulse", line_index + 1, iLight, cd + 1);
			pFile->setValue(ChannelIndexToName(index), keyNamePluse, m_stIllumChannelCfg[index][line_index].LightEnale[iLight].ipulse[cd], true);

		}
	}
	int CamEnableIndex = m_stIllumChannelCfg[index][line_index].CamEnableIndex;
	bool ChannelEnable = m_stIllumChannelCfg[index][line_index].ChannelEnable  & camIsOnline(CamEnableIndex);
	pFile->setValue(ChannelIndexToName(index), PRO_COMERAINDEX, CamEnableIndex, true);
	pFile->setValue(ChannelIndexToName(index), ENABELCHANNLE, ChannelEnable, true);
	lightParam->ChannelEnable = ChannelEnable;
	lightParam->CamEnableIndex = CamEnableIndex;
	return true;
}

bool lightcontrol_new::writellumChannelCfgToFlash(Channel_Index index /*= Channel_Index_A*/, Scan_line_num line_index /*= Scan_line_1*/, stIllumChannelCfg *lightParam /*= NULL*/,Light_Type type)
{
	m_stIllumChannelCfg[index][line_index] = *lightParam;
	return setMutFlashParamsByChannel(index, type) == 0;
}

bool lightcontrol_new::readllumChannelCfgToFlash( Channel_Index index /*= Channel_Index_A*/, Scan_line_num line_index /*= Scan_line_1*/, stIllumChannelCfg *lightParam /*= NULL*/)
{
	return true;
}

int lightcontrol_new::getCtrType(uint8_t &ls_type, Light_Type type /*= LIGHT_SIZE*/)
{
	if (type==LIGHT_SIZE)
	{
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			return m_lightControlTcp[type]->get_ctr_type(ls_type);
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int lightcontrol_new::setFlashMode(uint8_t ls_model, Light_Type type /*= LIGHT_SIZE*/)
{
	int ret = 0;
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE;i++)
		{
			if (m_lightControlTcp[i] != NULL)
			{
				ret = m_lightControlTcp[i]->set_flash_mode(ls_model);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			ret = m_lightControlTcp[type]->set_flash_mode(ls_model);
		}
	}
	else
	{
		return -1;
	}
	return ret;
}

int lightcontrol_new::setLsSwitch(Light_Type type /*= LIGHT_SIZE*/)
{	
	int ret = 0;
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				LightSourceSwitch_t  ls_switch;

				#if SOURCE_LIGHT_TYPE
				ls_switch.lss_switch.ls_switch_cd1_1 = m_currEleCfg.alwaysOn[i][0];
				ls_switch.lss_switch.ls_switch_cd1_2 = m_currEleCfg.alwaysOn[i][0];
				ls_switch.lss_switch.ls_switch_cd1_3 = 0;
				ls_switch.lss_switch.ls_switch_cd1_4 = m_currEleCfg.alwaysOn[i][0];

				ls_switch.lss_switch.ls_switch_cd2_1 = m_currEleCfg.alwaysOn[i][1];
				ls_switch.lss_switch.ls_switch_cd2_2 = m_currEleCfg.alwaysOn[i][1];
				ls_switch.lss_switch.ls_switch_cd2_3 = 0;
				ls_switch.lss_switch.ls_switch_cd2_4 = m_currEleCfg.alwaysOn[i][1];

				ls_switch.lss_switch.ls_switch_cd3_1 = m_currEleCfg.alwaysOn[i][2];
				ls_switch.lss_switch.ls_switch_cd3_2 = m_currEleCfg.alwaysOn[i][2];
				ls_switch.lss_switch.ls_switch_cd3_3 = 0;
				ls_switch.lss_switch.ls_switch_cd3_4 = m_currEleCfg.alwaysOn[i][2];

				ls_switch.lss_switch.ls_switch_cd4_1 = m_currEleCfg.alwaysOn[i][3];
				ls_switch.lss_switch.ls_switch_cd4_2 = m_currEleCfg.alwaysOn[i][3];
				ls_switch.lss_switch.ls_switch_cd4_3 = 0;
				ls_switch.lss_switch.ls_switch_cd4_4 = m_currEleCfg.alwaysOn[i][3];

				ls_switch.lss_switch.ls_switch_cd5_1 = m_currEleCfg.alwaysOn[i][4];
				ls_switch.lss_switch.ls_switch_cd5_2 = m_currEleCfg.alwaysOn[i][4];
				ls_switch.lss_switch.ls_switch_cd5_3 = 0;
				ls_switch.lss_switch.ls_switch_cd5_4 = m_currEleCfg.alwaysOn[i][4];

				ls_switch.lss_switch.ls_switch_cd6_1 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.ls_switch_cd6_2 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.ls_switch_cd6_3 = 0;
				ls_switch.lss_switch.ls_switch_cd6_4 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.reserve = 0;
				#else
				ls_switch.lss_switch.ls_switch_cd1_1 = m_currEleCfg.alwaysOn[i][0];
				ls_switch.lss_switch.ls_switch_cd1_2 = m_currEleCfg.alwaysOn[i][0];
				ls_switch.lss_switch.ls_switch_cd1_3 = m_currEleCfg.alwaysOn[i][0];
				ls_switch.lss_switch.ls_switch_cd1_4 = 0;

				ls_switch.lss_switch.ls_switch_cd2_1 = m_currEleCfg.alwaysOn[i][1];
				ls_switch.lss_switch.ls_switch_cd2_2 = m_currEleCfg.alwaysOn[i][1];
				ls_switch.lss_switch.ls_switch_cd2_3 = m_currEleCfg.alwaysOn[i][1];
				ls_switch.lss_switch.ls_switch_cd2_4 = 0;

				ls_switch.lss_switch.ls_switch_cd3_1 = m_currEleCfg.alwaysOn[i][2];
				ls_switch.lss_switch.ls_switch_cd3_2 = m_currEleCfg.alwaysOn[i][2];
				ls_switch.lss_switch.ls_switch_cd3_3 = m_currEleCfg.alwaysOn[i][2];
				ls_switch.lss_switch.ls_switch_cd3_4 = 0;

				ls_switch.lss_switch.ls_switch_cd4_1 = m_currEleCfg.alwaysOn[i][3];
				ls_switch.lss_switch.ls_switch_cd4_2 = m_currEleCfg.alwaysOn[i][3];
				ls_switch.lss_switch.ls_switch_cd4_3 = m_currEleCfg.alwaysOn[i][3];
				ls_switch.lss_switch.ls_switch_cd4_4 = 0;

				ls_switch.lss_switch.ls_switch_cd5_1 = m_currEleCfg.alwaysOn[i][4];
				ls_switch.lss_switch.ls_switch_cd5_2 = m_currEleCfg.alwaysOn[i][4];
				ls_switch.lss_switch.ls_switch_cd5_3 = m_currEleCfg.alwaysOn[i][4];
				ls_switch.lss_switch.ls_switch_cd5_4 = 0;

				ls_switch.lss_switch.ls_switch_cd6_1 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.ls_switch_cd6_2 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.ls_switch_cd6_3 = m_currEleCfg.alwaysOn[i][5];
				ls_switch.lss_switch.ls_switch_cd6_4 = 0;
				ls_switch.lss_switch.reserve = 0;

			#endif
				
				ret = m_lightControlTcp[i]->set_ls_switch(ls_switch);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			LightSourceSwitch_t  ls_switch;
	
			#if SOURCE_LIGHT_TYPE
			ls_switch.lss_switch.ls_switch_cd1_1 = m_currEleCfg.alwaysOn[type][0];
			ls_switch.lss_switch.ls_switch_cd1_2 = m_currEleCfg.alwaysOn[type][0];
			ls_switch.lss_switch.ls_switch_cd1_3 = 0;
			ls_switch.lss_switch.ls_switch_cd1_4 = m_currEleCfg.alwaysOn[type][0];

			ls_switch.lss_switch.ls_switch_cd2_1 = m_currEleCfg.alwaysOn[type][1];
			ls_switch.lss_switch.ls_switch_cd2_2 = m_currEleCfg.alwaysOn[type][1];
			ls_switch.lss_switch.ls_switch_cd2_3 = 0;
			ls_switch.lss_switch.ls_switch_cd2_4 = m_currEleCfg.alwaysOn[type][1];

			ls_switch.lss_switch.ls_switch_cd3_1 = m_currEleCfg.alwaysOn[type][2];
			ls_switch.lss_switch.ls_switch_cd3_2 = m_currEleCfg.alwaysOn[type][2];
			ls_switch.lss_switch.ls_switch_cd3_3 = 0;
			ls_switch.lss_switch.ls_switch_cd3_4 = m_currEleCfg.alwaysOn[type][2];

			ls_switch.lss_switch.ls_switch_cd4_1 = m_currEleCfg.alwaysOn[type][3];
			ls_switch.lss_switch.ls_switch_cd4_2 = m_currEleCfg.alwaysOn[type][3];
			ls_switch.lss_switch.ls_switch_cd4_3 = 0;
			ls_switch.lss_switch.ls_switch_cd4_4 = m_currEleCfg.alwaysOn[type][3];

			ls_switch.lss_switch.ls_switch_cd5_1 = m_currEleCfg.alwaysOn[type][4];
			ls_switch.lss_switch.ls_switch_cd5_2 = m_currEleCfg.alwaysOn[type][4];
			ls_switch.lss_switch.ls_switch_cd5_3 = 0;
			ls_switch.lss_switch.ls_switch_cd5_4 = m_currEleCfg.alwaysOn[type][4];

			ls_switch.lss_switch.ls_switch_cd6_1 = m_currEleCfg.alwaysOn[type][5];
			ls_switch.lss_switch.ls_switch_cd6_2 = m_currEleCfg.alwaysOn[type][5];
			ls_switch.lss_switch.ls_switch_cd6_3 = 0;
			ls_switch.lss_switch.ls_switch_cd6_4 = m_currEleCfg.alwaysOn[type][5];
			#else
			ls_switch.lss_switch.ls_switch_cd1_1 = m_currEleCfg.alwaysOn[type][0];
			ls_switch.lss_switch.ls_switch_cd1_2 = m_currEleCfg.alwaysOn[type][0];
			ls_switch.lss_switch.ls_switch_cd1_3 = m_currEleCfg.alwaysOn[type][0];
			ls_switch.lss_switch.ls_switch_cd1_4 = 0;

			ls_switch.lss_switch.ls_switch_cd2_1 = m_currEleCfg.alwaysOn[type][1];
			ls_switch.lss_switch.ls_switch_cd2_2 = m_currEleCfg.alwaysOn[type][1];
			ls_switch.lss_switch.ls_switch_cd2_3 = m_currEleCfg.alwaysOn[type][1];
			ls_switch.lss_switch.ls_switch_cd2_4 = 0;

			ls_switch.lss_switch.ls_switch_cd3_1 = m_currEleCfg.alwaysOn[type][2];
			ls_switch.lss_switch.ls_switch_cd3_2 = m_currEleCfg.alwaysOn[type][2];
			ls_switch.lss_switch.ls_switch_cd3_3 = m_currEleCfg.alwaysOn[type][2];
			ls_switch.lss_switch.ls_switch_cd3_4 = 0;

			ls_switch.lss_switch.ls_switch_cd4_1 = m_currEleCfg.alwaysOn[type][3];
			ls_switch.lss_switch.ls_switch_cd4_2 = m_currEleCfg.alwaysOn[type][3];
			ls_switch.lss_switch.ls_switch_cd4_3 = m_currEleCfg.alwaysOn[type][3];
			ls_switch.lss_switch.ls_switch_cd4_4 = 0;

			ls_switch.lss_switch.ls_switch_cd5_1 = m_currEleCfg.alwaysOn[type][4];
			ls_switch.lss_switch.ls_switch_cd5_2 = m_currEleCfg.alwaysOn[type][4];
			ls_switch.lss_switch.ls_switch_cd5_3 = m_currEleCfg.alwaysOn[type][4];
			ls_switch.lss_switch.ls_switch_cd5_4 = 0;

			ls_switch.lss_switch.ls_switch_cd6_1 = m_currEleCfg.alwaysOn[type][5];
			ls_switch.lss_switch.ls_switch_cd6_2 = m_currEleCfg.alwaysOn[type][5];
			ls_switch.lss_switch.ls_switch_cd6_3 = m_currEleCfg.alwaysOn[type][5];
			ls_switch.lss_switch.ls_switch_cd6_4 = 0;
			#endif
			
			ret = m_lightControlTcp[type]->set_ls_switch(ls_switch);
		}
	}
	else
	{
		return -1;
	}
	return ret;
}

int lightcontrol_new::setAutoCharge(uint8_t ls_auto_charge, Light_Type type /*= LIGHT_SIZE*/)
{
	int iRet = 0;
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				m_lightControlTcp[i]->set_auto_charge(ls_auto_charge);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightEnabled[type] && m_lightControlTcp[type] != NULL)
		{
			iRet = m_lightControlTcp[type]->set_auto_charge(ls_auto_charge);
		}
	}
	else
	{
		iRet= -1;
	}
	return iRet;
}

int lightcontrol_new::clearCounter(Light_Type type /*= LIGHT_SIZE*/)
{
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				m_lightControlTcp[i]->clear_counter(0);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightEnabled[type] && m_lightControlTcp[type] != NULL)
		{
			return m_lightControlTcp[type]->clear_counter(0);
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int lightcontrol_new::getCounter( uint16_t trig[8], Light_Type type /*= LIGHT_SIZE*/)
{
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				m_lightControlTcp[i]->get_counter(0,trig);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >= 0)
	{
		if (m_lightEnabled[type] && m_lightControlTcp[type] != NULL)
		{
			return m_lightControlTcp[type]->get_counter(0,trig);
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int lightcontrol_new::setLsCurrent(Light_Type type /*= LIGHT_SIZE*/)
{
	int ret = 0;
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				uint16_t ls_current[6][4];
				for (int icd = 0; icd < CD_Size;icd++)
				{
					for (int iSec = 0; iSec < Section_Size;iSec++)
					{
						ls_current[icd][iSec] = m_currEleCfg.EleValue[i][icd][iSec]* m_currEleCfg.EleValue[i][icd][Section_Size] /100;
					}
				}	
				
				ret = m_lightControlTcp[i]->set_ls_current(ls_current);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			uint16_t ls_current[6][4];
			for (int icd = 0; icd < CD_Size; icd++)
			{
				for (int iSec = 0; iSec < Section_Size; iSec++)
				{
					ls_current[icd][iSec] = m_currEleCfg.EleValue[type][icd][iSec] * m_currEleCfg.EleValue[type][icd][Section_Size] / 100;
				}
			}
			ret = m_lightControlTcp[type]->set_ls_current(ls_current);
		}
	}
	else
	{
		return -1;
	}
	return ret;
}

int lightcontrol_new::setMutFlashParamsByChannel( uint8_t channel, Light_Type type /*= LIGHT_SIZE*/)
{
	int iRet = 0;
	int ls_config_id = 0;
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				MutFlashParams mfp[5];
				for (int j = 0; j < 5;j++)
				{
					#if SOURCE_LIGHT_TYPE
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][3];
					#else
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[0][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[1][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[2][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[3][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[4][3];

					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][0];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][1];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][2];
					mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[channel][j].LightEnale[i].enable[5][3];
					#endif

					mfp[j].ls_carema_config.carema_config.ct_carema_1 = m_stIllumChannelCfg[channel][j].CamEnable[0];
					mfp[j].ls_carema_config.carema_config.ct_carema_2 = m_stIllumChannelCfg[channel][j].CamEnable[1];;
					mfp[j].ls_carema_config.carema_config.ct_carema_3 = m_stIllumChannelCfg[channel][j].CamEnable[2];;
					mfp[j].ls_carema_config.carema_config.ct_carema_4 = m_stIllumChannelCfg[channel][j].CamEnable[3];;
					memcpy(mfp[j].ls_pulse,m_stIllumChannelCfg[channel][j].LightEnale[i].ipulse,sizeof(mfp[j].ls_pulse));
					//mfp[j].ls_carema_delay = m_stIllumChannelCfg[ls_config_id][channel][j].icarema_delay;
					mfp[j].ls_scan_line_count = m_stIllumChannelCfg[channel][j].iscan_line_count;;
				}
					
				iRet = m_lightControlTcp[i]->set_MutFlashParams_by_channel(ls_config_id, channel, mfp);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{	
		if (m_lightControlTcp[type] != NULL)
		{
			MutFlashParams mfp[5];
			for (int j = 0; j < 5; j++)
			{
				#if SOURCE_LIGHT_TYPE
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][3];
				#else
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[0][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[1][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[2][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[3][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[4][3];

				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][0];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][1];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][2];
				mfp[j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[channel][j].LightEnale[type].enable[5][3];
				#endif
				mfp[j].ls_carema_config.carema_config.ct_carema_1 = m_stIllumChannelCfg[channel][j].CamEnable[0];
				mfp[j].ls_carema_config.carema_config.ct_carema_2 = m_stIllumChannelCfg[channel][j].CamEnable[1];
				mfp[j].ls_carema_config.carema_config.ct_carema_3 = m_stIllumChannelCfg[channel][j].CamEnable[2];
				mfp[j].ls_carema_config.carema_config.ct_carema_4 = m_stIllumChannelCfg[channel][j].CamEnable[3];	
				memcpy(mfp[j].ls_pulse, m_stIllumChannelCfg[channel][j].LightEnale[type].ipulse, sizeof(mfp[j].ls_pulse));
				mfp[j].ls_scan_line_count = m_stIllumChannelCfg[channel][j].iscan_line_count;;
			}
			iRet =  m_lightControlTcp[type]->set_MutFlashParams_by_channel(0, channel, mfp);
		}
	}
	else
	{
		iRet  = -1;
	}
	return iRet;
}

int lightcontrol_new::setMutFlashParams(Light_Type type /*= LIGHT_SIZE*/)
{
	int iRet = 0;
	if ( type > LIGHT_SIZE)
	{
		return -1;
	}
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				MutFlashParams mfp[8][5];
				for (int iChannel = 0; iChannel < Channel_Index_Size;iChannel++)
				{
					for (int j = 0; j < 5; j++)
					{
						#if SOURCE_LIGHT_TYPE
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][3];
						#else
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[0][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[1][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[2][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[3][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[4][3];

						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][0];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][1];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][2];
						mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[i].enable[5][3];
						#endif
						mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_1 = m_stIllumChannelCfg[iChannel][j].CamEnable[0];
						mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_2 = m_stIllumChannelCfg[iChannel][j].CamEnable[1];;
						mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_3 = m_stIllumChannelCfg[iChannel][j].CamEnable[2];;
						mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_4 = m_stIllumChannelCfg[iChannel][j].CamEnable[3];;
						//mfp[iChannel][j].ls_pulse = m_stIllumChannelCfg[ls_config_id][iChannel][j].ipulse;
						memcpy(mfp[iChannel][j].ls_pulse, m_stIllumChannelCfg[iChannel][j].LightEnale[i].ipulse, sizeof(mfp[iChannel][j].ls_pulse));
						//mfp[iChannel][j].ls_carema_delay = m_stIllumChannelCfg[ls_config_id][iChannel][j].icarema_delay;
						mfp[iChannel][j].ls_scan_line_count = m_stIllumChannelCfg[iChannel][j].iscan_line_count;;
					}
				}
				m_lightControlTcp[i]->set_MutFlashParams(0, mfp);
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			MutFlashParams mfp[8][5];
			for (int iChannel = 0; iChannel < Channel_Index_Size; iChannel++)
			{
				for (int j = 0; j < 5; j++)
				{
					#if SOURCE_LIGHT_TYPE
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][3];
					#else
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd1_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[0][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd2_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[1][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd3_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[2][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd4_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[3][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd5_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[4][3];

					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_1 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][0];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_2 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][1];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_3 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][2];
					mfp[iChannel][j].ls_switch_cofnig.lss_switch.ls_switch_cd6_4 = m_stIllumChannelCfg[iChannel][j].LightEnale[type].enable[5][3];
					#endif
					mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_1 = m_stIllumChannelCfg[iChannel][j].CamEnable[0];
					mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_2 = m_stIllumChannelCfg[iChannel][j].CamEnable[1];
					mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_3 = m_stIllumChannelCfg[iChannel][j].CamEnable[2];
					mfp[iChannel][j].ls_carema_config.carema_config.ct_carema_4 = m_stIllumChannelCfg[iChannel][j].CamEnable[3];
					memcpy(mfp[iChannel][j].ls_pulse, m_stIllumChannelCfg[iChannel][j].LightEnale[type].ipulse, sizeof(mfp[iChannel][j].ls_pulse));
					//mfp[iChannel][j].ls_pulse = m_stIllumChannelCfg[ls_config_id][iChannel][j].ipulse;
					//mfp[iChannel][j].ls_carema_delay = m_stIllumChannelCfg[ls_config_id][iChannel][j].icarema_delay;
					mfp[iChannel][j].ls_scan_line_count = m_stIllumChannelCfg[iChannel][j].iscan_line_count;;
				}
			}
			iRet =  m_lightControlTcp[type]->set_MutFlashParams(0, mfp);
		}
	}
	else
	{
		iRet =  -1;
	}
	return iRet;
}

int lightcontrol_new::setAllMutFlashParams(Light_Type type /*= LIGHT_SIZE*/)
{
	int iRet = 0;
	int ls_config_id = 0;
	if (type > LIGHT_SIZE)
	{
		return -1;
	}
	if (type == LIGHT_SIZE)
	{
		for (int i = 0; i < LIGHT_SIZE; i++)
		{
			if (m_lightEnabled[i] && m_lightControlTcp[i] != NULL)
			{
				for (int iChannel = 0; iChannel < Channel_Index_Size; iChannel++)
				{
					setMutFlashParamsByChannel( iChannel, (Light_Type)i);
					Sleep(300);
				}
			}
		}
	}
	else if (type < LIGHT_SIZE && type >=0)
	{
		if (m_lightControlTcp[type] != NULL)
		{
			ls_config_id = 0;

			for (int iChannel = 0; iChannel < Channel_Index_Size; iChannel++)
			{
				setMutFlashParamsByChannel( iChannel, type);
				Sleep(100);
			}
		}			 
	}
	else
	{
		return -1;
	}
	return iRet;
}

lightcontrol_new_manager* lightcontrol_new_manager::m_pManagerInstance = NULL;

lightcontrol_new_manager* lightcontrol_new_manager::GetInstance()
{
	if (m_pManagerInstance == NULL)
	{
		m_pManagerInstance = new lightcontrol_new_manager();
	}
	return m_pManagerInstance;

}

void lightcontrol_new_manager::ReleaseInstance()
{
	if (m_pManagerInstance != NULL)
	{
		delete m_pManagerInstance;
		m_pManagerInstance = NULL;
	}
}

lightcontrol_new * lightcontrol_new_manager::getLightcontrol(int index)
{
	if(index >=0 && index < sizeof(lightcontrols) / sizeof(lightcontrol_new*))
	{ 
		if (lightcontrols[index] == NULL)
		{
			lightcontrols[index] = new lightcontrol_new(index);
		}
		return lightcontrols[index];
	}
	return NULL;
}

void lightcontrol_new_manager::releaseLightcontrol(int index)
{
	if (index >= 0 && index < sizeof(lightcontrols) / sizeof(lightcontrol_new*))
	{
		if (lightcontrols[index] != NULL)
		{
			delete lightcontrols[index];
			lightcontrols[index] = NULL;
		}
	}
}

lightcontrol_new_manager::lightcontrol_new_manager()
{
	for ( int i = 0; i < sizeof(lightcontrols)/sizeof(lightcontrol_new*); i++)
	{
		lightcontrols[i] = NULL;
	}
}

lightcontrol_new_manager::~lightcontrol_new_manager()
{
	for (int i = 0; i < sizeof(lightcontrols) / sizeof(lightcontrol_new*); i++)
	{
		if (lightcontrols[i] != NULL)
		{
			delete lightcontrols[i];
			lightcontrols[i] = NULL;
		}
	}
}


motioncontrol_new_manager* motioncontrol_new_manager::m_pManagerInstance = NULL;
motioncontrol_new_manager * motioncontrol_new_manager::GetInstance()
{
	if (m_pManagerInstance == NULL)
	{
		m_pManagerInstance = new motioncontrol_new_manager();
	}
	return m_pManagerInstance;

}

void motioncontrol_new_manager::ReleaseInstance()
{
	if (m_pManagerInstance != NULL)
	{
		delete m_pManagerInstance;
		m_pManagerInstance = NULL;
	}
}

motioncontrol_new * motioncontrol_new_manager::getMotioncontrol(int index)
{
	if (index >= 0 && index < sizeof(motioncontrols) / sizeof(motioncontrol_new*))
	{
		if (motioncontrols[index] == NULL)
		{
			motioncontrols[index] = new motioncontrol_new(index);
		}
		return motioncontrols[index];
	}
	return NULL;
}

void motioncontrol_new_manager::releaseMotioncontrol(int index)
{
	if (index >= 0 && index < sizeof(motioncontrols) / sizeof(motioncontrol_new*))
	{
		if (motioncontrols[index] != NULL)
		{
			delete motioncontrols[index];
			motioncontrols[index] = NULL;
		}
	}
}

motioncontrol_new_manager::motioncontrol_new_manager()
{
	for (int i = 0; i < sizeof(motioncontrols) / sizeof(motioncontrol_new*); i++)
	{
		motioncontrols[i] = NULL;
	}
}

motioncontrol_new_manager::~motioncontrol_new_manager()
{
	for (int i = 0; i < sizeof(motioncontrols) / sizeof(motioncontrol_new*); i++)
	{
		if (motioncontrols[i] != NULL)
		{
			delete motioncontrols[i];
			motioncontrols[i] = NULL;
		}
	}
}

motioncontrol_new::motioncontrol_new(int index)
{
	m_index = index;
	m_Control = NULL;
	m_bEnalbe = false;

	IFileVariable *  pfile = getLightComFile();

	char ip[128] = { 0 };
	char port[128] = { 0 };
	char enable[128] = { 0 };

	sprintf(ip, "motionctl_ip_%d", m_index);
	sprintf(port, "motionctl_port_%d", m_index);
	sprintf(enable, "motionctl_enable_%d", m_index);

	string strIP = pfile->getValue("motioncontrol", ip).toCString();
	uint16_t uPort = pfile->getValue("motioncontrol", port).toUInt();
	m_bEnalbe = pfile->getValue("motioncontrol", enable).toBool();

	LightControlTcp* pTcp = new LightControlTcp((char*)strIP.c_str(), uPort);
	if (0 == pTcp->get_create_tcp_ok())  //连接成功才使用
	{
		m_Control = pTcp;
	}
	else 
	{
		delete pTcp;
		pTcp = NULL;
	}
}

int motioncontrol_new::setToController(ControllerParams_t Params)
{
	int ires = -1;
	if (Params.net_id > 4 || Params.net_id < 1 || m_Control == NULL)
	{
		return ires;
	}

	return m_Control->set_ControllerParams(Params);
}

int motioncontrol_new::setOrderToController(uint8_t NetId, Order_num Order, uint8_t CrlNumber)
{
	int ires = -1;
	if (NetId > 4 || NetId < 1 || m_Control == NULL)
	{
		return ires;
	}

	if (Order == Order_GetCrl)
	{

		ires = m_Control->get_Controller_type(NetId);

	}
	else if (Order == Order_WorkType || Order == Order_WorkStus)
	{

		ires = m_Control->set_Order_To_Controller(NetId, Order, CrlNumber);

	}

	return ires;
}

int motioncontrol_new::getControllerTargetStatus(uint8_t NetId, Mark_Type stType)
{
	int ires = -1;
	if (NetId > 4 || NetId < 1 || m_Control == NULL)
	{
		return ires;
	}
	return m_Control->get_Controller_TargetStatus(NetId, stType);
}

bool motioncontrol_new::isEnable()
{
	return m_bEnalbe;
}
