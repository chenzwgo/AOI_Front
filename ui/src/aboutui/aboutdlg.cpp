#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include "commonutils.h"
#include <stdio.h>
#include "change.h"
#include "ititlebar.h"
#include "iuicontainer.h"
#include "iuiplugin.h"

#define PLUGIN_PATH "plugins/aboutuiplugin"
#define PLUGIN_NAME "aboutuiplugin"

static const char *toolbarText[] = {
     QT_TRANSLATE_NOOP( "AboutDlg", "aboutuiplugin" ),
 };

AboutDlg* AboutDlg::m_pInstance = NULL;

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
    
}

AboutDlg::~AboutDlg()
{
    delete ui;

}

AboutDlg* AboutDlg::getInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new AboutDlg;
    }

    return m_pInstance;
}

void AboutDlg::releaseInstance()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
    }
}

bool AboutDlg::init()
{
    return true;
}

bool AboutDlg::deinit()
{
    return true;
}

bool AboutDlg::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QDialog::setWindowFlags(Qt::Dialog);
        if(parent != NULL)
        {
            QPoint point (parent->geometry().center() - QPoint (this->width() / 2,
                this->height() / 2));
            move(point);
        }
    }
    else
    {
        resize(rect.width(), rect.height());
        move(rect.x(), rect.y());
    }
    setObjectName("aboutuiplugin");
    readAboutInfo();
    show();
    return true;
}

bool AboutDlg::hideWindow()
{
    hide();
    return true;
}

void AboutDlg::setPluginContainer(void *pPluginContainer)
{
    Q_UNUSED(pPluginContainer);
}

void AboutDlg::fitSize(const QRect& rect)
{
    Q_UNUSED(rect);
}

bool AboutDlg::notify(QString event, OS_WPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);

    return true;
}


void AboutDlg::SetLanguage(QString strlanguage)
{
    QString strLangFilePath = Change::GetUITranslatorPath(PLUGIN_NAME);
    if (m_translator.load(strLangFilePath))
    {
        qApp->installTranslator(&m_translator);
        ui->retranslateUi(this);
    }
    else
    {
        qWarning("%s:%d(%s) translator load LangFile\"%s\" failed, or is not exist",
                  __FILE__, __LINE__, __FUNCTION__, qPrintable(strLangFilePath));
    }
    ui->textEdit_about->setText(Change::tr("IDS_TEST_NAME",PLUGIN_NAME));
}

void AboutDlg::readAboutInfo()
{
    FILE *fileVersion = NULL;
    char *szVersion;
    int nLen = -1;

    QString strPath = "./plugins/aboutuiplugin/Version.txt";

    fileVersion = fopen(strPath.toLatin1().data(), "r+");

    if(NULL == fileVersion)
    {
        return;
    }

    fseek(fileVersion, 0, SEEK_END);
    nLen = ftell(fileVersion);

    if (0 == nLen)
    {
        fclose(fileVersion);
        return;
    }

    rewind(fileVersion);

    szVersion = (char *)malloc(sizeof(char) * nLen + 1);

    QString strVersion;
    while (nLen != ftell(fileVersion))
    {
        fgets(szVersion, sizeof(char) * nLen + 1, fileVersion);
        strVersion += QString::fromLocal8Bit(szVersion);
    }

    if (NULL == szVersion)
    {
        fclose(fileVersion);
        return;
    }
    ui->textEdit_about->clear();
    ui->textEdit_about->setText(strVersion);
    

    fclose(fileVersion);
    free(szVersion);
}

void AboutDlg::keyPressEvent( QKeyEvent *e )
{
    switch(e->key())
    {
        //ÆÁ±ÕEsc¼ü 
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(e);
    }
}
