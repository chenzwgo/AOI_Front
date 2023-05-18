#ifndef _I_FILE_PATH_VARIABLE_H_
#define _I_FILE_PATH_VARIABLE_H_
#include "cvariantvalue.h"
#include <string>
using namespace std;

class IFilePathVariable
{
public :
    virtual ~IFilePathVariable(){}
    virtual unsigned int getChildrenNum(const string &ValuePath) = 0;
    virtual string getChildrenName(const string &ValuePath,unsigned int index) = 0;
    virtual void setValue ( const string &ValuePath, const CVariantValue & value ,bool bSyn = false) =0;
    virtual CVariantValue getValue ( const string &ValuePath, const CVariantValue & defaultValue = CVariantValue())  =0;
};



#endif // _I_FILE_PATH_VARIABLE_H_
