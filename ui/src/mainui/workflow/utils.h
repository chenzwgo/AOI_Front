#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QDateTime>
#include <QTime>
#include <string>
using namespace std;

enum MachineType {UnKown=0,Machine_Front=1,Machine_Mid=2,Machine_Tail=3}; // ǰ��,�ж�,ĩ��,β��
class ISocketproxy;
class IStation;
class ICom;
class Utils
{
public:
    Utils();
    ~Utils();
	static bool myping( const char *pIP, int nOverTime ); 
	
	// ��ȡ��ǰĿ¼�����µ��ļ�������ʱ��
	static bool GetNewFileNameAndTime(const QString strFilePath,QString& strFileName,QDateTime& createTime);
	// ��ȡ�ļ�������
	static bool GetFileContent(const QString strFileName, QString& strContent);
	// �����ļ�������
	static bool ParseFile(const QString strContent,const QString strkey,QString & strValue);

    static QString  getConfigPath();
    static QString  getConfigFilePath(const QString& fileName);
    static QString  getBakPath();
    static QString  getBakFilePath(const QString& fileName);
    static MachineType getMachineType();
	static void saveRawImgage(unsigned char* pImageData, unsigned int iwidth, unsigned iHeight, QString name);
	static ISocketproxy* getPCSocketproxy(QString strPcName = "");

    static int WaitGlassLeave(const char*IOName,int iTimeoutMin,int iTimeoutMax,bool& bRun);
    static int checkSigForStop(const char* pIoName ,int itimeout,bool& bRun);
    static IStation* getStation();
    static void saveSn0Log(int sn);
    static bool CheckProductIsOnIO();
	static int callFunCheckAutoClearImage();
	static int callFunCheckAutoClearData();
	static void clearFile(int timeInterval, QString path);

	static ICom* getPenmaCom();
	static string getCamSuffix(int station);
private :
	static ICom* m_pPenmaCom;
	static ISocketproxy* m_pCSocketproxy;
};


#endif // UTILS_H
