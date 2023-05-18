
#include <QtCore/QCoreApplication>
#include <QString>
#include <QtGui/QImage>
#include <QThread>
#include <QDebug>
#include <QDateTime>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "CLog.h"
#include "classification_interface.h"


int runSV4(QString threadName)
{
	QString appPath = QCoreApplication::applicationDirPath();
	dv::BJInputParam bjInputParam;
	const char* config_file = "D:/config/config.json";
	dv::DefectResultInfo defectResult[6] = { NULL };
	InitBJParam(&bjInputParam, config_file);
	//≥ı ºªØ log
	initLogger(LOG_DEBUG | LOG_ERROR | LOG_WARNING);
	std::vector<QString> img_namevec;

	img_namevec.push_back("image/procuct(46)-SEQUENCE_A_15-32-17_rawImge.bmp");
	img_namevec.push_back("image/procuct(46)-SEQUENCE_B_15-32-17_rawImge.bmp");
	img_namevec.push_back("image/procuct(46)-SEQUENCE_C_15-32-17_rawImge.bmp");
	img_namevec.push_back("image/procuct(46)-SEQUENCE_D_15-32-17_rawImge.bmp");
	img_namevec.push_back("image/procuct(46)-SEQUENCE_E_15-32-17_rawImge.bmp");
	bjInputParam.image = new dv::Image[5];
	for (int i = 0; i<5; i++)
	{
		QString  img_name = appPath + "/" + img_namevec[i];
		QImage* m_qImg = new QImage(img_name);


		bjInputParam.image[i].data = m_qImg->bits();
		bjInputParam.image[i].width = m_qImg->width();
		bjInputParam.image[i].height = m_qImg->height();
		bjInputParam.image[i].channels = 1;
		bjInputParam.image[i].origin_height = m_qImg->height();
		bjInputParam.image[i].origin_width = m_qImg->width();
		bjInputParam.image[i].x = 0;
		bjInputParam.image[i].y = 0;
		bjInputParam.image[i].width_step = m_qImg->bytesPerLine();
	}


	int count = 0;
	for (size_t i = 0; i < 2; i++)
	{
		clock_t start_t, end_t;
		start_t = clock();
		for (size_t k = 0; k < 5; k++)
		{
			setImageName(std::to_string(count).c_str(), 5, k);
			count++;
		}

		int start_timer = 0;
		int ret = classifySiyinDetect(&bjInputParam, defectResult);
		std::ofstream ofs("defect.info", std::ios::app);
		for (size_t j = 0; j < 5; j++)
		{
			for (size_t k = 0; k < defectResult[j].len; k++)
			{
				ofs << "j = " << j << " " << "k = " << k << " " << defectResult[j].defectAttr[k].type << std::endl;
			}
		}
		ofs.close();

		if (ret == dv::ErrorTimerOut) {
			qDebug() << "detect time out";
			std::cout << "detect time out" << std::endl;
			//qDebug() << clock() - start_t;
			continue;
		}



		end_t = clock();
		qDebug() << "i= " << i << "API cost time : " << end_t - start_t;
		qDebug() << "=============================================";
	}
	qDebug() << "----->" << threadName << "< -----" << " ClassifyBandImageBasedOnThreshold_TwoDimensionOutput End" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");



	qDebug() << "----->" << threadName << "< -----" << " ClassifyBandImageBasedOnThreshold_TwoDimensionOutput End" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz ");
	UnInitBJParam(&bjInputParam,defectResult);
	return 0;
}

class WorkerThread : public QThread
{

public:

	WorkerThread(QString strthreadName) :m_strthreadName(strthreadName) {}
	void run() override {
		QString result;
		runSV4(m_strthreadName);
	}
private:
	QString m_strthreadName;
};

int main11(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	runSV4("main thread");

	return a.exec();
}



