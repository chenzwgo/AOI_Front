#ifndef _G12ARITHMETIC_H_
#define _G12ARITHMETIC_H_
#include "filevariable/cvariantvalue.h"
#include <map>
class g12arithmetic
{
public:
    g12arithmetic();
    virtual ~g12arithmetic();
    string getArithmeticName();
    bool init();
    bool isInit();
    bool reInit();
    bool unInit();
    bool runArithmetic();
    void            setAttribute(const string &key,const CVariantValue& value);
    CVariantValue   getAttribute(const string &key);
private:
    map<string,CVariantValue> m_Attributes;

};


#endif _G12ARITHMETIC_H_

