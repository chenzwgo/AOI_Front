#include "cimageprocess.h"

bool CImageProcess::SaveImageToFile(const unsigned char *buff,int width,int height,QString strPath, QImage::Format format )
{
    bool bRet = false;
    QImage* img = NULL;
    do 
    {
        img= new QImage(width,height, format);
        if (NULL != img)
        {
            uchar* pPixData = img->bits();
            int iLineCount = img->bytesPerLine();
            int iVCount = iLineCount > width ? width : iLineCount;
            switch (format)
            {
            case QImage::Format_Indexed8:
                {
                    QVector<QRgb> colors;
                    for (int i=0; i<256; ++i)
                    {
                        colors.push_back(qRgb(i,i,i));
                    }
                    img->setColorTable(colors);

                    for (int i = 0; i < height; i++)
                    {
                        memcpy(pPixData + i*iLineCount, buff + i*width, iVCount);
                    }
                }
                break;
            }
            QString strSavePath ;
            strSavePath+=strPath;//+".bmp";
            img->save(strSavePath);
            bRet = true;
            break;
        }
    } while (0);
    if (NULL != img)
    {
        delete img;
        img = NULL;
    }
    return bRet;
}
