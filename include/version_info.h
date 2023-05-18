#ifndef _VERSION_INFO_H_
#define _VERSION_INFO_H_



/**************** sdk version information ***************/

#ifdef CARDADSPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardads"
    #define FILE_DESCRIPTION    "cardads"
    #define FILE_VERSION        "1.1.1.0"
#endif

/*
1.1.1.1 
去掉回原时跑负限位的速度设置，使用当前速度
1.1.1.2
回原模式优化  
1.1.1.3
1、回原bug修复,多线程调用同一张卡的回原会有异常 
*/
#ifdef CARDADTPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardadt"
    #define FILE_DESCRIPTION    "cardadt"
    #define FILE_VERSION        "1.1.1.3"
#endif

#ifdef CARDGGPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardgg"
    #define FILE_DESCRIPTION    "cardgg"
    #define FILE_VERSION        "1.1.1.0"
#endif

#ifdef CARDLSPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardls"
    #define FILE_DESCRIPTION    "cardls"
    #define FILE_VERSION        "1.1.1.0"
#endif


/*
1.1.1.1 
首个测试OK版本   
1.1.1.2 
优化连续插补输出电平
1.1.1.3 
3/21调机完善by有福
1.1.1.4
增加界面限位电平设置
*/
#ifdef CARDMCSPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardmcs"
    #define FILE_DESCRIPTION    "cardmcs"
    #define FILE_VERSION        "1.1.1.4"
#endif

/*
1.1.1.0 
首个版本 
1.1.1.1 
1、增加轴专用IO sensor
2、卡重连设置轴参数
1.1.1.2
增加界面限位电平设置
*/
#ifdef CARDZMCPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardzmc"
    #define FILE_DESCRIPTION    "cardzmc"
    #define FILE_VERSION        "1.1.1.2"
#endif

#ifdef LOCALDATABASEMGR_LIBRARY
    #define FILE_NAME           "localdatabasemgr"
    #define FILE_DESCRIPTION    "localdatabasemgr"
    #define FILE_VERSION        "1.1.1.0"
#endif

#ifdef MAINFLOW_RESOURCE_LIBRARY
    #define FILE_NAME           "mainflow"
    #define FILE_DESCRIPTION    "mainflow"
    #define FILE_VERSION        "1.1.1.0"
#endif

/*
1.1.1.1 ：更新轴时设置轴参数
1.1.1.2： 优化readio,不用每次初始化IO实例，by 小虎
*/
#ifdef MOTION_RESOURCE_LIBRARY
    #define FILE_NAME           "motion"
    #define FILE_DESCRIPTION    "motion"
    #define FILE_VERSION        "1.1.1.2"
#endif

#ifdef ROBOTSTATIONPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "robot"
    #define FILE_DESCRIPTION    "robot"
    #define FILE_VERSION        "1.1.1.0"
#endif

/* 1.1.1.1 ： 串口停止位设置bug修复 */
#ifdef SDKCOMMON_RESOURCE_LIBRARY
    #define FILE_NAME           "sdkcommon"
    #define FILE_DESCRIPTION    "sdkcommon"
    #define FILE_VERSION        "1.1.1.1"
#endif


/*  sdkcontainer
1.1.1.0 ： 最初发布版本   
1.1.1.1 ： 修改加载lib 无法打印资源名的bug
*/
#ifdef SDKCONTAINERPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "sdkcontainer"
    #define FILE_DESCRIPTION    "sdkcontainer"
    #define FILE_VERSION        "1.1.1.0"
#endif

#ifdef SDK_CORE_LIBRARY
    #define FILE_NAME           "sdkcore"
    #define FILE_DESCRIPTION    "sdkcore"
    #define FILE_VERSION        "1.1.1.0"
#endif

#ifdef SDKPROXY_RESOURCE_LIBRARY
    #define FILE_NAME           "sdkproxy"
    #define FILE_DESCRIPTION    "sdkproxy"
    #define FILE_VERSION        "1.1.1.0"
#endif

/*  station
1.1.1.1 
1、修改工站速度比例设置，增加加速度，加速度比例设置
2、增加工站速度设置，即工站下说有轴同时设置为该速度
1.1.1.2
1、是否在连续插补中增加判段，如果没启动连续插补，直接返回false     
1.1.1.3
1、修复工站暂停继续功能 
1.1.1.4
1、工站未初始化时允许读取点位信息
2、MoveSucceed offset参数未传递
3、homepri 初始化vector只传递了5个轴号
1.1.1.5
1、回原bug修复,多线程调用同一张卡的回原会有异常
2、增加工站运动接口
*/
#ifdef AXISSTATIONPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "station"
    #define FILE_DESCRIPTION    "station"
    #define FILE_VERSION        "1.1.1.5"
#endif

/**************** ui version information ***************/

/*  aboutui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_ABOUTUI
#define FILE_NAME           "aboutui"
#define FILE_DESCRIPTION    "aboutui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  callsdk
1.1.1.0 ： 最初发布版本   
1.1.1.1 ： 出去获取接口失败的log
*/
#ifdef UI_CALLSDK
#define FILE_NAME           "callsdk"
#define FILE_DESCRIPTION    "callsdk"
#define FILE_VERSION        "1.1.1.1"
#endif

/*  cardui
1.1.1.0 ： 最初发布版本  
1.1.1.1 ： 输入输出默认电平改为分开设置
1.1.1.2 ： 去除多余的log 打印
*/ 
#ifdef UI_CARDUI
#define FILE_NAME           "cardui"
#define FILE_DESCRIPTION    "cardui"
#define FILE_VERSION        "1.1.1.2"
#endif


/*  lightui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_LIGHTUI
#define FILE_NAME           "lightui"
#define FILE_DESCRIPTION    "lightui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  mainflowui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_MAINFLOWUI
#define FILE_NAME           "mainflowui"
#define FILE_DESCRIPTION    "mainflowui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  mainui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_MAINUI
#define FILE_NAME           "mainui"
#define FILE_DESCRIPTION    "mainui"
#define FILE_VERSION        "1.1.1.1"
#endif

/*  motionui
1.1.1.0 ： 最初发布版本   
1.1.1.1 ： showevent中的初始化UI动作放到init中，防止界面切换时很卡
1.1.1.2 ： 回原模式优化
*/
#ifdef UI_MOTIONUI
#define FILE_NAME           "motionui"
#define FILE_DESCRIPTION    "motionui"
#define FILE_VERSION        "1.1.1.2"
#endif

/*  robotui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_ROBOTUI
#define FILE_NAME           "robotui"
#define FILE_DESCRIPTION    "robotui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  stationui
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_STATIONUI
#define FILE_NAME           "stationui"
#define FILE_DESCRIPTION    "stationui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  uicommon
1.1.1.0 ： 最初发布版本   
*/
#ifdef UI_UICOMMON
#define FILE_NAME           "uicommon"
#define FILE_DESCRIPTION    "uicommon"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  uitoolbox
1.1.1.0 ： 最初发布版本 
1.1.1.1 ： uilog打印第二参数改为int，原来为bool
1.1.1.2 :  uilog颜色改为：警告-blue,错误-red,其他-black
*/
#ifdef UI_UITOOLBOX
#define FILE_NAME           "uitoolbox"
#define FILE_DESCRIPTION    "uitoolbox"
#define FILE_VERSION        "1.1.1.2"
#endif

/*  titlebarplugin
1.1.1.0 ： 最初发布版本 
1.1.1.1 ： 增加关闭按钮的提示框
1.1.1.2 ： 增加过期时间的显示
*/
#ifdef UI_TITLEBARPLUGIN
#define FILE_NAME           "titlebarplugin"
#define FILE_DESCRIPTION    "titlebarplugin"
#define FILE_VERSION        "1.1.1.1"
#endif


#ifndef  FILE_NAME
#define FILE_NAME  "None"
#endif

#ifndef  FILE_DESCRIPTION
#define FILE_DESCRIPTION  "None"
#endif

#ifndef  FILE_VERSION
#define FILE_VERSION  "1.1.1.0"
#endif

#endif
