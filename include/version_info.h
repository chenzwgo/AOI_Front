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
ȥ����ԭʱ�ܸ���λ���ٶ����ã�ʹ�õ�ǰ�ٶ�
1.1.1.2
��ԭģʽ�Ż�  
1.1.1.3
1����ԭbug�޸�,���̵߳���ͬһ�ſ��Ļ�ԭ�����쳣 
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
�׸�����OK�汾   
1.1.1.2 
�Ż������岹�����ƽ
1.1.1.3 
3/21��������by�и�
1.1.1.4
���ӽ�����λ��ƽ����
*/
#ifdef CARDMCSPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "cardmcs"
    #define FILE_DESCRIPTION    "cardmcs"
    #define FILE_VERSION        "1.1.1.4"
#endif

/*
1.1.1.0 
�׸��汾 
1.1.1.1 
1��������ר��IO sensor
2�����������������
1.1.1.2
���ӽ�����λ��ƽ����
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
1.1.1.1 ��������ʱ���������
1.1.1.2�� �Ż�readio,����ÿ�γ�ʼ��IOʵ����by С��
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

/* 1.1.1.1 �� ����ֹͣλ����bug�޸� */
#ifdef SDKCOMMON_RESOURCE_LIBRARY
    #define FILE_NAME           "sdkcommon"
    #define FILE_DESCRIPTION    "sdkcommon"
    #define FILE_VERSION        "1.1.1.1"
#endif


/*  sdkcontainer
1.1.1.0 �� ��������汾   
1.1.1.1 �� �޸ļ���lib �޷���ӡ��Դ����bug
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
1���޸Ĺ�վ�ٶȱ������ã����Ӽ��ٶȣ����ٶȱ�������
2�����ӹ�վ�ٶ����ã�����վ��˵����ͬʱ����Ϊ���ٶ�
1.1.1.2
1���Ƿ��������岹�������жΣ����û���������岹��ֱ�ӷ���false     
1.1.1.3
1���޸���վ��ͣ�������� 
1.1.1.4
1����վδ��ʼ��ʱ�����ȡ��λ��Ϣ
2��MoveSucceed offset����δ����
3��homepri ��ʼ��vectorֻ������5�����
1.1.1.5
1����ԭbug�޸�,���̵߳���ͬһ�ſ��Ļ�ԭ�����쳣
2�����ӹ�վ�˶��ӿ�
*/
#ifdef AXISSTATIONPLUGIN_RESOURCE_LIBRARY
    #define FILE_NAME           "station"
    #define FILE_DESCRIPTION    "station"
    #define FILE_VERSION        "1.1.1.5"
#endif

/**************** ui version information ***************/

/*  aboutui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_ABOUTUI
#define FILE_NAME           "aboutui"
#define FILE_DESCRIPTION    "aboutui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  callsdk
1.1.1.0 �� ��������汾   
1.1.1.1 �� ��ȥ��ȡ�ӿ�ʧ�ܵ�log
*/
#ifdef UI_CALLSDK
#define FILE_NAME           "callsdk"
#define FILE_DESCRIPTION    "callsdk"
#define FILE_VERSION        "1.1.1.1"
#endif

/*  cardui
1.1.1.0 �� ��������汾  
1.1.1.1 �� �������Ĭ�ϵ�ƽ��Ϊ�ֿ�����
1.1.1.2 �� ȥ�������log ��ӡ
*/ 
#ifdef UI_CARDUI
#define FILE_NAME           "cardui"
#define FILE_DESCRIPTION    "cardui"
#define FILE_VERSION        "1.1.1.2"
#endif


/*  lightui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_LIGHTUI
#define FILE_NAME           "lightui"
#define FILE_DESCRIPTION    "lightui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  mainflowui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_MAINFLOWUI
#define FILE_NAME           "mainflowui"
#define FILE_DESCRIPTION    "mainflowui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  mainui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_MAINUI
#define FILE_NAME           "mainui"
#define FILE_DESCRIPTION    "mainui"
#define FILE_VERSION        "1.1.1.1"
#endif

/*  motionui
1.1.1.0 �� ��������汾   
1.1.1.1 �� showevent�еĳ�ʼ��UI�����ŵ�init�У���ֹ�����л�ʱ�ܿ�
1.1.1.2 �� ��ԭģʽ�Ż�
*/
#ifdef UI_MOTIONUI
#define FILE_NAME           "motionui"
#define FILE_DESCRIPTION    "motionui"
#define FILE_VERSION        "1.1.1.2"
#endif

/*  robotui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_ROBOTUI
#define FILE_NAME           "robotui"
#define FILE_DESCRIPTION    "robotui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  stationui
1.1.1.0 �� ��������汾   
*/
#ifdef UI_STATIONUI
#define FILE_NAME           "stationui"
#define FILE_DESCRIPTION    "stationui"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  uicommon
1.1.1.0 �� ��������汾   
*/
#ifdef UI_UICOMMON
#define FILE_NAME           "uicommon"
#define FILE_DESCRIPTION    "uicommon"
#define FILE_VERSION        "1.1.1.0"
#endif

/*  uitoolbox
1.1.1.0 �� ��������汾 
1.1.1.1 �� uilog��ӡ�ڶ�������Ϊint��ԭ��Ϊbool
1.1.1.2 :  uilog��ɫ��Ϊ������-blue,����-red,����-black
*/
#ifdef UI_UITOOLBOX
#define FILE_NAME           "uitoolbox"
#define FILE_DESCRIPTION    "uitoolbox"
#define FILE_VERSION        "1.1.1.2"
#endif

/*  titlebarplugin
1.1.1.0 �� ��������汾 
1.1.1.1 �� ���ӹرհ�ť����ʾ��
1.1.1.2 �� ���ӹ���ʱ�����ʾ
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
