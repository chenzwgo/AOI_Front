#ifndef _STATIC_CORRECTION_H
#define _STATIC_CORRECTION_H

/* Version 2 : V2
    ˵����ֻ���Unsigned char ���ݽ��д��� �Ӷ�ͨ���Ӽ�����ʵ�־�̬У׼��
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

	/*������ѵ��
	���������
	data �� ͼ��
	imageWidth ��ͼ����
	imageHeight��ͼ��߶�
	standardGv ����׼ֵ
	startLine ����ͼ��ĵ�startLine��ʼѵ��
	endLine ��ͼ��ѵ������endLine����
	���������
	factors �����У׼ϵ��������Ϊͼ��Ŀ�

	���أ�0��ѵ���ɹ���-1��ѵ��ʧ��
	*/

	int Teaching(unsigned char *data, int imageWidth, int imageHeight, int standardGv, int startLine, int endLine, int startCol, int endCol, float *factors);

	/*��������ʼ����̬У׼
	���������
	imageWidth �� ͼ����
	imageHeight ��ͼ��߶�
	factors ��У׼ϵ��
	���أ�0��У׼�ɹ���-1��У׼ʧ��
	*/
	int InitStatCorr(int imageWidth, int imageHeight, float *factors);
	/*��������̬У׼
	���������
	image �� ͼ��
	���أ�
	correctImage ��У׼��ͼ��
	���أ�0��У׼�ɹ���-1��У׼ʧ��
	*/
	int StatCorr(unsigned char *image, unsigned char *correctImage);

	/*��������ȡ������ͼ��Ĳ���
	���������
	maxGray �� У����ͼ�����ֵ
	minGray ��У����ͼ����Сֵ
	mean ��У����ͼ���ֵ
	devi ��У����ͼ�񷽲�
	���أ�0����ȡ�ɹ���-1����ȡʧ��
	*/
	int GetStatParam(unsigned char *image, int imageWidth, int imageHeight, unsigned char &maxGray, unsigned char &minGray, double &mean, double &devi);

	/*
	�������ͷ���Դ
	*/
	void ReleaseStatCorr();
};

#endif
