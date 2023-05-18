#include "SerialPortControl.h"
#pragma comment(lib, "Advapi32.lib")
/*
 * �������ֵ
 */
const char  *OK = ">Ok\r";					//��ȷ
const char  *NotExist = ">128\r";			//ָ�����
const char  *Chrsh = ">130\r";				//�����������໥��ͻ
const char  *Out  = ">131\r";				//������ֵԽ��򲻷���Ҫ��
const char  *NotAllow = ">132\r";		//��ǰ״̬������ִ�д�ָ��
const char  *PraseError = ">133\r";		//����ָ���޷���������

CPerfTimer::CPerfTimer(void)
{
	QueryPerformanceFrequency(&m_liFreq);
}

CPerfTimer::~CPerfTimer()
{

}

void CPerfTimer::Start()
{
	QueryPerformanceCounter(&m_liStart);
}

void CPerfTimer::ReStart()
{
	Start();
}

int CPerfTimer::GetTimeSpan()
{
	QueryPerformanceCounter(&m_liEnd);
	return  (int)(1000 * (m_liEnd.QuadPart - m_liStart.QuadPart)/m_liFreq.QuadPart);
}

CSerialPortControl::CSerialPortControl()
{
	m_bOpenPort = false;
	m_hCom=INVALID_HANDLE_VALUE;
	InitializeCriticalSection(&m_cs);
}


CSerialPortControl::~CSerialPortControl(void)
{
	ClosePort();
	DeleteCriticalSection(&m_cs);
}


std::vector<std::string > CSerialPortControl::GetAllPort()
{
	std::vector<std::string > allPortList;
	std::string strPort;

	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_READ, &hKey) != ERROR_SUCCESS) 
	{
		RegCloseKey( hKey );
		return allPortList;
	}

	TCHAR    achClass[MAX_PATH] = {0};  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys;                 // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	TCHAR  achValue[MAX_PATH]; 
	DWORD cchValue = MAX_PATH; 
	BYTE  achBuff[80]; 
	DWORD chValue = 80; 
	DWORD type = REG_SZ;
	// Get the class name and the value count. 
	if(RegQueryInfoKey(hKey,        // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime)!=ERROR_SUCCESS)      // last write time 
	{
		RegCloseKey( hKey );
		return allPortList;
	}
	if (cValues) 
	{
		for (DWORD j = 0, retValue = ERROR_SUCCESS; 
			j < cValues; j++) 
		{ 
			chValue = 60;
			cchValue = 60;
			retValue = RegEnumValue(hKey, j, achValue, 
				&cchValue, 
				NULL, 
				&type,    // &dwType, 
				achBuff, // &bData, 
				&chValue);   // &bcData 
			if(retValue == ERROR_SUCCESS)
			{
				strPort = (LPTSTR)achBuff;
				allPortList.push_back(strPort);
			}
		}
	}
	RegCloseKey( hKey );

	return allPortList;
}

bool CSerialPortControl::OpenPort(const char* port,UINT baud)
 {
	 char parity='N';
	 UINT databits=8;
	 UINT stopbits=1;

	 char szPort[50];
     char szDCBparam[50];
	  
	 sprintf_s(szPort,"\\\\.\\%s",port);
     sprintf_s(szDCBparam,"baud=%d parity=%c data=%d stop=%d",baud,parity,databits,stopbits);

	 EnterCriticalSection(&m_cs);
	 // ͬ���򿪴���
	 m_hCom=CreateFile(szPort,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,0);
	 if (m_hCom==INVALID_HANDLE_VALUE)
	 {
		 LeaveCriticalSection(&m_cs);
		 return false;
	 }

     BOOL bIsSuccess=TRUE;
     //���ô��ڳ�ʱʱ�䡣ȫΪ0����ʹ�ô��ڳ�ʱ����,��ʱ
     COMMTIMEOUTS comTms;
     comTms.ReadIntervalTimeout=MAXWORD;
     comTms.ReadTotalTimeoutConstant=1000;
     comTms.ReadTotalTimeoutMultiplier=1000;
     comTms.WriteTotalTimeoutConstant=1000;
     comTms.WriteTotalTimeoutMultiplier=1000;
     if (bIsSuccess)
         bIsSuccess=SetCommTimeouts(m_hCom,&comTms);
 
     //����DCB�ṹ
     DCB dcb;
     if (bIsSuccess)
     {
         bIsSuccess=GetCommState(m_hCom,&dcb);
         dcb.BaudRate=baud;
         dcb.ByteSize=8;
         dcb.Parity=NOPARITY;
         dcb.StopBits=stopbits;
     }
 
	 //ʹ��dcb�������ô���״̬
     if (bIsSuccess)
         bIsSuccess=SetCommState(m_hCom,&dcb);
 
     //��մ��ڻ�����
     PurgeComm(m_hCom,PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR);
     LeaveCriticalSection(&m_cs);
 
	 if (!bIsSuccess)
		ClosePort();
	 else
		m_bOpenPort  = true;

     return m_bOpenPort;
 }
 
bool CSerialPortControl::IsOpen()
{
	 if (m_hCom == INVALID_HANDLE_VALUE)
		 return false;
	return true;
}

void CSerialPortControl::ClosePort()
{
	m_bOpenPort = false;
	EnterCriticalSection(&m_cs);
	// �ж��Ƿ�򿪴���
    if (m_hCom!=INVALID_HANDLE_VALUE)
    {
		 // �رմ���
        CloseHandle(m_hCom);
        m_hCom=INVALID_HANDLE_VALUE;
    }
	LeaveCriticalSection(&m_cs);
}

bool CSerialPortControl::WriteDataToPort(char* pData,int length,int nTimeOut)
{
	bool bReturn = false;
	bReturn = WriteDataToPort(pData,length);
	if (!bReturn)
		return bReturn;

	std::string strReceive = "";
	ReadFromPort(strReceive,nTimeOut);
	if (strReceive.find(OK) == -1 )
		return  false;

	return true;
}

bool CSerialPortControl::WriteDataToPort(char* pData,int length)
{
    BOOL bResult=TRUE;
    DWORD bytesToSend=0;
    if (m_hCom==INVALID_HANDLE_VALUE)
        return false;

    EnterCriticalSection(&m_cs);
	PurgeComm(m_hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    bResult=WriteFile(m_hCom,pData,(unsigned int)length,&bytesToSend,NULL);
    if (bResult==FALSE)
    {
        PurgeComm(m_hCom,PURGE_TXABORT|PURGE_TXCLEAR);
        LeaveCriticalSection(&m_cs);
        return false;
    }
    LeaveCriticalSection(&m_cs);
    return true;
}

bool CSerialPortControl::ReadDataFromPort(char* pData,int nLen , int* nReadLen,int nTimeOut)
{
	std::string strReceive = "";
	ReadFromPort(strReceive,nTimeOut);

	// ��������
	int nCopyLen = (int)min(nLen,strReceive.length());
	memcpy(pData , strReceive.c_str(),nCopyLen);
	*nReadLen = nCopyLen;

	if (strReceive.find(OK) == -1 )
		return  false;

    return true;
}

void CSerialPortControl::ReadFromPort(std::string& strReceive,int nTimeOut)
{
	strReceive = "";
	BOOL bResult=false;
	DWORD readSize=0;
	char readData;
	EnterCriticalSection(&m_cs);
	m_perfTimer.Start();
	while (m_perfTimer.GetTimeSpan() <= nTimeOut)
	{
		if (!IsOpen())
			break;

		bResult=ReadFile(m_hCom,&readData,1,&readSize,NULL);
		if (readSize <= 0)
			continue;
		strReceive.append(&readData,readSize);

		// ֻ�е�ָ��ִ�к�ŷ���;
		if (strReceive.find(OK) != -1 ||
			strReceive.find(NotExist) != -1 ||
			strReceive.find(Chrsh) != -1 ||
			strReceive.find(Out) != -1 ||
			strReceive.find(NotAllow) != -1 ||
			strReceive.find(PraseError) != -1 )
		{
			break;
		}
	}
	LeaveCriticalSection(&m_cs);
}

