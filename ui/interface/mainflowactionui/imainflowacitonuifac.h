#ifndef IMAINFLOWACITONFAC_H
#define IMAINFLOWACITONFAC_H
#include <QString>

class IActionPropertyView;
class IMainflowAcitonuiFac
{
public:
    enum
    {
        EN_IMAINFLOWACITONUIFAC_IID = 0x123
    };
    virtual ~IMainflowAcitonuiFac() {}
    virtual void addActionPropertWidget(QString type,IActionPropertyView* view) = 0;
    virtual IActionPropertyView* getActionPropertWidget(QString type) = 0;
    virtual QStringList getActionTypeList() =0;
};

#endif // IMAINFLOWACITONFAC_H
