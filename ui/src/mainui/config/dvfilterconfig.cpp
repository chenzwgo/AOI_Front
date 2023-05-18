#include "dvfilterconfig.h"
#include "ui_dvfilterconfig.h"
#include <QGridLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>

dvFilterConfig::dvFilterConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dvFilterConfig)
{
    ui->setupUi(this);
    m_iCount = 0;
    memset(m_groupBox_item, 0, sizeof(m_groupBox_item));
    memset(m_spinBox_area, 0, sizeof(m_spinBox_area));
    memset(m_spinBox_level, 0, sizeof(m_spinBox_level));
}

dvFilterConfig::~dvFilterConfig()
{
    delete ui;
}

void dvFilterConfig::initUI(stDefectItemInfo *pItemArray, int iCount)
{
    m_iCount = iCount; 
	QScrollArea *scrollArea = new QScrollArea;
	QWidget  *pContext = new QWidget(scrollArea);
	QVBoxLayout *layoutS = new QVBoxLayout(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    for (int i = 0; i < m_iCount; i++)
    {
        m_pItemArray[i] = pItemArray[i];
        m_groupBox_item[i] = new QGroupBox(QString::fromLocal8Bit(m_pItemArray[i].name.c_str()), pContext);
        m_spinBox_area[i] = new QSpinBox();
        m_spinBox_area[i]->setRange(0, 9999999);
        m_spinBox_area[i]->setValue(m_pItemArray[i].length);
        m_spinBox_level[i] = new QSpinBox();
        m_spinBox_level[i]->setRange(0, 500);
        m_spinBox_level[i]->setValue(m_pItemArray[i].level);
        m_checkBox_enable[i] = new QCheckBox();
        m_checkBox_enable[i]->setChecked(m_pItemArray[i].bEnable);
        QLabel *pLabelArea = new QLabel(QString::fromLocal8Bit("面积"));
        QLabel *pLabelLevel = new QLabel(QString::fromLocal8Bit("强度"));
        QLabel *pLabelEnable = new QLabel(QString::fromLocal8Bit("使能"));
        QGridLayout *pItemLayout = new QGridLayout;
        pItemLayout->addWidget(pLabelArea, 0, 0);
        pItemLayout->addWidget(m_spinBox_area[i], 0, 1);
        pItemLayout->addWidget(pLabelLevel, 1, 0);
        pItemLayout->addWidget(m_spinBox_level[i], 1, 1);
        pItemLayout->addWidget(pLabelEnable, 2, 0);
        pItemLayout->addWidget(m_checkBox_enable[i], 2, 1);
        m_groupBox_item[i]->setLayout(pItemLayout);
        layout->addWidget(m_groupBox_item[i]);
    }
    layout->setContentsMargins(0, 0, 0, 0);
	pContext->setLayout(layout);
	scrollArea->setWidget(pContext);
	layoutS->addWidget(scrollArea);
	layoutS->setContentsMargins(0, 0, 0, 0);
	setLayout(layoutS);
}

const stDefectItemInfo * dvFilterConfig::getItemsValue(int &iCount)
{
    for (int i = 0; i < m_iCount; i++)
    {
        m_pItemArray[i].length = m_spinBox_area[i]->value();
        m_pItemArray[i].level = m_spinBox_level[i]->value();
        m_pItemArray[i].bEnable = m_checkBox_enable[i]->isChecked();
    }
    iCount = m_iCount;
    return m_pItemArray;
}
