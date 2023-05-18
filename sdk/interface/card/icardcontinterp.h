#ifndef _CONT_INTERP_INTERFACE_H_
#define _CONT_INTERP_INTERFACE_H_

#include "motor_common.h"
typedef struct
{
	short encx;
	short ency;
	short source;
	short outputType;
	short startLevel;
	short time;
	short maxerr;
	short threshold;
} CardGGT2DComparePrm;

typedef struct
{
	long px;
	long py;
} CardGGT2DCompareData;
class ICardContInterp 
{
public:
	virtual ~ICardContInterp(){}
	 /**
	 * @brief 初始化插补,可以多次调用。首次调用建立坐标系，后续用于更新坐标系参数
	 * @param axis[]					-- [in]插补轴列表
	 * @param count      				-- [in]插补轴数目
	 * @return 成功返回ture，失败返回false
	 */
    virtual bool InitContinueInter(int axis[], int count ,int &hContinueInterHandle) = 0;

	 /**
	 * @brief 设置坐标系偏移。先建立坐标系，再设置坐标系偏移，再重新调用InitContinueInter()更新参数
	 * @param axis[]					-- [in]插补轴列表
	 * @param count      				-- [in]插补轴数目
	 * @param dOffset      			-- [doule]各个坐标系的偏移量
	 * @return 成功返回ture，失败返回false
	 */
    virtual bool SetCoordinateOffset(int hContinueInterHandle, double dOffset[4]) = 0;

	/**
	 * @brief 使用前瞻预处理功能。先建立坐标系，启用前瞻预处理功能后再重新调用InitContInterp()更新参数
	 * @return 成功返回ture，失败返回
	 */
    virtual bool EnableLookAhead(int hContinueInterHandle, bool bEnable) = 0;

	/**
	 * @brief 插补合成速度设置
	 * @param axis				-- [in]坐标系包含的轴索引
	 * @param count				-- [in]坐标系轴个数
	 * @param speed				-- [in]插补速度(mm/s)
	 * @return 成功返回ture，失败返回false	 */
    virtual bool SetContinueInterSpeed(int hContinueInterHandle, const StSpeed &speed) = 0;

	/**
	 * @brief 两维直线插补
	 * @param xAxis						-- [in]插补段x轴终点坐标值
	 * @param yAxis						-- [in]插补段x轴终点坐标值
	 * @param xPos						-- [in]插补段x轴终点坐标值
	 * @param yPos						-- [in]插补段y轴终点坐标值
	 * @return 成功返回ture，失败返回false
	 */
	virtual bool LineXY(int xAxis, int yAxis, double xPos,double yPos) = 0;

	/**
	 * @brief 三维直线插补
	 * @param xAxis					-- [in]插补X轴索引
	 * @param yAxis					-- [in]插补Y轴索引
	 * @param zAxis					-- [in]插补Z轴索引
	 * @param xPos				    -- [in]坐标号,取值范围[1,2]
	 * @param yPos					-- [in]插补段x轴终点坐标值
	 * @param zPos					-- [in]插补段y轴终点坐标值
	 * @return 成功返回ture，失败返回
	 */
	virtual bool LineXYZ(int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos) = 0;

	/**
	 * @brief 四维直线插补
	 * @param xAxis						-- [in]插补段x轴终点坐标值
	 * @param yAxis					    -- [in]插补段y轴终点坐标值
	 * @param zAxis						-- [in]插补段z轴终点坐标值
	 * @param uAxis						-- [in]插补段a轴终点坐标值
	 * @return 成功返回ture，失败返回false
	 */
	virtual bool LineXYZU(int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos) = 0;

	/**
	 * @brief XY平面三点坐标圆弧插补
	 * @param xAxis					-- [in]插补X轴索引
	 * @param yAxis					-- [in]插补Y轴索引
	 * @param xPos1				    -- [in]初始点坐标值
	 * @param yPos1					-- [in]初始点坐标值
	 * @param xPos2				    -- [in]圆弧起始点坐标值
	 * @param yPos2					-- [in]圆弧起始点坐标值
	 * @param xPos3				    -- [in]圆弧终点坐标值
	 * @param yPos3					-- [in]圆弧终点坐标值
	 * @return 成功返回ture，失败返回false
	 */
	virtual bool ArcXY(int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3) = 0;

	 /************************************* 连续插补运动 *******************************/

	 /**
	 * @brief 两维直线插补
	 * @param xAxis						-- [in]插补段x轴终点坐标值
	 * @param yAxis						-- [in]插补段x轴终点坐标值
	 * @param xPos						-- [in]插补段x轴终点坐标值
	 * @param yPos						-- [in]插补段y轴终点坐标值
	 * @return 成功返回ture，失败返回false
	 */
	virtual bool BufLineXY(int xAxis, int yAxis, double xPos,double yPos) = 0;

	/**
	 * @brief 三维直线插补
	 * @param xAxis					-- [in]插补X轴索引
	 * @param yAxis					-- [in]插补Y轴索引
	 * @param zAxis					-- [in]插补Z轴索引
	 * @param xPos				    -- [in]坐标号,取值范围[1,2]
	 * @param yPos					-- [in]插补段x轴终点坐标值
	 * @param zPos					-- [in]插补段y轴终点坐标值
	 * @return 成功返回ture，失败返回
	 */
	virtual bool BufLineXYZ(int xAxis, int yAxis, int zAxis, double xPos,double yPos,double zPos) = 0;

	/**
	 * @brief 四维直线插补
	 * @param xAxis						-- [in]插补段x轴终点坐标值
	 * @param yAxis					    -- [in]插补段y轴终点坐标值
	 * @param zAxis						-- [in]插补段z轴终点坐标值
	 * @param uAxis						-- [in]插补段a轴终点坐标值
	 * @return 成功返回ture，失败返回false
	 */
	virtual bool BufLineXYZU(int xAxis, int yAxis, int zAxis, int uAxis, double xPos,double yPos,double zPos,double uPos) = 0;

	/**
	 * @brief 二维圆弧插补
	 * @param xAxis					-- [in]插补X轴索引
	 * @param yAxis					-- [in]插补Y轴索引
	 * @param xPos1				    -- [in]初始点坐标值
	 * @param yPos1					-- [in]初始点坐标值
	 * @param xPos2				    -- [in]圆弧起始点坐标值
	 * @param yPos2					-- [in]圆弧起始点坐标值
	 * @param xPos3				    -- [in]圆弧终点坐标值
	 * @param yPos3					-- [in]圆弧终点坐标值
	 * @return 成功返回ture，失败返回
	 */
	virtual bool BufArcXY(int xAxis, int yAxis, double xPos1,double yPos1,double xPos2,double yPos2,double xPos3,double yPos3) = 0;

	/**
	 * @brief 二维圆弧插补
	 * @param xAxis					-- [in]插补X轴索引
	 * @param yAxis					-- [in]插补Y轴索引
	 * @param xPos1				    -- [in]圆弧起始点坐标值
	 * @param yPos1					-- [in]圆弧起始点坐标值
	 * @param xPos2				    -- [in]圆弧终点坐标值
	 * @param yPos2					-- [in]圆弧终点坐标值
	 * @param Radius				    -- [in]圆弧半径
	 * @param idir				    -- [in]运动方向，0-顺时针， 1-逆时针
	 * @param synVel				    -- [in]速度
	 * @param synAcc				    -- [in]加速度
	 * @param velEnd				    -- [in]停止速度
	 * @return 成功返回ture，失败返回
	 */
	virtual bool BufArcXYR(int xAxis, int yAxis, double xPos1,double yPos1, double xPos2,double yPos2,double Radius, int idir) = 0;

	/**
	 * @brief 插补延时
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @param time					-- [in]延时时间
	 * @return 成功返回ture，失败返回
	 */
    virtual bool BufDelay(int hContinueInterHandle, int time) = 0;

	/**
	 * @brief 插补IO延时输出
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @param index					-- [in]IO索引
	 * @param bOpen					-- [in]打开/关闭
	 * @param delay					-- [in]IO输出延后时间
	 * @return 成功返回ture，失败返回
	 */
    virtual bool BufIO(int hContinueInterHandle, int index, bool bOpen, int delay=0) = 0;

    /**
	 * @brief 插补中输出电压
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @param channel					-- [in]通道[1-8]
	 * @param value						-- [in]电压值(V)
	 *@return 成功返回ture，失败返回false
	 */
    virtual bool BufDA(int hContinueInterHandle, int channel, short value) = 0;

	/**
	 * @brief 清空缓存
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */

    virtual bool ClearBuf(int hContinueInterHandle) = 0;

    /**
	 * @brief 缓存去区是否已满
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回false
	 */
    virtual bool IsBufFull(int hContinueInterHandle) = 0;

	/**
	 * @brief 启动连续插补
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */
    virtual bool StartContInterp(int hContinueInterHandle) = 0;

	/**
	 * @brief 暂停连续插补，没有相应函数就直接停止轴
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */
    virtual bool PauseContInterp(int hContinueInterHandle) = 0;

	/**
	 * @brief 停止连续插补
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */
    virtual bool StopContInterp(int hContinueInterHandle) = 0;

	/**
	 * @brief 插补是否完成
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */
    virtual bool IsContInterpFinished(int hContinueInterHandle) = 0;

	/**
	 * @brief 获取当前在执行的插补段
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @return 成功返回ture，失败返回
	 */
    virtual int  GetCurSegNum(int hContinueInterHandle) = 0;

	/**
	 * @brief 启动轴的刀向跟随
	 * @param axis					-- [in]坐标系包含的轴索引
	 * @param count					-- [in]坐标系轴个数
	 * @param iprofile				-- [in]轴索引
	 * @param lpulse					-- [in]相对位置偏移脉冲数
	 * @param icrd					-- [in]FIFO索引
	 * @return 成功返回ture，失败返回
	 */
    virtual bool BufGear(int hContinueInterHandle, int iprofile, long lpulse, int icrd=0) = 0;
	
	/**
	 * @brief 查询位置比较状态
	 * @param status				-- [in]指示位置比较是否结束。1 表示位置比较结束，0 表示正在进行位置比较。
	 * @param count					-- [in]读取位置比较已输出的上升沿个数，必须传递一个长度为 2 的数组。
	 * @return 成功返回ture，失败返回
	 */


    virtual bool GetCompareStatus(short &pStatus,long pCount[2]) = 0;

    virtual bool SetCompareLiner(short encoder, short channel,double startPos,long repeatTimes,double interval,short time,short source) = 0;

    virtual bool SetComparePluse(short level,short outputType, unsigned int time) = 0;

    virtual bool SetPrfMapAxis(short encoder, short prfMapAxis) = 0;

    virtual int GetCompareCount() = 0;


	// 固高专用
	// 位置比较输出电平
	// 设置比较位置 axis:轴号，startLevel：初始电平, pluseWidth:脉冲宽度,1脉冲1us 
	// pos1:触发HSIO通道1的位置列表，count1:位置个数，pos2:触发HSIO通道2的位置列表，count2:位置个数，
	virtual bool setComparePos(int axis, short startLevel, long pluseWidth, double pos1[], int count1, double pos2[], int count2){ return false; }

	// 取消位置比较输出
	virtual bool stopComparePos(int axis){ return false; }

	// 直接输出电平,level，取值范围[0,3], 0表示1，2通道关，1表示HSIO通道1开, 2表示HSIO通道2开, 3表示两个通道都开
	// type
	virtual bool setCompareOutput(short level, short type, short time = 0){ return false; }
	virtual short SetCompareData(short encoder, short source, short pulseType, short startLevel, short time, long *pBuf1, short count1, long *pBuf2, short count2){ return -1; }
	virtual short StopCompare(){ return -1; }
	
	virtual bool  CompareMode2D(short chn,short mode)=0;// chn 0 1 ;mode 0 一维，1 二维 

	/************************************************************************/
	/*     指令原型short GT_2DComparePulse(short chn,short level,short outputType,short time);
	指令说明二维位置比较输出单次输出。
	指令类型立即指令，调用后立即生效。章节页码7
	指令参数该指令共有4 个参数，参数的详细信息如下。
	chn 通道号0：HSIO0 通道号1：HSIO1
	level
	位置比较输出电平方式时起始电平（当outputType=0 时，level 应该为1）
	level=0，位置比较输出引脚电平复位（恢复到初始化后的电平状态）
	level=1，位置比较输出引脚电平取反（与初始化后的电平状态相反）
	outputType 输出类型0：脉冲方式1：电平方式
	time 脉冲方式时的脉冲时间，单位：us
	指令返回值请参照指令返回值列表。
	相关指令无
	指令示例例程1 二维位置比较输出指令详细的用法
                                                                 */
	/************************************************************************/

	virtual bool  ComparePulse2D(short chn,short level,short outputType,short time)=0;

	/*
	指令说明
	停止二维位置比较输出。停止后位置比较输出端口的控制权移交给原一维位置比较
	输出模块
	指令类型立即指令，调用后立即生效。
	指令参数该指令共有1 个参数，参数的详细信息如下。
	chn 通道号0：HSIO0 通道号1：HSIO1

	*/
	virtual bool  CompareStop2D(short chn)=0;

	/*
	指令说明清空二维位置比较输出数据。清空所有缓冲区剩余的位置数据。
	指令类型立即指令，调用后立即生效。章节页码7
	指令参数该指令共有1 个参数，参数的详细信息如下。
	chn 通道号0：HSIO0 通道号1：HSIO1

	*/
	virtual bool  CompareClear2D(short chn)=0;
	/*
	指令说明读取二维位置比较输出状态。
	指令类型立即指令，调用后立即生效。章节页码7
	指令参数该指令共有6 个参数，参数的详细信息如下。
	chn 通道号0：HSIO0 通道号1：HSIO1
	pStatus 0 :正在进行比较输出1：比较输出完成
	pCount 位置比较已输出次数
	pFifo 当前空闲FIFO
	pFifoCount 当前空闲FIFO 剩余空间
	pBufCount
	FPGA 中FIFO 剩余空间，FPGA 的FIFO 总大小为512，启动位置比较之前，压入
	的数据先进入FPGA 的FIFO

	*/
	virtual bool  CompareStatus2D(short chn,short *pStatus,long *pCount,short *pFifo,short *pFifoCount,short *pBufCount =NULL)=0;
	/*
	设置二维位置比较输出参数。
	指令类型立即指令，调用后立即生效。章节页码7
	指令参数该指令共有2 个参数，参数的详细信息如下。
	chn 通道号0：HSIO0 通道号1：HSIO1
	pPrm
	typedefstruct
	{
	short encx; //X比较轴选择取值范围，以GTS为例：GTS-400 1-4
	GTS-800 1-8
	short ency; //Y比较轴选择取值范围，以GTS为例：GTS-400 1-4
	GTS-800 1-8
	short source; //比较源0 ：规划1：编码器
	short outputType; //输出方式0：脉冲1：电平
	short startLevel; //起始电平方式0：位置比较输出引脚电平复位
	1：位置比较输出引脚电平取反
	short time; //脉冲方式脉冲时间
	short maxerr; //比较范围最大误差
	short threshold; //最优算法阈值
	} T2DComparePrm;
	source=0（比较源为规划器），控制器的“脉冲计数源”必须设置为“内部脉冲计数器”。

	*/
	virtual bool  CompareSetPrm2D(short chn,CardGGT2DComparePrm *pPrm)=0;

	/*
	指令说明设置二维位置比较输出数据。
	指令类型立即指令，调用后立即生效。指令参数该指令共有4 个参数，参数的详细chn 通道号0：HSIO0 通道号1：HSIO1
	count 位置比较数据个数
	pBuf
	比较数据点输入
	typedefstruct
	{
	long px; //X轴long py; //Y 轴} T2DCompareData;
	fifo 数据压入的FIFO 取值范围：0-1

	*/

	virtual bool  CompareData2D(short chn,short count,CardGGT2DCompareData *pBuf,short fifo)=0;


	/*
	short GT_API GT_2DCompareStart(short chn);
	启动二维位置比较输出。
	立即指令，调用后立即生效。该指令共有1 个参数，参数的详细信息如下。
	通道号0：HSIO0 通道号1：HSIO1

	*/
	virtual bool  CompareStart2D(short chn)=0;
	/*
	指令说明设置位置比较输出端口。
	指令类型立即指令，调用后立即生效。章节页码7
	指令参数该指令共有3 个参数，参数的详细信息如下。
	channel COMPARE_PORT_GPO : 位置比较输出到GPO 端口
	二维位置比较输出
	13 © 1999-2015 固高科技版权所有
	COMPARE_PORT_HSIO：位置比较输出到HSIO 端口
	hsio0 位置比较输出到GPO 端口时有效，指定通道0 输出端口
	hsio1 位置比较输出到GPO 端口时有效，指定通道1 输出端口
	补充说明
	hsio0 和hsio1 对应于通用输出口，取值范围为0~15，调用该指令把位置比较输出
	配置到通用输出口之前应该先把该通用输出口设置为高电平。

	*/
	virtual bool  SetComparePort2D(short channel,short hsio0,short hsio1)=0;
	//按频率自动触发高速io
	// mode 0  不启用 1  启用连续触发
	// 触发count 次 
	//间隔 standTime us  // 步骤 1SetCompareContinuePulseMode 需要配合函数  步骤 2 SetComparePluse();
	virtual bool SetCompareContinuePulseMode(short mode, short count, short standTime) = 0;
};

#endif
