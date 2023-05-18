#include "lightmodelsetting.h"
#include "ui_lightmodelsetting.h"

#define IO_ON_IMG  ":/image/IoOn.png"
#define IO_OFF_IMG ":/image/IoOff.png"

bool g_bLightOn[10][LIGHT_LIGTH_ITEM_COUNT] = {  {false , false , false , false , false , false }    //0
                        , {true, true, true, false, false, false}            //1
                        , {true, false, true, false, false, false }           //2
                        , {false, true, false, false, false, false }         //3
                        , {false, false, false,true, true, true }            //4
                        , {false, false, false, true, false, true }           //5
                        , {false, false, false, false, true, false}
                        , {true, true, true, true, true, true}
                        , {true, false, true, true, false, true}
                        , {false, true, false, false, true, false} };

CLightModelSetting::CLightModelSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLightModelSetting)
    , m_ctrlType(0)
    , m_startLine(0)
    , m_endLine(0)
{
    ui->setupUi(this);
    int index = 0;
    m_spinSectionStart[index] = ui->spinBox_1;
    m_btLightGroup[index][0] = ui->pushButton_D5_L1;
    m_btLightGroup[index][1] = ui->pushButton_D5_M1;
    m_btLightGroup[index][2] = ui->pushButton_D5_R1;
    m_btLightGroup[index][3] = ui->pushButton_D10_L1;
    m_btLightGroup[index][4] = ui->pushButton_D10_M1;
    m_btLightGroup[index][5] = ui->pushButton_D10_R1;
    index++;

    m_spinSectionStart[index]    = ui->spinBox_2;
    m_btLightGroup[index][0] = ui->pushButton_D5_L2;
    m_btLightGroup[index][1] = ui->pushButton_D5_M2;
    m_btLightGroup[index][2] = ui->pushButton_D5_R2;
    m_btLightGroup[index][3] = ui->pushButton_D10_L2;
    m_btLightGroup[index][4] = ui->pushButton_D10_M2;
    m_btLightGroup[index][5] = ui->pushButton_D10_R2;
    index++;

    m_spinSectionStart[index] = ui->spinBox_3;
    m_btLightGroup[index][0] = ui->pushButton_D5_L3;
    m_btLightGroup[index][1] = ui->pushButton_D5_M3;
    m_btLightGroup[index][2] = ui->pushButton_D5_R3;
    m_btLightGroup[index][3] = ui->pushButton_D10_L3;
    m_btLightGroup[index][4] = ui->pushButton_D10_M3;
    m_btLightGroup[index][5] = ui->pushButton_D10_R3;
    index++;

    m_spinSectionStart[index] = ui->spinBox_4;
    m_btLightGroup[index][0] = ui->pushButton_D5_L4;
    m_btLightGroup[index][1] = ui->pushButton_D5_M4;
    m_btLightGroup[index][2] = ui->pushButton_D5_R4;
    m_btLightGroup[index][3] = ui->pushButton_D10_L4;
    m_btLightGroup[index][4] = ui->pushButton_D10_M4;
    m_btLightGroup[index][5] = ui->pushButton_D10_R4;
    index++;

    m_spinSectionStart[index] = ui->spinBox_5;
    m_btLightGroup[index][0] = ui->pushButton_D5_L5;
    m_btLightGroup[index][1] = ui->pushButton_D5_M5;
    m_btLightGroup[index][2] = ui->pushButton_D5_R5;
    m_btLightGroup[index][3] = ui->pushButton_D10_L5;
    m_btLightGroup[index][4] = ui->pushButton_D10_M5;
    m_btLightGroup[index][5] = ui->pushButton_D10_R5;
    index++;

    m_spinSectionStart[index] = ui->spinBox_6;
    m_btLightGroup[index][0] = ui->pushButton_D5_L6;
    m_btLightGroup[index][1] = ui->pushButton_D5_M6;
    m_btLightGroup[index][2] = ui->pushButton_D5_R6;
    m_btLightGroup[index][3] = ui->pushButton_D10_L6;
    m_btLightGroup[index][4] = ui->pushButton_D10_M6;
    m_btLightGroup[index][5] = ui->pushButton_D10_R6;
    index++;

    m_spinSectionStart[index] = ui->spinBox_7;
    m_btLightGroup[index][0] = ui->pushButton_D5_L7;
    m_btLightGroup[index][1] = ui->pushButton_D5_M7;
    m_btLightGroup[index][2] = ui->pushButton_D5_R7;
    m_btLightGroup[index][3] = ui->pushButton_D10_L7;
    m_btLightGroup[index][4] = ui->pushButton_D10_M7;
    m_btLightGroup[index][5] = ui->pushButton_D10_R7;
    index++;

    m_spinSectionStart[index] = ui->spinBox_8;
    m_btLightGroup[index][0] = ui->pushButton_D5_L8;
    m_btLightGroup[index][1] = ui->pushButton_D5_M8;
    m_btLightGroup[index][2] = ui->pushButton_D5_R8;
    m_btLightGroup[index][3] = ui->pushButton_D10_L8;
    m_btLightGroup[index][4] = ui->pushButton_D10_M8;
    m_btLightGroup[index][5] = ui->pushButton_D10_R8;
    index++;

    m_spinSectionStart[index] = ui->spinBox_9;
    m_btLightGroup[index][0] = ui->pushButton_D5_L9;
    m_btLightGroup[index][1] = ui->pushButton_D5_M9;
    m_btLightGroup[index][2] = ui->pushButton_D5_R9;
    m_btLightGroup[index][3] = ui->pushButton_D10_L9;
    m_btLightGroup[index][4] = ui->pushButton_D10_M9;
    m_btLightGroup[index][5] = ui->pushButton_D10_R9;
    index++;

    m_spinSectionStart[index] = ui->spinBox_10;
    m_btLightGroup[index][0] = ui->pushButton_D5_L10;
    m_btLightGroup[index][1] = ui->pushButton_D5_M10;
    m_btLightGroup[index][2] = ui->pushButton_D5_R10;
    m_btLightGroup[index][3] = ui->pushButton_D10_L10;
    m_btLightGroup[index][4] = ui->pushButton_D10_M10;
    m_btLightGroup[index][5] = ui->pushButton_D10_R10;
    index++;

    m_spinSectionStart[index] = ui->spinBox_11;
    m_btLightGroup[index][0] = ui->pushButton_D5_L11;
    m_btLightGroup[index][1] = ui->pushButton_D5_M11;
    m_btLightGroup[index][2] = ui->pushButton_D5_R11;
    m_btLightGroup[index][3] = ui->pushButton_D10_L11;
    m_btLightGroup[index][4] = ui->pushButton_D10_M11;
    m_btLightGroup[index][5] = ui->pushButton_D10_R11;
    index++;

    m_spinSectionStart[index] = ui->spinBox_12;
    m_btLightGroup[index][0] = ui->pushButton_D5_L12;
    m_btLightGroup[index][1] = ui->pushButton_D5_M12;
    m_btLightGroup[index][2] = ui->pushButton_D5_R12;
    m_btLightGroup[index][3] = ui->pushButton_D10_L12;
    m_btLightGroup[index][4] = ui->pushButton_D10_M12;
    m_btLightGroup[index][5] = ui->pushButton_D10_R12;
    index++;
    for (int i = 0; i < U_MUT_SECTION_COUNT; i++)
    {
        for (int j = 0; j < LIGHT_LIGTH_ITEM_COUNT; j++)
        {
            connect(m_btLightGroup[i][j], SIGNAL(clicked(bool)), this, SLOT(on_button_cliced(bool)));
        }
    }
}

CLightModelSetting::~CLightModelSetting()
{
    delete ui;
}

void CLightModelSetting::setModelParam(const IllumChannelValue *pLightParam)
{
    bool bD5LOn = false;
    bool bD5MOn = false;
    bool bD5ROn = false;
    bool bD10LOn = false;
    bool bD10MOn = false;
    bool bD10ROn = false;
    for (int i = 0; i < U_MUT_SECTION_COUNT; i++)
    {
        //m_pLightParam.f_draklightmodel[i] = pLightParam->f_draklightmodel[i];
        m_spinSectionStart[i]->setValue(pLightParam->f_sectionlines[i]);
        if (pLightParam->f_draklightmodel[i] < U_MUT_SECTION_COUNT)
        {
            for (int j = 0; j < LIGHT_LIGTH_ITEM_COUNT; j++)
            {
                pushbutton_on(m_btLightGroup[i][j], g_bLightOn[pLightParam->f_draklightmodel[i]][j]);
            }
        }
    }
}

void CLightModelSetting::getModelParam(IllumChannelValue *pLightParam)
{
    for (int i = 0; i < U_MUT_SECTION_COUNT; i++)
    {
        pLightParam->f_sectionlines[i] = m_pLightParam.f_sectionlines[i];
        pLightParam->f_draklightmodel[i] = m_pLightParam.f_draklightmodel[i];
    }
}

void CLightModelSetting::on_pushButton_save_clicked()
{
    bool bLightOn[6] = { 0 };
    for (int i = 0; i < U_MUT_SECTION_COUNT; i++)
    {
        m_pLightParam.f_sectionlines[i] = m_spinSectionStart[i]->value();
        for (int k = 0; k < LIGHT_LIGTH_ITEM_COUNT; k++)
        {
            bLightOn[k] = m_btLightGroup[i][k]->isChecked();
        }
        for (int j = 0; j < U_MUT_SECTION_COUNT; j++)
        {
            if (bLightOn[0] == g_bLightOn[j][0]
                && bLightOn[1] == g_bLightOn[j][1]
                && bLightOn[2] == g_bLightOn[j][2]
                && bLightOn[3] == g_bLightOn[j][3]
                && bLightOn[4] == g_bLightOn[j][4]
                && bLightOn[5] == g_bLightOn[j][5])
            {
                m_pLightParam.f_draklightmodel[i] = j;
                break;
            }
        }
    }
    done(1);
}

void CLightModelSetting::on_pushButton_cancel_clicked()
{
    done(0);
}

void CLightModelSetting::on_button_cliced(bool bclicked)
{
    QPushButton *bt = qobject_cast<QPushButton *>(sender());

    if (bclicked)
    {
        bt->setIcon(QIcon(IO_ON_IMG));
    }
    else
    {
        bt->setIcon(QIcon(IO_OFF_IMG));
    }
    bt->setChecked(bclicked);
    QPushButton *pbt = NULL;
    for (int i = 0; i < U_MUT_SECTION_COUNT; i++)
    {
        for (int j = 0; j < LIGHT_LIGTH_ITEM_COUNT; j++)
        {
            if (bt == m_btLightGroup[i][0])
            {
                pbt = m_btLightGroup[i][2];
                break;
            }
            else if(bt == m_btLightGroup[i][2])
            {
                pbt = m_btLightGroup[i][0];
                break;
            }
            else if (bt == m_btLightGroup[i][3])
            {
                pbt = m_btLightGroup[i][5];
                break;
            }
            else if (bt == m_btLightGroup[i][5])
            {
                pbt = m_btLightGroup[i][3];
                break;
            }
        }
        if (NULL!= pbt)
        {
            break;
        }
    }
    if (NULL != pbt)
    {
        if (bclicked)
        {
            pbt->setIcon(QIcon(IO_ON_IMG));
        }
        else
        {
            pbt->setIcon(QIcon(IO_OFF_IMG));
        }
        pbt->setChecked(bclicked);
    }
}

void CLightModelSetting::pushbutton_on(QPushButton  *bt, bool bOpen)
{
    bt->clicked(bOpen);
}
