#ifndef DVFILTERCONFIG_H
#define DVFILTERCONFIG_H

#include <QWidget>
#include <string>
#include "maindefine.h"
using namespace std;

struct stDefectItemInfo
{
    bool bEnable;
    string name;
    int length;
    int level;
    stDefectItemInfo()
    {
        bEnable = false;
        name = "Õý³£";
        length = 0;
        level = 0;
    }
    stDefectItemInfo(const stDefectItemInfo &other)
    {
        bEnable = other.bEnable;
        name = other.name;
        length = other.length;
        level = other.level;
    }
    stDefectItemInfo operator=(const stDefectItemInfo &other)
    {
        bEnable = other.bEnable;
        name = other.name;
        length = other.length;
        level = other.level;
        return *this;
    }
};

namespace Ui {
class dvFilterConfig;
}
class QGroupBox;
class QSpinBox;
class QCheckBox;
class dvFilterConfig : public QWidget
{
    Q_OBJECT

public:
    explicit dvFilterConfig(QWidget *parent = 0);
    ~dvFilterConfig();

    void initUI(stDefectItemInfo *pItemArray, int iCount);

    const stDefectItemInfo *getItemsValue(int &iCount);
private:
    Ui::dvFilterConfig *ui;
    stDefectItemInfo   m_pItemArray[MAX_DEFECT_CLASS_NUMBS];
    int                m_iCount;

    QGroupBox   *m_groupBox_item[MAX_DEFECT_CLASS_NUMBS];
    QSpinBox    *m_spinBox_area[MAX_DEFECT_CLASS_NUMBS];
    QSpinBox    *m_spinBox_level[MAX_DEFECT_CLASS_NUMBS];
    QCheckBox   *m_checkBox_enable[MAX_DEFECT_CLASS_NUMBS];
};

#endif // DVFILTERCONFIG_H
