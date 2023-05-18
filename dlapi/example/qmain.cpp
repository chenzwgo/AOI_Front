#include <QtCore/QCoreApplication>
#include <QString>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <Windows.h>
#include "classification_interface.h"
#include <iostream>
#include <time.h>
#include <fstream>

int runSV(QString threadName)
{
	QString appPath = QCoreApplication::applicationDirPath();
	QString symbol_file = appPath + "/bojie/bojie_06_10/sample-symbol.json";
	QString params_file = appPath + "/bojie/bojie_06_10/sample-0120.params";
    QString fsymbol_file = appPath + "/bojie/defect/classify_defect-symbol.json";
    QString fparams_file = appPath + "/bojie/defect/classify_defect-0060.params";
	int batch_size = 64;
	int model_width = 256;
	int model_height = 256;
    int fbatch_size = 64;
    int fmodel_width = 64;
    int fmodel_height = 64;
	int model_channels = 1;
	int device_type = 2;//1 -> cpu 2 ->gpu
	int device_id = 0;

	const int len = 4;
	
	dv::DefectResultInfo defectResult[2] = { NULL };
    dv::DefectController defectController = { 0 };

	int data_end = 1;
	dv::Image image[4];
	dv::Image image2[4];
    int step = 7 * model_width / 8;

	std::vector<QString> img_namevec;
	img_namevec.push_back("0-SEQUENCE_A-camU1-2018-04-19_09-58-42.bmp");
	img_namevec.push_back("0-SEQUENCE_A-camU1-2018-04-19_09-58-42.bmp");
	img_namevec.push_back("0-SEQUENCE_A-camU1-2018-04-19_09-58-42.bmp");
	img_namevec.push_back("0-SEQUENCE_A-camU1-2018-04-19_09-58-42.bmp");
	for (int i = 0; i<len; i++)
	{
		QString  img_name =   img_namevec[i];
		QImage* m_qImg = new QImage(img_name);
		image[i].data = m_qImg->bits();
		image[i].width = m_qImg->width();
		image[i].height = m_qImg->height();
		image[i].channels = 1;
		image[i].width_step = m_qImg->bytesPerLine();
        image[i].x = 0;
        image[i].y = 0;
        image[i].origin_height = m_qImg->height();
        image[i].origin_width = m_qImg->width();
	}

	std::vector<QString> img_namevec2;
	img_namevec2.push_back("SEQUENCE_AcamU1.bmp");
	img_namevec2.push_back("SEQUENCE_BcamU1.bmp");
	img_namevec2.push_back("SEQUENCE_CcamU1.bmp");
	img_namevec2.push_back("SEQUENCE_DcamU2.bmp");
	for (int i = 0; i < len; i++)
	{
		QString  img_name = img_namevec2[i];
		QImage* m_qImg1 = new QImage(img_name);

		image2[i].data = m_qImg1->bits();
		image2[i].width = m_qImg1->width();
		image2[i].height = m_qImg1->height();
		image2[i].channels = 1;
		image2[i].width_step = m_qImg1->bytesPerLine();
        image2[i].origin_height = m_qImg1->height();
        image2[i].origin_width = m_qImg1->width();
        image2[i].x = 0;
        image2[i].y = 0;

	}

	RefineScoreHandle refiner[5], refiner_2[5];
	for (size_t i = 0; i < 5; i++)
	{
		refiner[i] = InitRefineScoreHandle();//初始化
        refiner_2[i] = InitRefineScoreHandle();
	}
	//cv::Mat imag = cv::imread("data_5_14/0-SEQUENCE_B-camU1-2018-04-19_09-58-42.bmp",cv::IMREAD_GRAYSCALE);
	ClassifierHandle handle = InitClassifier(symbol_file.toStdString().c_str(), params_file.toStdString().c_str(), batch_size, model_width, model_height, model_channels, device_type, device_id);
    ClassifierHandle fhandle = InitClassifier(fsymbol_file.toStdString().c_str(), fparams_file.toStdString().c_str(), batch_size, fmodel_width, fmodel_height, model_channels, device_type, device_id);
    std::ofstream ofs("a.txt");
	for (size_t i = 0; i <10; i++)
	{
    int data_end = 1;

	int iRet = ClassifyBandImageBasedOnThreshold_TwoDimensionOutput(handle, fhandle,refiner, 5, image, 4,step,
		0, NULL, -1.f, 1.2f, &defectController, &data_end, defectResult);
	qDebug() << "over";

    for (size_t j = 0; j < 2; j++)
    {
        for (size_t k = 0; k < defectResult[j].len; k++)
        {
            ofs << defectResult[j].defectAttr[k].type << " " << defectResult[j].defectAttr[k].num << " " << defectResult[j].defectAttr[k].rect.x << " "
                << defectResult[j].defectAttr[k].rect.y << " " << defectResult[j].defectAttr[k].rect.width << " " << defectResult[j].defectAttr[k].rect.height << std::endl;
        }
    }
    data_end = 1;
    clock_t s1 = clock();
  
	iRet = ClassifyBandImageBasedOnThreshold_TwoDimensionOutput(handle, fhandle, refiner, 5, image2, 4,step,
		0, NULL, -1.f, 1.2f, &defectController ,&data_end, defectResult);
	qDebug() << "over1";
    clock_t s2 = clock();
    std::cout << "total time: "<<s2 - s1 << std::endl;

	}

    ofs.close();

	


	ReleaseClassifier(&handle);
    ReleaseClassifier(&fhandle);
	ReleaseDefectResult(defectResult, 2);

	for (size_t i = 0; i < 5; i++)
	{
		ReleaseRefineScoreHandle(&refiner[i]);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	runSV("main thread");
	return a.exec();
}
