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
	MeasureSize(int width, int height, int x1 = 0, int y1 = 0, int x2=-1, int y2=-1);  //    (像素/mm)

	int calcSize(unsigned char* srcImage); // 计算结果
	int getResultPix(map<string,double> &result);       // 获取结果  
	int getResultmm(map<string, double> &result);     // 获取结果，换算成mm
	int setmmPerPix(double dPixelsPerMetricH, double dPixelsPerMetricV);    /// 设置标定值

	~MeasureSize();
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
	// 测出来的结果用map字典保存：
	// mySizes["A"], mySizes["B"], mySizes["C"], mySizes["E"], mySizes["F"],mySizes["G"], ......  
	// 表示A,B,C,E,F,G...各段的尺寸，单位：mm
	map<string, double> m_sizes;
	Mat m_resultImg;    // 用来显示结果的图，绘制显示结果

	double m_dPixelsPerMetricH;  // 水平方向一个像素代表多少mm
	double m_dPixelsPerMetricV;

	// 图像的宽度和高度,像素
	int m_imageWidth;
	int m_imageHeight;

	Rect m_roi;
};



#endif 