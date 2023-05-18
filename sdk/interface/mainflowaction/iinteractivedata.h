#ifndef IINTERACTIVEDATA_H
#define IINTERACTIVEDATA_H

class IInteractiveData
{
public:
    virtual ~IInteractiveData(){}

    // …Ë÷√≤Œ ˝
    virtual bool setInputParam(const char* key,void *param) = 0;
    
    virtual bool getOnputParam(const char* key,void *param) = 0;

};

#endif // IINTERACTIVEDATA_H

