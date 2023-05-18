#ifndef LIGHTMODELSETTING_H
#define LIGHTMODELSETTING_H

#include "g12globalenv.h"
//#include "lightcontrol.h"
#include <QDialog>

#define LIGHT_CTRL_MODEL_MAX_TYPE 23
#define LIGHT_LIGTH_ITEM_COUNT  6

class QSpinBox;
class QPushButton;
namespace Ui {
class CLightModelSetting;
}

class CLightModelSetting : public QDialog
{
    Q_OBJECT

public:
    explicit CLightModelSetting(QWidget *parent = 0);
    ~CLightModelSetting();

public:
    void setModelParam(const IllumChannelValue *pLightParam);

    void getModelParam(IllumChannelValue *pLightParam);

public slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

    void on_button_cliced(bool bclicked);
private:
    void pushbutton_on(QPushButton  *bt,  bool bOpen);

private:
    Ui::CLightModelSetting *ui;
    IllumChannelValue m_pLightParam;
    QSpinBox  *m_spinSectionStart[U_MUT_SECTION_COUNT];
    QPushButton  *m_btLightGroup[U_MUT_SECTION_COUNT][LIGHT_LIGTH_ITEM_COUNT];
    int m_ctrlType;
    unsigned int m_startLine;
    unsigned int m_endLine;
    //QRadioButton *m_radioButtonType[LIGHT_CTRL_MODEL_MAX_TYPE];
};

#endif // LIGHTMODELSETTING_H
