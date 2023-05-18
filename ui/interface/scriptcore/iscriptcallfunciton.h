#ifndef _ISCRIPTCALLFUNCITON_H_
#define _ISCRIPTCALLFUNCITON_H_
#include "rtmetadatadef.h"

class IScriptCallFunciton
{
public:
    virtual ~IScriptCallFunciton(){};

    virtual bool CallFunction(const char* pFunctionName,
                              ReturnArg Ret,
                              ParArg val0 = ParArg( 0 ),
                              ParArg val1 = ParArg(),
                              ParArg val2 = ParArg(),
                              ParArg val3 = ParArg(),
                              ParArg val4 = ParArg(),
                              ParArg val5 = ParArg(),
                              ParArg val6 = ParArg(),
                              ParArg val7 = ParArg(),
                              ParArg val8 = ParArg(),
                              ParArg val9 = ParArg()
                              ) = 0;
};

#endif
