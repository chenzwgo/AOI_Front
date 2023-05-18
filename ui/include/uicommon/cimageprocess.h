#ifndef CIMAGEPROCESS_H
#define CIMAGEPROCESS_H

#include "dllexport.h"
#include "uidefine.h"

#include <QImage>

class RT_API CImageProcess
{
public:
    static bool SaveImageToFile(const unsigned char *buff,int width,int height,QString strPath, QImage::Format format = QImage::Format_Indexed8);
};

#endif // CIMAGEPROCESS_H
