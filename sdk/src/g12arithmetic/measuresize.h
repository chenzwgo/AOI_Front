//#include "stdafx.h"
#ifndef _MEASURESIZE_H 
#define _MEASURESIZE_H

#include <stdio.h>
#include <tchar.h>

#include <iostream>  
#include <vector>  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <map>
#include "common_data.h"

#include <stdio.h>
using namespace std;
using namespace cv;

class  MeasureSize
{
public:
	MeasureSize(int width, int height, int x1 = 0, int y1 = 0, int x2=-1, int y2=-1);  //    (����/mm)

	int calcSize(unsigned char* srcImage); // ������
	int getResultPix(map<string,double> &result);       // ��ȡ���  
	int getResultmm(map<string, double> &result);     // ��ȡ����������mm
	int setmmPerPix(double dPixelsPerMetricH, double dPixelsPerMetricV);    /// ���ñ궨ֵ

	~MeasureSize();
private:

	// �ұ���
	bool findEdge(Mat &imgSrc, const Rect &rect, Vec4f &line, bool bLeft);

	// �����ߵ��ߵľ���
	float distancLineToLine(Vec4f &line1, Vec4f &line2);

	// ����㵽�ߵľ���
	float distancPointToLine(Point2f &point, Vec4f &line);

	// ROIȷ��
	Rect getRoi(unsigned char* srcImage);

	// ��ȡ�м�X
	int getHPos(Mat pic, int rowCount);

	// ��ȡ�м�Y
	int getVPos(Mat pic, int colCount);

	// ��ȡСͼroi
	Rect getSmallRoi(Point center, int width = 600, int height = 600);

	// ��ROI
	void drawRoi(Mat &pic, const Rect &rect);

	// ��Բ
	void findCircle(Mat &imgSrc, const Rect &rect, Vec4f &line);

private:
	// ������Ľ����map�ֵ䱣�棺
	// mySizes["A"], mySizes["B"], mySizes["C"], mySizes["E"], mySizes["F"],mySizes["G"], ......  
	// ��ʾA,B,C,E,F,G...���εĳߴ磬��λ��mm
	map<string, double> m_sizes;
	Mat m_resultImg;    // ������ʾ�����ͼ��������ʾ���

	double m_dPixelsPerMetricH;  // ˮƽ����һ�����ش������mm
	double m_dPixelsPerMetricV;

	// ͼ��Ŀ�Ⱥ͸߶�,����
	int m_imageWidth;
	int m_imageHeight;

	Rect m_roi;
};



#endif 