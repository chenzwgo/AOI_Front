#include "staticcorrection.h"
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>

using namespace cv;

CFastStaticCorrection::CFastStaticCorrection()
{
	m_imageWidth = 0;
	m_imageHeight = 1;
	m_factors = NULL;
}


CFastStaticCorrection::~CFastStaticCorrection()
{
}

void CFastStaticCorrection::ReleaseStatCorr()
{
	if (m_factors != NULL)
	{
		delete[] m_factors;
	}
}

/*函数：获取矫正后图像的参数
输出参数：
maxGray ： 校正后图像最大值
minGray ：校正后图像最小值
mean ：校正后图像均值
devi ：校正后图像方差
返回：0，获取成功，-1，获取失败
*/
int CFastStaticCorrection::GetStatParam(unsigned char *image, int imageWidth, int imageHeight, unsigned char &maxGray, unsigned char &minGray, double &mean, double &devi)
{
	Mat imgSrc(imageHeight, imageWidth, CV_8U, image);
	double minVal = 0.0;
	double maxVal = 0.0;
	minMaxIdx(imgSrc, &minVal, &maxVal);
	maxGray = maxVal;
	minGray = minVal;

	Mat means, stddev;
	meanStdDev(imgSrc.row(0), means, stddev);//计算src图片的均值和标准差
	double sd = 0;
	mean = means.at<double>(0, 0);
	devi = stddev.at<double>(0, 0);
	return 0;
}



/********************************************** Float*******************************************************/
int CFastStaticCorrection::Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol,float *factors)
{
	int i;
	int j;
	int index;
	int NO = 1;
	float gv = standardGv;	
	if (endLine >= imageHeight) endLine = imageHeight -1;
	//memset(factors, 0, sizeof(float)*imageWidth); 

	Mat imgData(imageHeight, imageWidth, CV_8U, data);
	for (int i = startCol; i < endCol; i++)
	{
		Mat col = imgData(Rect(i, startLine, 1, endLine - startLine));
		Scalar meanVal = mean(col);
		if (meanVal[0] > 0)
		{
			factors[i] = gv / meanVal[0];
		}
		else
		{
			factors[i] = 1;
		}
	}	

	return 0;
}

int CFastStaticCorrection::InitStatCorr(int imageWidth, int imageHeight, float *factors)
{
	ReleaseStatCorr();

	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;

	m_factors = new float[m_imageWidth*m_imageHeight];

	int i, j;
	float *p = m_factors;
	for (i = 0; i < imageHeight; i++)
	{		
		memcpy(p, factors, sizeof(float)*imageWidth);		
		p += imageWidth;
	}

	return 0;
}

int CFastStaticCorrection::StatCorr(unsigned char *image, unsigned char *correctImage)
{
	Mat imgSrc(m_imageHeight, m_imageWidth, CV_8U, image);
	Mat imgDst(m_imageHeight, m_imageWidth, CV_8U, correctImage);
	Mat imgSrcF;
	imgSrc.convertTo(imgSrcF, CV_32F);
	Mat mapFactors(m_imageHeight, m_imageWidth, CV_32F, m_factors);
	Mat imgDstF = imgSrcF.mul(mapFactors);
	imgDstF.convertTo(imgDst, CV_8U);
	return 0;
}