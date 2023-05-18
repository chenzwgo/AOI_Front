#ifndef _IIO_CTRL_INTERFACE_
#define _IIO_CTRL_INTERFACE_

class IIOCtrlInterface
{
public:
    virtual ~IIOCtrlInterface(){}
    virtual bool readio(const char *name, bool &bopen) = 0;

    virtual bool writeio(const char *name, bool bopen) = 0;

};

#endif