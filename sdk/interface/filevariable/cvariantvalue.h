#ifndef _CVARIANTVALUE_H_
#define _CVARIANTVALUE_H_
#include <string>
#include <string>
#include <sstream>
#include "dllexport.h"
using namespace std;


class  CVariantValue
{
public :
    CVariantValue(){strValue  ="";}
    ~CVariantValue(){}

    CVariantValue(const CVariantValue& rhs){strValue = rhs.getValueString();}
    CVariantValue(bool Vaule){setValue(Vaule);}
    CVariantValue(int Vaule){setValue(Vaule);}
    CVariantValue(unsigned int Vaule){setValue(Vaule);}
    CVariantValue(long Vaule){setValue(Vaule);}
    CVariantValue(unsigned long Vaule){setValue(Vaule);}
    CVariantValue(float Vaule){setValue(Vaule);}
    CVariantValue(double Vaule){setValue(Vaule);}
    CVariantValue(string Vaule){setValue(Vaule);}
    CVariantValue(const char* Vaule){setValue(Vaule);}

    CVariantValue& operator=(const CVariantValue& rhs) {strValue = rhs.getValueString();return (*this);}
	CVariantValue& operator=(bool& x){setValue(x);return (*this);}
    CVariantValue& operator=(int& x){setValue(x);return (*this);}
    CVariantValue& operator=(unsigned int& x){setValue(x);return (*this);}
    CVariantValue& operator=(long & x){setValue(x);return (*this);}
    CVariantValue& operator=(unsigned long & x){setValue(x);return (*this);}
    CVariantValue& operator=(double& x){setValue(x);return (*this);}
    CVariantValue& operator=(float& x){setValue(x);return (*this);}
    CVariantValue& operator=(string& x){setValue(x);return (*this);}
	CVariantValue& operator=(const char* x){setValue(x);return (*this);}

    void setValue(bool bDefault = false){ostringstream convert;convert << bDefault;strValue =  convert.str();}
    void setValue(int iDefault = 0){ostringstream convert;convert << iDefault;strValue =  convert.str();}
    void setValue(unsigned int uiDefault = 0){ostringstream convert;convert << uiDefault;strValue =  convert.str();}
    void setValue(long lDefault = 0){ostringstream convert;convert << lDefault;strValue =  convert.str();}
    void setValue(unsigned long lDefault = 0){ostringstream convert;convert << lDefault;strValue =  convert.str();}
    void setValue(double dDefault =0.0){ostringstream convert;convert << dDefault;strValue =  convert.str();}
    void setValue(float fDefault = 0.0){ostringstream convert;convert << fDefault;strValue =  convert.str();}
    void setValue(const char* strDefault= ""){strValue =  strDefault;}
    void setValue(string strDefault= ""){strValue =  strDefault;}

    bool isInvalide()
    {
        return !strValue.empty();
    }
    bool toBool(bool bDefault = false)
	{
		bool toret;
		istringstream convert( strValue );
		if ( !( ( convert >> toret) && convert.eof() ) )
			return bDefault;
		return toret;
    }
    int toInt(int iDefault = 0)
	{
		int toret;
		istringstream convert( strValue );
		if ( !( ( convert >> toret) && convert.eof() ) )
			return iDefault;
		return toret;
    }
    int toUInt(unsigned int iDefault = 0)
    {
        unsigned int toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return iDefault;
        return toret;
    }

    int toLong(long iDefault = 0)
    {
        long toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return iDefault;
        return toret;
    }
    int toULong(unsigned long iDefault = 0)
    {
        unsigned long toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return iDefault;
        return toret;
    }

    float toFloat(float fDefault = 0.0)
	{
		float toret;
		istringstream convert( strValue );
		if ( !( ( convert >> toret) && convert.eof() ) )
			return fDefault;
		return toret;
    }
    double toDouble(double dDefault =0.0)
	{
		double toret;
		istringstream convert( strValue );
		if ( !( ( convert >> toret) && convert.eof() ) )
			return dDefault;
		return toret;
    }
    string  toCString(string strDefault= ""){return strValue;}
    string getValueString() const {return strValue;}
private: 
    string strValue;
};

class  CVariantPoint
{
public :
    enum VariantPointType
    {
        VariantPoint_NONE= 0,
        VariantPoint_Bool,
        VariantPoint_Int,
        VariantPoint_Float,
        VariantPoint_Double,
        VariantPoint_String
    };
    CVariantPoint(){pValue  =NULL; pointType = VariantPoint_NONE;}
    ~CVariantPoint(){}
    CVariantPoint(const CVariantPoint& rhs){pValue  =rhs.pValue; pointType = rhs.pointType;}
    CVariantPoint(bool* pVaule){setPointAndType(pVaule,VariantPoint_Bool);}
    CVariantPoint(int* pVaule){setPointAndType(pVaule,VariantPoint_Int);}
    CVariantPoint(float* pVaule){setPointAndType(pVaule,VariantPoint_Float);}
    CVariantPoint(double* pVaule){setPointAndType(pVaule,VariantPoint_Double);}
    CVariantPoint(string* pVaule){setPointAndType(pVaule,VariantPoint_String);}

    CVariantPoint& operator=(const CVariantPoint& rhs) {pValue  =rhs.pValue; pointType = rhs.pointType;return (*this);}
    CVariantPoint& operator=(bool* x){setPointAndType(x,VariantPoint_Bool);return (*this);}
    CVariantPoint& operator=(int* x){setPointAndType(x,VariantPoint_Int);return (*this);}
    CVariantPoint& operator=(double* x){setPointAndType(x,VariantPoint_Double);return (*this);}
    CVariantPoint& operator=(float* x){setPointAndType(x,VariantPoint_Float);return (*this);}
    CVariantPoint& operator=(string* x){setPointAndType(x,VariantPoint_String);return (*this);}

    bool setValue( CVariantValue& value)
    {
        if(pValue  == NULL || pointType == VariantPoint_NONE)
        {
            return false;
        }
        switch(pointType)
        {
            case VariantPoint_NONE: break;
            case VariantPoint_Bool:*((bool*)pValue) = value.toBool();break;
            case VariantPoint_Int: *((int*)pValue) = value.toInt();break;
            case VariantPoint_Float: *((float*)pValue) = value.toFloat();break;
            case VariantPoint_Double: *((double*)pValue) = value.toDouble();break;
            case VariantPoint_String: *((string*)pValue) = value.toCString();break;
            default :break;
        }
        return true;
    }
    void clearPoint(){pValue  =NULL; pointType = VariantPoint_NONE;}
private:
    void setPointAndType(void* point =NULL ,VariantPointType type = VariantPoint_NONE)
    {
        if (point == NULL)
        {
            type = VariantPoint_NONE;
        }
        pValue=point; 
        pointType=type;
    }
    VariantPointType pointType;
    void* pValue;
};

#endif //_CVARIANTVALUE_H_
