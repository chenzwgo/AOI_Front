
#ifndef IMAINFRAMEUISERVICE_H
#define IMAINFRAMEUISERVICE_H

#include <QFrame>

class IMainFrameUIService
{
public:
    enum
    {
        IID_IMAINFRAMEUISERVICE = 0x001
    };
    virtual ~IMainFrameUIService(){}

    virtual QWidget *getMainFrameWindow () = 0;
};


#endif //IMAINFRAMEUISERVICE_H
