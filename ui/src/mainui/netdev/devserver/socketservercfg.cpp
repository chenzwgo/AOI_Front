#include "socketservercfg.h"
#include <QDomElement>
#include <QFile>
#include <QTextStream>
#define  SOCKET_SERVER_FILE_PATH "./plugins/mainui/ServerConfig.xml"

CSocketServerCfg* CSocketServerCfg::m_pInstance = NULL;


CSocketServerCfg::CSocketServerCfg()
{

    readServerConfig();
}

CSocketServerCfg::~CSocketServerCfg()
{

}

serverCfg* CSocketServerCfg::getServerCfg()
{
    return &m_serverCfg;
}

bool CSocketServerCfg::addClientCfg(const clientCfg &cfg)
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
    clientCfg* pstclientCfg = new clientCfg(cfg);
    m_ClientInfomap.insert(pair<string,clientCfg*>(pstclientCfg->ClientName,pstclientCfg));
    return true;
}

bool CSocketServerCfg::delClientCfg(const string &ClientName)
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

//    if(iterCliet->second->isConnected())
//    {
//        return false;
//    }

//    if(iterCliet->second != NULL)
//    {
//        delete iterCliet->second;
//        m_mapSMCameraClient.erase(iterCliet);
//    }

    if(iterCfg->second != NULL)
    {
        delete iterCfg->second;
        m_ClientInfomap.erase(iterCfg);
    }

    return true;
}

bool CSocketServerCfg::ModifyClientCfg(const string &ClientName, const clientCfg &cfg)
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

    clientCfg*pstSMCameraCfg = m_ClientInfomap[ClientName];
    if(pstSMCameraCfg == NULL)
    {
        return false;
    }
    *pstSMCameraCfg = cfg;
    return true;
}

bool CSocketServerCfg::getClientList(vector<string> &ClientNamelist)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);

    map<string,clientCfg*>::const_iterator iter;
    for (iter = m_ClientInfomap.begin();iter != m_ClientInfomap.end();iter++)
    {
        ClientNamelist.push_back(iter->second->ClientName);
    }
    return true;
}

clientCfg *CSocketServerCfg::getClientCfg(const string &ClientName)
{
    CISAPUGuard<JMutex> autoLock(m_mapMutex);
    map<string,clientCfg*>::iterator iter = m_ClientInfomap.find(ClientName);
    if(iter != m_ClientInfomap.end())
    {
        return iter->second;
    }
    return NULL;
}

bool CSocketServerCfg::readServerConfig()
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
                    m_ClientInfomap.insert(std::pair<string,clientCfg*>(stTemp->ClientName,stTemp));
                }
            }
        }
    }
    return true;
}

bool CSocketServerCfg::writeServerConfig()
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
        Net_elem.appendChild(specifiedClient_elem);
    }
    NetType_Server_elem.appendChild(Net_elem);

    QTextStream out(&localfile);
    dom.save(out, 4);
    localfile.close();
    return true;
}

CSocketServerCfg *CSocketServerCfg::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CSocketServerCfg();
    }
    return m_pInstance;
}

void CSocketServerCfg::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance;
    }
    return ;
}


