#ifndef BJCVRUNNER_COMMON_H__
#define BJCVRUNNER_COMMON_H__
#include "string.h"

#ifdef __cplusplus
extern "C"
{
#endif
  

namespace bjcv
{
        typedef void* ModelHandle;



        // x        left top point column of the rect
        // y        left top point row of the rect
        // width    width of the rect
        // height   height of the rect
        typedef struct RectF_ {
            float x;
            float y;
            float width;
            float height;

            RectF_() : x(), y(), width(), height() {

            }

            RectF_(float x1, float y1, float w, float h) {
                x = x1;
                y = y1;
                width = w;
                height = h;
            }
        } RectF;

        typedef struct PointF_ {
            PointF_() {};
            PointF_(float x1, float y1) : x(x1), y(y1) {};
            float x;
            float y;
        } PointF;

        typedef struct LineF_ {
            int point_num = 0;      // �ߵĵ���
            PointF* points = NULL;  // ���ϵĵ�
        } LineF;

        typedef struct PolygonF_ {
            int point_num = 0;      // polygon�ĵ���
            PointF* points = NULL;  // polygon�ϵĵ�
        } PolygonF;

        typedef struct Color_ {
            Color_() {};
            Color_(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {};
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
        } Color;

        /*
        *data              ͼ�������
        *width             ͼ��Ŀ��
        *height            ͼ��ĸ߶�
        *channels          ͼ���ͨ����
        *width_step        ͼ��ÿ�е��ֽ�����bytes��
        *
        */
        typedef struct Image_ {
            unsigned char* data;                                // ͼ�������
            int width;                                          // ͼ��Ŀ��
            int height;                                         // ͼ��ĸ߶�
            int channels;                                       // ͼ���ͨ����
            int width_step;                                     // ͼ��ÿ�е��ֽ�����bytes��
            bool hwc = true;                                   // ��ʾ�Ƿ��ǲ�ɫͼƬ��rgb�Ƿֿ�ͨ������ģ���r��g��b�ֱ�Ϊһ��ƽ�档Ĭ����
            unsigned char* r;
            unsigned char* g;
            unsigned char* b;                            // ���hwcΪfalse����������Ҫ�ֱ���д��r��g��b��
            Image_() {
                data = NULL;
                width = 0;
                height = 0;
                channels = 0;
                width_step = 0;
                hwc = true;
                r = NULL;
                g = NULL;
                b = NULL;
            }
            Image_(unsigned char* data1, int w, int h, int c, int step, bool hwc1=true, unsigned char* r1=NULL, unsigned char* g1 = NULL, unsigned char* b1 = NULL) {
                data = data1;
                width = w;
                height = h;
                channels = c;
                width_step = step;
                hwc = hwc1;
                r = r1;
                g = g1;
                b = b1;
            }
        } Image;

        typedef struct RotatedRectF_ {
            float xc;     // �ı��ε���С��Ӿ�������x����
            float yc;     // �ı��ε���С��Ӿ�������y����
            float width;
            float height;    // height, width�ֱ��ʾ�ı��ε���С��Ӿ��ε�2���ߵĳ��ȣ����Ʋ������С����width��height�Ĵ�С��ϵ��ȷ��
            float angle;  // ��ʾ�ı��ε���С��Ӿ��ε���б�Ƕ�


            RotatedRectF_() : xc(), yc(), width(), height(), angle() {
            }

            RotatedRectF_(float xc_, float yc_, float width_, float height_, float angle_) {
                xc = xc_;
                yc = yc_;
                width = width_;
                height = height_;
                angle = angle_;
            }
        } RotatedRectF;   // ����ȱ�ݵ���Ϣ

        typedef struct Defect_ {
            const char* name = NULL;                          // ȱ�����ƣ����ܳ���50���ַ�
            RectF rect;                                       // λ��
            float area;                                       // ȱ�����ظ���,���ڼ������
            int type;                                         // ���� class
            int shape;                                        // ��������
            float score;                                      // ȱ�����÷�
            const char* zone = NULL;                          // ���Դ��һ��zone������
            const char* zone2 = NULL;
            float incline_width = 0;
            float incline_length = 0;
            float contrast = 0.0;                               // ��ʾȱ������ĶԱȶ�
            float delta = 0.0;                                  // ��ʾȱ������Χ����ɫ����
            float std_err = 0.0;                                // ��ʾȱ����Χ������ֵ��׼��
            float avg = 0.0;                                    // ��ʾȱ�������ƽ���Ҷ�ֵ
            int category = 0;                                   // ��ʾȱ�ݵ����ͣ���ʱ��ʹ�ô�����
            int channel = -1;                                   // ��ʾ����һ��ͨ����⵽ȱ��
            Color color;                                        // ��ɫ
            const char* text = NULL;                            // �����ά����ı�
            int num_points = 0;                                 // ���������ĵ��������Ϊ0������û�����������Ϣ!
            PointF* contour = NULL;                             // �б����ڴ洢ȱ�ݵ����������꣬��ĸ���Ϊnum_points
            bool has_mask = false;
            Image* mask = NULL;                                 // ��Ĥ
            bool rotated = 0;                                   // �Ƿ�����ת������Ϣ
            RotatedRectF rot_rect;
            int int_num = 0;                                    // �������ԣ�����ĳЩ����������ڲ��ı�ӿڵ������ʵ�����
            int* int_values = NULL;
            int float_num = 0;
            float* float_values = NULL;
            int string_num = 0;
            const char** strings = NULL;
            


            Defect_() {
                area = 0;
                type = 0;
                shape = 0;
                score = 0;
                rotated = false;
                contrast = 0.0;
                delta = 0.0;
                std_err = 0;
                avg = 0.0;
                category = 0;
                channel = -1;
                num_points = 0;
                contour = NULL;
            }

            Defect_(RectF rect_p, const char* name_chars = "none", float area_p = -1, int type_p = -1, int shape_p = -1, float cs = 0,
                                bool rotated_p = false, float contrast_p =0, float delta_p = 0, float std_err_p = 0, float avg_p = 0, int category_p = 0) {
                rect = rect_p;
                name = name_chars;
                area = area_p;
                type = type_p;
                shape = shape_p;
                score = cs;
                rotated = rotated_p;
                contrast = contrast_p;
                delta = delta_p;
                std_err = std_err_p;
                avg = avg_p;
                category = category_p;
                channel = -1;
                num_points = 0;
                contour = NULL;
            }
        } Defect;   // ����ȱ�ݵ���Ϣ

        typedef struct PartResult_ {                                // ���������Ľ����Ϣ
            int defect_num = 0;                                                     // ͼƬ�����е�ȱ����Ŀ
            int object_num = 0;                                           //  ��ȱ����Ŀ��ĸ���
            int point_num = 0;
            int line_num = 0;
            int rect_num = 0;
            int rot_rect_num = 0;
            int polygon_num = 0;
            int image_num = 0;
            int string_num = 0;
            int int_num = 0;
            int float_num = 0;
            Defect* defects;
            Defect* objects;
            PointF* points;
            LineF* lines;
            RectF* rects;
            RotatedRectF* rot_rects;
            PolygonF* polygons;
            Image* images;
            const char** strings;
            int* int_values;
            float* float_values;
        } PartResult;                                                     // ���������ļ����

        typedef struct ImageResult_ {
            int part_num = 0;
            int src_num = 0;                                            // ����Ǻϳɶ�ͨ�������ж��src�����߶��ͼƬ��ͬһ�ǶȲ�ͬ���գ�Ҳ���ܱ��ϲ���һ��
            PartResult* part_results = NULL;                            // ���������ļ����
            RotatedRectF* part_rects = NULL;                                           // ����������λ��
            int* src_image_ids = NULL;                                  // ��ӦԴͼƬ�ı�ţ��еļ�������Զ��ͼƬ����ʱ��һ��id�б�����ϳɶ�ͨ�������
            unsigned long long sn = 0;                                      // �����ڼ�¼ͼƬ��ʱ���
        } ImageResult;                                                     // ����ͼƬ�ļ����

        // ȱ�ݵĴ�С�����ͣ�λ��
        typedef struct BJCVResults_ {
            int result_num = 0;
            ImageResult* all_results = NULL;
            unsigned long long sn = 0;                              // �����ڼ�¼ͼƬsn����ĳ��ͼƬ�ύ�Ļ�ִ���
            // ������Щ������ͼƬ�Ļ�����Ϣ
            int part_num = 0;                                       
            int defect_num = 0;
            int object_num = 0;
            int point_num = 0;
            int line_num = 0;
            int rect_num = 0;
            int rot_rect_num = 0;
            int polygon_num = 0;
            int image_num = 0;             // ��ʾ�����ͼƬ����                      
            int string_num = 0;
            int int_num = 0;
            int float_num = 0;
        } BJCVResults;                                                          // һ�μ�������ͼƬ������ȱ����Ϣ

        // error code
        typedef enum class ErrorCode_ {
            Success = 0,        // �ɹ�
            ErrorOpenJsonFileFailed = 1,        // config.json ��ʧ��
            ErrorJsonFileFormatInvalid = 2,        // config�ĸ�ʽ������߲������ò��Ϸ�          
            ErrorCreateThreadFailed = 3,       // �����߳�ʧ��
            ErrorCreateFileFailed = 4,       // ���ܴ����ļ�
            ErrorOpenFileFailed = 5,       // ���ļ�ʧ��
            ErrorDetectFailed = 6,       // ���ʧ��
            ErrorLoadModelFailed = 7,      // ����ģ��ʧ��
            ErrorInvalidHandle = 8,      // ��Ч��handle
            ErrorInvalidImageSize = 9,      // ����ͼƬ�ߴ����ͨ�������Ϸ�
            ErrorInvalidDeviceInfo = 10,      // device��Ϣ��Ч
            ErrorOpenDeviceFailed = 11,      // ���ܳɹ��򿪻��߷���gpu�豸
            ErrorUnknown = 12,     // δ֪����
            ErrorTimerOut = 13,     // Timer out.
            ErrorInvalidPath = 14,    // ��Ч·��

            ErrorUnrecognizedModelName = 15,
            ErrorModelDirNotExist = 16,
            ErrorModelNotInit = 17,
            ErrorMaskImageReadFailed = 18,
            ErrorImgNumIncorrect = 19,
            ErrorUninit = 20,  // ģ�ͻ�û�гɹ�����
            ErrorParamsInvalid = 21, // ���ڷǷ���������
            ErrorWorkpieceUnfound = 22, // �Ҳ�����������roi
            ErrorMemAllocFailed = 23,  // �����ڴ�ʧ��
            ErrorRunInferenceFailed = 24, // inference����ʧ��

            ErrorCreateNodeFailed = 101,    // �����ڵ�ʧ��
            ErrorNodeNameUsed = 102,                // �ڵ����ѱ�ʹ��
            ErrorGraphPathInvalid = 103,            // ��ָ������ͼ·��������
            ErrorNodePathInvalid = 104,             // ��ָ���Ľڵ�·��������
            ErrorNodeNotExist = 105,
            ErrorNodeIsRunning = 106,
            ErrorTypeNotSame = 107,                 // �����������ӻ��߲���ʱ���������Ͳ�ƥ��
            ErrorAssignUnknown = 108,               // ������ֵʱ��δ֪����
            ErrorNotHasDefault = 109,               // Ҫ��û������Ĭ��ֵ�Ĳ����ָ�ΪĬ��ֵ
            ErrorAppendUnknown = 110,               // ��̬���������������ʱ��δ֪����

            ErrorGraphOutputSrcNodeInvalid = 111,       // ͼ�ڵ���������ָ����ת��Ŀ��ڵ㲻����
            ErrorInputLinkExist = 112,
            ErrorTargetNodeInvalid = 113,               // ͼ�ڵ�ת������ʱ��Ŀ��ڵ㲻����
            ErrorInputLinkNotExist = 114,               // �����Ƴ�����ʱ������û������
            ErrorJsonMissKey = 115,                     // json�ļ�ȱ�ٱ���Ĳ���
            ErrorOutputFileFailed = 116,                // ������ļ�ʧ��
            ErrorNodeUnReady = 117,                     // �ڵ㻹���߱�ִ������
            ErrorToolParamUnProvided = 118,             // tool��param��û����
            ErrorUnSupportedPythonValue = 119,          // �����˲���֧�ֵ�python��������
            ErrorInputsNotYetProvided = 120,            // ��û���ṩ����ͼƬ

            ErrorInputNotNumpy = 121,                   // �ṩ������ͼƬ����ndarray
            ErrorImgNotUInt8 = 122,                     // ����ͼƬ��uint8
            ErrorImgNotHWC = 123,                       // ����ͼƬ����hwc��ʽ��������
            ErrorImgsNumIncorrect = 124,                // ����ͼƬ��������ȷ
            ErrorLinkSelf = 125,                        // ���Խ��Լ���������ӵ��Լ�������
            ErrorImageNotRGB = 126,                     // ����Ҫ�������ɫͼƬ�Ľڵ㣬���벻��RGB
            ErrorScriptFailed = 127,                    // �ű�ִ�д���
            ErrorNodeNumNotEnough = 128,                // ���봴����ͼ�Ľڵ���Ŀ̫�٣���������2��
            ErrorSubGraphFromNodeWithOuterLink = 129,   // ��ָ�������ڴ�����ͼ�Ľڵ��У���������ԭͼ�������������
            ErrorFilepathNotExist = 130,                // �ļ�·��������

            ErrorPathNotFile = 131,                     // ָ����·�������ļ�
            ErrorNotJsonFile = 132,                     // ���ṩ��·������json�ļ�
            ErrorParamNameUsed = 133,                   // ����¶ͼ�ڵ�Ĳ���ʱ��Ĭ�����ɵĲ������ѱ�ռ��
            ErrorParamAlreadyExposed = 134,             // ָ���Ĳ����Ѿ���¶Ϊͼ�Ĳ�����
            ErrorParamUnExposed = 135,                  // Ҫ��ȡ����¶ĳ����������ʵ�ʱ���δ��¶
            ErrorOutputNodeInvalid = 136,               // ������ڵ�ȡ����������Ľ������Ԥ�����͵�
            ErrorInputEmptyVector = 137,                // ��һ��vector������ӵ�ֻ���ܵ���Ԫ�ص����룬����ʵ�������vector�ǿյģ�
            ErrorInputParamNotExist = 138,              // ָ��������������Ʋ�����
            ErrotIsAlreadyLoopStartOrEnd = 139,         // ��ѡ�ڵ��Ѿ���ѭ����ʼ�����յ㣬Ҫ��ȡ��������������
            ErrorParamNotDynamicLinked = 140,           // �������û�����Ӷ�̬���룬��������Ϊѭ������

            ErrorLoopNameEmpty = 141,                   // ���õ�ѭ������Ϊ��
            ErrorNodeNotLoopStartOrEnd = 142,           // ��ָ���Ľڵ㲢��ѭ����ʼ�����յ�
            ErrorNodeNotLoopEnd = 143,                  // �ڵ㲢��ѭ���յ�
            ErrorLoopMissStartEnd = 144,                // ѭ��ȱ�ٿ�ʼ�ڵ������ֹ�ڵ�
            ErrorLoopStrcutureInvalid = 145,            // ���ڷǷ���ѭ���ṹ
            ErrorLoopNodeParamsNotProvided = 146,       // ѭ���ڵ���������û���ṩ����Դ
            ErrorLoopNodeParamSrcsNotUniformWithinOtOutLoop = 147,      // ѭ���ڽڵ�����������Դ����һ���Ե�����ѭ���ڻ���ѭ����
            ErrorTrySetUpdateModeForNonLoopNode = 148,                  // ����Ϊ��ѭ���еĽڵ����ò������·�ʽ
            ErrorLoopStartInputInvalid = 149,                // ѭ����ʼ�ڵ�����벻��vector���ߺ��ж�����ݵ�hobject
            ErrorVectorOutOfIndex = 150,                // ��vector��ȡ���ݵ�ʱ�򳬳���Χ

            ErrorLoopEndInputInvalid = 151,             // ѭ���յ���������ͷǷ���Ҫ����T����vector<T>������halcon��hobject����concat
            ErrorSingleStepUseStreamline = 152,         // ����ˮ��ģʽ�³���ֻ����һ���ڵ�
            ErrorToolExecFailed = 153,                  // ִ��toolʱʧ��
            ErrorUnAppendableParamGetMultiInput = 154,  // ��֧��appendable�Ĳ��������˶����blocked������
            ErrorInputPreprocess = 155,                      //ͼ��Ԥ��������벻���Ϲ涨                        
            ErrorInputIsNull = 156,                     //����Ĳ���Ϊ��
            ErrorInitInfer = 157,                        //��ʼ��inferʧ��
            ErrorInitDetector = 158,          
            ErrorSetNullValueFailed = 159,              // Ϊ�������ÿյ�ֵʱʧ��
            ErrorReadCloud = 160,                       //��ȡ����ʧ��

            ErrorSaveCloud = 161,                       //�������ʧ��
            ErrorMatchCloud = 162,                       //������׼ʧ��
            ErrorWaitRun = 163,                         // ��ִ�нڵ�֮ǰ�ȳ�ʼ��run_err_ΪErrorWaitRun����ʾ���ڵȴ�ִ��
            ErrorLoopStartNodesInConsistentBlock = 164, // ѭ����ʼ�ڵ㱨���˲�һ�µ�blocked״̬
            ErrorSetStaticParamForUnStaticParam = 165,  // Ϊ��֧�־�̬�Ĳ������þ�ֵ̬
            ErrorInvalidParamUpdateMode = 166,          // ��̬��������ģʽ���Ϸ�
            ErrorNotHasNodes = 167,                   //  ͼ��û�нڵ�
            ErrorFindCamera = 168,                   //  û���ҵ������Ҫô��û�����ӣ�Ҫô���ǲ���ͬһ������
            ErrorLinkCamera = 169,                   //  �������ʧ��

            ErrorTakePhoto = 169,                   //  ����ʧ�ܣ�����Ƿ����
            ErrorRunPhoXi = 170,                    //û������photoneo������������
            ErrorOutputNodeWrongIdx = 171,             //outputtool�����˴����idx������0��ʼ��idx��������idx�ظ�

            ErrorOpenSocket = 173,                  //socket������ʧ��
            ErrorCloseSocket = 174,                  //socket������ʧ��
            ErrorLinkSocket = 175,                  //����socket������ʧ��
            ErrorSendData = 176,                  //socket��������ʧ��
            ErrorFindCalibrationPose = 177,       //û���ҵ��궨���pose

            ErrorGraphStartNodeIsGate = 178,        // ͼ�Ŀ�ʼ�ڵ��Ƿ�֧�ڵ�
            ErrorGraphStartNodeIsLoopStart = 179,        // ͼ�Ŀ�ʼ�ڵ���ѭ����ʼ�ڵ�
            ErrorExposeGateNodeDataInput = 180,         // ����֧�ڵ���������뱩¶Ϊͼ�ڵ�Ĳ���
            ErrorInputImageMustSingleChannel = 181,     //����ͼƬ����Ϊ��ͨ��
            ErrorRotational_boxesOrHorizontal_boxesBeSet = 182,      //Rotational_boxes��Horizontal_boxes������һ��������
            ErrorRotational_boxesAndRotational_boxes_namesSizeNotSame = 183,        //Rotational_boxes��Rotational_boxes_names�ߴ粻һ��
            ErrorHorizontal_boxesAndHorizontal_boxes_namesSizeNotSame = 184,        //Horizontal_boxesAndHorizontal_boxes_names�ߴ粻һ��
            ErrorInputImageMustThreeChannels = 185,         //����ͼƬ����Ϊ3ͨ��
            ErrorLower_thresholdLessThanOrEqualToUpper_threshold = 186,         //Lower_threshold����С�ڵ���Upper_threshold
            ErrorNames_inAndRegions_inSizeNotSame = 187,            //Names_in And Regions_in�ߴ粻һ��
            ErrorScores_inAndRegions_inSizeNotSame = 188,           //Scores_in And Regions_in�ߴ粻һ��
            ErrorScores_inNames_inAndRegions_inSizeNotSame = 189,           //Scores_in Names_in And Regions_in�ߴ粻һ��
            ErrorAreaMustHaveName = 190,            //����û������
            ErrorAreaNameRepeat = 191,              //�����ظ���������
            ErrorAreaMustHaveBottomEdge = 192,          //������������еױ�
            ErrorNeed_nameNotInPatch_names = 193,           //Need_name����������Patch_names��
            ErrorImage_patchesIsEmpty = 194,            //Image_patches����Ϊ��
            ErrorRegions1_names_inAndRegions1_inSizeNotSame = 195,              //Regions1_names_in And Regions1_in ������������ߴ粻һ��
            ErrorRegions1_names_inAndregions1_scores_in = 196,      //regions1_names_in��regions1_scores_in���߲�Ϊ��ʱ, ���ߵĳ��ȱ������.��������regions1_scores_inΪ��ʱ, regions1_names_in����Ϊ��"
            ErrorClassesRepeat = 197,               //�����ظ����������
            ErrorNeed_classesRepeat = 198,              //Need_classes�����ظ����������
            ErrorNeed_classesNotInClasses = 199,            //Need_classes���������Class��
            ErrorTemplateMatchingFailed = 200,              //ģ��ƥ��ʧ�ܣ�δ�ҵ���ģ��������������ģ��Ĳ�������
            ErrorTemplateAndMaskSizeNotSame = 201,          //ģ�����Ĥ�ߴ粻һ��
            ErrorRoiIsEmpty = 202,                      //��Ĥ�в����ڸ���Ȥ����
            ErrorIndexGreaterThanRegions_inSize = 203,          //��ǰ�����������������б�ķ�Χ
            ErrorNamesIsEmpty = 204,                //"names��������Ϊ��,��Ϊ��̬����ʱ,������ֻ��������1������!") =
            ErrorRegionsAndNamesSizeNotSame = 205,      //"names��������Ϊ��,��Ϊ��̬����ʱ, ����������������������������ȣ�
            ErrorRegionsAndScoreSizeNotSame = 206,      //"������scoresΪ��̬����ʱ,���������������������������ȣ�
            ErrorAngle_EndMustGreaterOrEqualAngle_start = 207,          //Angle_End������ڵ���Angle_start
            ErrorMax_grayMustGreaterThanMin_gray = 208,             //Max_gray�������Min_gray
            ErrorImgAndBoxesSizeNotSame = 209,          //ģ��ͼƬ��ģ������ߴ粻һ��
            ErrorTrainData = 210,           //ѵ��ͼƬ��ѵ����ǩ�ߴ粻һ��
            ErrorCalibration = 211,           //�궨ʧ��
            ErrorTransformationPose = 212,           //�������ת����������ʧ��
            ErrorSetTriggerMode = 213,            //���ô���ģʽʧ��
            ErrorCloseCamera = 214,               //�ر����ʧ��
            ErrorSetParamFail = 215,
            ErrorGetParamFail = 216,
            ErrorNoImage = 217,
        } ErrorCode;

} // namespace bjcv

#ifdef __cplusplus
}
#endif

#endif
