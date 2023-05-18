//#include "stdafx.h"
#ifndef _COORDTRANSFORM_H 
#define _COORDTRANSFORM_H

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
#include "rmatrix.h"

#include <stdio.h>
using namespace std;
using namespace cv;

class  CoordTransform
{
public:
	CoordTransform();  //    (����/mm)
	~CoordTransform();

	// ��������ƫ��
	int calcOffset(unsigned char* pSrc, int width, int height, int &x, int &y, double &angle);
	// ��ʼ������ƫ��
	int initOffset(int x, int y, double angle);
	// ���껻��
	void map(int x, int y, int &tx, int &ty);

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
	Mat m_resultImg;    // ������ʾ�����ͼ��������ʾ���

	// ͼ��Ŀ�Ⱥ͸߶�,����
	int m_imageWidth;
	int m_imageHeight;

	Rect m_roi;
	RMatrix m_matrix;
};



#endif 