#ifndef _STATIC_CORRECTION_H
#define _STATIC_CORRECTION_H

/* Version 2 : V2
    说明：只针对Unsigned char 数据进行处理（ 从而通过加减运算实现静态校准）
*/
class CFastStaticCorrection
{
public:
	CFastStaticCorrection();
	~CFastStaticCorrection();

private:

	int m_imageWidth;
	int m_imageHeight;
	//float
	float *m_factors;

public:

	/*函数：训练
	输入参数：
	data ： 图像
	imageWidth ：图像宽度
	imageHeight：图像高度
	standardGv ：标准值
	startLine ：从图像的第startLine开始训练
	endLine ：图像训练到第endLine结束
	输出参数：
	factors ：输出校准系数（长度为图像的宽）

	返回：0，训练成功，-1，训练失败
	*/

	int Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol, float *factors);

	/*函数：初始化静态校准
	输入参数：
	imageWidth ： 图像宽度
	imageHeight ：图像高度
	factors ：校准系数
	返回：0，校准成功，-1，校准失败
	*/
	int InitStatCorr(int imageWidth, int imageHeight, float *factors);
	/*函数：静态校准
	输入参数：
	image ： 图像
	返回：
	correctImage ：校准后图像
	返回：0，校准成功，-1，校准失败
	*/
	int StatCorr(unsigned char *image, unsigned char *correctImage);

	/*函数：获取矫正后图像的参数
	输出参数：
	maxGray ： 校正后图像最大值
	minGray ：校正后图像最小值
	mean ：校正后图像均值
	devi ：校正后图像方差
	返回：0，获取成功，-1，获取失败
	*/
	int GetStatParam(unsigned char *image, int imageWidth, int imageHeight, unsigned char &maxGray, unsigned char &minGray, double &mean, double &devi);

	/*
	函数：释放资源
	*/
	void ReleaseStatCorr();
};

#endif
