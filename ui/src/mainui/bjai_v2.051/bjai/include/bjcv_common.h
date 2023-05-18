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
            int point_num = 0;      // 线的点数
            PointF* points = NULL;  // 线上的点
        } LineF;

        typedef struct PolygonF_ {
            int point_num = 0;      // polygon的点数
            PointF* points = NULL;  // polygon上的点
        } PolygonF;

        typedef struct Color_ {
            Color_() {};
            Color_(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {};
            unsigned char r = 0;
            unsigned char g = 0;
            unsigned char b = 0;
        } Color;

        /*
        *data              图像的数据
        *width             图像的宽度
        *height            图像的高度
        *channels          图像的通道数
        *width_step        图像每行的字节数（bytes）
        *
        */
        typedef struct Image_ {
            unsigned char* data;                                // 图像的数据
            int width;                                          // 图像的宽度
            int height;                                         // 图像的高度
            int channels;                                       // 图像的通道数
            int width_step;                                     // 图像每行的字节数（bytes）
            bool hwc = true;                                   // 表示是否是彩色图片且rgb是分开通道保存的，即r、g、b分别为一个平面。默认是
            unsigned char* r;
            unsigned char* g;
            unsigned char* b;                            // 如果hwc为false，则数据需要分别填写到r、g、b中
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
            float xc;     // 四边形的最小外接矩形中心x坐标
            float yc;     // 四边形的最小外接矩形中心y坐标
            float width;
            float height;    // height, width分别表示四边形的最小外接矩形的2条边的长度，名称不代表大小，即width与height的大小关系不确定
            float angle;  // 表示四边形的最小外接矩形的倾斜角度


            RotatedRectF_() : xc(), yc(), width(), height(), angle() {
            }

            RotatedRectF_(float xc_, float yc_, float width_, float height_, float angle_) {
                xc = xc_;
                yc = yc_;
                width = width_;
                height = height_;
                angle = angle_;
            }
        } RotatedRectF;   // 单个缺陷的信息

        typedef struct Defect_ {
            const char* name = NULL;                          // 缺陷名称，不能超过50个字符
            RectF rect;                                       // 位置
            float area;                                       // 缺陷像素个数,用于计算面积
            int type;                                         // 类型 class
            int shape;                                        // 暂无意义
            float score;                                      // 缺陷类别得分
            const char* zone = NULL;                          // 可以存放一个zone的名字
            const char* zone2 = NULL;
            float incline_width = 0;
            float incline_length = 0;
            float contrast = 0.0;                               // 表示缺陷区域的对比度
            float delta = 0.0;                                  // 表示缺陷与周围的颜色差异
            float std_err = 0.0;                                // 表示缺陷周围的像素值标准差
            float avg = 0.0;                                    // 表示缺陷区域的平均灰度值
            int category = 0;                                   // 表示缺陷的类型，暂时不使用此属性
            int channel = -1;                                   // 表示从哪一个通道检测到缺陷
            Color color;                                        // 颜色
            const char* text = NULL;                            // 比如二维码的文本
            int num_points = 0;                                 // 代表轮廓的点数，如果为0，代表没有输出轮廓信息!
            PointF* contour = NULL;                             // 列表，用于存储缺陷的轮廓点坐标，点的个数为num_points
            bool has_mask = false;
            Image* mask = NULL;                                 // 掩膜
            bool rotated = 0;                                   // 是否含有旋转矩形信息
            RotatedRectF rot_rect;
            int int_num = 0;                                    // 备用属性，方便某些特殊的需求，在不改变接口的情况下实现输出
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
        } Defect;   // 单个缺陷的信息

        typedef struct PartResult_ {                                // 单个工件的结果信息
            int defect_num = 0;                                                     // 图片内所有的缺陷数目
            int object_num = 0;                                           //  非缺陷型目标的个数
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
        } PartResult;                                                     // 单个工件的检测结果

        typedef struct ImageResult_ {
            int part_num = 0;
            int src_num = 0;                                            // 如果是合成多通道，则有多个src。或者多个图片是同一角度不同光照，也可能被合并到一起
            PartResult* part_results = NULL;                            // 各个工件的检测结果
            RotatedRectF* part_rects = NULL;                                           // 各个工件的位置
            int* src_image_ids = NULL;                                  // 对应源图片的编号，有的检测结果来自多个图片，此时有一个id列表，比如合成多通道的情况
            unsigned long long sn = 0;                                      // 可用于记录图片的时间戳
        } ImageResult;                                                     // 单张图片的检测结果

        // 缺陷的大小，类型，位置
        typedef struct BJCVResults_ {
            int result_num = 0;
            ImageResult* all_results = NULL;
            unsigned long long sn = 0;                              // 可用于记录图片sn或者某组图片提交的回执编号
            // 以下这些是所有图片的汇总信息
            int part_num = 0;                                       
            int defect_num = 0;
            int object_num = 0;
            int point_num = 0;
            int line_num = 0;
            int rect_num = 0;
            int rot_rect_num = 0;
            int polygon_num = 0;
            int image_num = 0;             // 表示输出的图片数量                      
            int string_num = 0;
            int int_num = 0;
            int float_num = 0;
        } BJCVResults;                                                          // 一次检测的所有图片的所有缺陷信息

        // error code
        typedef enum class ErrorCode_ {
            Success = 0,        // 成功
            ErrorOpenJsonFileFailed = 1,        // config.json 打开失败
            ErrorJsonFileFormatInvalid = 2,        // config的格式错误或者参数设置不合法          
            ErrorCreateThreadFailed = 3,       // 创建线程失败
            ErrorCreateFileFailed = 4,       // 不能创建文件
            ErrorOpenFileFailed = 5,       // 打开文件失败
            ErrorDetectFailed = 6,       // 检测失败
            ErrorLoadModelFailed = 7,      // 加载模型失败
            ErrorInvalidHandle = 8,      // 无效的handle
            ErrorInvalidImageSize = 9,      // 输入图片尺寸或者通道数不合法
            ErrorInvalidDeviceInfo = 10,      // device信息无效
            ErrorOpenDeviceFailed = 11,      // 不能成功打开或者访问gpu设备
            ErrorUnknown = 12,     // 未知错误
            ErrorTimerOut = 13,     // Timer out.
            ErrorInvalidPath = 14,    // 无效路径

            ErrorUnrecognizedModelName = 15,
            ErrorModelDirNotExist = 16,
            ErrorModelNotInit = 17,
            ErrorMaskImageReadFailed = 18,
            ErrorImgNumIncorrect = 19,
            ErrorUninit = 20,  // 模型还没有成功加载
            ErrorParamsInvalid = 21, // 存在非法参数设置
            ErrorWorkpieceUnfound = 22, // 找不到工件或者roi
            ErrorMemAllocFailed = 23,  // 分配内存失败
            ErrorRunInferenceFailed = 24, // inference运行失败

            ErrorCreateNodeFailed = 101,    // 创建节点失败
            ErrorNodeNameUsed = 102,                // 节点名已被使用
            ErrorGraphPathInvalid = 103,            // 所指定的子图路径不存在
            ErrorNodePathInvalid = 104,             // 所指定的节点路径不存在
            ErrorNodeNotExist = 105,
            ErrorNodeIsRunning = 106,
            ErrorTypeNotSame = 107,                 // 设置输入链接或者参数时，参数类型不匹配
            ErrorAssignUnknown = 108,               // 参数赋值时的未知错误
            ErrorNotHasDefault = 109,               // 要求没有设置默认值的参数恢复为默认值
            ErrorAppendUnknown = 110,               // 向动态输入参数附加输入时的未知错误

            ErrorGraphOutputSrcNodeInvalid = 111,       // 图节点的输出参数指定的转发目标节点不存在
            ErrorInputLinkExist = 112,
            ErrorTargetNodeInvalid = 113,               // 图节点转发参数时的目标节点不存在
            ErrorInputLinkNotExist = 114,               // 申请移除链接时，发现没有链接
            ErrorJsonMissKey = 115,                     // json文件缺少必须的参数
            ErrorOutputFileFailed = 116,                // 打开输出文件失败
            ErrorNodeUnReady = 117,                     // 节点还不具备执行条件
            ErrorToolParamUnProvided = 118,             // tool的param还没设置
            ErrorUnSupportedPythonValue = 119,          // 传入了不受支持的python数据类型
            ErrorInputsNotYetProvided = 120,            // 还没有提供输入图片

            ErrorInputNotNumpy = 121,                   // 提供的输入图片不是ndarray
            ErrorImgNotUInt8 = 122,                     // 输入图片非uint8
            ErrorImgNotHWC = 123,                       // 输入图片不是hwc格式连续排列
            ErrorImgsNumIncorrect = 124,                // 输入图片数量不正确
            ErrorLinkSelf = 125,                        // 尝试将自己的输出连接到自己的输入
            ErrorImageNotRGB = 126,                     // 对于要求输入彩色图片的节点，输入不是RGB
            ErrorScriptFailed = 127,                    // 脚本执行错误
            ErrorNodeNumNotEnough = 128,                // 申请创建子图的节点数目太少，不能少于2个
            ErrorSubGraphFromNodeWithOuterLink = 129,   // 所指定的用于创建子图的节点中，含有来自原图外的输入或者输出
            ErrorFilepathNotExist = 130,                // 文件路径不存在

            ErrorPathNotFile = 131,                     // 指定的路径不是文件
            ErrorNotJsonFile = 132,                     // 所提供的路径并非json文件
            ErrorParamNameUsed = 133,                   // 当暴露图节点的参数时，默认生成的参数名已被占用
            ErrorParamAlreadyExposed = 134,             // 指定的参数已经暴露为图的参数了
            ErrorParamUnExposed = 135,                  // 要求取消暴露某参数，但是实际本身还未暴露
            ErrorOutputNodeInvalid = 136,               // 从输出节点取的输出参数的结果不是预期类型的
            ErrorInputEmptyVector = 137,                // 将一个vector输出连接到只接受单个元素的输入，但是实际输入的vector是空的！
            ErrorInputParamNotExist = 138,              // 指定的输入参数名称不存在
            ErrotIsAlreadyLoopStartOrEnd = 139,         // 所选节点已经是循环开始或者终点，要先取消才能重新设置
            ErrorParamNotDynamicLinked = 140,           // 输入参数没有链接动态输入，不能设置为循环输入

            ErrorLoopNameEmpty = 141,                   // 设置的循环名称为空
            ErrorNodeNotLoopStartOrEnd = 142,           // 所指定的节点并非循环开始或者终点
            ErrorNodeNotLoopEnd = 143,                  // 节点并非循环终点
            ErrorLoopMissStartEnd = 144,                // 循环缺少开始节点或者终止节点
            ErrorLoopStrcutureInvalid = 145,            // 存在非法的循环结构
            ErrorLoopNodeParamsNotProvided = 146,       // 循环节点的输入参数没有提供输入源
            ErrorLoopNodeParamSrcsNotUniformWithinOtOutLoop = 147,      // 循环内节点的输入参数的源并非一致性的来自循环内或者循环外
            ErrorTrySetUpdateModeForNonLoopNode = 148,                  // 尝试为非循环中的节点设置参数更新方式
            ErrorLoopStartInputInvalid = 149,                // 循环开始节点的输入不是vector或者含有多个数据的hobject
            ErrorVectorOutOfIndex = 150,                // 从vector中取数据的时候超出范围

            ErrorLoopEndInputInvalid = 151,             // 循环终点的输入类型非法，要求是T输入vector<T>，或者halcon的hobject进行concat
            ErrorSingleStepUseStreamline = 152,         // 在流水线模式下尝试只运行一个节点
            ErrorToolExecFailed = 153,                  // 执行tool时失败
            ErrorUnAppendableParamGetMultiInput = 154,  // 不支持appendable的参数遇到了多个不blocked的输入
            ErrorInputPreprocess = 155,                      //图像预处理的输入不符合规定                        
            ErrorInputIsNull = 156,                     //输入的参数为空
            ErrorInitInfer = 157,                        //初始化infer失败
            ErrorInitDetector = 158,          
            ErrorSetNullValueFailed = 159,              // 为参数设置空的值时失败
            ErrorReadCloud = 160,                       //读取点云失败

            ErrorSaveCloud = 161,                       //保存点云失败
            ErrorMatchCloud = 162,                       //点云配准失败
            ErrorWaitRun = 163,                         // 在执行节点之前先初始化run_err_为ErrorWaitRun，表示正在等待执行
            ErrorLoopStartNodesInConsistentBlock = 164, // 循环开始节点报告了不一致的blocked状态
            ErrorSetStaticParamForUnStaticParam = 165,  // 为不支持静态的参数设置静态值
            ErrorInvalidParamUpdateMode = 166,          // 动态参数更新模式不合法
            ErrorNotHasNodes = 167,                   //  图中没有节点
            ErrorFindCamera = 168,                   //  没有找到相机，要么是没有链接，要么就是不在同一个网段
            ErrorLinkCamera = 169,                   //  连接相机失败

            ErrorTakePhoto = 169,                   //  拍照失败，检查是否断线
            ErrorRunPhoXi = 170,                    //没有运行photoneo的相机控制软件
            ErrorOutputNodeWrongIdx = 171,             //outputtool设置了错误的idx，不从0开始，idx不连续，idx重复

            ErrorOpenSocket = 173,                  //socket服务开启失败
            ErrorCloseSocket = 174,                  //socket服务开启失败
            ErrorLinkSocket = 175,                  //连接socket服务器失败
            ErrorSendData = 176,                  //socket发送数据失败
            ErrorFindCalibrationPose = 177,       //没有找到标定板的pose

            ErrorGraphStartNodeIsGate = 178,        // 图的开始节点是分支节点
            ErrorGraphStartNodeIsLoopStart = 179,        // 图的开始节点是循环开始节点
            ErrorExposeGateNodeDataInput = 180,         // 将分支节点的数据输入暴露为图节点的参数
            ErrorInputImageMustSingleChannel = 181,     //输入图片必须为单通道
            ErrorRotational_boxesOrHorizontal_boxesBeSet = 182,      //Rotational_boxes和Horizontal_boxes必须有一个被设置
            ErrorRotational_boxesAndRotational_boxes_namesSizeNotSame = 183,        //Rotational_boxes和Rotational_boxes_names尺寸不一致
            ErrorHorizontal_boxesAndHorizontal_boxes_namesSizeNotSame = 184,        //Horizontal_boxesAndHorizontal_boxes_names尺寸不一致
            ErrorInputImageMustThreeChannels = 185,         //输入图片必须为3通道
            ErrorLower_thresholdLessThanOrEqualToUpper_threshold = 186,         //Lower_threshold必须小于等于Upper_threshold
            ErrorNames_inAndRegions_inSizeNotSame = 187,            //Names_in And Regions_in尺寸不一致
            ErrorScores_inAndRegions_inSizeNotSame = 188,           //Scores_in And Regions_in尺寸不一致
            ErrorScores_inNames_inAndRegions_inSizeNotSame = 189,           //Scores_in Names_in And Regions_in尺寸不一致
            ErrorAreaMustHaveName = 190,            //区域没有名字
            ErrorAreaNameRepeat = 191,              //存在重复的区域名
            ErrorAreaMustHaveBottomEdge = 192,          //矩形区域必须有底边
            ErrorNeed_nameNotInPatch_names = 193,           //Need_name中有区域不在Patch_names中
            ErrorImage_patchesIsEmpty = 194,            //Image_patches不能为空
            ErrorRegions1_names_inAndRegions1_inSizeNotSame = 195,              //Regions1_names_in And Regions1_in 区域和区域名尺寸不一致
            ErrorRegions1_names_inAndregions1_scores_in = 196,      //regions1_names_in与regions1_scores_in两者不为空时, 两者的长度必须相等.但是允许regions1_scores_in为空时, regions1_names_in不能为空"
            ErrorClassesRepeat = 197,               //存在重复的类别名字
            ErrorNeed_classesRepeat = 198,              //Need_classes存在重复的类别名字
            ErrorNeed_classesNotInClasses = 199,            //Need_classes存在类别不在Class中
            ErrorTemplateMatchingFailed = 200,              //模板匹配失败，未找到与模板相似区域，请检查模板的参数设置
            ErrorTemplateAndMaskSizeNotSame = 201,          //模板和掩膜尺寸不一致
            ErrorRoiIsEmpty = 202,                      //掩膜中不存在感兴趣区域
            ErrorIndexGreaterThanRegions_inSize = 203,          //当前区域索引超出区域列表的范围
            ErrorNamesIsEmpty = 204,                //"names参数不能为空,当为静态参数时,必须且只允许设置1个名称!") =
            ErrorRegionsAndNamesSizeNotSame = 205,      //"names参数不能为空,当为动态参数时, 区域数量与区域名称数量必须相等！
            ErrorRegionsAndScoreSizeNotSame = 206,      //"当分数scores为动态参数时,区域数量与区域分数数量必须相等！
            ErrorAngle_EndMustGreaterOrEqualAngle_start = 207,          //Angle_End必须大于等于Angle_start
            ErrorMax_grayMustGreaterThanMin_gray = 208,             //Max_gray必须大于Min_gray
            ErrorImgAndBoxesSizeNotSame = 209,          //模板图片和模板区域尺寸不一致
            ErrorTrainData = 210,           //训练图片和训练标签尺寸不一致
            ErrorCalibration = 211,           //标定失败
            ErrorTransformationPose = 212,           //相机坐标转机器人坐标失败
            ErrorSetTriggerMode = 213,            //设置触发模式失败
            ErrorCloseCamera = 214,               //关闭相机失败
            ErrorSetParamFail = 215,
            ErrorGetParamFail = 216,
            ErrorNoImage = 217,
        } ErrorCode;

} // namespace bjcv

#ifdef __cplusplus
}
#endif

#endif
