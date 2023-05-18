#include "g12arithmetic.h"

g12arithmetic::g12arithmetic()
{

}

g12arithmetic::~g12arithmetic()
{

}

string g12arithmetic::getArithmeticName()
{
    return "";
}

bool g12arithmetic::init()
{
    return true;
}

bool g12arithmetic::isInit()
{
    return true;
}

bool g12arithmetic::reInit()
{
    return true;
}

bool g12arithmetic::unInit()
{
    return true;
}

bool g12arithmetic::runArithmetic()
{
    return true;
}

void g12arithmetic::setAttribute(const string &key, const CVariantValue &value)
{
    m_Attributes[key] = value;
}

CVariantValue g12arithmetic::getAttribute(const string &key)
{
    if(m_Attributes.find(key) == m_Attributes.end())
    {
        return CVariantValue();
    }
}
