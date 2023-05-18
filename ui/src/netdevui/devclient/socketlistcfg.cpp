#include "socketlistcfg.h"
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#include "socketclient.h"
#include "socketproxy.h"

#define  SOCKET_SERVER_FILE_PATH "./plugins/mainui/ServerConfig.xml"

CSocketClientListCfg* CSocketClientListCfg::m_pInstance = NULL;


CSocketClientListCfg::CSocketClientListCfg()
{

    readServerConfig();
}

CSocketClientListCfg::~CSocketClientListCfg()
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    map<string,clientCfg*>::const_iterator iter;
    for (iter = m_ClientInfomap.begin();iter != m_ClientInfomap.end();iter++)
    {
        delete iter->second;
    }

    map<string,CSocketproxy*>::const_iterator iter2;
    for (iter2 = m_Clientmap.begin();iter2 != m_Clientmap.end();iter2++)
    {
        delete iter2->second;
    }
    m_ClientInfomap.clear();
    m_Clientmap.clear();   
}

ISocketproxy *CSocketClientListCfg::getSocketClientProxy(string strName)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    map<string,CSocketproxy*>::iterator iter = m_Clientmap.find(strName);
    if(iter != m_Clientmap.end())
    {
        return iter->second;
    }
    else if (strName.length()==0 && m_Clientmap.size() >0 && m_Clientmap.begin() != m_Clientmap.end())
    {
        return m_Clientmap.begin()->second;
    }
    return NULL;
}

serverCfg* CSocketClientListCfg::getServerCfg()
{
    return &m_serverCfg;
}
bool CSocketClientListCfg::addClientCfg(const clientCfg &cfg)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    if(cfg.ClientName.length() == 0)
    {
        return false;
    }
    if(m_ClientInfomap.find(cfg.ClientName) != m_ClientInfomap.end())
    {
        return false;
    }
    if(m_Clientmap.find(cfg.ClientName) != m_Clientmap.end())
    {
        return false;
    }

    clientCfg* pstclientCfg = new clientCfg(cfg);
    m_ClientInfomap.insert(pair<string,clientCfg*>(pstclientCfg->ClientName,pstclientCfg));

    CSocketproxy* pSocketproxy = new CSocketproxy(pstclientCfg->ClientName);
	pSocketproxy->getSocketClient()->initClient(pstclientCfg->ClientIP.c_str(), pstclientCfg->ClientPort);
	pSocketproxy->getSocketClient()->setServerCfg(pstclientCfg->DesIP.c_str(), pstclientCfg->DesPort);
	m_Clientmap.insert(std::pair<string, CSocketproxy*>(pstclientCfg->ClientName, pSocketproxy));

    return true;
}

bool CSocketClientListCfg::delClientCfg(const string &ClientName)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    if(ClientName.length() == 0)
    {
        return false;
    }
    map<string,clientCfg*>::iterator iterCfg = m_ClientInfomap.find(ClientName) ;
    if(iterCfg == m_ClientInfomap.end())
    {
        return false;
    }
    map<string,CSocketproxy*>::iterator iterCliet = m_Clientmap.find(ClientName) ;
    if(iterCliet == m_Clientmap.end())
    {
        return false;
    }

    if(iterCliet->second->getSocketClient()->isConnected())
    {
        return false;
    }

    if(iterCliet->second != NULL)
    {
        delete iterCliet->second;
        m_Clientmap.erase(iterCliet);
    }

    if(iterCfg->second != NULL)
    {
        delete iterCfg->second;
        m_ClientInfomap.erase(iterCfg);
    }

    return true;
}

bool CSocketClientListCfg::ModifyClientCfg(const string &ClientName, const clientCfg &cfg)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    if(cfg.ClientName.length() == 0)
    {
        return false;
    }
    if(m_ClientInfomap.find(ClientName) == m_ClientInfomap.end())
    {
        return false;
    }
    if(m_Clientmap.find(ClientName) == m_Clientmap.end())
    {
        return false;
    }

    clientCfg*pstSMCameraCfg = m_ClientInfomap[ClientName];
    if(pstSMCameraCfg == NULL)
    {
        return false;
    }
    CSocketClient *pCSocketClient = m_Clientmap[ClientName]->getSocketClient();
    if(pCSocketClient == NULL)
    {
        return false;
    }

    if (cfg.ClientIP != pstSMCameraCfg->ClientIP|| cfg.ClientPort != pstSMCameraCfg->ClientPort)
    {
        pCSocketClient->initClient(pstSMCameraCfg->ClientIP.c_str(),pstSMCameraCfg->ClientPort);
    }
    *pstSMCameraCfg = cfg;
    return true;
}

bool CSocketClientListCfg::getClientList(vector<string> &ClientNamelist)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);

    map<string,clientCfg*>::const_iterator iter;
    for (iter = m_ClientInfomap.begin();iter != m_ClientInfomap.end();iter++)
    {
        ClientNamelist.push_back(iter->second->ClientName);
    }
    return true;
}

clientCfg *CSocketClientListCfg::getClientCfg(const string &ClientName)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    map<string,clientCfg*>::iterator iter = m_ClientInfomap.find(ClientName);
    if(iter != m_ClientInfomap.end())
    {
        return iter->second;
    }
    return NULL;
}

bool CSocketClientListCfg::readServerConfig()
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);

    QString strPath = SOCKET_SERVER_FILE_PATH;

    QFile localfile(strPath);
    if(!localfile.open(QFile::ReadOnly))
    {
        return  false;
    }

    QDomDocument dom;
    if(!dom.setContent(&localfile))
    {
        localfile.close();
        return false;
    }
    QDomElement rootNode = dom.documentElement();
    QDomNodeList NetTypeList = rootNode.childNodes();
    for (int i = 0; i < NetTypeList.size(); ++i)
    {
        QDomElement NetType_elem = NetTypeList.at(i).toElement();
        QDomNodeList NetNodeList = NetType_elem.childNodes();
        if ("SocketServer" == NetType_elem.attribute("Type"))
        {
            for (int j = 0; j < NetNodeList.count(); ++j)
            {
                QDomElement NetNode = NetNodeList.at(j).toElement();
                m_serverCfg.serverName = NetNode.attribute("strID"  ).toStdString();
                m_serverCfg.serverIP = NetNode.attribute("strIP"  ).toStdString();
                m_serverCfg.serverPort = NetNode.attribute("strPort").toInt();
                QDomNodeList SpecifiedNodeList = NetNode.childNodes();
                for (int k = 0; k < SpecifiedNodeList.size(); ++k)
                {
                    QDomElement SpecifiedNode = SpecifiedNodeList.at(k).toElement();
                    clientCfg* stTemp = new clientCfg();
                    stTemp->ClientName  = SpecifiedNode.attribute("strID"  ).toStdString();
                    stTemp->ClientIP   = SpecifiedNode.attribute("strIP"  ).toStdString();
                    stTemp->ClientPort = SpecifiedNode.attribute("strPort").toInt();
                    stTemp->DesIP   = SpecifiedNode.attribute("desIP"  ).toStdString();
                    stTemp->DesPort = SpecifiedNode.attribute("desPort").toInt();
                    m_ClientInfomap.insert(std::pair<string,clientCfg*>(stTemp->ClientName,stTemp));

                    CSocketproxy* pSocketproxy = new CSocketproxy(stTemp->ClientName);
                    pSocketproxy->getSocketClient()->initClient(stTemp->ClientIP.c_str(),stTemp->ClientPort);
                    pSocketproxy->getSocketClient()->setServerCfg(stTemp->DesIP.c_str(), stTemp->DesPort);
                    m_Clientmap.insert(std::pair<string,CSocketproxy*>(stTemp->ClientName,pSocketproxy));
                }
            }
        }
    }
    return true;
}

bool CSocketClientListCfg::writeServerConfig()
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);

    QFile localfile(SOCKET_SERVER_FILE_PATH);
    if(!localfile.open(QFile::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument dom;
    dom.appendChild(dom.createProcessingInstruction("xml", "version=\"2.0\" encoding=\"utf-8\""));

    QDomElement root_elem = dom.createElement("vstudiodoc");
    root_elem.setAttribute("version",2.0);
    dom.appendChild(root_elem);

    QDomElement NetType_Server_elem = dom.createElement("NetType");
    NetType_Server_elem.setAttribute("Type", "SocketServer");
    root_elem.appendChild(NetType_Server_elem);

    QDomElement Net_elem = dom.createElement("SocketServer");
    Net_elem.setAttribute("strID"  , QString::fromStdString(m_serverCfg.serverName));
    Net_elem.setAttribute("strIP"  , QString::fromStdString(m_serverCfg.serverIP));
    Net_elem.setAttribute("strPort", m_serverCfg.serverPort);
    map<string,clientCfg*>::iterator specifiedClient = m_ClientInfomap.begin();
    for (; specifiedClient!= m_ClientInfomap.end(); ++specifiedClient)
    {
        QDomElement specifiedClient_elem = dom.createElement("SpecifiedClient");
        specifiedClient_elem.setAttribute("strID"  , QString::fromStdString(specifiedClient->first));
        specifiedClient_elem.setAttribute("strIP"  , QString::fromStdString(specifiedClient->second->ClientIP));
        specifiedClient_elem.setAttribute("strPort", specifiedClient->second->ClientPort);
        specifiedClient_elem.setAttribute("desIP"  , QString::fromStdString(specifiedClient->second->DesIP));
        specifiedClient_elem.setAttribute("desPort", specifiedClient->second->DesPort);
        Net_elem.appendChild(specifiedClient_elem);
    }
    NetType_Server_elem.appendChild(Net_elem);

    QTextStream out(&localfile);
    dom.save(out, 4);
    localfile.close();
    return true;
}

CSocketClientListCfg *CSocketClientListCfg::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CSocketClientListCfg();
    }
    return m_pInstance;
}

void CSocketClientListCfg::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
    }
    return ;
}


