#include "defectlevelthreads.h"
#include "defect/idefectqualification.h"
#include "callsdk.h"
#include "maindefine.h"
#include "trace.h"

CDefectLevelThreads::CDefectLevelThreads()
{
	//i当前无效类--
	//
    m_bRunning = true;
    //m_pIDefectQualification = (IDefectQualification*)GetSDKInstance("defectqualification", IDefectQualification::IDD_DEFECT_QULIFICATION);
	//for (int i = 0; i < DEFECT_TYPE_SIZE;i++)
	//{
	//	m_threads.push_back(new JThreadTP<CDefectLevelThreads, void*, void>());
	//	m_DefectFeaturevec.push_back(new vector<stDefectFeature*>());
	//}


}

CDefectLevelThreads::~CDefectLevelThreads()
{
    m_bRunning = false;
    for(int i=0 ;i <m_threads.size();i++)
    {
        JThreadTP<CDefectLevelThreads,void*,void>*pThread = m_threads.at(i);
        if(pThread != NULL &&pThread->IsRunning())
        {
            pThread->EndThread();
        }
        delete pThread;
    }
	for (int i = 0; i < m_DefectFeaturevec.size(); i++)
	{
		if (m_DefectFeaturevec.at(i) != NULL)
		{
			delete m_DefectFeaturevec.at(i);
			m_DefectFeaturevec.at(i) = NULL;
		}
	}
    m_threads.clear();
	m_DefectFeaturevec.clear();

}

void CDefectLevelThreads::setDefectVect(vector<stDefectFeature *> *pAllDefectFeature)
{
	clearDataBuffer();
    if(pAllDefectFeature == NULL || pAllDefectFeature->size() == 0)
    {
        return;
    }
    int iDefectFeatureSize =pAllDefectFeature->size();
	for (int i = 0; i < iDefectFeatureSize;i++)
	{
		stDefectFeature *pstDefectFeature  =pAllDefectFeature->at(i);
        if (pstDefectFeature->iDefectType >= m_DefectFeaturevec.size() || pstDefectFeature->iDefectType <0)
		{
			continue;
		}
		m_DefectFeaturevec[pstDefectFeature->iDefectType]->push_back(pstDefectFeature);
	}
}

bool CDefectLevelThreads::isRunning()
{
    for(int i = 0 ;i <m_threads.size();i++)
    {
        JThreadTP<CDefectLevelThreads,void*,void>*pThread = m_threads.at(i);
        if(pThread != NULL &&pThread->IsRunning())
            return true;
    }
    return false;
}

bool CDefectLevelThreads::isFinish()
{
    for(int i= 0 ;i <m_threads.size();i++)
    {
        JThreadTP<CDefectLevelThreads,void*,void>*pThread = m_threads.at(i);
        if(pThread != NULL &&pThread->IsRunning())
            return false;
    }
    return true;
}

bool CDefectLevelThreads::start()
{
    for(int i = 0 ;i<m_threads.size();i++)
    {
        JThreadTP<CDefectLevelThreads,void*,void>*pThread = m_threads.at(i);
        vector<stDefectFeature*> *pData = m_DefectFeaturevec.at(i);
        if(pThread != NULL && pData !=NULL && pData->size() >0)
        {
			if (!pThread->IsRunning())
			{
				pThread->StartThread(this,&CDefectLevelThreads::processFun,pData);
			}
            
        }
    }
    return true;
}

bool CDefectLevelThreads::stop()
{
	m_bRunning = false;
	for (int i = 0; i < m_threads.size(); i++)
	{
		JThreadTP<CDefectLevelThreads, void*, void>*pThread = m_threads.at(i);
		if (pThread != NULL &&pThread->IsRunning())
		{
			pThread->EndThread();
		}
	}
    return true;
}

void CDefectLevelThreads::clearDataBuffer()
{
    for(int i = 0;i<m_DefectFeaturevec.size();i++)
    {
        if(m_DefectFeaturevec.at(i) != NULL)
        {
			m_DefectFeaturevec.at(i)->clear(); ;
        }
    }
}

void CDefectLevelThreads::processFun(void *pdata)
{
    vector<stDefectFeature *>* vecdata = (vector<stDefectFeature *>*)pdata;
    for(int i =0 ;i<vecdata->size();i++)
    {
        stDefectFeature *pDefectFeature = vecdata->at(i);
        getDefectLevel(pDefectFeature);
    }
}

bool CDefectLevelThreads::getDefectLevel(stDefectFeature *pDefectRes)
{
    if (NULL == pDefectRes || m_pIDefectQualification == NULL)
    {
        return false;
    }
    int iSize = m_defectNameVec.size();
    int iDefectType = pDefectRes->iDefectType;
    if (iDefectType > iSize || iDefectType <= 0)
    {
        return false;
    }
    return m_pIDefectQualification->getDefectSeverityLevel(iDefectType, pDefectRes);
}
