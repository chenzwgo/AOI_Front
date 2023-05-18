#include "defectimage.h"
#include "ui_defectimage.h"

DefectImage::DefectImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectImage),
    m_expandWidth(0),
    m_expandHeight(0)
{
    ui->setupUi(this);
    resize(400, 300);
    ui->widget->showStatusBar(XTV_IMAGE_SHOW::VS_STATUSBARIMAGESIZE | XTV_IMAGE_SHOW::VS_STATUSBARCOORDINATE | XTV_IMAGE_SHOW::VS_STATUSBARGRAY);
}

DefectImage::~DefectImage()
{
    delete ui;
}

void DefectImage::setExpandSize(int width, int height)
{
    m_expandWidth = width > 0 ? width : 0;
    m_expandHeight = height > 0 ? height : 0;
}

void DefectImage::setWidgetSize(int width, int height)
{
    resize(width, height);
}

void DefectImage::setImageBuffer(unsigned char *szBuf, int width, int height)
{
    ui->widget->showPictureInLabel(szBuf, NULL, NULL, width, height);
}

void DefectImage::setImageBuffer(unsigned char *szSourceBuf, int width, int height, int xBegin, int yBegin, int swidth, int sheight)
{
    unsigned char *szBuf = NULL;
    int destWidth = swidth;
    int destHeight = sheight;
    int destXBegin = xBegin;
    int destYBegin = yBegin;
    //×ó²àÀ©Õ¹
    if (destXBegin - m_expandWidth < 0)
    {
        destWidth += destXBegin;
        destXBegin = 0;
    }
    else
    {
        destWidth += m_expandWidth;
        destXBegin -= m_expandWidth;
    }
    //¶¥²¿À©Õ¹
    if (destYBegin - m_expandHeight < 0)
    {
        destHeight += destYBegin;
        destYBegin = 0;
    }
    else
    {
        destHeight += m_expandHeight;
        destYBegin -= m_expandHeight;
    }
    //ÓÒ²àÀ©Õ¹
    if (width - xBegin - swidth - m_expandWidth < 0)
    {
        destWidth = width - destXBegin;
    }
    else
    {
        destWidth += m_expandWidth;
    }
    //¶¥²¿À©Õ¹
    if (height - yBegin - sheight - m_expandHeight < 0)
    {
        destHeight = height - destYBegin;
    }
    else
    {
        destHeight += m_expandHeight;
    }
    if (NULL != szSourceBuf && width > 0 && height > 0 && destXBegin + destWidth <= width && destYBegin + destHeight <= height)
    {
        szBuf = new unsigned char[destWidth*destHeight];
		memset(szBuf, 0, destWidth*destHeight);
        for (int i = 0; i < destHeight; i++)
        {
            memcpy(szBuf + i*destWidth, szSourceBuf + ((destYBegin + i)*width + destXBegin), destWidth);
        }
    }
    ui->widget->showPictureInLabel(szBuf, NULL, NULL, destWidth, destHeight);
	if (NULL != szBuf)
	{
		delete szBuf;
	}
}
