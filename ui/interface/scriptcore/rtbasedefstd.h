#ifndef _BASE_DEF_STD_H_
#define _BASE_DEF_STD_H_
#include <string>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <memory.h>
using namespace std;
#define ReturnArg  Param

typedef struct stFunctionInfo
{
	string strFunctionName;
	string strRetType;
	string Argslist[8];
	unsigned int iArgCount;
	stFunctionInfo()
	{
		strFunctionName="";
        strRetType = "";
		iArgCount = 0;
        for (unsigned int index = 0 ;index < sizeof(Argslist)/sizeof(Argslist[0]);index++)
        {
            Argslist[index] = "";
        }
	}

	bool operator==(const stFunctionInfo& other)
	{
		if (&other == this)
		{
			return true;
		}
		if (strFunctionName== other.strFunctionName &&
			this->iArgCount == other.iArgCount )
		{
			for (unsigned int i = 0;i< this->iArgCount;i++)
			{
				if (this->Argslist[i] != other.Argslist[i]) 
				{
					return false;
				}
			}
			return true;
		}	
		return false;
	}
	stFunctionInfo& operator=(const stFunctionInfo& other)
	{
		if (&other == this)
		{
			return *this;
		}
		iArgCount = other.iArgCount;
        strFunctionName = other.strFunctionName;
        strRetType = other.strRetType;
        for (unsigned int index = 0 ;index < sizeof(Argslist)/sizeof(Argslist[0]);index++)
        {
            Argslist[index] = other.Argslist[index];
        }
		return *this;
	}

}stFunctionInfo;

typedef struct keybyvalue
{
	char type[128];
	void *value;
	keybyvalue()
	{
		memset(type,0,sizeof(type));
		value = NULL;
	}
	~keybyvalue()
	{
		if (value != NULL )
		{
			delete value;
			value = NULL;
		}
	}
}stkeybyvalue;

typedef struct stArgs
{
	unsigned int iArgcount;
	stkeybyvalue arg[8];

}stArgs;


class CMallocFunction
{
public:
	template<typename T>
	static stFunctionInfo* MallocFunctionInfo(const string& pFunctionName)
	{
        if(pFunctionName.length() == 0)
		{
                        return NULL;
		}

		stFunctionInfo *pInfo = new stFunctionInfo();
		if (pInfo == NULL)
		{
			return NULL;
		}
		pInfo->iArgCount = 0;
        pInfo->strFunctionName = pFunctionName;
	    pInfo->strRetType = typeid(T).name();
		return pInfo;
	}

	template<typename T1,typename T2>
    static stFunctionInfo* MallocFunctionInfo(const string& pFunctionName)
	{
        if(pFunctionName.length() == 0)
		{
			return NULL;
		}

		stFunctionInfo* pInfo= new stFunctionInfo();
		if (pInfo == NULL)
		{
			return NULL;
		}

		pInfo->iArgCount = 1;
        pInfo->strFunctionName = pFunctionName;
        pInfo->strRetType = typeid(T1).name();
        pInfo->Argslist[0] = typeid(T2).name();
		return pInfo;

	}

	template<typename T1,typename T2,typename T3>
    static stFunctionInfo* MallocFunctionInfo(const string& pFunctionName)
	{
        if(pFunctionName.length() == 0)
		{
			return NULL;
		}

		stFunctionInfo* pInfo= new stFunctionInfo();
		if (pInfo == NULL)
		{
			return NULL;
		}
		pInfo->iArgCount = 2;
        pInfo->strFunctionName = pFunctionName;
        pInfo->strRetType = typeid(T1).name();
        pInfo->Argslist[0] = typeid(T2).name();
        pInfo->Argslist[1] = typeid(T3).name();
		return pInfo;
	}

	template<typename T1,typename T2,typename T3,typename T4>
    static stFunctionInfo* MallocFunctionInfo(const string& pFunctionName)
	{
        if(pFunctionName.length() == 0)
		{
			return NULL;
		}

		stFunctionInfo* pInfo= new stFunctionInfo();
		if (pInfo == NULL)
		{
			return NULL;
		}

		pInfo->iArgCount = 3;
        pInfo->strFunctionName = pFunctionName;
        pInfo->strRetType = typeid(T1).name();
        pInfo->Argslist[0] = typeid(T2).name();
        pInfo->Argslist[1] = typeid(T3).name();
        pInfo->Argslist[2] = typeid(T4).name();

		return pInfo;
	}

	template<typename T1,typename T2,typename T3,typename T4,typename T5>
    static stFunctionInfo* MallocFunctionInfo(const string& pFunctionName)
	{
        if(pFunctionName.length() == 0)
		{
			return NULL;
		}

		stFunctionInfo* pInfo= new stFunctionInfo();
		if (pInfo == NULL)
		{
			return NULL;
		}

		pInfo->iArgCount = 4;
        pInfo->strFunctionName = pFunctionName;
        pInfo->strRetType = typeid(T1).name();
        pInfo->Argslist[0] = typeid(T2).name();
        pInfo->Argslist[1] = typeid(T3).name();
        pInfo->Argslist[2] = typeid(T4).name();
        pInfo->Argslist[3] = typeid(T5).name();
		return pInfo;
	}

	template<typename T>
	static T convert(void* pValue)
	{
		return *((T*)pValue);
	}
};

class Param
{
public: 
	template<class T>
    Param(T tvalue):m_vaule(NULL),idatasize(0)
	{
		strtype = typeid(T).name();
		m_vaule = (void*) new T(tvalue);
        idatasize = sizeof(T);

	}
	~Param()
	{
		if (m_vaule !=NULL)
		{
			delete m_vaule;
			m_vaule = NULL;
		}
		strtype = "";
        idatasize = 0;
	}

	template<typename T>
	T convert()
	{	
		if (strtype== typeid(T).name())
		{
			return *((T*)m_vaule);
		}
		else
		{
			string tmp = "argument convert error :";
			tmp.append(typeid(T).name());
			tmp.append(" to ");
			tmp.append(strtype);
			tmp.append("error!");
			throw std::invalid_argument(tmp);
		}
	}
    string tostdString()
    {
        if (strtype== typeid(string).name())
        {
            return *((string*)m_vaule);
        }
        return "";
    }
    const char* toConstCharPoint()
    {
        if (strtype== typeid(const char*).name() || strtype== typeid(char*).name())
        {
            return *((const char**)m_vaule);
        }
        return NULL;
    }
    const char toChar()
    {
        if (strtype== typeid(const char).name() || strtype== typeid(char).name())
        {
            return *((const char*)m_vaule);
        }
        return '0';
    }
    int toInt()
    {
        if (strtype== typeid(int).name())
        {
            return *((int*)m_vaule);
        }
        return 0;
    }
    unsigned int toUInt()
    {
        if (strtype== typeid(unsigned int).name())
        {
            return *((unsigned int*)m_vaule);
        }
        return 0;
    }

    bool tobool()
    {
        if (strtype== typeid(bool).name())
        {
            return *((bool*)m_vaule);
        }
        return false;
    }

    long tolong()
    {
        if (strtype== typeid(long).name())
        {
            return *((long*)m_vaule);
        }
        return 0;
    }

    unsigned long toUlong()
    {
        if (strtype== typeid(unsigned long).name())
        {
            return *((unsigned long*)m_vaule);
        }
        return 0;
    }

    float tofloat()
    {
        if (strtype== typeid(float).name())
        {
            return *((float*)m_vaule);
        }
        return 0;
    }

    double toDouble()
    {
        if (strtype== typeid(double).name())
        {
            return *((double*)m_vaule);
        }
        return 0;
    }
    long long tolonglong()
    {
        if (strtype== typeid(long long).name())
        {
            return *((long long*)m_vaule);
        }
        return 0;
    }


	string type() 
	{
		return strtype;
	}

    const Param& operator=(const Param& other)
    {
        if(&other == this)
        {
            return *this;
        }
        if (m_vaule !=NULL)
        {
            delete m_vaule;
            m_vaule = NULL;
        }
        strtype = other.strtype;
        idatasize = other.idatasize;
        memcpy(m_vaule,other.m_vaule,idatasize);
    }
private:
	void *m_vaule;
	string strtype;
    unsigned int idatasize;
};

class ArgList
{
public:
	explicit ArgList()
	{

	}

	~ArgList()
	{
		for(unsigned int i=0;i < count();i++)
		{
			if (args[i] != NULL)
			{
				delete args[i];
				args[i] = NULL;
			}
		}
	}

	int count()
	{
		return args.size();
	}

    ArgList& operator=(const ArgList& other)
    {
        for(unsigned int i=0;i < count();i++)
        {
            if (args[i] != NULL)
            {
                delete args[i]; 
            }
            args[i] = other.args[i];
        }
    }

	template<class T>
	void AddArg(T tvalue)
	{
		Param* p = new Param(tvalue);
		args.push_back(p);
	}

	Param* at(unsigned int index)
	{
		if (index >= args.size())
		{
			return NULL;
		}
		return args[index];
	}
private:
	vector<Param*> args;
};

#endif
