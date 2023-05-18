
#include <QtCore/QCoreApplication>
#include <QString>
#include <QtGui/QImage>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <vector>
#include <Windows.h>
#include "CLog.h"
#include "classification_interface.h"


int runSV1(QString threadName)
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString heat_symbol = appPath + "/bojie/up/defection/defect-symbol.json";
    QString heat_params = appPath + "/bojie/up/defection/defect-0120.params";

    QString defect_symbol = appPath + "/bojie/up/classify/defect_classify-symbol.json";
    QString defect_params = appPath + "/bojie/up/classify/defect_classify-0120.params";

    QString heat_symbol_d = appPath + "/bojie/down/defection/defect-symbol.json";
    QString heat_params_d = appPath + "/bojie/down/defection/defect-0114.params";

    QString defect_symbol_d = appPath + "/bojie/down/classify/defect_classify-symbol.json";
    QString defect_params_d = appPath + "/bojie/down/classify/defect_classify-0118.params";
    /**********************************************************************/

    int batch_size = 64;
    int model_width = 256;
    int model_height = 256;
    int defect_model_width = 64;
    int defect_model_height =64;
    int model_channels = 1;//检测
    int defect_model_channels = 3;//分类
    int device_type = 2;//1 -> cpu 2 ->gpu
    int device_id = 0 ;


    int arr[2][20] = { { 125,5,125,4,125,10,125,5,125,3,125,5,0,0,0,0,125,3,0,0 },//第一组 ABCE
    { 175,5,175,5,175,5,175,5,175,3,125,5,175,5,175,5,125,5,175,5 } };//第二组 D
    dv::DefectControler defectControler[2];
    int class_num = 10;
    for (size_t i = 0; i < 2; i++)
    {
        defectControler[i].num_classes = class_num;//代表10个缺陷类别 0是正常，1是脏污，2是毛发，3是尘点，4是划伤，5是崩边 6 dent，7 漏光，8 异墨，9 异色。
        defectControler[i].size = new int[class_num];
        defectControler[i].level = new int[class_num];
        defectControler[i].enable = new bool[class_num];
        for (size_t j = 0; j < defectControler[i].num_classes; j++)
        {
            defectControler[i].size[j] = arr[i][2 * j];
            defectControler[i].level[j] = arr[i][2 * j + 1];
            if (j != 0) {
                defectControler[i].enable[j] = true;//显示
            }
            else {
                defectControler[i].enable[j] = false;//不显示
            }
        }

    }
    dv::Image image[5];
    dv::DefectResultInfo defectResult[6] = { NULL };
 
    //初始化 log
    initLogger(LOG_DEBUG | LOG_ERROR | LOG_WARNING, "C:/Users/60205/");

    dv::Rect rect[2];
    int step = model_width*7/8;
    int data_end = 1;

    std::vector<QString> img_namevec;

    qDebug() << "----->" << threadName << "< -----" << " InitRefineScoreHandle Start" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    RefineScoreHandle refiner[5];
    for (size_t i = 0; i < 5; i++)
    {
        refiner[i] = InitRefineScoreHandle();//鍒濆鍖?
    }
    qDebug() << "----->" << threadName << "< -----" << " InitRefineScoreHandle End" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");

    img_namevec.push_back("image/procuct(46)-SEQUENCE_A_15-32-17_rawImge.bmp");
    img_namevec.push_back("image/procuct(46)-SEQUENCE_B_15-32-17_rawImge.bmp");
    img_namevec.push_back("image/procuct(46)-SEQUENCE_C_15-32-17_rawImge.bmp");
    img_namevec.push_back("image/procuct(46)-SEQUENCE_D_15-32-17_rawImge.bmp");
    img_namevec.push_back("image/procuct(46)-SEQUENCE_E_15-32-17_rawImge.bmp");



    for (int i = 0;i<5;i++)
    {
        QString  img_name = appPath+ "/"+img_namevec[i];
        QImage* m_qImg = new QImage(img_name);

        image[i].data = m_qImg->bits();

        image[i].width =  m_qImg->width();
        image[i].height = m_qImg->height();
        image[i].channels = 1;
        image[i].origin_height = m_qImg->height();
        image[i].origin_width = m_qImg->width();
        image[i].x = 0;
        image[i].y = 0;
        image[i].width_step =  m_qImg->bytesPerLine();

        if (i == 1) {
            CutImage(&refiner[0], &image[i], 4, 20, rect[0]);
        }
        if (i == 3) {
            CutImage(&refiner[0], &image[i], 4, 20, rect[1]);
        }
    }

    qDebug()<< "----->" << threadName <<"< -----"  << " InitClassifier Start" <<QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    ClassifierHandle heat_handle = InitClassifier(heat_symbol.toStdString().c_str(), heat_params.toStdString().c_str(), batch_size, model_width, model_height, model_channels, device_type, device_id);//鍒濆鍖栫儹搴﹀浘
    ClassifierHandle defect_handle = InitClassifier(defect_symbol.toStdString().c_str(), defect_params.toStdString().c_str(), batch_size, defect_model_width, defect_model_height, defect_model_channels, device_type, device_id);//鍒濆鍖栫儹搴﹀浘

    ClassifierHandle heat_d_handle = InitClassifier(heat_symbol_d.toStdString().c_str(), heat_params_d.toStdString().c_str(), batch_size, model_width, model_height, model_channels, device_type, device_id);//鍒濆鍖栫儹搴﹀浘
    ClassifierHandle defect_d_handle = InitClassifier(defect_symbol_d.toStdString().c_str(), defect_params_d.toStdString().c_str(), batch_size, defect_model_width, defect_model_height, defect_model_channels, device_type, device_id);//鍒濆鍖栫儹搴﹀浘
    if (heat_handle == nullptr || defect_handle == nullptr || heat_d_handle == nullptr || defect_d_handle ==nullptr) {
        qDebug() << " init failed";
        return -1;
    }
    //qDebug()<< "----->" << threadName <<"< -----"  << " InitClassifier End" <<QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");


    qDebug()<< "----->" << threadName <<"< -----"  << " ClassifyBandImageBasedOnThreshold_TwoDimensionOutput Start" <<QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
    
    for (size_t i = 0; i < 2; i++)
    {
        clock_t start_t, end_t;
        start_t = clock();
        //鍒涘缓涓�涓畾鏃跺櫒銆?
        int start_timer = 0;
        int ret = ClassifySiYinImageBasedOnThreshold(heat_handle, heat_d_handle, defect_handle, defect_d_handle, refiner, 5, image, 5, step,
            2, rect, -1.0f, 3.0f, defectControler, &data_end, defectResult, start_timer,6000,10,3,24,24);
        if (ret == dv::ErrorTimerOut) {
            qDebug() << "detect time out" ;
            qDebug() << clock() - start_t ;
            continue;
        }

        end_t = clock();
        qDebug() << "i= " << i << "API cost time : " << end_t - start_t;
        qDebug() << "=============================================" ;
    }
    qDebug()<< "----->" << threadName <<"< -----"  << " ClassifyBandImageBasedOnThreshold_TwoDimensionOutput End" <<QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");

    for (size_t i = 0; i < 5; i++)
    {
        delete[] image[i].data;
        image[i].data = NULL;
    }
    closeLogger();
    ReleaseDefectResult(defectResult, 6);
    ReleaseClassifier(&heat_handle);
    ReleaseClassifier(&heat_d_handle);
    ReleaseClassifier(&defect_handle);
    ReleaseClassifier(&defect_d_handle);
    for (size_t i = 0; i < 5; i++)
    {
        ReleaseRefineScoreHandle(&refiner[i]);
    }

    qDebug()<< "----->" << threadName <<"< -----"  << " ClassifyBandImageBasedOnThreshold_TwoDimensionOutput End" <<QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");

    return 0;
}

class WorkerThread : public QThread
{

public:

        WorkerThread(QString strthreadName):m_strthreadName(strthreadName){}
        void run() override {
            QString result;
            runSV1(m_strthreadName);
        }
private:
    QString m_strthreadName;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    runSV1("main thread");

    return a.exec();
}



