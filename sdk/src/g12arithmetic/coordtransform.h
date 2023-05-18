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
	CoordTransform();  //    (像素/mm)
	~CoordTransform();

	// 计算坐标偏移
	int calcOffset(unsigned char* pSrc, int width, int height, int &x, int &y, double &angle);
	// 初始化坐标偏移
	int initOffset(int x, int y, double angle);
	// 坐标换算
	void map(int x, int y, int &tx, int &ty);

private:

	// 找边线
	bool findEdge(Mat &imgSrc, const Rect &rect, Vec4f &line, bool bLeft);

	// 计算线到线的距离
	float distancLineToLine(Vec4f &line1, Vec4f &line2);

	// 计算点到线的距离
	float distancPointToLine(Point2f &point, Vec4f &line);

	// ROI确定
	Rect getRoi(unsigned char* srcImage);

	// 获取中间X
	int getHPos(Mat pic, int rowCount);

	// 获取中间Y
	int getVPos(Mat pic, int colCount);

	// 获取小图roi
	Rect getSmallRoi(Point center, int width = 600, int height = 600);

	// 画ROI
	void drawRoi(Mat &pic, const Rect &rect);

	// 找圆
	void findCircle(Mat &imgSrc, const Rect &rect, Vec4f &line);

private:
	Mat m_resultImg;    // 用来显示结果的图，绘制显示结果

	// 图像的宽度和高度,像素
	int m_imageWidth;
	int m_imageHeight;

	Rect m_roi;
	RMatrix m_matrix;
};



#endif 