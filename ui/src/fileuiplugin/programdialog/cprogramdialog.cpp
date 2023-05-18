#include "cprogramdialog.h"
#include "ui_cprogramdialog.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>

#include "uicommon/commonutils.h"
#include "inotifyservice.h"

#define  PROGRAM_PATH "./programs/"

CProgramDialog::CProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CProgramDialog)
{
    m_bIsModify = false; 
    m_RenameDlg = new QDialog(this);
    m_RenameUI = new Ui::newfiledlg();
    m_RenameUI->setupUi(m_RenameDlg);
    m_RenameDlg->setWindowTitle(QString::fromLocal8Bit("������"));
    ui->setupUi(this);
    connect(ui->listWidget_prolist,SIGNAL(currentRowChanged(int)),this,SLOT(onCurrentRowChanged(int)));
    //connect(ui->pushButton_addpro,SIGNAL(clicked()),this,SLOT(onAddPro()));
    connect(ui->pushButton_delpro,SIGNAL(clicked()),this,SLOT(onDelPro()));
    connect(ui->pushButton_modifypro,SIGNAL(clicked()),this,SLOT(onRenamePro()));
    //connect(ui->pushButton_backuppro,SIGNAL(clicked()),this,SLOT(onBackupPro()));
    ui->textEdit_prodes->setReadOnly(true);
    m_pNotifyService = CommonUtils::getNotifyService();

	QRegExp rx("[a-zA-Z0-9][a-zA-Z0-9-_]+$");
	QValidator *validator = new QRegExpValidator(rx,m_RenameUI->lineEdit_filename);
	m_RenameUI->lineEdit_filename->setValidator(validator);
}


CProgramDialog::~CProgramDialog()
{
    delete ui;
}

void CProgramDialog::initProList()
{
    ui->listWidget_prolist->clear();
    for (int i=0; i<m_mapProInfo.keys().size(); i++)
    {
        ui->listWidget_prolist->addItem(m_mapProInfo.keys().at(i)); 
    }
}

void CProgramDialog::onCurrentRowChanged( int currentRow )
{
    if (currentRow < 0)
    {
        return;
    }
    QListWidgetItem * pItem = ui->listWidget_prolist->item(currentRow);
    QString strItemText = pItem->text();
    updateProInfo(strItemText);
    if (0 == strItemText.compare(m_strCurrentPro))
    {
        ui->pushButton_delpro->setDisabled(true);
        ui->pushButton_modifypro->setDisabled(true);
    }
    else
    {
        if (!ui->pushButton_delpro->isEnabled())
        {
            ui->pushButton_delpro->setDisabled(false);
        }
        if (!ui->pushButton_modifypro->isEnabled())
        {
            ui->pushButton_modifypro->setDisabled(false);
        }
    }
}

void CProgramDialog::updateProInfo( QString strProName )
{
    if (!m_mapProInfo.contains(strProName))
    {
        return;
    }
    ui->textEdit_prodes->setText(m_mapProInfo.value(strProName).strProgramDesc); 
}

void CProgramDialog::onDelPro()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, QString::fromLocal8Bit("ɾ������"), QString::fromLocal8Bit("ȷ��Ҫɾ��������?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes); 
    if(rb != QMessageBox::Yes) 
    { 
        return ;
    }
    QListWidgetItem * pListWidgetItem = ui->listWidget_prolist->currentItem();
    if (pListWidgetItem == NULL)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ�ж�Ӧ����"));
        return;
    }
    if (pListWidgetItem->text() == m_strCurrentPro)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ǰ���еĳ�����ɾ��"));
        return;
    }
    QString strProDir = QString(PROGRAM_PATH)+pListWidgetItem->text();


    if (!CommonUtils::RemoveDir(strProDir))
    {
         QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ļ���ռ��,ɾ��ʧ��"));
         return;
    }
    m_mapProInfo.remove(pListWidgetItem->text());
    initProList();
    m_bIsModify = true;
}

void CProgramDialog::onRenamePro()
{
    QListWidgetItem * pListWidgetItem = ui->listWidget_prolist->currentItem();
    if (pListWidgetItem == NULL)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ�ж�Ӧ����"));
        return;
    }
    if (pListWidgetItem->text() == m_strCurrentPro)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ǰ���еĳ�����ɾ��"));
        return;
    }
    m_RenameUI->lineEdit_filename->setText(pListWidgetItem->text());
    m_RenameUI->lineEdit_desc->setText(ui->textEdit_prodes->toPlainText());
    if (QDialog::Accepted == m_RenameDlg->exec())
    {
        QString oldProName = pListWidgetItem->text();
        QString strNewName = m_RenameUI->lineEdit_filename->text();
        QString strNewDesc = m_RenameUI->lineEdit_desc->text();
        pListWidgetItem->setText(strNewName);
        ui->textEdit_prodes->setText(strNewDesc);

        m_mapProInfo.remove(oldProName);
        StProgramInfo info;
        info.strProgramName = strNewName;
        info.strProgramDesc = strNewDesc;
        m_mapProInfo.insert(strNewName,info);

        QDir dir;
        dir.rename(QString(PROGRAM_PATH)+oldProName, QString(PROGRAM_PATH)+strNewName);
        writeProgramDesc(strNewName, strNewDesc);
        m_bIsModify = true;
    }
}

void CProgramDialog::showEvent( QShowEvent *e )
{
    QDialog::showEvent(e);
}
 
/**
  qCopyDirectory -- ����Ŀ¼
  fromDir : ԴĿ¼
  toDir   : Ŀ��Ŀ¼
  bCoverIfFileExists : ture:ͬ��ʱ����  false:ͬ��ʱ����false,��ֹ����
  ����: ture�����ɹ� false:����δ���
*/
bool CProgramDialog::qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists)
{
    QDir formDir_(fromDir);
    QDir toDir_(toDir);
 
    if(!toDir_.exists())
    {
        if(!toDir_.mkdir(toDir_.absolutePath()))
            return false;
    }
 
    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
 
        //������Ŀ¼
        if(fileInfo.isDir())
        {
            //�ݹ���ÿ���
            if(!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName()),true))
                return false;
        }
        //�������ļ�
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;
}

void CProgramDialog::initProgramList( QMap<QString,StProgramInfo> mapProInfo, QString currentPro)
{
    m_mapProInfo.clear();
    m_mapProInfo = mapProInfo;
    m_strCurrentPro = currentPro;
    initProList();
    updateProInfo(currentPro);
    ui->label_currentProName->setText(m_strCurrentPro);
}

QMap<QString,StProgramInfo> CProgramDialog::getMapProInfo()
{
    return m_mapProInfo;
}

bool CProgramDialog::isModify()
{
    return m_bIsModify;
}

void CProgramDialog::writeProgramDesc( const QString &programName, const QString &programDesc )
{
    QString strProDir = QString(PROGRAM_PATH)+programName;
    QDir dir(strProDir);
    QString strPath = dir.absolutePath();
    if(!dir.exists())
    {		
        bool bflag = dir.mkdir(strPath);
        if(!bflag) return ;
    }

    QFile file(strPath +"/"+programName+".txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<programDesc;
    }
}
