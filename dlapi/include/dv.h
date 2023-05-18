#ifndef DV_H_
#define DV_H_
//备注20200427之前得项目需要写0 后面的就不需要
#define MONITOR_20200427_OPEN 0
//启用新版本的过滤参数 如果是兼容旧接口，就设置成0
#define NEW_FILTER_PARAMS 1

#define DV_DEBUG_LOG_NEW_WLJ0   0  //新的调试开关
#include <string>
#include <vector>


// detect-config handle
typedef void* DetectConfigHandle;

// classify handle
typedef void* ClassifierHandle;

// multi-classify handle
typedef void* MultiClassifierHandle;

// detector handle
typedef void* DetectorHandle;

// refine score handle
typedef void* RefineScoreHandle;

// other handle
typedef void* DvHandle;

typedef void* MatchModelHandle;

typedef void* ParamSetHandle;

typedef void* HeatmapParam_1;


typedef void* EdgeSystemHandle;//边缘系统

typedef void* ROIMatchHandle;//roi匹配句柄

// monitor handle
typedef void* MonitorDbHandle;

//用于多通道联合分类
typedef void* MultiChannel_ClassifierHandle;

//过滤 句柄
typedef void* FilterHandle;

namespace dv
{

    typedef struct GapPix_ {
        int scrapIndex;       // srrapIndex 划痕索引
        int sraGapHolePix;    // 划痕 到 孔的过滤距离
        int sraGapEdgePix;    // 划痕 到 孔的过滤距离
        int chipIndex;        // chipIndex 崩边的索引
        int chipGapEdgePix;   // 崩边的到边缘的距离的索引
    } GapPix;

    // x        left top point column of the rect
    // y        left top point row of the rect
    // width    width of the rect
    // height   height of the rect
    typedef struct Rect_ {
        int x;
        int y;
        int width;
        int height;
    } Rect;

    typedef struct OcrRect_ {
        int left;
        int top;
        int right;
        int bottom;
        int id;
        float score;
    } OcrRect;

    // 定位识别的结果
    typedef OcrRect FinalOcrResults;

    // 定位识别的结果
    typedef OcrRect FinalObjectDetectResults;

    // 定位识别的结果
    typedef OcrRect ObjectDetectRect;

    typedef struct Point_ {
        int x;
        int y;
    } Point;

    typedef struct Rect4point_
    {
        Point point[4];    //RECT的四个顶点
    } Rect4point;

    typedef struct Color_ {
        unsigned char r;
        unsigned char g;
        unsigned char b;
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
    } Image;

    typedef struct DefectControler_ {
        int *size;                                          // 缺陷的过滤尺寸，即像素点数量，保留大于size的缺陷
        int *level;                                         // 缺陷的过滤强度，即对比度，保留大于level的缺陷
        bool *enable;                                       // 缺陷的检测开关，检出置为true，不检出测置为false
        Color color;                                        // 每组过滤参数均有独自的颜色标识，相同的颜色使用相同的阈值
        int num_classes;                                    // 缺陷的类别，当没有使用分类模型时，默认使用形状：点、线、面三个类别过滤
    } DefectControler;

    typedef struct DefectResults_ {
        int defectInfo[55];                                 // 0: ID, 1: R0, 2: C0, 3: R1, 4: C1
                                                            // 5: defectLen, 6: defectShape, 7: defectLevel
                                                            // 8: defectClass, 9: defectPixelNum
                                                            // 10: defectWidth, 11: defectHeight 12: defectAveryGreay 灰度值
                                                            // 13~15 gray[0], gray[1], gray[2], 16~18 contrast[0], contrast[1], contrast[2]
                                                            // 19~21 nelts[0], nelts[1], nelts[2], 22 flag 继续分类检测标志(0 ->正常多分类 1->三分类,2->四分类 3->二分类), 
                                                            // 23 崩边二分类标志位, 24模糊标志位 25 是否是点划伤候选标志位 0 不是 ，1 是点划伤候选标志位 ,26~28 gray[3], gray[4] gray[5] ,  29~31,contrast[3], contrast[4]  contrast[5]
                                                            // 32~34 nelts[3], nelts[4],nelts[5]  35 缺陷是崩边形状标志位 ,36 缺陷是否检出标志位  37,//划伤判尘点标志位 38,39,40 底部划伤异色过杀标志位,41 听筒拐角外轮廓划伤过杀标志位, //~42 合成缺陷来自于哪个通道标志位  43记录过滤前的缺陷索引号  44 defect_id ,45 缺陷过杀标志位  46 组划伤标志位（参与了组划伤的缺陷）
                                                            // 47 边缘拐角和上下区域标志位，用于分级  48 缺陷所在的位置标志位，1  内边缘，2外边缘，3 油墨区 49 缺陷是否在圆弧区 50 缺陷12宫格索引
        float class_score;                                  // current class score, 0.0-1.0
        int sec_class_index;                                // second class index
        float sec_class_score;                              // second class score, 0.0-1.0
        float detect_score;                                 // defect detection score
        float whole_class_score[20];                        // 15个类别得分
        DefectControler controler;
    } DefectResults;



    // 模型参数
    typedef struct _DLParam {
        char symbol_file[1024];                             // 模型的symbol文件路径
        char params_file[1024];                             // 模型的param文件路径
        int batch_size;                                     // GPU 一次处理的模型宽高大小的图像的个数
        int model_width;                                    // 模型的宽
        int model_height;                                   // 模型的高
        int model_channels;                                 // 模型的通道数，热度图检测采用1通道，分类采用3通道。
        int device_type;                                    // 1 -> cpu 2 ->gpu
        int device_id[2];                                   // 设备id ,单GPU默认是0，双GPU是 0,1
    } DLParam;

    typedef struct HoleDistortDefectResults_ {
        int roi_point_size;                                 // 当前roi的点的个数
        int module_point_size;                              // 标准模型roi的point的个数
        float m_intensity;                                  // 衡量圆孔变形的大小
        bool have_test;                                     // 是否已经圆孔检测
        Point *points;                                      // detect_roi
        Point *module_points;                               // 对应的标准模型的roi
        bool is_edge;                                       // 该ROI区域是否是最外围
    } DistortDefectResults;

    typedef struct WLJ_Param_ {                             // 算法要求得参数
        int edge[4];                                        // 0:上边到视窗区距离; 1:右边到视窗区距离; 2:底边到视窗区距离; 3:左边到视窗区距离;

                                                            // 边缘信息映射到D通道参数
        int shiftX;                                         // shiftX:x方向位移; 
        int shiftY;                                         // shiftY:y方向位移
        float x_rate_zoom;                                  // x_rate_zoom:x方向的缩放参数
        float y_rate_zoom;                                  // y_rate_zoom:y方向的缩放参数
        float sin_in;                                       // 旋转度的正玄值  
        int out_edge_offset[2][8];                          // 前一个是B通道，后一个是D通道 外边缘偏移量 0:上边; 1:右边; 2:下边; 3:左边; 4:左上角圆弧; 5:右上角圆弧; 6:右下角圆弧; 7左下角圆弧;
        int distortion[2];                                  // distortion[0]右边y方向扭曲参数; distortion[1]:左边y方向扭曲参数
    } WLJ_Param;

    typedef struct ROIRect {
        int flag; //正面/反面标志位
        dv::Rect rect;
    }ROIRect;

    typedef struct GlassPos_
    {
        dv::Point point;  //顶点
        float angle;   //角度 偏移角度(顺时针为正，逆时针为负);
    }GlassPos;

    typedef struct BJInputParam {
        int glass_index;                                    // 0 白玻璃，1 丝印玻璃
        bool bCutImage;                                     // 是否切图
        Image* image;                                       // 输入图像的数据
        int num_of_images;                                  // 输入图像的数量 白玻璃是7，丝印玻璃是7;

        DLParam *dlParam;                                   // dlParam 模型的参数
        int dlParam_size;                                   // 模型的个数
        int data_end;                                       // 图像数据传入结束标志位 *data_end = 1 数据传入结束 *data_end = 0 数据传入未结束 （用于分块处理）
        int step;                                           // 切图步长
        int iDelaytime;                                     // 定时超时响应时间
        int bStart_timer;                                   // 0 -> 不启动定时器， 1-> 启动定时器

        float defect_thresh;                                // 得分过滤阈值
        float heat_thresh[2];                               // 热度图阈值 heat_thresh[0] 边缘区域 heat_thresh[1] 视窗区域
        float area_ratio;                                   // 面积占比
        int left_right_offset;                              // 左右偏移量
        int internal_offset[2];                             // 丝印玻璃视窗区域缩放系数 internal_offset[0] C通道   internal_offset[1] D通道
        int outer_offset[2];                                // 丝印玻璃外接矩形外扩偏移量outer_offset[0] C通道  outer_offset[1] D通道
        int mfilterSize;                                    // 过滤大小
        int mfilterLevel;                                   // 过滤强度
        int dst_thr0;                                       // 连通域上阈值
        int dst_thr1;                                       // 连通域下阈值
        int cut_Image_index;                                // ABC或ABCE 选择的切图通道数。因为D通道是默认切图的，可以不管。
        int match_image_index;                              // ABC或ABCE 选择的匹配的通道数。因为D通道是默认切图的，可以不管。

        WLJ_Param  wlj_param;                               // 算法需要输入的参数
        int gray_val_control[4];                            // 灰度值控制参数 0 ->A 1->BEF 2->C 3->D
        int contrast_val_control;                           // 对比度控制参数 对ABCD

        int contrast_Area;                                  //对比度面积
        int isLessThan;                                       //对比度的阈值，用来过滤划伤


        int Special_defectLevel;                           // 对比度控制参数 对ABCD

        int edge_area_control[4];                           // 边角区域偏移控制参数
        DefectControler defect_controller[14];              // 缺陷过滤,白玻璃用defect_controller[0~11] ，丝印玻璃用defect_controller[0~13]

        RefineScoreHandle* refiner;                         // 过滤句柄
        int refiner_len;                                    // 句柄长度



        bool mark_offset;                                   // mask 线是否偏移  true 偏移  false 不偏移
        bool letter_defect;                                 //字符是否破损 true 破损  false 不破损
        bool hole_miss_print;                               //圆孔漏印 true 漏印 false 不漏印

        int deformation_thresh[4];                          //窄边阈值 [0] 左边阈值 [1] 右边阈值 [2] 上阈值 [3] 下阈值

        ClassifierHandle heat_handle[8];                    // 白玻璃 用heat_handle[0],heat_handle[2]边缘检测模型，heat_handle[1]，heat_handle[3]，丝印玻璃heat_handle[0]，heat_hanle[3] 边缘abce 和heat_hanle[1] ，heat_hanle[4] window abcde和 heat_handle[2] ,heat_handle[5] 边缘 d
        ClassifierHandle classify_handle[14];               // 白玻璃 用classify_handle[0]，classify_handle[1],（双GPU模式）丝印玻璃classify_handle[0]和classify_handle[2]用于ABCE通道和classify_handle[1]和classify_handle[3]用于D通道 （双GPU模式）

        ClassifierHandle heat_supply_handle[8];                    // 补充,白玻璃 用heat_handle[0],heat_handle[2]边缘检测模型，heat_handle[1]，heat_handle[3]，丝印玻璃heat_handle[0]，heat_hanle[3] 边缘abce 和heat_hanle[1] ，heat_hanle[4] window abcde和 heat_handle[2] ,heat_handle[5] 边缘 d
        ClassifierHandle classify_supply_handle[14];

        int slikClassifySize;
        int PolytomousSize;

        // 匹配参数
        bool bMatchShape;
        char image_set_c_path[1024];                        // 边沿缺陷匹配文件路径
        char image_set_d_path[1024];                        // 边沿缺陷匹配文件路径
 
        int Match_Channels;                                 // 匹配中的标准图像的通道 

        Rect roi_rect[4];                                   // 用于保存ROI数据 第一组ABCEF ，或 ABCEFG 第二组D  //(C通道 3d 水平 上矩形 roi_rect[0] 3d 水平 下矩形 roi_rect[1] D 通道 3d 水平 上矩形 roi_rect[2] 3d 水平 下矩形 roi_rect[3] ) 
        int gpuMode;                                        // GPU检测模式，gpuMode 1 单GPU 2 双GPU 模式

        ROIMatchHandle c_match;                             // c通道匹配句柄
        ROIMatchHandle d_match;                             // d通道匹配句柄
        Point* window_point[2];                             // 视窗区点集 window_point[0]-> abc ,window_point[1]->d
        int window_point_size[2];                           // 视窗区点集的长度window_point_size[0] ->abc window_point_size[1] ->d 
        Point* edge_point[2];                               // 白玻璃外边沿点集 edge_point[0]-> abc ,edge_point[1]->d ,丝印玻璃的内轮廓的点集 edge_point[0]-> abce ,edge_point[1]->d
        int edge_point_size[2];                             // 白玻璃外边沿点集的长度,外边沿点集edge_point_size[0] ->abc 外边沿点集edge_point_size[1] ->d ,丝印玻璃的内轮廓点集的长度 内边沿点集edge_point_size[0] ->abce 内边沿点集edge_point_size[1] ->d
        Point* extern_edge_point[2];                        // 丝印玻璃的外轮廓点集 extern_edge_point[0] ->abce 外轮廓点集 extern_edge_point[1] ->d 外轮廓点集
        int extern_edge_point_size[2];                      // 丝印玻璃的外轮廓点集长度  内边沿点集extern_edge_point_size[0] ->abce 内边沿点集extern_edge_point_size[1] ->d

        int defect_distance;                                // 缺陷距离边缘的距离
        int defect_lenth;                                   // 缺陷的长度
        int mark_offset_thresh;                                //偏移阈值
        int group_scratch_thresh;                            //组划伤联通距离阈值
        float damaged_rate;                                    //破损率阈值
         

        bool bTwoClassification_window;                     // 是否启用二分类视窗区脏污划伤分类算法
        bool bThreeClassification;                          // 是否启用三分类边缘拐角正常脏污划伤分类算法
        bool bFourClassification;                           // 是否启用四分类边缘四周正常脏污毛发划伤崩边分类算法
        bool bFourClassification_window;                    // 是否启用四分类视窗区域模糊脏污划伤分类算法
        bool bRippleWater;                                  // 是否启用水波纹检测
        bool bUseDChannel;                                  // 是否水平方向丝印启用D通道

        int directional_joint_width[4][2];                  // 方向联合宽度 directional_joint_width[0][0]->A left directional_joint_width[0][1] ->A right directional_joint_width[1][0]->B left directional_joint_width[2][0]->C left directional_joint_width[3][0]->D left  
        int directional_joint_height[4][2];                 // 方向联合高度 directional_joint_height[0][0]->A left directional_joint_height[1][0]->B left directional_joint_height[2][0]->C left directional_joint_height[3][1]->D right
        int coefficient;                                    // 变形系数
        int coefficient_offset;                             // 变形系数偏移量
        char EdgePointSetTrainingPicturePath[1024];         // 标准图json路径
        int ripple_edge_lenth[4];                           // 水波纹检测起始终止长度 ripple_edge_lenth[0]->左边起始长度，ripple_edge_lenth[1]->左边终止长度，ripple_edge_lenth[2]->右边边起始长度，ripple_edge_lenth[3]->右边终止长度，
#if 1

        int vtc_side_range , contrast_thr , sqr_thr;
        //对于 bump缺陷设置的参数 (fsk专用)
        int m_thresh;// 对比度阈值 m_thresh;
        int n_thresh;// 对比度阈值 n_thresh;
        int x_thresh; // 对比度阈值 x_thresh;

        ////内缩指定像素（组成的区域）用于气泡判断算法 (fsk专用)
        int h_top_thresh;  //上
        int h_bottom_thresh;  //下
        int h_left_thresh;   //左
        int h_right_thresh;  //右

        //外边缘内缩像素值 ，用于消除划伤过杀
        int top_outer_edge_retraction_thresh;  //上
        int bottom_outer_edge_retraction_thresh;   //下
        int left_outer_edge_retraction_thresh;     //左
        int right_outer_edge_retraction_thresh;   //右

        //左右竖边，在白玻璃中用于压痕消过杀 ,丝印玻璃中用于 漏光强改边漏光
        int top_indentation_thresh; //上边阈值
        int bottom_indentation_thresh;//下边阈值
        int left_indentation_thresh; //左边阈值
        int right_indentation_thresh; //右边阈值

        //指定ROI区域检测
        dv::ROIRect* input_detect_roi; //输入的模板图上的roi区域
        int input_detect_roi_size; //输入的roi个数
        dv::ROIRect* output_detect_roi;//输出的当前产品的检测roi区域
        int output_detect_roi_size; //输出的roi个数。

        EdgeSystemHandle label_c_edge_system_; //正面
        EdgeSystemHandle label_d_edge_system_; //反面

        //计算相对坐标
        //正面的玻璃的顶点信息
        GlassPos c_pos;
        //反面的玻璃的顶点信息
        GlassPos d_pos;
		
		//对比度控制系数
		float contrast_ratio;

        //输出玻璃的最小外接矩形。
#endif  
        // 消除全局变量
        HeatmapParam_1    stage1_heatmapParam;
        EdgeSystemHandle c_edge_system_;                    // c通道的边缘系统     内部使用
        EdgeSystemHandle c_internal_edge_system_;           // c通道的内边缘系统    内部使用
        EdgeSystemHandle d_edge_system_;                    // d通道的边缘系统      内部使用
        EdgeSystemHandle d_internal_edge_system_;           // D通道的内边缘系统     内部使用

        EdgeSystemHandle chip_edge_system_;                 // 检测崩边的边缘系统  内部使用

        EdgeSystemHandle new_para_contrl_;                     // 新增加的控制参数列表 ，来自wlj， 内部使用
        EdgeSystemHandle model_match_c_;                        //C图匹配模型
        EdgeSystemHandle model_match_d_;                     //d图匹配模型 

    } BJInputParam;


    typedef struct _gHistogram {
        int gray_value; //灰度值
        int pixel_num;  //像素个数
    } gHistogram;

    typedef struct _DefectAttr {
        Rect rect;                                          // 位置
        int num;                                            // 缺陷像素个数,用于计算面积
        int level;                                          // 缺陷强度
        int type;                                           // 类型 class
        int shape;                                          // 缺陷形状 0->点，1->线 ，2->面
        int width;                                          // defect width  厚度
        int height;                                         // defect height 长度
        float score;                                        // 缺陷类别得分
        float detect_score;                                 // 缺陷检测得分
        int vague_flag;                                     // 模糊标志位。只针对脏污划伤，0 不模糊 1 脏污划伤模糊 //临时测试 当作是否检出标志位
        int average_gray;                                   // 缺陷的灰度值
        int gray[6];                                        // 白玻璃 五合一后 缺陷的灰度值,只对五合一结果有，单通道无效，gray[0] ->A, gray[1] ->B,gray[2] ->C  gray[3] ->E gray[4] ->F    丝印6合一  abcefg
        int contrast[6];                                    // 白玻璃 五合一后 缺陷的对比度 只对五合一结果有效，contrast[0] ->A, contrast[1] ->B,contrast[2] ->C   contrast[3] ->E,contrast[4] ->F     丝印6合一  abcefg
        int defect_area[6];                                 // 白玻璃 五合一后 缺陷的面积， 只对五合一结果有效，defect_area[0] -> A ,defect_area[1] -> B ,defect_area[2] -> C defect_area[3] -> E ,defect_area[4] -> F  丝印6合一  abcefg
        float class_score[15];                              // 缺陷的10类得分 对白玻璃  丝印 15类 
        int position;                                       // 缺陷在哪个区域  0 边缘区域 1 视窗区域  
        int detect_flag;                                    // 缺陷是否要检出，（通过svm分级算法确定）
        gHistogram* histogram;                              // 灰度直方图信息，单通道缺陷有效，对多合一的缺陷默认为0。
        int histogram_size;                                 // 灰度直方图数据长度
        int defect_id;                                      // 单通道的缺陷对应合成缺陷的id
        int scratch_defect_area_flag;                       // 在边缘拐角和上下区域的划伤的标志是1 ，其他的是0
        int grid_num;                                       // 12宫格索引。
        int diagonal;                                       // 输出外接矩形框的对角线
    } DefectAttr;


    // 缺陷的大小，类型，位置
    typedef struct _DefectResultInfo {
        DefectAttr* defectAttr;
        int len;                                            // 长度
    } DefectResultInfo;

    // 目标多分类的结果
    typedef struct FinalMultiClassifyResults_ {
        int category_size;                                  // 类别的个数
        float *category_score;                              // 类别的得分
        int max_category;                                   // 得分最高的类别
    } FinalMultiClassifyResults;

    //缺陷图片分类的属性
    typedef struct RippleResultInfo_ {
		int defect_type;											//缺陷类型 0 水波纹 1 大压痕 
		int* channels_index;								// 每个框的所在的通道索引
        int rect_size;                                      // 多少个缺陷检测框
        Rect *rect;                                         // 每个框的位置
        FinalMultiClassifyResults* finalMultiClassifyResult;// 每个缺陷检测框的类别判断结果
    } RippleResultInfo;

    typedef struct NarrawOrDeformation_{

        dv::Rect rect;// 变形或者窄边位置
		int channels;//通道数， 指明E或者F通道变形或者窄边位置(每个框位置都有一个指定的通道) 
		int type;// 1 变形 2 窄边
    }NarrawOrDeformation;

	typedef struct DeformationInfo_{
		NarrawOrDeformation* narrawDeformation; //缺陷数据
		int narrawDeformation_size;
        int edge_width[4];//窄边左右厚度 [0] 左边厚度，[1] 右边厚度  [2] 上边厚度 [3] 下边厚度
	}DeformationInfo;

    typedef struct FinalDefectResults_ {
        int defect_size;                                    // 缺陷精度尺寸，4×4或8×8
        int defect_class;                                   // 缺陷类别，当没有使用分类模型时，值为-1
        int defect_shape;                                   // 缺陷形状：点、线、面
        int defect_level;                                   // 缺陷对比度（即缺陷强度）
        int defect_pixel_num;                               // 缺陷像素点数量（即缺陷面积）
        int defect_width;                                   // 缺陷宽度
        int defect_height;                                  // 缺陷高度
#if NEW_FILTER_PARAMS
        int defect_average_contrast;                        // 缺陷对比度平均值
        int defect_midian_contrast;                         // 缺陷对比度中位值
        int defect_max_contrast;                            // 缺陷对比度最大值
        
        int defect_perimeter;                               // 缺陷周长
        int defect_relative_length;                         // 缺陷相对长度
        int defect_relative_thickness;                      // 缺陷相对厚度
        int defect_connect_span;                            // 缺陷连通域跨度
        float defect_connect_score;                         // 缺陷连通域得分
        float defect_first_class_score;                     // 缺陷一类得分
        //int defect_condition_id;                          // 缺陷过滤条件id
        char defect_condition_id[30];                       // 缺陷过滤条件id
#endif
        int point_size;                                     // 缺陷格子点数量
        Point *points;                                      // 缺陷格子点坐标，左上角
        Point center_point;                                 // 缺陷中心点坐标
        Rect defect_rect;                                   // 缺陷最小外接矩形
#if MONITOR_20200427_OPEN
        int score_size;                                     // 分类的类别数
        float *class_score;                                 // 分类类别的得分
#endif

    } FinalDefectResults;




    typedef struct FinalTobaccoClassifyResults_ {
        //(一个图像数据返回的结果)
        int model_size;                                     // 模型数量
        FinalMultiClassifyResults *model_result;            // 每个模型返回的结果
    } FinalTobaccoClassifyResults;

    typedef struct SliceTobaccoInfo_
    {//(一个图像数据返回的结果)
        int point_size;                                     // 轮廓点的数量
        Point *point_xy;                                    // 轮廓点
        unsigned char *img_data;                            // 图像数据地址（在接口外面已分配好内存）
        Rect slice_rect;                                    // 切图的边框
    } SliceTobaccoInfo;

    typedef struct Image_jud_ {
        int image_num;                                      // 图像数量
        int *finish_flag;                                   // 图像信息传输完成标志,传输完成后置0
    } Image_jud;

    typedef struct LedInputParam_ {
        MultiClassifierHandle multiClassifierHandle;        // 多分类句柄
        DLParam dlParam;                                    // 模型参数
    } LedInputParam;

    //led 输出结果结构体
    typedef struct LedResult_ {
        int rect_size;                                      // 多少个led
        Rect *rect;                                         // 每个led 的外接矩形
        FinalMultiClassifyResults* finalMultiClassifyResult;// 每个led的类别判断结果
    } LedResult;

    typedef struct DetectRegion_ {
        int detect_region_point_size;
        int *detect_region_point_index;
        Point *detect_region_point;
        DefectControler *defect_controler;
    } DetectRegion;

    typedef struct CleanRegion_ {
        int clean_region_point_size;
        int *clean_region_point_index;
        Point *clean_region_point;
    } CleanRegion;

    typedef struct ImageDetectInfo_
    {
        Image *org_image;
        DetectRegion *detect_region;
        CleanRegion *clean_region;
    } ImageDetectInfo;

    typedef struct TotalFinalDefectResult_
    {
        int final_defect_results_size;
        FinalDefectResults* final_defect_results;
    } TotalFinalDefectResults;

    struct DefectParams {
        float clean_thresh;
        int connect_thresh_low;
        int connect_thresh_high;
        float detect_step_scale;
        int filter_level;
        int pass_class_index;
        float pass_class_threshold;
        DefectControler defect_controler;
    };

    // error code
    enum ErrorCode {
        ErrorNone                               = 0,        // No error.

        ErrorOpenJsonFileFailed                 = 1,        // The json file can not open.
        ErrorJsonFileFormatInvalid              = 2,        // The json file has invalid format.
        ErrorOpenImageFileFailed                = 3,        // Image can not open.
        ErrorOpenFolderFailed                   = 4,        // Folder can not open.
        ErrorCreateFolderFailed                 = 5,        // Create folder failed.
        ErrorCreateProgressFileFailed           = 6,        // Create progress file failed.
        ErrorSampleImageNeedAtLeastOne          = 7,        // Sample image need at least one.
        ErrorCreateLabelImageFailed             = 8,        // Create label image failed.
        ErrorInvalidInputParam                  = 9,        // Invalid input parameter.
        ErrorCreateTestingTimeFileFailed        = 10,       // Create testing time file failed.

        ErrorPrepareTrainImageSetFailed         = 11,       // Prepare train image set failed, train images must larger than 0.
        ErrorPrepareValImageSetFailed           = 12,       // Prepare val image set failed, val images must larger than 0.
        ErrorPrepareSampleFailed                = 13,       // Prepare sample failed, can't create any samples.

        ErrorFineTuningModelTypeMismatching     = 17,       // Fine tuning model type mismatching.
        ErrorFineTuningModelSizeMismatching     = 18,       // Fine tuning model size mismatching.
        ErrorFineTuningModelChannelMismatching  = 19,       // Fine tuning model channel mismatching.
        ErrorFineTuningDefectSizeMismatching    = 20,       // Fine tuning model defect size mismatching.
        ErrorCreateTrainProcessFailed           = 21,       // Create the training process failed.
        ErrorTerminateTrainProcessFailed        = 22,       // Terminate the training process failed.
        ErrorValidateProcessTerminated          = 23,       // Validate process terminated.
        ErrorTestProcessTerminated              = 24,       // Test process terminated.
        ErrorCreateThreadFailed                 = 25,       // Create thread failed.

        ErrorCreateResultTxtFailed              = 31,       // Create result txt failed.
        ErrorOpenResultTxtFailed                = 32,       // Result txt can not open.
        ErrorCreateHeatMapTxtFailed             = 33,       // Create heat map txt failed.
        ErrorCreateTrainingListFailed           = 34,       // Create training list failed.
        ErrorDecryptStringFailed                = 35,       // Decrypt string failed.
        ErrorEncryptStringFailed                = 36,       // Encrypt string failed.
        ErrorInvalidTrainingListFile            = 37,       // Invalid training list file.
        ErrorDelDataFolderFailed                = 38,       // Del folder failed.

        ErrorSampleModelTypeMismatching         = 41,       // Fine sample image model type mismatching.
        ErrorSampleModelSizeMismatching         = 42,       // Fine sample image model size mismatching.
        ErrorSampleModelChannelMismatching      = 43,       // Fine sample image model channel mismatching.
        ErrorSampleDefectSizeMismatching        = 44,       // Fine sample image model defect size mismatching.
        ErrorSampleLableModelMismatching        = 45,       // Fine sample image label model mismatching.
        ErrorClassifySampleModelSizeMismatching = 46,       // Fine classify sample image model size mismatching.
        ErrorClassifySampleModelChannelMismatching = 47,       // Fine classify  sample image model channel mismatching.

        ErrorDetectFailed                       = 51,       // Detect failed, out of memory.

        ErrorInitClassifierFailed               = 101,      // Init classifier failed.
        ErrorInitRefineScoreFailed              = 102,      // Init refine score failed.
        ErrorInitClassifierDefectFailed         = 103,      // Init classifier defect failed.

        ErrorWrongHeatMapModelSize              = 104,      // Wrong heat map model size.
        ErrorWrongHeatMapModelChannel           = 105,      // Wrong heat map model channel.
        ErrorWrongClassifyModelSize             = 106,      // Wrong classify model size.
        ErrorWrongClassifyModelChannel          = 107,      // Wrong classify model channel.

        ErrorInvalidHandle                      = 110,      // Invalid handle.
        ErrorInvalidModelChannels               = 111,      // Invalid model channels.
        ErrorInvalidImageSize                   = 112,      // Invalid image size, can't process images smaller than model size.
        ErrorInvalidTrainMode                   = 113,      // Invalid train mode.
        ErrorInvalidTestMode                    = 114,      // Invalid test mode.

        ErrorInvalidSampleMode                  = 115,      // Invalid sample mode.
        ErrorInvalidModelShape                  = 116,      // Invalid model shape.
        ErrorInvalidSsdBoundsShape              = 117,      // Invalid ssd's Bounds shape.

        ErrorAddDefectToImageFailed             = 120,      // Add defect to image failed.
        ErrorExtraDefectToImageFailed           = 121,      // Extra defect to image failed.

        ErrorImageAndLabelSizeNotMatch          = 150,      // Image size and label size does not match.
        ErrorGPUDataDoesNotMatchModel           = 151,      // GPU data does not match existing models.
        ErrorCutForeGroundAreaFailed            = 152,      // Get a valid foreground area in the big picture failed.
        ErrorImageAndLabelNumberNotMatch        = 153,      // Image and label number does not match.

        ErrorInvalidDeviceInfo                  = 200,      // Invalid device info.
        ErrorInvalidSuperDogInfo                = 201,      // Invalid super dog info.
        ErrorDeviceInfoNotMatch                 = 202,      // Device info not match.

        ErrorInitHoleDistortDetectFailed_1      = 297,      // The input parameter error.
        ErrorInitHoleDistortDetectFailed_2      = 299,      // Init the distort module failed.

        ErrorInitDistortDetect                  = 400,      // Init the distort handle failed.
        ErrorResultPointOutRange                = 401,      // Disort detect or RegionalCalibration result point have reset.

        ErrorInitRegionalCalibration            = 450,      // Init the handle of Regional Calibration failed.
        ErrorGetRegionContour                   = 460,      // Get region contour failed.

        ErrorTemplateMatchingFailed             = 470,      // Template matching failed.

        ErrorNumberModelsSetFail                = 500,      // The number of detect models must larger than 0.

        ErrorSaveDvImageFailed                  = 600,      // Save dv image failed.

        ErrorGetGPUMemoryInfoFailed             = 700,      // Get GPU memory information failed.

        ErrorUnknown                            = 1001,     // Unknown error.

        ErrorTimerOut                           = 2000,     // Timer out.
        ErrorCreateMatImageFailed               = 2001,     // CreateMatImageFailed.
        ErrorConvertToGPUDataFailed             = 2002,     // ConvertGPUDataFailed.
        ErrorInvalidGPUData                     = 2003,     // InValidGPUData.
        ErrorModelOutputNotMatch                = 2004,     // Model Output Not Match.
        ErrorInvalidPatchList                   = 2005,     // Invalid PatchList.
        ErrorInvalidGlassIndex                  = 2006,     // Invalid classIndex.
        ErrorInvalidDeviceIdFormat              = 2007,     // Invalid device id.
        ErrorWaterRippleDetectionFailed         = 2008,     // Detect waterripple failed.
        ErrorInValidMatchImageSizeOrDetectImageSize = 2009, // Size inconsistency between detection and matching graphs.
        ErrorTraditionalAlgoFailed              = 2010,     // wlj traditional algorithm failed
        ErrorInconsistentCategories             = 2011,     //视窗区域和边缘区域分类类别不一致
        ErrorInValiadParamSize                  = 2012,     //传入的BJInputParam 的size 是非法的。
        ErrorContourFailed                      = 2050,     //提取轮廓异常的错误码

        ErrorDB                                 = 10000,    // 数据库操作失败
        ErorInvalidPath                         = 10001,    // 无效路径
        ErrorCalcDefaultParamsFailed            = 10002     // 计算默认参数失败
    };

    // defect mode
    enum DefectMode {
        DefectStandard                          = 0,        // Standard mode. 该模式是旧版本的，可以直接使用Fast模式。
        DefectFast                              = 1,        // Fast mode. 该模式检测速度比标准模式快，当有过滤roi时，检测结果与标准模式有略微差别。
        DefectGlass                             = 2,        // Glass mode. 该模式只用于手机玻璃产品。
        DefectCap                               = 3         // Capacitance mode. 该模式只用于电容产品。
    };

    // train type
    enum TrainType {
        TrainHeatMap                            = 0,        // Heat map.
        TrainDefectClassify                     = 1,        // Defect classification.
        TrainMultiClassify                      = 2         // Multi classification.
    };

    // sample mode
    enum SampleMode {
        SampleTexture                           = 0,        // Texture sample.
        SamplePure                              = 1         // Pure sample.
    };

    enum JsonConfigType {
        DetectBaseType                          = 0,        // base config file
        ReClassifyDetectType                    = 1         // reclassify config file
    };
} // namespace dv



#endif // DV_H_
