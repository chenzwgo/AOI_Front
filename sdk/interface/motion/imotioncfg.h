#ifndef _IMOTIONCFG_H_
#define _IMOTIONCFG_H_

#include "motor_common.h"

#include <string>
#include <map>
#include <vector>

struct CardMsg			//读取卡的配置项信息，填充一个卡信息和这个卡的所有轴信息
{
	StCardInfo   pCard;

	map<string,StAxisParam> mapAxis;
	map<string,StIOConfig>  mapIO;
} ;

class iMotionCfg
{
	
public:
		enum
		{
			IID_MOTION_CONFIG = 0x66
		};

	    virtual ~iMotionCfg(){}
		//
		virtual int Init() = 0;
		/////station//
		virtual std::vector<StStationInfo>* const getStationsInfo()  = 0;
		virtual int AddStation( const StStationInfo& pStation ) = 0;
		virtual int RemoveStation( const char* pStationName ) = 0;
		virtual int ReplaceStation( const char* pStationName,const StStationInfo& newStationInfo ) = 0;
		virtual int QueryStation( const char* name ,StStationInfo& pStation) = 0;
		virtual int IsStationExist(const char* name) = 0;
		virtual int getStationInfo(const char* pName,StStationInfo& config) = 0;
		virtual void GetAllStationName(vector<string>& stationsName) = 0;

		//card
		virtual int GetAllCard(map<string,StCardInfo> &map_Card) = 0;
		virtual int DeleCard(char* cardName) = 0;
		virtual int AddCard(StCardInfo* pCard) = 0;
		virtual int ModifyCard(char* oldCardName,StCardInfo* pCard) = 0;
		virtual int getCardInfo(const char* cardName,StCardInfo& config) =0;
		virtual void GetAllCardName(vector<string>& cardsName) =0;
		
		//axis
		virtual int GetAllAxis(map<string,StAxisParam> &map_Axis) = 0;
		virtual int DeleAxis(char* AxisName) = 0;
		virtual int AddAxis(StAxisParam *pAxis) = 0;
                virtual int ModifyAxis(const char* oldAxisName,StAxisParam* pAxisParam) = 0;
		virtual int getAxisInfo(const char* AxisName,StAxisParam& config) = 0;
		virtual void GetALLAxisName(vector<string>& AxisesName) = 0;
		virtual void GetAxisNameByCard(const char* cardName,vector<string>& AxisName) = 0;

		//io
		virtual int GetAllIOmsg(map<string,StIOConfig> &map_IOmsg) = 0;
		virtual int DeleIOmsg(char* ioName) = 0;
		virtual int AddIOmsg(StIOConfig *pIO) = 0;
                virtual int ModifyIOmsg(const char* oldIOName,StIOConfig* pIOconfig) = 0;
		virtual int getIOInfo(const char* pIO,StIOConfig& config) = 0;
		virtual void GetAllIOName(vector<string>& IOName) = 0;
        virtual void GetIONameByCard(const char* cardName,vector<string>& IOName) = 0;

        //add by huanghucheng  添加接口
        virtual int DeleteIOLists(const list<string>& delNames) = 0;
        virtual int AddIOLists(const list<StIOConfig>& ioconfigs) = 0;

};
#endif  //防止重复包含
