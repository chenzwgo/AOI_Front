#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_

#include <cmath>
#include <vector>

#define VS_STR_DATA_TYPE_REAL           "real"
#define VS_STR_DATA_TYPE_CIRCLE         "circle"
#define VS_STR_DATA_TYPE_LINE           "line"
#define VS_STR_DATA_TYPE_POINT          "point"
#define VS_STR_DATA_TYPE_POSITION       "position"
#define VS_STR_DATA_TYPE_RECTANGLE2     "rectangle2"
#define VS_STR_DATA_TYPE_STRING         "string"
#define VS_STR_DATA_TYPE_DOUBLE         "double"
#define VS_STR_DATA_TYPE_IMAGE          "image"

#define DOUBLE_ACCURACY   0.0001

// 点
struct STPoint
{
    double x;
    double y;

    STPoint() : x(0.0), y(0.0)
    {
    }

    STPoint(double _x, double _y)
        : x(_x), y(_y)
    {
    }

    STPoint(const STPoint &other)
    {
        y = other.y;
        x = other.x;
    }
    STPoint & operator= (const STPoint &other)
    {
        if (this != &other)
        {
            y = other.y;
            x = other.x;
        }
        return *this;
    }

    bool operator!= (const STPoint &other)
    {
        return (fabs(other.y - y) > DOUBLE_ACCURACY || fabs(other.x - x) > DOUBLE_ACCURACY);
    }

    bool operator== (const STPoint &other)
    {
        return !(*this != other);
    }
};

// 线
struct STLine
{
    double y_begin;
    double x_begin;
    double y_end;
    double x_end;
    double phi;

    STLine() : x_begin(10.0), y_begin(10.0), x_end(40.0), y_end(40.0), phi(0.0)
    { 
    }
    STLine(const STLine &other)
    {
        y_begin = other.y_begin;
        x_begin = other.x_begin;
        y_end   = other.y_end;
        x_end   = other.x_end;
        phi       = other.phi;
    }

    STLine( double _x_begin, double _y_begin, double _x_end, double _y_end, double phi)
        : x_begin(_x_begin), y_begin(_y_begin), x_end(_x_end), y_end(_y_end), phi(phi)
    {
    }

    STLine & operator= (const STLine &other)
    {
        if (this != &other)
        {
            y_begin = other.y_begin;
            x_begin = other.x_begin;
            y_end   = other.y_end;
            x_end   = other.x_end;
            phi       = other.phi;
        }
        return *this;
    }

    bool operator!= (const STLine &other)
    {
        return (fabs(y_begin  - other.y_begin) > DOUBLE_ACCURACY
            ||  fabs(x_begin  - other.x_begin) > DOUBLE_ACCURACY
            ||  fabs(y_end    - other.y_end  ) > DOUBLE_ACCURACY
            ||  fabs(x_end    - other.x_end  ) > DOUBLE_ACCURACY
            ||  fabs(phi        - other.phi      ) > DOUBLE_ACCURACY);
    }

    bool operator== (const STLine &other)
    {
        return !(*this != other);
    }
};

// 圆
struct STCircle
{
    double x;
    double y;
    double radius;

    STCircle() : x(128), y(128), radius(64)
    {
    }

    STCircle(double _x , double _y, double _radius)
        : x(_x), y(_y), radius(_radius)
    {
    }

    STCircle(const STCircle &other)
    {
        x    = other.x;
        y    = other.y;
        radius = other.radius;
    }

    STCircle & operator= (const STCircle &other)
    {
        if (this != &other)
        {
            x    = other.x;
            y    = other.y;
            radius = other.radius;
        }
        return *this;
    }

    bool operator!= (const STCircle &other)
    {
        return( fabs(y    - other.y   ) > DOUBLE_ACCURACY
            ||  fabs(x    - other.x   ) > DOUBLE_ACCURACY
            ||  fabs(radius - other.radius) > DOUBLE_ACCURACY);
    }                               
    bool operator== (const STCircle &other)
    {
        return !(*this != other);
    }
};
// 可旋转矩形
struct STRectangle2
{
    double y;
    double x;
    double phi;
    double width;
    double height;

    STRectangle2() : x(100.0), y(100.0), phi(0.0), width(160.0), height(60.0)
    {
    }

    STRectangle2(double _x, double _y, double _phi, double _width, double _height)
        : x(_x), y(_y), phi(_phi), width(_width), height(_height)
    {
    }

    STRectangle2(const STRectangle2 &other)
    {
        y    = other.y;
        x    = other.x;
        phi    = other.phi;
        width  = other.width;
        height = other.height;
    }

    STRectangle2 & operator= (const STRectangle2 &other)
    {
        if (this != &other)
        {
            y    = other.y;
            x    = other.x;
            phi    = other.phi;
            width  = other.width;
            height = other.height;
        }
        return *this;
    }

    bool operator!= (const STRectangle2 &other)
    {
        return ( fabs(y    - other.y)    > DOUBLE_ACCURACY
            ||   fabs(x    - other.x)    > DOUBLE_ACCURACY
            ||   fabs(phi    - other.phi)    > DOUBLE_ACCURACY
            ||   fabs(width  - other.width)  > DOUBLE_ACCURACY
            ||   fabs(height - other.height) > DOUBLE_ACCURACY);

    }
    bool operator== (const STRectangle2 &other)
    {
        return !(*this != other);
    }
};

struct STPosition
{
    double x;
    double y;
    double angle;

    STPosition() : x(0.0), y(0.0),angle(0.0)
    {
    }

    STPosition(double _x, double _y, double _angle)
        : x(_x), y(_y), angle(_angle)
    {
    }
    STPosition(const STPosition &other)
    {
        x = other.x;
        y = other.y;
        angle = other.angle;
    }

    STPosition & operator= (const STPosition &other)
    {
        if (this != &other)
        {
            x = other.x;
            y = other.y;
            angle = other.angle;
        }
        return *this;
    }

    bool operator!= (const STPosition &other)
    {
        return (fabs(other.x - x) > DOUBLE_ACCURACY || fabs(other.y - y) > DOUBLE_ACCURACY || fabs(other.angle - angle) > DOUBLE_ACCURACY);
    }

    bool operator== (const STPosition &other)
    {
        return !(*this != other);
    }
};

struct STRatioPoint
{
    double x;
    double y;
    double ratio;

    STRatioPoint() : y(0.0), x(0.0), ratio(0.0)
    {
    }

    STRatioPoint(double _x, double _y, double _ratio)
        : x(_x), y(_y), ratio(_ratio)
    {
    }

    STRatioPoint(const STRatioPoint &other)
    {
        y   = other.y;
        x   = other.x;
        ratio = other.ratio;
    }

    STRatioPoint & operator= (const STRatioPoint &other)
    {
        if (this != &other)
        {
            y   = other.y;
            x   = other.x;
            ratio = other.ratio;
        }
        return *this;
    }

    bool operator!= (const STRatioPoint &other)
    {
        return (fabs(y   - other.y  ) > DOUBLE_ACCURACY
            ||  fabs(x   - other.x  ) > DOUBLE_ACCURACY
            ||  fabs(ratio - other.ratio) > DOUBLE_ACCURACY);
    }

    bool operator== (const STRatioPoint &other)
    {
        return !(*this != other);
    }
};

#define UPDATE_IMAGE_MEMORY(S, D, size)    \
if (NULL != D) \
{ \
	delete []D; \
	D = NULL; \
} \
if (NULL != S) \
{ \
    D    = new unsigned char[size]; \
    if (NULL != D) \
    { \
        memcpy(D, S, size); \
    } \
}

struct stImageInfo
{
    enum {IMG_UNKNOW, IMG_8BITS, IMG_RGB, IMG_RGBA} imgType;
    //void *pImage;
    long width;				// 图片宽度
    long height;			// 图片高度
    unsigned char *pixelR;	// 红色通道
    unsigned char *pixelG;	// 绿色通道
    unsigned char *pixelB;  // 蓝色通道
    unsigned char *pixelA;  // 透明通道
private:

public:
    stImageInfo()
        : imgType(IMG_UNKNOW)
        //, pImage(NULL)
        , width(0)
        , height(0)
        , pixelR(NULL)
        , pixelG(NULL)
        , pixelB(NULL)
        , pixelA(NULL)
    {
    }
    stImageInfo (const stImageInfo &other)
        : imgType(IMG_UNKNOW)
        //, pImage(NULL)
        , width(0)
        , height(0)
        , pixelR(NULL)
        , pixelG(NULL)
        , pixelB(NULL)
        , pixelA(NULL)
    {
        if (this == &other)
        {
            return;
        }
        int iSize = other.width * other.height;
        imgType   = other.imgType;
        //pImage    = other.pImage;
        width     = other.width;
        height    = other.height;
        UPDATE_IMAGE_MEMORY(other.pixelR, pixelR, iSize);
        UPDATE_IMAGE_MEMORY(other.pixelG, pixelG, iSize);
        UPDATE_IMAGE_MEMORY(other.pixelB, pixelB, iSize);
        UPDATE_IMAGE_MEMORY(other.pixelA, pixelA, iSize);
    }
    stImageInfo(const unsigned char* pPixR, const unsigned char* pPixG, const unsigned char* pPixB, const unsigned char* pPixA, int widthD, int heightD)
        : imgType(IMG_UNKNOW)
        //, pImage(NULL)
        , width(widthD)
        , height(heightD)
        , pixelR(NULL)
        , pixelG(NULL)
        , pixelB(NULL)
        , pixelA(NULL)
    {
        int iSize = widthD * heightD;
        UPDATE_IMAGE_MEMORY(pPixR, pixelR, iSize);
        UPDATE_IMAGE_MEMORY(pPixG, pixelG, iSize);
        UPDATE_IMAGE_MEMORY(pPixB, pixelB, iSize);
        UPDATE_IMAGE_MEMORY(pPixA, pixelA, iSize);
    }
    stImageInfo & operator= (const stImageInfo &other)
    {
        if (this != &other)
        {
            int iSize = other.width * other.height;
            imgType   = other.imgType;
            //pImage    = other.pImage;
            width     = other.width;
            height    = other.height;
            UPDATE_IMAGE_MEMORY(other.pixelR, pixelR, iSize);
            UPDATE_IMAGE_MEMORY(other.pixelG, pixelG, iSize);
            UPDATE_IMAGE_MEMORY(other.pixelB, pixelB, iSize);
            UPDATE_IMAGE_MEMORY(other.pixelA, pixelA, iSize);
        }
        return *this;
    }

    bool operator!= (const stImageInfo &other)
    {
        return (imgType   != other.imgType 
            //|| pImage     != other.pImage
            || width      != other.width   
            || height     != other.height  
            || pixelR     != other.pixelR  
            || pixelG     != other.pixelG  
            || pixelB     != other.pixelB  
            || pixelA     != other.pixelA);
    }

    bool operator== (const stImageInfo &other)
    {
        return !(*this != other);
    }
    //stImageInfo &clone(stImageInfo &other)
    //{
    //    bIsAutoRelease = true;
    //    if (this != &other)
    //    {
    //        int iSize = other.width * other.height;
    //        imgType   = other.imgType;
    //        width     = other.width;
    //        height    = other.height;
    //        if (NULL != other.pixelR)
    //        {
    //            pixelR    = new unsigned char[iSize];
    //            memcpy(pixelR, other.pixelR, iSize);
    //        }
    //        if (NULL != other.pixelG)
    //        {
    //            pixelG    = new unsigned char[iSize];
    //            memcpy(pixelG, other.pixelG, iSize);
    //        }
    //        if (NULL != other.pixelB)
    //        {
    //            pixelB    = new unsigned char[iSize];
    //            memcpy(pixelB, other.pixelB, iSize);
    //        }
    //        if (NULL != other.pixelA)
    //        {
    //            pixelA    = new unsigned char[iSize];
    //            memcpy(pixelA, other.pixelA, iSize);
    //        }
    //    }
    //    return *this;
    //}

    ~stImageInfo()
    {
        //if (bIsAutoRelease)
        //{
            if (NULL != pixelR)
            {
                delete []pixelR;
                pixelR = NULL;
            }
            if (NULL != pixelG)
            {
                delete []pixelG;
                pixelG = NULL;
            }
            if (NULL != pixelB)
            {
                delete []pixelB;
                pixelB = NULL;
            }
            if (NULL != pixelA)
            {
                delete []pixelA;
                pixelA = NULL;
            }
        }
    //}
};

typedef std::vector<double>				DValues;
typedef std::vector<STPoint>			STPoints;
typedef std::vector<STCircle>			STCircles;
typedef std::vector<STLine>				STLines;
typedef std::vector<STRectangle2>		STRectangle2s;

#endif
