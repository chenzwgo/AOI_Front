#include "parseerrormessage.h"
#include "trace.h"
#include "tinyxml.h"
using namespace std;
ParseErrorMessage* ParseErrorMessage::m_self = NULL;
const char* logTypeList[] ={"INFO","WARN","ERRO"};
ParseErrorMessage::ParseErrorMessage():
    m_isParseComplete(false)
{
	m_udpsocket.Init("127.0.0.1",12034);
}

bool ParseErrorMessage::parseErrorMessage()
{
    string errorFilePath  = "./config/ErrorInfo.xml";
	
    TiXmlDocument doc(errorFilePath.c_str());
    if (!doc.LoadFile(TIXML_DEFAULT_ENCODING))
    {
        return false;
    }
	
    TiXmlNode *pRoot = doc.RootElement();
    if (NULL == pRoot)
    {
        return false;
    }

    TiXmlNode *pItemValue = NULL;
    pItemValue = pRoot->FirstChild("ErrorName");

    while (pItemValue)
    {
		int id = 0;
		string strdec ="";
		
        TiXmlElement *pElement = NULL;
        pElement = pItemValue->ToElement();
        TiXmlAttribute* attributeOfID = pElement->FirstAttribute();

		if (0 == strcmp(attributeOfID->Name(), "id"))
		{
			id = atoi(attributeOfID->Value());
		}
	
		strdec = pElement->GetText();
		m_errorInfor.insert(pair<int,string>(id,strdec));
        pItemValue = pItemValue->NextSibling("ErrorName");
    }

    m_isParseComplete = true;
    return true;
}

const char* ParseErrorMessage::FindIDmessage(int errorid)
{
	if(!m_isParseComplete)
	{
		parseErrorMessage();
	}
    map<int ,string>::iterator it;
    it=m_errorInfor.find(errorid);

    if(it != m_errorInfor.end())
    {
        return m_errorInfor[errorid].c_str();
    }

    return "cant find the id message.";
}

void ParseErrorMessage::sendLogMessage(int logType,string logMoudle,string logContent)
{
	if(logType > 2 || logType < 0)
		return;
	m_MutexLock.Lock();
	string logResult = logTypeList[logType];
	logResult.append("=");
	logResult.append(logMoudle);
	logResult.append("=");
	logResult.append(logContent);
	logResult.append("\r\n");
	m_udpsocket.SendMsg("127.0.0.1",12038,logResult.c_str(),logResult.length());
	m_MutexLock.Unlock();
}

ParseErrorMessage *ParseErrorMessage::GetInstance()
{
    if(NULL == m_self)
        m_self = new ParseErrorMessage();
	return m_self;
}
void ParseErrorMessage::DestroyInstance()
{
    if(m_self != NULL)
    {
        delete m_self;
        m_self = NULL;
    }
}
