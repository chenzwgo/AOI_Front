#ifndef DEFECTLEVELTHREADS_H
#define DEFECTLEVELTHREADS_H
#include "jthreadt.h"
#include "maindefine.h"
#include <vector>
#define THREAD_PROCESS_CAPACITY     20
class IDefectQualification;
class CDefectLevelThreads
{
public:
    CDefectLevelThreads();
    ~CDefectLevelThreads();
    void setDefectVect(vector<stDefectFeature*> *pAllDefectFeature);
    bool isRunning();
    bool isFinish();
    bool start();
    bool stop();
    void clearDataBuffer();
private:
    vector<JThreadTP<CDefectLevelThreads,void*,void>* > m_threads;
    vector<vector<stDefectFeature*> *> m_DefectFeaturevec;
    bool m_bRunning;
    IDefectQualification *m_pIDefectQualification;
	vector<string> m_defectNameVec;

private:
    void processFun(void * pdata);
    bool getDefectLevel(stDefectFeature *pDefectRes);
};

#endif // DEFECTLEVELTHREADS_H
