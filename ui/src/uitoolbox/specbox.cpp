#include "uitoolbox/specbox.h"
#include "uicommon/commonutils.h"
SpecBox::SpecBox(QWidget *parent)
    : QGroupBox(parent)
    ,m_gridLayout(NULL)
    ,m_bModifyEnable(false)
{
    m_gridLayout = new QGridLayout;
    initUi();
    updateUi();
}

SpecBox::SpecBox(QString title, QString specConfigFilePath ,bool bMotifyEnable /*= false*/,QWidget *parent /*= 0*/ ):QGroupBox(parent)
    ,m_gridLayout(NULL)
    ,m_bModifyEnable(bMotifyEnable)
{
    m_gridLayout = new QGridLayout;
    this->setTitle(title);
    this->setSpecFilePath(specConfigFilePath);
    initUi();
    updateUi();
}

SpecBox::~SpecBox()
{
    qDebug("destroy SpecSetting.....");
}

bool SpecBox::ReadSpecInformation( QXmlStreamReader& reader )
{
    m_vecSpecInfo.clear();

    bool bRes = true;
    while (!reader.atEnd ())
    {

        if (reader.error ())
        {
            qDebug() << "Error: " << qPrintable (reader.errorString ()) <<
                " at line " << reader.lineNumber () <<
                ", column " << reader.columnNumber ();
            bRes = false;
            break;
        }
        if (reader.isEndElement ())
        {
            reader.readNext ();
            if (reader.error ())
            {}
            break;
        }

        if (reader.isStartElement ())
        {
            while (!reader.atEnd ())
            {
                if (reader.isEndElement ())
                {
                    reader.readNext ();
                    if (reader.error ())
                    {}
                    break;
                }
                if (reader.isStartElement ())
                {
                    if ("item" == reader.name ())
                    {
                        specInfoItem pSpecInfo;
                        pSpecInfo.name = reader.attributes ().value ("name").toString ();
                        pSpecInfo.minValue = reader.attributes ().value ("min").toString().toDouble();
                        pSpecInfo.maxValue = reader.attributes ().value ("max").toString().toDouble();
                        m_vecSpecInfo.push_back(pSpecInfo);
                    }
                }
                reader.readNext ();
            }
        }
        reader.readNext ();
    }

    return bRes;
}

bool SpecBox::LoadSpecConfig( QString strfile )
{
    QString strConfigFile = strfile;

    qDebug ("%s: %s: %s: called config file is %s", _TIME_, __FILE__, __FUNCTION__,qPrintable(strConfigFile));
    // Open the file
    QFile cfgFIle (strConfigFile);
    if (!QFile::exists (strConfigFile))
    {
        qDebug ("%s: %s: File %s does not exist", __FILE__, __FUNCTION__, qPrintable (strConfigFile));
        return false;
    }

    if (!cfgFIle.open (QFile::ReadOnly | QFile::Text))
    {
        qDebug ("%s: %s: Error open file:%s", __FILE__, __FUNCTION__, qPrintable (strConfigFile));
        strfile = "";
        return false;
    }

    QXmlStreamReader reader;
    reader.setDevice (&cfgFIle);

    // In read file, all content should be saved in memory
    //bool bRes = readFile (reader);

    qDebug ("%s: %s: %s: called", _TIME_, __FILE__, __FUNCTION__);
    QIODevice *pDevice = reader.device ();

    if (NULL == pDevice)
    {
        qDebug ("%s: %s: Stream reader not bind with device", __FILE__, __FUNCTION__);
        return false;
    }
    bool bRes = true;
    while (!reader.atEnd ())
    {
        if (reader.error ())
        {            
            qDebug() << "Error: " << qPrintable (reader.errorString ()) <<
                " at line " << reader.lineNumber () <<
                ", column " << reader.columnNumber ();
            bRes = false;
            break;
        }
        bRes = ReadSpecInformation(reader);//解析位置信息
    }

    cfgFIle.close ();
    //m_strLoadFileName = strfile;
    return bRes;
}

bool SpecBox::updateUi()
{
    //每次调用都重新读配置文件
    bool bres = LoadSpecConfig(m_specConfigFilePath);
    if (!bres)
    {
        return false;
    }
    vector<specInfoItem>::iterator iter;
    int row = 1;
    for (iter = m_vecSpecInfo.begin();iter != m_vecSpecInfo.end();iter++,row++)
    {
        if (iter->specItemInfo == NULL)
        {
            specUiItem *item  = new specUiItem();
            item->name = new QLabel(iter->name);
            ((QLabel*)(item->name))->setAlignment(Qt::AlignLeft);
            item->minEdit = new QLineEdit();
            item->minEdit->setText(QString("%1").arg(iter->minValue));
            item->maxEdit = new QLineEdit();
            item->maxEdit->setText(QString("%1").arg(iter->maxValue));

            //double input
            QRegExp regExp("^(-?\\d+)(\\.\\d{0,3})?$");
            item->minEdit->setValidator(new QRegExpValidator(regExp, this)); 
            item->maxEdit->setValidator(new QRegExpValidator(regExp, this)); 

            item->changeBtn = new QPushButton(QString::fromLocal8Bit("Modify"));
            m_gridLayout->addWidget(item->name,row,0);
            m_gridLayout->addWidget(item->minEdit,row,1);
            m_gridLayout->addWidget(item->maxEdit,row,2);
            m_gridLayout->addWidget(item->changeBtn,row,3);
            item->row = row;
            connect(item->changeBtn,SIGNAL(clicked()),this,SLOT(changeSpec()));
            iter->specItemInfo = item;
        }
        else
        {
            iter->specItemInfo->minEdit->setText(QString("%1").arg(iter->minValue));
            iter->specItemInfo->maxEdit->setText(QString("%1").arg(iter->maxValue));
        }

        iter->specItemInfo->minEdit->setEnabled(m_bModifyEnable);
        iter->specItemInfo->maxEdit->setEnabled(m_bModifyEnable);
        iter->specItemInfo->changeBtn->setVisible(m_bModifyEnable);
    }
    return true;
}

bool SpecBox::changeSpec()
{
    QString strName;	
    for (vector<specInfoItem>::iterator iter = m_vecSpecInfo.begin();iter != m_vecSpecInfo.end();iter++)
    {
        if ( iter->specItemInfo->changeBtn == QObject::sender())
        {
            strName = iter->name;
            iter->maxValue = iter->specItemInfo->maxEdit->text().toDouble();
            iter->minValue = iter->specItemInfo->minEdit->text().toDouble();
            qDebug() <<"click the change button at row name: " << strName;
            break;
        }
    }

    QString strConfigFile = m_specConfigFilePath;

    if (!QFile::exists (strConfigFile))
    {
        strConfigFile = CommonUtils::getPluginLibraryPath () + "/plugins/robotui/" + strConfigFile;
    }

    QFile file(strConfigFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Error: Cannot write file: "
            << qPrintable(file.errorString());
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("doc");
    xmlWriter.writeAttribute("type", "spec");

    for (vector<specInfoItem>::iterator iter = m_vecSpecInfo.begin();iter != m_vecSpecInfo.end();iter++)
    {
        xmlWriter.writeStartElement("item");
        xmlWriter.writeAttribute("name", iter->name);
        xmlWriter.writeAttribute("min", QString("%1").arg(iter->minValue));
        xmlWriter.writeAttribute("max", QString("%1").arg(iter->maxValue));
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        qDebug() << "Error: Cannot write file: "
            << qPrintable(file.errorString());
        return false;
    }
    return true;
}

void SpecBox::setSpecFilePath( QString specConfigFilePath )
{
    m_specConfigFilePath = specConfigFilePath;
}

bool SpecBox::initUi()
{
    QLabel *nameTitie = NULL;
    nameTitie = new QLabel("name");
    QLabel *minTitie = NULL;
    minTitie = new QLabel("min");
    QLabel *maxTitie = NULL; 
    maxTitie = new QLabel("max");

    if (nameTitie && minTitie && maxTitie && m_gridLayout)
    {
        nameTitie->setAlignment(Qt::AlignCenter);
        minTitie->setAlignment(Qt::AlignCenter);
        maxTitie->setAlignment(Qt::AlignCenter);

        m_gridLayout->addWidget(nameTitie,0,0);
        m_gridLayout->addWidget(minTitie,0,1);
        m_gridLayout->addWidget(maxTitie,0,2);

        this->setLayout(m_gridLayout);
        this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed));
        qDebug("init SpecSetting ui succeed.");
        return true;
    }
    else
    {
        qDebug("init SpecSetting ui failed.");
        return false;
    }

}

double SpecBox::getMinSpecByName( QString name )
{
    for (vector<specInfoItem>::iterator iter = m_vecSpecInfo.begin();iter != m_vecSpecInfo.end();iter++)
    {
        if (iter->name.compare(name) == 0)
        {
            return iter->minValue;
        }
    }
    return 0;
}

double SpecBox::getMaxSpecByName( QString name )
{
    for (vector<specInfoItem>::iterator iter = m_vecSpecInfo.begin();iter != m_vecSpecInfo.end();iter++)
    {
        if (iter->name.compare(name) == 0)
        {
            return iter->maxValue;
        }
    }
    return 0;
}

void SpecBox::setModifyEnable( bool state )
{
    m_bModifyEnable = state;
}
