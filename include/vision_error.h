#ifndef    _VISION_ERROR_H_
#define    _VISION_ERROR_H_

#ifndef INVALID_VALUE
#define  INVALID_VALUE -1
#endif

enum EM_ERROR_VISION_RESULT
{
    VISION_FAILURE  =-1,
	VISION_SUCCESS  = 0,
	VISION_INVALID_PARAM,
	VISION_TRANSLATEROI_FAILURE,	    // ROIģ��ת��ʧ��
	VISION_READ_INNER_PARAM_FAILURE,	// �ڲ�������ȡʧ��
	VISION_SAVE_INNER_PARAM_FAILURE,	// �ڲ���������ʧ��
	VISION_EXEC_FAILURE,				// ִ��ʧ��
	VISION_JOB_NOT_EXIST,               // Job ������
	VISION_JOB_INIT_FAIL,               // Job ��ʼ��ʧ��
	VISION_JOB_GET_RESULT_FAIL,         // Job ��ȡ���ʧ��
	VISION_GLOBAL_IO_PARAM_INVALID,     // ȫ��IO����ʵ����ȡʧ��
    VISION_ACTION_TIME_OUT,             // ������ʱ
    VISION_ACTION_RUNNING,              // ����ִ����

	/* ÿ��ģ��100 ������code */
	
    //VISION_FIND_CIRCLE                            = 200, //��Բ
    //VISION_FIND_LINE                              = 300, //����
    //VISION_CUSTOM_HALCON                          = 400, //�Զ���halcon����
    //VISION_AFFINETRANSPOINT                       = 500, //���궨���أ�vsaffinetranspoint��
    //VISION_VSCALIBMULTIPOINT                      = 600, //���궨��vscalibmultipoint��
    //VISION_VSCALIBRATIOFACTORY                    = 700, //�����궨(vscalibratiofactory)
    //VISION_VSDATAASSEMBLED                        = 800, //����ƴװ(vsdataassembled)
    //VISION_VSMODELSHAPEFACTORY                    = 900, //ģ��ƥ��(vsmodelshapefactory)
    //VISION_VSRESULTCOUNTFACTORY                   = 1000,//�������(vsresultcountfactory)
    //VISION_JOBCAMERA                              = 1100,//ͼƬ�ɼ�(jobcamera)
    //VISION_JOBIMAGEFILE                           = 1200,//�ļ�����(jobimagefile)
    //VISION_VSIMAGESHOW                            = 1300,//ͼƬ��ʾ(vsimageshow)
    //VISION_VSGENCONTOURPOLYGON                    = 1400,//��������(vsgencontourpolygon)
    //VISION_CIRCLEFITTINGITEM                      = 1500,//Բ�����(circlefittingitem)
    //VISION_LINEFITTINGITEM                        = 1600,//ֱ�����(linefittingitem)
    //VISION_DISTANCELLJOBITEM                      = 1700,//���������߾���(distancelljobitem)
    //VISION_DISTANCEPPJOBITEM                      = 1800,//�����������(distanceppjobitem)
    //VISION_SAVEDATA                               = 1900,//���ݱ���(vssavedatatofile)
    //VISION_SAVEIMAGE                              = 2000,//ͼƬ����(vssaveimagetofile)
    //VISION_VSFINDRECTCENTER                       = 2100,//�Ҿ�������(vsfindrectcenter)
    //VISION_VSFINDINTERSECTIONLL                   = 2200,//��ʮ�ֲ潻��(vsfindintersectionll)
    //VISION_VSDATAINTERACTIVE                      = 2300,//���ݽ�����vsdatainteractive��
    //VISION_VSFINDDATACODE2D                       = 2400,//���Ҷ�ά�루vsfinddatacode2d��
    //VISION_VSFINDIRREGULARSHAPE                   = 2500,//���Ҳ�����ͼ�����ģ�vsfindirregularshape��
    //VISION_COMM_UNDEFINE_CMD                      = 2600,//ͨ�Ŵ�����Ϣ
    //VISION_VSGETMULTIPOINTFACTORY                 = 2700,//���Ҷ�㣨vsgetmultipointfactory��
    //VISION_VSGET9POINTSFACTORY                    = 2800,//����9�㣨vsget9pointsfactory��
    //VISION_VSGETCENTERBY3POINTS                   = 2900,//3�㶨԰��vsgetcenterby3points��
    //VISION_DISTANCEPLJOBITEM                      = 3000,//�������߾���(distanceppjobitem)
    //VISION_INTERSECTIONCCJOBITEM                  = 3100,//���湤�ߣ�intersectionccjobitem��
    //VISION_INTERSECTIONLCJOBITEM                  = 3200,//���湤�ߣ�intersectionlcjobitem��
    //VISION_INTERSECTIONLEJOBITEM                  = 3300,//���湤�ߣ�intersectionlejobitem��
    //VISION_INTERSECTIONLLJOBITEM                  = 3400,//���湤�ߣ�intersectionlljobitem��
    //VISION_FITCIRCLEPP                            = 3500,//�������Բ��fitcircleppjobitem��
    //VISION_VSDATAFORMATITEM                       = 3600,//���ݸ�ʽ��(vsdataformatitem)
    //VISION_IMAGE_AREA_SIZE_TOO_B_ERROR            = 3700,//ͼ����(��ֵ�������)
    //VISION_VSDATAINPUTITEM_INPUTPARAM_NULL_ERROR  = 3800,//��������
    //VISION_VSDATAOUTPUTITEM_INPUTPARAM_NULL_ERROR = 3900,//�������
	//VISION_VISIONDEBUG                            = 4000,//����ģ��
    //VISION_GENCOORDINATESJOBITEM                  = 4200,//��ͻ�׼���������ϵ(gencoordinatesjobitem)
    //VISION_CAMERAMANAGER                          = 4300,//�������(cameramanager)

    VISION_USER_ERROR = 50000,                                                   //�Զ��������ʼλ��
};


#endif     //_VISION_ERROR_H_
