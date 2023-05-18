#include "bjai.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
using namespace std;


/*
说明：这个文件是bjai库的使用方法示例文件。
bjai库一共提供3个api函数，分别是init_model（初始化模型）、inspect（检测图片）、uninit_model（注销模型）。
注意事项：(1) 输出检测结果的形式是通过传入bjai::DefectResultInfo results的指针，将该结构体中的检测结果指针指向bjai内部分配的固定空间。
                     该空间中的数据会在下一次运行时被覆盖，因此请对results中的内容处理完毕后再开始下一次检测。
                (2) 如果遇到strcpy编译警告，在项目属性-C++-命令行中添加/D_CRT_SECURE_NO_WARNINGS
                (3)  bjai V0.1\bjai\third_party和bjai V0.1\bjai\bin\release这两个目录需要添加到path
              （4）模型路径暂时只支持相对路径，可以把模型文件夹放到工作目录下
输出数据的格式：参考bjai_common.h中的DefectResultInfo结构体的注释。
*/
cv::Mat mark_defects(cv::Mat img, bjai::DefectResultInfo results, std::vector<std::string> defect_names, float resize_width = 480, int idx = 0) {
    if (1 == img.channels()) {
        cv::cvtColor(img, img, cv::COLOR_GRAY2RGB);
    }
    float ratio = (float)resize_width / img.cols;
    int new_h = img.rows * ratio;
    cv::resize(img, img, cv::Size(resize_width, new_h));
    for (int i = 0; i < results.count; i++) {
        auto defect = results.imgs_defect_results[idx].defectAttr[i];
        int xmin = ratio * results.imgs_defect_results[idx].defectAttr[i].rect.x;
        int ymin = ratio * results.imgs_defect_results[idx].defectAttr[i].rect.y;
        int xmax = xmin + ratio * results.imgs_defect_results[idx].defectAttr[i].rect.width;
        int ymax = ymin + ratio * results.imgs_defect_results[idx].defectAttr[i].rect.height;
        int type = results.imgs_defect_results[idx].defectAttr[i].type;
        std::string defect_name = results.imgs_defect_results[idx].defectAttr[i].name;
        double fNumber = defect.class_score;

        stringstream ss;

        ss << fNumber << flush;

        string sNumber = ss.str();
        defect_name += sNumber;
        cv::rectangle(img, cv::Point(xmin, ymin), cv::Point(xmax, ymax), cv::Scalar(0, 0, 255), 1);
        cv::putText(img, defect_name, cv::Point(xmin, ymin), cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1);
    }    
    return img;
}

int main()
{
    // std::string img_path = "./test_images/foreign_color.bmp";
    // std::string img_path = "./test_images/2.4.bmp";
    // std::string img_path = "./test_images/ink2.jpg";
    //std::string img_path = "./test_images/detect/1.png";
    //std::string img_path = "./test_images/glass frame images/1.bmp";
    // std::string img_path = "C:\\Users\\xiaogaobo\\Desktop\\earpod20200729\\2827Wduoliao4.bmp";
    std::string img_path = "C:\\xiaogaobo\\dev\\bjAI\\bjAItest\\test_images\\connector_imgs\\3623.bmp";
    std::vector<std::string> defect_names = {"edge", "excess"};
    int n = 1;

    bjai::Image* imgs_ptr;
    imgs_ptr = new bjai::Image[n];
    cv::Mat img_raw = cv::imread(img_path.c_str(), cv::IMREAD_UNCHANGED);
    cv::Mat img;
    if (img_raw.channels() == 3) {
        cv::cvtColor(img_raw, img, cv::COLOR_BGR2RGB);
    }
    else if (img_raw.channels() == 1) {
        img = img_raw;
    }
    else {
        throw std::exception("image channels must be 1 or 3!");
    }
    for (int i = 0; i < n; i++) {
        (imgs_ptr)[i].data = (unsigned char*)img.data;
        (imgs_ptr)[i].width = img.cols;
        (imgs_ptr)[i].height = img.rows;
        (imgs_ptr)[i].channels = img.channels();
        (imgs_ptr)[i].width_step = img.step;
    }
    


    std::cout << "Hello World!\n";
    bjai::version();
    std::string model_dir = "..\\..\\bjai\\models";
    bjai::ModelHandle my_model;
    
    bjai::DefectResultInfo results;
    bjai::ErrorCode init_success = bjai::init_model(model_dir.c_str(), &my_model);
    if (init_success == bjai::ErrorCode::Success) {
        for (int i = 0; i < 1; i++) {
            cout << i << endl;
            bjai::ErrorCode run_success = bjai::inspect(imgs_ptr, n, &results, my_model);
            if (bjai::ErrorCode::Success == run_success) {
                cv::Mat img_marked = mark_defects(img_raw, results, defect_names, 640);
                cv::namedWindow("result");
                cv::imshow("result", img_marked);
                cv::waitKey();
            }
            else {
                std::cout << "inspect returned with error code: " << int(run_success) << std::endl;
            }
        }        
                
        bjai::uninit_model(my_model);
    }
    else {
        std::cout << "init model failed !!! errorcode: " << int(init_success) << std::endl;
    }    
    delete[]imgs_ptr;
    return 0;
}


