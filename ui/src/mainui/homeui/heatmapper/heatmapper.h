﻿#ifndef _HEATMAPPER_H_
#define _HEATMAPPER_H_

#include <QVector>


QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

class GradientPalette;



class HeatMapper
{
public:
    HeatMapper(QImage *image, GradientPalette *palette, int radius, int opacity);
	virtual ~HeatMapper();

	void initData(const QVector<int> &data);
	void clearData();
	void save(const QString &fname);
	void addPoint(int x, int y);
	void setPalette(GradientPalette *palette);
	int  getCount(int x, int y);
	void colorize(int x, int y);
	void colorize();


	virtual void drawAlpha(int x, int y, int count, bool colorize_now = true);
	
protected:
	virtual void colorize(int left, int top, int right, int bottom);
	void redraw();
	
private:
	int increase(int x, int y, int delta = 1);

	
private:
	// 存储点频率的数组，大小和图像一样
	QVector<int> data_;
	// 用于存储渐变透明数据的图像副本
	QImage *alphaCanvas_;
	// 用于显示输出的图像
	QImage *mainCanvas_;
	// 调色板
	GradientPalette *palette_;
	// 半径
	int radius_;
	// 不透明度
	int opacity_;
	// 最大命名数
	qreal max_;
	// 图像宽度
	int width_;
	// 图像高度
	int height_;


};

#endif /* _HEATMAPPER_H_ */


















