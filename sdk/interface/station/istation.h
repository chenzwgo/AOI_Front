/**
 * 文件说明: 包含axis的专用接口
 * 作者: 
 * 日期:
 * 修改日志:              
 */ 
#ifndef _I_STATION_H_
#define _I_STATION_H_

#include "motor_common.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

class IStation
{
public:
    virtual ~IStation(){}

	/************************************************************************/
	/******************************初始化************************************/
	/************************************************************************/
	// @brief 初始化工站
	// @param pStation					-- [in ]工站信息
	// @return EM_ERR_CONTROL                                     
	/************************************************************************/ 
	virtual ERROR_CODE Init(StStationInfo* pStation) = 0;

	/************************************************************************/
	// @brief 释放一个工站
	// @return EM_ERR_CONTROL                                          
	/************************************************************************/
	virtual ERROR_CODE DeInit() = 0;

	/************************************************************************/
	// @brief 复位工站，注意：不是回原
	// 暂停后调用可清除暂停状态
	// @return EM_ERR_CONTROL                                          
	/************************************************************************/
	virtual ERROR_CODE Reset() = 0;

    /************************************************************************/
    // @brief 获取工站信息
    // @return StStationInfo                                         
    /************************************************************************/
    virtual StStationInfo GetStationInfo() = 0;

	/*******************************启动-暂停-停止***************************/
	/************************************************************************/
	// @brief 停止工站
	// @param iStopType					-- [in]停止方式（0 平滑/1 急停)
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE Stop(EnStopType iStopType = STOP_SMOOTH) = 0;

	/************************************************************************/
	// @brief 暂停工站
	// @param 
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE Pause() = 0;
	/************************************************************************/
	// @brief 继续工站
	// @param 
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE Continue() = 0;

	/************************************************************************/
	// @brief //获取工站状态
	// @return EnStationStatus -1 未初始化 ,0 就绪, 1运动中, 2,跑点完成，3, 跑点失败
	/************************************************************************/
	virtual EnStationStatus GetStatus()= 0;


	/***************************速度设置*************************************/
	/************************************************************************/
	// @brief 设置工站整体速度(工站下所有轴都使用该速度)
	// @param vel					-- [double]速度
	// @param acc					-- [double]加速度
	// @param dec					-- [double]减速度
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE SetGlobalSpeed(double vel, double acc, double dec) = 0;

	/************************************************************************/
	// @brief 设置工站速度百分比(以卡轴配置里的轴速度为基准)
	// @param velRatio		-- [in]速度百分比 取值范围0-100, 即:1%-100%
	// @param accRatio		-- [in]加速度百分比 取值范围0-100, 即:1%-100%
	//                             为0表示与速度百分比一致
	// @param decRatio		-- [in]减速度百分比 取值范围0-100, 即:1%-100%
	//                             为0表示与速度百分比一致
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE SetRatioSpeed(int velRatio=10, int accRatio=0, int decRatio=0) = 0;

	/************************************************************************/
	// @brief 获取工站速度百分比(以卡轴配置里的速度为基准)
	// @param velRatio		-- [in]速度百分比 取值范围0-100, 即:1%-100%
	// @param accRatio		-- [in]加速度百分比 取值范围0-100, 即:1%-100%
	// @param decRatio		-- [in]减速度百分比 取值范围0-100, 即:1%-100%
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE GetRatioSpeed(int &velRatio, int &accRatio, int &decRatio) = 0;

	/*******************************回原*************************************/
	/************************************************************************/
	// @brief  工站回原,不阻塞，调用HomeStatus查回原状态
	// @param  priorityX-priorityW,[-1,5],6个轴的优先级,-1不回原，0优先级最低,5优先级最高，级别相同则同时回原
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE Home(int priorityX,int priorityY=0,int priorityZ=0,int priorityU=0,int priorityV=0,int priorityW=0) = 0;

    /************************************************************************/
    // @brief  工站回原,不阻塞，调用HomeStatus查回原状态
    // @param  使用工站设置好的优先级回原
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE Home() = 0;

	/************************************************************************/
	// @brief  判读工站是否回原完成
	// @return EnMoveBool, 0回原失败，1成功 2进行中                                                             
	/************************************************************************/
	virtual EnMoveBool HomeStatus() = 0;
	
    /************************************************************************/
    // @brief 清零工站位置
    // @return ERROR_CODE
    /************************************************************************/
    virtual ERROR_CODE ResetStationPos() = 0;

	/*****************************跑点运动***********************************/
	/************************************************************************/
	// @brief 移动到位置
	// @param stPos					-- [in]位置信息
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MovePos( StPos &stPos ) = 0;

    /************************************************************************/
    // @brief 移动相对位置
    // @param stPos					-- [in]位置信息
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MovePosRel( StPos &stPos ) = 0;

	/************************************************************************/
	// @brief 移动到多个位置
	// @param stPos[]				-- [in]位置信息数组
	// @param iPosCnt				-- [in]位置个数
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MovePoses( StPos stPos[] , int iPosCnt ) = 0;

	/************************************************************************/
	// @brief PTP运动
	// @param iPtIndex					-- [in]点索引
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MovePoint(int iPtIndex) = 0;

    /************************************************************************/
    // @brief PTP运动,跑相对点,即 终点位置=当前位置+点iPtindex
    // @param iPtIndex					-- [in]点索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MovePointRel(int iPtIndex) = 0;

    /************************************************************************/
    // @brief PTP运动,跑点的偏移量,即 终点位置=点iPtindex+stPos
    //        多用与跑到某点的上方位置
    // @param iPtIndex					-- [in]点索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MovePointOffset(int iPtIndex, StPos &stPos) = 0;

    /************************************************************************/
    // @brief PTP运动,跑点的偏移量,即 终点位置=点iPtindex+点iPtIndexOffset
    //        多用与跑到某点的上方位置
    // @param iPtIndex					-- [in]点索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MovePointOffset(int iPtIndex, int iPtIndexOffset) = 0;

	/************************************************************************/
	// @brief PTP运动，连续几个点运动
	// @param pIndex 连续移动点列表，iPtCnt 连续移动点的个数	
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MovePoints(int iPtIndex[] , int iPtCnt)= 0;

	/************************************************************************/
	// @brief 判断工站是否移动到指定位置
	// @param stPos					    -- [in]点坐标
	// @param dOffset					-- [in]误差范围
	// @return EnMoveBool, 0运动失败，1成功 2进行中
	/************************************************************************/
	virtual EnMoveBool MoveSucceed(const StPos &stPos, double dOffset = 0.01) = 0;

	/************************************************************************/
	// @brief判断工站是否移动到指定位置
	// @param iPtIndex					-- [in]点索引
	// @param dOffset					-- [in]误差范围
	// @return EnMoveBool, 0运动失败，1成功 2进行中
	/************************************************************************/
	virtual EnMoveBool MoveSucceed(int iPtIndex, double dOffset = 0.01) = 0;

	/********************************* 插补运动 *****************************/

	/************************************************************************/
	// @brief 插补合成速度设置
	// @param StSpeed				-- [in]插补段的合成速度
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE SetInterpSpeed(const StSpeed &speed) = 0;


	/************************************************************************/
	// @brief 直线插补运动
	// @param iPtIndex					-- [in]点索引
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MoveLine(int iPtIndex) = 0;

    /************************************************************************/
    // @brief 直线插补运动
    // @param pos					-- [in]点坐标
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MoveLine( StPos &pos ) = 0;

    /************************************************************************/
    // @brief 直线插补运动+偏移量
    // @param pos					-- [in]偏移位置
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MoveLineOffset(int iPtIndex, StPos pos) = 0;

    /************************************************************************/
    // @brief PTP直线插补运动,跑相对点,即 终点位置=当前位置+点iPtindex
    // @param iPtIndex					-- [in]点索引
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MoveLineRel(int iPtIndex) = 0;

	/************************************************************************/
	// @brief 圆弧插补运动
	// @param iPtIndexA					-- [in]起始点
	// @param iPtIndexB					-- [in]中间点
	// @param iPtIndexC					-- [in]终点
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE MoveArc(int iPtIndexA, int iPtIndexB, int iPtIndexC) = 0;

	/************************************************************************/
	// @brief 设置插补坐标系轴，默认为工站轴数量，可以通过该接口更改坐标系，只需设置一次
	// @param axis						-- [in]坐标系维号， x-0, y-1,z-2,u-3
	// @param coordcnt					-- [in]坐标系维数，最小2维，最大4维，且不能大于工站轴数量
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE SetCoordAxis(int icoordaxis[], int cnt) = 0;

	/************************************************************************/
	// @brief 初始化插补运动，如果要更改坐标系轴号，必须先调用SetCoordAxi()更改后再初始化
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE InitContInterp() = 0;

	/************************************************************************/
	// @brief 连续插补运动,加线段
	// @param pIndex					-- [in]点索引数组
	// @param iPtCnt					-- [in]点数量
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE BufMoveLine(int iPtIndex[], int iPtCnt) = 0;

    /************************************************************************/
    // @brief 连续插补运动,加线段
    // @param stPosArray				-- [in]点数组
    // @param iPtCnt					-- [in]点数量
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE BufMoveLine( StPos stPosArray[], int iPtCnt) = 0;

	/************************************************************************/
	// @brief 连续插补运动,加圆弧
	// @param iPtIndexA					-- [in]起始点
	// @param iPtIndexB					-- [in]中间点
	// @param iPtIndexC					-- [in]终点
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE BufMoveArc( int iPtIndexA, int iPtIndexB, int iPtIndexC ) = 0;

    /************************************************************************/
    // @brief 连续插补运动,加圆弧
    // @param posA					-- [in]起始点
    // @param posB					-- [in]中间点
    // @param posC					-- [in]终点
    // @return EM_ERR_CONTROL
    /************************************************************************/
    virtual ERROR_CODE MoveArc(const StPos &posA , const StPos &posB , const StPos &posC) = 0;

	/************************************************************************/
	// @brief 圆弧插补运动(以终点和半径)
	// @param iPtIndexA					-- [in]起始点
	// @param iPtIndexB					-- [in]终点
	// @param dRadius						-- [in]半径
	// @param iDir						-- [in]运动方向
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE BufMoveArcR(int iPtIndexA, int iPtIndexB, double dRadius, int iDir) = 0;

	/************************************************************************/
	// @brief 连续插补运动，增加IO
	// @param ioName				-- [in]IO名称
	// @param bOpen					-- [in]开关
	// @param delay，单位毫秒       -- [in]延后打开IO的时间
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE BufOutput(const char *ioName, bool bOpen=true, int delay=0) = 0;

	/************************************************************************/
	// @brief 连续插补运动，增加延时
	// @param time，单位毫秒		-- [in]延时时间
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE BufDelay(int time) = 0;

	/************************************************************************/
	// @brief 连续插补运动,启动
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE StartContInterp() = 0;

	/***********************************************************************/
	/******************************单轴控制*********************************/
    /************************************************************************/
    // @brief 获取轴参数
    // @param dimension				-- [in]工站维度，如0代表X维,1代表Y维
    // @return StAxisParam                                                            
    /************************************************************************/
    virtual StAxisParam GetAxisInfo(int dimension) = 0;

	/************************************************************************/
	// @brief 单轴回原
	// @param dimension				-- [in]工站维度，如0代表X维,1代表Y维
	// @return EM_ERR_CONTROL                                                            
	/************************************************************************/
	virtual ERROR_CODE AxisHome(int dimension) = 0;

	/************************************************************************/
	//@brief 等待轴回原完成
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @return EnMoveBool, 0回原失败，1成功 2进行中
	/************************************************************************/
	virtual EnMoveBool AxisHomeFinished(int dimension) = 0;

	/************************************************************************/
	// @brief 轴设置速度
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param speed					-- [in]轴速度
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE AxisSetSpeed(int dimension, const StSpeed &speed) = 0;

    /************************************************************************/
	// @brief 轴移动到指定位置,PTP运动
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param dPos					-- [in]轴位置
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE AxisMovePos(int dimension, double dpos) = 0;

	/************************************************************************/
	// @brief 轴在当前的位置进行偏移，PTP运动
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param dOffset				-- [in]偏移量
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE AxisMoveOffset(int dimension, double dOffset) = 0;

	/************************************************************************/
	// @brief 指定轴连续运动
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param bDirection			-- [in]ture正向移动，false负向移动
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE AxisMoveJog(int dimension, bool bDirection = true) = 0;

	/************************************************************************/
	// @brief 停止轴
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param iStopType				-- [in]停止方式（平滑/急停)
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE StopAxis(int dimension, EnStopType iStopType = STOP_SMOOTH) = 0;

    /************************************************************************/
	// @brief 清除轴状态，用于轴工站
	// @param iStationID			-- [in]工站ID
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE ClearAxisStatus(int dimension) = 0;

	/************************************************************************/
	// @brief 清除位置信息，用于轴工站
	// @param iStationID				-- [in]工站ID
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE ResetAxisPos(int dimension) = 0;

	/************************************************************************/
	// @brief 获取轴工站某一轴正限位状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isLimitP				    -- [in]正限位状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE IsAxisLimitP(int dimension, bool &isLimitP) = 0;

	/************************************************************************/
	// @brief 获取轴工站某一轴负限位状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isLimitN				-- [in]负限位状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE IsAxisLimitN(int dimension, bool &isLimitN) = 0;

	/************************************************************************/
	// @brief 获取轴工站某一轴报警状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isDrvAlarm			-- [in]报警状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE IsAxisDrvAlarm(int dimension, bool &isDrvAlarm) = 0;

	/************************************************************************/
	// @brief 获取轴工站某一轴使能状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isOn				    -- [in]使能状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE IsAxisEnabled(int dimension, bool &isEnabled) = 0;

    /************************************************************************/
    // @brief 获取轴工站某一轴运动状态
    // @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
    // @param isMoving				-- [in]移动状态
    // @return ERROR_CODE
    /************************************************************************/
    virtual ERROR_CODE IsAxisMoving(int dimension, bool &isMoving) = 0;

	/************************************************************************/
	// @brief 获取轴工站某一轴使能状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isHome				-- [in]原点状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE IsAxisHomeSig(int dimension, bool &isHome) = 0;

	/************************************************************************/
	// @brief 设置轴工站某一轴使能状态
	// @param dimension				-- [in]工站维度,如0代表X维,1代表Y维
	// @param isOn				    -- [in]使能状态
	// @return ERROR_CODE
	/************************************************************************/
	virtual ERROR_CODE EnableAxis(int dimension, bool bEnable) = 0;


	
	/******************************* 点位配置 *******************************/

	/************************************************************************/
	// @brief 设置点位保存文件夹，文件夹不存在，自动创建，并创建空点位文件
	// @param dir				-- [in]点位保存文件夹
	// @return 
	/************************************************************************/
	virtual ERROR_CODE SetPointDir(const string &dir) = 0;

	/************************************************************************/
	// @brief 获取当前位置
	// @return 
	/************************************************************************/
	virtual ERROR_CODE GetCurrentPos(StPos &pos) = 0;

	/************************************************************************/
	// @brief 获取点位位置
	// @return 
	/************************************************************************/
	virtual ERROR_CODE GetPoint(int iPtIndex, StPoint &pt) = 0;

    /************************************************************************/
    // @brief 获取点位位置
    // @return 
    /************************************************************************/
    virtual ERROR_CODE GetPoint(const string &pointName, StPoint &pt) = 0;

	/************************************************************************/
	// @brief 示教点,只保存pPt信息，不会获取当前点位
	// @param pPt						-- [in]点信息
	// @param bWriteFile				-- [in]是否写入点文件
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE TeachPoint(const StPoint& pPt, bool bWriteFile = true) = 0;

	/************************************************************************/
	// @brief 删除指定点
	// @param iPtIndex					-- [in]点索引
	// @return EM_ERR_CONTROL
	/************************************************************************/
	virtual ERROR_CODE DeletePoint(int iPtIndex) = 0;

	/************************************************************************/
	// @brief 获取工站所有点信息
	// @param StPoint*					-- [in]点信息数组
	// @return 所有点信息
	/************************************************************************/
	virtual const map<int,StPoint>* GetAllPoint() = 0;

	/************************************************************************/
	// @brief 根据点索引获取工站点名称
	// @param string					-- [in]点索引
	// @return 点名称
	/************************************************************************/
	virtual string GetPointName(int pointIndex) = 0;

	/************************************************************************/
	// @brief 根据点名称获取工站点索引
	// @param string					-- [in]点名称
	// @return 点索引
	/************************************************************************/
	virtual int GetPointIndex(const string &pointName) = 0;
};

#endif
