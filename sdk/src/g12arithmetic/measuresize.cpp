//#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>

#include <iostream>  
#include <vector>  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>
#include <stdio.h>

#include <math.h>

//#include <QtGui/qbitmap.h>
//#include <QtWidgets/qlabel.h>

#include "measuresize.h"

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

#define TEST_SHOW     1      // 画出图像

using namespace std;
using namespace cv;

//////////////////////////////////////////

MeasureSize::MeasureSize(int width, int height, int x1, int y1, int x2, int y2)
{
	m_imageWidth = width;
	m_imageHeight = height;

	m_dPixelsPerMetricH = 1.0;
	m_dPixelsPerMetricV = 1.0;

	if (x2 == -1)
	{
		x2 = m_imageWidth-1;
	}
	if (y2 == -1)
	{
		y2 = m_imageHeight-1;
	}

	m_roi.x = x1;
	m_roi.y = y1;
	m_roi.width = x2 - x1+1;
	m_roi.height = y2 - y1+1;
}

MeasureSize::~MeasureSize()
{
}

int MeasureSize::calcSize(unsigned char* srcImage)
{
	int printWidth = 70;
	Rect bigRoi = getRoi(srcImage);
	if (bigRoi.width <= 0 || bigRoi.height <= 0)
	{
		return -1;
	}
	Mat imgSrc = Mat(m_imageHeight, m_imageWidth, CV_8UC1, srcImage, m_imageWidth);

	// 中部(左)
	Point centerML(bigRoi.x + printWidth, bigRoi.y + bigRoi.height / 2);
	Rect rectML = getSmallRoi(centerML);
	Vec4f lineML_1;
	bool bRet = findEdge(imgSrc, rectML, lineML_1, true);
	if (!bRet) return -1;

	Vec4f lineML_2;
	bRet = findEdge(imgSrc, rectML, lineML_2, false);
	if (!bRet) return -1;

	// 中部(右)
	Point centerMR(bigRoi.x + bigRoi.width - printWidth, bigRoi.y + bigRoi.height / 2);
	Rect rectMR = getSmallRoi(centerMR);
	Vec4f lineMR_1;
	bRet = findEdge(imgSrc, rectMR, lineMR_1, true);
	if (!bRet) return -1;

	Vec4f lineMR_2;
	bRet = findEdge(imgSrc, rectMR, lineMR_2, false);
	if (!bRet) return -1;

	float distance_ML = distancLineToLine(lineML_1, lineML_2);  //左丝印宽度
	float distance_MR = distancLineToLine(lineMR_1, lineMR_2);  //右丝印宽度
	float distance_M_Width = distancLineToLine(lineML_1, lineMR_2); // 玻璃宽度
	m_sizes["D"] = distance_ML;
	m_sizes["E"] = distance_M_Width;
	m_sizes["F"] = distance_MR;

	// 测量上端距离
	// 上部(左)
	Point centerTL(bigRoi.x + printWidth, bigRoi.y + bigRoi.height / 4);
	Rect rectTL = getSmallRoi(centerTL);
	Vec4f lineTL_1;
	bRet = findEdge(imgSrc, rectTL, lineTL_1, true);
	if (!bRet) return -1;

	Vec4f lineTL_2;
	bRet = findEdge(imgSrc, rectTL, lineTL_2, false);
	if (!bRet) return -1;

	// 上部(右)
	Point centerTR(bigRoi.x + bigRoi.width - printWidth, bigRoi.y + bigRoi.height / 4);
	Rect rectTR = getSmallRoi(centerTR);
	Vec4f lineTR_1;
	bRet = findEdge(imgSrc, rectTR, lineTR_1, true);
	if (!bRet) return -1;

	Vec4f lineTR_2;
	bRet = findEdge(imgSrc, rectTR, lineTR_2, false);
	if (!bRet) return -1;

	float distance_TL = distancLineToLine(lineTL_1, lineTL_2);  //左丝印宽度
	float distance_TR = distancLineToLine(lineTR_1, lineTR_2);  //右丝印宽度
	float distance_T_Width = distancLineToLine(lineTL_1, lineTR_2); // 玻璃宽度
	m_sizes["A"] = distance_TL;
	m_sizes["B"] = distance_T_Width;
	m_sizes["C"] = distance_TR;

	// 测量下端距离
	// 下部(左)
	Point centerDL(bigRoi.x + printWidth, bigRoi.y + bigRoi.height * 3 / 4);
	Rect rectDL = getSmallRoi(centerDL);
	Vec4f lineDL_1;
	bRet = findEdge(imgSrc, rectDL, lineDL_1, true);
	if (!bRet) return -1;
	Vec4f lineDL_2;
	bRet = findEdge(imgSrc, rectDL, lineDL_2, false);
	if (!bRet) return -1;

	// 下部(右)
	Point centerDR(bigRoi.x + bigRoi.width - printWidth, bigRoi.y + bigRoi.height * 3 / 4);
	Rect rectDR = getSmallRoi(centerDR);
	Vec4f lineDR_1;
	bRet = findEdge(imgSrc, rectDR, lineDR_1, true);
	if (!bRet) return -1;
	Vec4f lineDR_2;
	bRet = findEdge(imgSrc, rectDR, lineDR_2, false);
	if (!bRet) return -1;

	float distance_DL = distancLineToLine(lineDL_1, lineDL_2);  //左丝印宽度
	float distance_DR = distancLineToLine(lineDR_1, lineDR_2);  //右丝印宽度
	float distance_D_Width = distancLineToLine(lineDL_1, lineDR_2); // 玻璃宽度
	m_sizes["G"] = distance_DL;
	m_sizes["H"] = distance_D_Width;
	m_sizes["I"] = distance_DR;

	//CIRCLE TEST BEGIN
	Point centerCircle(bigRoi.x + bigRoi.width / 2, bigRoi.y + printWidth);
	Rect rectCircle = getSmallRoi(centerCircle, 800, 800);
	Vec4f lineCircle;
	findCircle(imgSrc, rectCircle, lineCircle);
	m_sizes["J"] = lineCircle[2];
	//CIRCLE TEST END 

	//TEST BEGIN------------------------------------------------------------------------
	//Mat show;
	//cv::cvtColor(imgSrc, show, COLOR_GRAY2RGB);

	// 画玻璃ROI
	//drawRoi(show, bigRoi);

	// 画找线ROI
	//vector<Rect> roiList = {rectML,rectMR,rectTL,rectTR,rectDL,rectDR};
	//for (int i = 0; i < roiList.size();i++)
	//{
	//	drawRoi(show, roiList[i]);
	//}

	// 画找出的线
	//vector<Vec4f> lineList = { lineML_1,lineML_2,/*lineMR_1 ,*/lineMR_2 ,lineTL_1 ,lineTL_2 ,lineTR_1 ,lineTR_2 ,lineDL_1 ,lineDL_2 ,lineDR_1,lineDR_2 };
	//for (int i = 0; i < lineList.size();i++)
	//{
	//	line(show, Point2f(lineList[i][0], lineList[i][1]), Point2f(lineList[i][2], lineList[i][3]), Scalar(0, 0, 255), 1);
	//}

	//circle(show, Point2f(lineCircle[0], lineCircle[1]), lineCircle[2]/2, Scalar(0, 0, 255), 1);
	//TEST END---------------------------------------------------------------------------
	return 0;
}

int MeasureSize::getResultPix(map<string, double> &result)
{
	result = m_sizes;
	return 0;
}


int MeasureSize::getResultmm(map<string, double> &result)     // 计算结果，换算成mm
{
	//   用设定的标定值调用，换算成mm
	map<string, double>::iterator iter = m_sizes.begin();
	while (iter != m_sizes.end())
	{
		string key = iter->first;
		double value = iter->second*m_dPixelsPerMetricH;
		result[key] = value;
		iter++;
	}
	return 0;
}

int MeasureSize::setmmPerPix(double dPixelsPerMetricH, double dPixelsPerMetricV)   /// 设置标定值
{
	m_dPixelsPerMetricH = dPixelsPerMetricH;
	m_dPixelsPerMetricV = dPixelsPerMetricV;
	return 0;
}

bool MeasureSize::findEdge(Mat &imgSrc, const Rect &rect, Vec4f &line, bool bLeft)
{
	// roi区域小图
	Mat imgRoi = imgSrc(rect);
	// roi往右偏移1像素
	Rect rectR = rect;
	rectR.x += 1;
	Mat imgRoiOffset = imgSrc(rectR);
	Mat imgGradient;
	Mat imgThread;

	if (bLeft)
	{
		// 两图相减，得出梯度最大点
		cv::compare(imgRoi, imgRoiOffset+30,imgThread,cv::CMP_GT);
	}
	else
	{
		// 两图相减，得出梯度最大点
		cv::compare(imgRoiOffset, imgRoi + 30, imgThread, cv::CMP_GT);
	}

	// 找到边缘轮廓
	vector<vector<Point>> contours;
	int nOffset = bLeft ? 0 : 0;
	findContours(imgThread, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(nOffset, 0));

	// 提取轮廓点
	vector<Point> points;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size()>100)
		{
			points.insert(points.end(), contours.at(i).begin(), contours.at(i).end());
		}
	}

	// 点太少，放弃
	if (points.size() < 50)
	{
		return false;
	}

	// 线性拟合
	Vec4f outLine;
	fitLine(points, outLine, DIST_HUBER, 0, 0.01, 0.01);

	// 找出直线两端点
	double cos_theta = outLine[0];
	double sin_theta = outLine[1];
	double x0 = outLine[2], y0 = outLine[3];

	if (fabs(cos_theta) > 0.000001)
	{
		double k = sin_theta / cos_theta;
		double b = y0 - k * x0;
		double y1 = rect.y;
		double x1 = (y1 - rect.y - b) / k + rect.x;
		double y2 = rect.y + rect.height;
		double x2 = (y2 - rect.y - b) / k + rect.x;

		line[0] = x1;
		line[1] = y1;
		line[2] = x2;
		line[3] = y2;
	}
	else
	{
		line[0] = x0 + rect.x;
		line[1] = rect.y;
		line[2] = x0 + rect.x;
		line[3] = rect.y + rect.height;
	}

	// 画线	
	//cv::line(imgSrc, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(255, 255, 255));

	return true;
}

float MeasureSize::distancPointToLine(Point2f &point, Vec4f &line)
{
	float x0 = point.x;
	float y0 = point.y;
	float x1 = line[0];
	float y1 = line[1];
	float x2 = line[2];
	float y2 = line[3];
	return fabs((y1 - y2)*x0 - (x1 - x2)*y0 + y2*x1 - y1*x2) / sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

Rect MeasureSize::getRoi(unsigned char* srcImage)
{
	if (m_roi.x+m_roi.width>m_imageWidth)
	{
		return Rect();
	}

	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	int rowcount = 100;
	// 左边X
	Mat hline = Mat(m_imageHeight, m_imageWidth, CV_8UC1, srcImage).rowRange(m_imageHeight / 2 - rowcount / 2, m_imageHeight / 2 + rowcount / 2);
	Mat hLineCopy = hline.clone();
	hLineCopy.colRange(0,m_roi.x) = 255;
	hLineCopy.colRange(m_roi.x+m_roi.width-1, m_imageWidth) = 255;
	Mat picBinary;
	threshold(hLineCopy, picBinary, 50, 255, THRESH_BINARY_INV);
	x1 = getHPos(picBinary, rowcount);

	// 右边X
	Mat picBinaryR;
	// 水平翻转
	flip(picBinary, picBinaryR, 1);
	int x2R = getHPos(picBinaryR, rowcount);
	x2 = m_imageWidth - x2R;

	// 上边Y
	Mat vline = Mat(m_imageHeight, m_imageWidth, CV_8UC1, srcImage).colRange(m_imageWidth / 2 - rowcount / 2, m_imageWidth / 2 + rowcount / 2);
	Mat vLineCopy = vline.clone();
	vLineCopy.rowRange(0, m_roi.y) = 255;
	vLineCopy.rowRange(m_roi.y + m_roi.height-1, m_imageHeight) = 255;

	Mat picBinaryV;
	threshold(vline, picBinaryV, 50, 255, THRESH_BINARY_INV);
	y1 = getVPos(picBinaryV, rowcount);

	// 下边Y
	Mat picBinaryVR;
	// 垂直翻转
	flip(picBinaryV, picBinaryVR, 0);
	int y2R = getVPos(picBinaryVR, rowcount);
	y2 = m_imageHeight - y2R;

	return Rect(x1, y1, x2 - x1, y2 - y1);
}

int MeasureSize::getHPos(Mat pic, int rowCount)
{
	double minValue = 0, maxVlue = 0;
	Point minPos, maxPos;
	vector<int> poslist;
	Mat picMask(pic.size(), pic.type(), Scalar::all(0));
	for (int i = 0; i < rowCount; i += 10)
	{
		picMask = 0;
		picMask.row(i) = 255;
		minMaxLoc(pic, &minValue, &maxVlue, &minPos, &maxPos, picMask);
		poslist.push_back(maxPos.x);
	}
	int listSize = poslist.size();
	if (listSize == 0) return 0;

	std::sort(poslist.begin(), poslist.end());
	return poslist.at(listSize / 2);
}

int MeasureSize::getVPos(Mat pic, int colCount)
{
	double minValue = 0, maxVlue = 0;
	Point minPos, maxPos;
	vector<int> poslist;
	Mat picMask(pic.size(), pic.type(), Scalar::all(0));
	for (int i = 0; i < colCount; i += 10)
	{
		picMask = 0;
		picMask.col(i) = 255;
		minMaxLoc(pic, &minValue, &maxVlue, &minPos, &maxPos, picMask);
		poslist.push_back(maxPos.y);
	}

	int listSize = poslist.size();
	if (listSize == 0) return 0;

	std::sort(poslist.begin(), poslist.end());
	return poslist.at(listSize / 2);
}

Rect MeasureSize::getSmallRoi(Point center, int width/*=600*/, int height/*=1000*/)
{
	int x1 = center.x - width / 2;
	if (x1 < 0) x1 = 0;
	int x2 = center.x + width / 2;
	if (x2 > m_imageWidth - 1) x2 = m_imageWidth - 1;
	int y1 = center.y - height / 2;
	if (y1 < 0) y1 = 0;
	int y2 = center.y + height / 2;
	if (y2 > m_imageHeight - 1) y2 = m_imageHeight - 1;
	return Rect(x1, y1, x2 - x1, y2 - y1);
}

void MeasureSize::drawRoi(Mat &pic, const Rect &rect)
{
	line(pic, Point(rect.x, rect.y), Point(rect.x, rect.y + rect.height), Scalar(255, 0, 0), 2);
	line(pic, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y), Scalar(255, 0, 0), 2);
	line(pic, Point(rect.x + rect.width, rect.y + rect.height), Point(rect.x, rect.y + rect.height), Scalar(255, 0, 0), 2);
	line(pic, Point(rect.x + rect.width, rect.y + rect.height), Point(rect.x + rect.width, rect.y), Scalar(255, 0, 0), 2);
}

void MeasureSize::findCircle(Mat &imgSrc, const Rect &rect, Vec4f &line)
{
	// roi区域小图
	Mat imgRoi = imgSrc(rect);
	Mat imgEdges;
	Canny(imgRoi, imgEdges, 80, 150);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgEdges, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	cv::RotatedRect circleRect;
	for (int i = 0; i < contours.size(); i++)
	{
		if (contours[i].size()>100)
		{
			cv::Rect boundRect  = boundingRect(contours[i]);
			if (abs(boundRect.width-boundRect.height)<50)
			{
				cv::RotatedRect rotateRect = minAreaRect(contours[i]);
				if (abs(rotateRect.size.width - rotateRect.size.height)<50
					&& abs(rotateRect.size.width - boundRect.width)<50
					&& abs(rotateRect.size.height - boundRect.height)<50
					&& rotateRect.size.width>100)
				{
					cv::RotatedRect circleRect = fitEllipse(contours[i]);
					Point2f vertices[4];
					circleRect.points(vertices);//外接矩形的4个顶点
					for (int i = 0; i < 4; i++)//绘制外接矩形
					{
						cv::line(imgEdges, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0));
					}
					line[0] = rect.x + circleRect.center.x;
					line[1] = rect.y + circleRect.center.y;

					// 90度附近，width为垂直边
					if (abs(circleRect.angle-90)<40)
					{
						line[2] = circleRect.size.height;
						line[3] = circleRect.size.width;
					}
					else
					{
						line[2] = circleRect.size.width;
						line[3] = circleRect.size.height;
					}				
					
					break;
				}
			}
		}
	}
}

float MeasureSize::distancLineToLine(Vec4f &line1, Vec4f &line2)
{
	float xMid = (line1[0] + line1[2]) / 2;
	float yMid = (line1[1] + line1[3]) / 2;

	float distance = distancPointToLine(Point2f(xMid, yMid), line2);
	return distance;
}

