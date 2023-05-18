#include <QtGui>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include "inotifyservice.h"
#include "uicommon/commonutils.h"
#include "fileuimanager.h"
#include "programdialog/cprogramdialog.h"
#include "g12common.h"
#include "g12globalenv.h"
#include "filevariable/ifilevariable.h"
#include "openfiledlg.h"
#include "stringcommon.h"
FileUiManager::FileUiManager(QWidget *parent):QWidget(parent)
  ,m_newDlg(NULL)
  ,m_newUI(NULL)
  ,m_pNotifyService(NULL)
  , m_bDayModel(true)
{
    m_newDlg = new QDialog(this);
    m_newUI = new Ui::newfiledlg();
    m_newUI->setupUi(m_newDlg);

	QRegExp rx("[a-zA-Z0-9][a-zA-Z0-9-_]+$");
	QValidator *validator = new QRegExpValidator(rx,m_newUI->lineEdit_filename);
	m_newUI->lineEdit_filename->setValidator(validator);
	m_pNotifyService = CommonUtils::getNotifyService();
}

bool FileUiManager::init()
{
	QDir dir(PROGRAM_PATH);
	if(!dir.exists())
	{
		dir.mkdir(PROGRAM_PATH);
	}
	m_mapProInfo.clear();

	//dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);delete 20210624
	//QFileInfoList fileList = dir.entryInfoList(); 
	//foreach (QFileInfo fi, fileList)
	//{
	//	if (fi.isDir() && fi.fileName() != "." &&  fi.fileName() != "..")
	//	{
	//		// ��������б�
	//		StProgramInfo tmp;
	//		tmp.strProgramName = fi.fileName();
	//		tmp.strProgramDesc = readProgramDesc(fi.fileName());
	//		if(!m_mapProInfo.contains(fi.fileName()) && 0 != fi.fileName().compare("program_model"))
	//		{
	//			m_mapProInfo.insert(fi.fileName(),tmp);
	//		}
	//	}           
	//}delete 20210624
	string programs = getProgramNameList();
	vector<string> vectPrograms = SString::split(programs, ",");
	for (size_t i = 0; i < vectPrograms.size(); i++)
	{
		// ��������б�
		StProgramInfo tmp;
		tmp.strProgramName = QString::fromStdString(vectPrograms.at(i));
		tmp.strProgramDesc = readProgramDesc(tmp.strProgramName);
		m_mapProInfo.insert(tmp.strProgramName, tmp);
	}

	m_currentProgram = QString::fromStdString(getCurrentProgramName());
	return true;
}

bool FileUiManager::deinit()
{
    return true;
}

bool FileUiManager::showWindow(QWidget* parent, QRect& rect,bool bPopFlag)
{
    if (parent != NULL)
    {
        this->setParent(parent);

    }
    if (bPopFlag)
    {
        QWidget::setWindowFlags(Qt::Dialog);
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
    show();

    return true;
}

bool FileUiManager::hideWindow()
{
    hide();
    return true;
}

void FileUiManager::setPluginContainer(void *pPluginContainer)
{
    Q_UNUSED(pPluginContainer);
}

void FileUiManager::fitSize(const QRect& rect)
{
    Q_UNUSED(rect);
}

bool FileUiManager::notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam)
{
    qDebug("%s:%d(%s) event is %s, wparam is %d, lparam is %d",
           __FILE__, __LINE__, __FUNCTION__, qPrintable(event), wparam, lparam);

	if (event == "file_new")
	{
		onFileNew();
	}
	else if (event == "file_open")
	{
		onFileOpen();
	}
	else if (event == "file_save")
	{
		onFileSave();
	}
	else if (event == "file_saveas")
	{
		onFileSaveAs();
    }
    else if (event == "file_modify")
    {
        onFileModify();
    }
	/*��ui�޸�*/
	else if(event == "styleMode_change")
	{
        if (0 ==(int) wparam)
		{
			SetDayModel();
		}
        else if(1 ==(int) wparam)
		{
			SetNightModel();
		}
	}
	else
	{

	}

    return true;
}


void FileUiManager::SetLanguage(QString strlanguage)
{

}

void FileUiManager::onFileOpen()
{
	init();

	OpenFileDlg openDlg(this);
	openDlg.InitUi(m_currentProgram, m_mapProInfo);
	m_bDayModel ? openDlg.SetDayModel() : openDlg.SetNightModel();
	int iRs = openDlg.exec();
    if(OpenFileDlg::CANCEL == iRs)
    {
        return ;
    }
	
    
    if (OpenFileDlg::SAVE_CURRENT == iRs)
    {
        //m_pNotifyService->broadcast("program_save", 0,0);
        m_pNotifyService->send("mainui", NOTIFY_PROGRAM_SAVE, 0, 0); //�������ñ��棬 ��ֹ��ǰ���û�û�����꣬ �Ϳ�ʼ�л���Ʒ
    }

    // �㲥�򿪳����ź�
	QString strFileName = openDlg.GetCurrentProgram();
    setCurrentProgram(strFileName);
	m_pNotifyService->broadcast(NOTIFY_PROGRAM_OPEN, 0,0);
	QTimer::singleShot(50,this,SLOT(slotShowMessageOpen()));
}

void FileUiManager::onFileNew()
{
    m_newUI->lineEdit_filename->clear();
	m_newUI->lineEdit_desc->clear();
    if(m_newDlg->exec() != QDialog::Accepted)
    {
        return ;
    }
    QString strName = m_newUI->lineEdit_filename->text();
	if(strName.isEmpty())
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������Ϊ��!"));
		return;
	}
    if(m_mapProInfo.contains(strName))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�������ظ�!"));
        return;
    }

    int iRes = QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�Ƿ񱣴浱ǰ����"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
    if (QMessageBox::Cancel == iRes)
    {
        return;
    }
    if (QMessageBox::Yes == iRes)
    {
        m_pNotifyService->broadcast(NOTIFY_PROGRAM_SAVE, 0,0);
    }

	QString strDesc = m_newUI->lineEdit_desc->text();
    writeProgramDesc(strName,strDesc);

	// ��������б�
	m_mapProInfo.insert(strName,StProgramInfo(strName,strDesc));
	QString programs = QString::fromStdString(getProgramNameList());
	QString programsList = programs + "," + strName;
	string stList = programsList.toStdString();
	setProgramNameList(stList);
	//// ����ģ������ļ����³���Ŀ¼��   dele 20210624  
	//QDir dir;
	//FileUiCommon::qCopyDirectory(dir.absoluteFilePath("programs/program_model"), dir.absoluteFilePath("programs/"+strName),true);
	//setCurrentProgram(strName);dele 20210624  


	// ������ǰʹ���ͺ��ļ����³���Ŀ¼��
	QDir dir;
	QString CurProDir = "programs/" + m_currentProgram;
	QString NewProDir = "programs/" + strName;
	FileUiCommon::qCopyDirectory(dir.absoluteFilePath(CurProDir), dir.absoluteFilePath(NewProDir), true);

	// �㲥�½������ź�
	m_pNotifyService->broadcast(NOTIFY_PROGRAM_NEW, 0,0);
	QTimer::singleShot(50,this,SLOT(slotShowMessageNew()));
}

void FileUiManager::onFileSave()
{
	// �㲥��������ź�
	m_pNotifyService->broadcast(NOTIFY_PROGRAM_SAVE, 0,0);
	QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���򱣴����."));
}

void FileUiManager::onFileSaveAs()
{
	// �㲥���Ϊ�����ź�
    if(m_newDlg == NULL)
    {
        m_newDlg = new QDialog(this);
    }
    if(m_newUI == NULL)
    {
        m_newUI = new Ui::newfiledlg();
        m_newUI->setupUi(m_newDlg);
    }
    m_newUI->lineEdit_filename->setText(m_currentProgram+"_1");
	QString desc = m_mapProInfo[m_currentProgram].strProgramDesc;
	m_newUI->lineEdit_desc->setText(desc);
    if(m_newDlg->exec() != QDialog::Accepted)
    {
        return ;
    }
    QString strName = m_newUI->lineEdit_filename->text();
	if(strName.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������Ϊ��!"));
		return;
	}
    if (m_mapProInfo.contains(strName))
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�������ظ�!"));
        return;
    }

	desc = m_newUI->lineEdit_desc->text();
    QString srcPath = QString(PROGRAM_PATH) + m_currentProgram;
    QString desPath =  QString(PROGRAM_PATH) + strName;
    if (FileUiCommon::qCopyDirectory(srcPath,desPath,true))
    {
		QFile::remove(desPath+"/"+m_currentProgram+".txt");
		writeProgramDesc(strName,desc);
		m_mapProInfo[strName] = StProgramInfo(strName, desc);
        QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����ɹ�"));
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����ʧ��"));
    }
}


void FileUiManager::onFileModify()
{
	init();

    CProgramDialog dlg;
    dlg.initProgramList(m_mapProInfo, m_currentProgram);
    dlg.exec();
    if (dlg.isModify())
    {
        m_mapProInfo = dlg.getMapProInfo();
        // �㲥�򿪳����ź�
        m_pNotifyService->broadcast(NOTIFY_PROGRAM_SAVE, 0,0);
        QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����޸����."));
        init();
    }
}

void FileUiManager::setCurrentProgram( const QString &programName )
{
	setCurrentProgramName(programName.toStdString());
	m_currentProgram = programName;
}

QString FileUiManager::readProgramDesc( const QString &programName )
{
	QString strPorgramDesc;
	QString strProgramDir = QString(PROGRAM_PATH)+programName;
	QFile file(strProgramDir +"/"+programName+".txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		strPorgramDesc = in.readAll();	
    }
    IFileVariable* pFileVariable = getProgramFileVariable(m_currentProgram.toLocal8Bit().data(), PRUDUCT_INFO_FILE);
    if (NULL != pFileVariable)
    {
        string strDes = pFileVariable->getValue(PRUDUCT_DES).toCString();
        if (0 != strPorgramDesc.compare(QString::fromLocal8Bit(strDes.c_str())))
        {
            strDes = strPorgramDesc.toLocal8Bit().data();
            pFileVariable->setValue(PRUDUCT_DES, strDes, true);
        }
    }

	return strPorgramDesc;
}

void FileUiManager::writeProgramDesc( const QString &programName, const QString &programDesc )
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
    IFileVariable* pFileVariable = getProgramFileVariable(m_currentProgram.toLocal8Bit().data(), PRUDUCT_INFO_FILE);
    if (NULL != pFileVariable)
    {
        string strDes = programDesc.toLocal8Bit();
        pFileVariable->setValue(PRUDUCT_DES, strDes, true);
    }
}

void FileUiManager::SetDayModel()
{
	m_bDayModel = true;
}

void FileUiManager::SetNightModel()
{
	m_bDayModel = false;
}

void FileUiManager::slotShowMessageOpen()
{
	QMessageBox::information(this, QString::fromLocal8Bit("�����л����"), QString::fromLocal8Bit("���������´����У�"));
}

void FileUiManager::slotShowMessageNew()
{
	QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����½����."));
}
