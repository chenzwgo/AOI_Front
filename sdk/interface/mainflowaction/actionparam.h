#ifndef _ACTION_PARAM_H_
#define _ACTION_PARAM_H_
#include <string>
using namespace std;

typedef struct ActionVar
{
    string strVarTypename;
    const void* pVarPoint;
    ActionVar()
    {
        pVarPoint = NULL;
    }

    ActionVar(ActionVar&& rhs) 
    {
        pVarPoint = rhs.pVarPoint;
        strVarTypename = rhs.strVarTypename;
    }

    ActionVar(const ActionVar& rhs) 
    {
        pVarPoint = rhs.pVarPoint;
        strVarTypename = rhs.strVarTypename;
    }


    template<class Type> ActionVar(Type& x) 
    {
        pVarPoint = (void*) x;
        strVarTypename = typeid(Type).name();
    }


    template<class Type> ActionVar(const Type& x) 
    {
        pVarPoint = (void*) x;
        strVarTypename = typeid(Type).name();
    }

    ActionVar& operator=(const ActionVar& rhs) {
        pVarPoint = rhs.pVarPoint;
        strVarTypename = rhs.strVarTypename;
        return (*this);
    }

    template<class Type> ActionVar& operator=(const Type&& x) {
        pVarPoint = (void*) x;
        strVarTypename = typeid(Type).name();
        return (*this);
    }

    bool operator ==(const ActionVar& rhs)
    {
        return pVarPoint == rhs.pVarPoint && strVarTypename.compare(rhs.strVarTypename) == 0;
    }

    bool operator ==(ActionVar& rhs)
    {
        return pVarPoint == rhs.pVarPoint && strVarTypename.compare(rhs.strVarTypename) == 0;
    }

};

typedef struct ActionVarInfo
{
    int iVarIndex;
    string strVardesc;
    ActionVar ActVar;
    ActionVarInfo()
    {
        iVarIndex = -1;  
    }

    ActionVarInfo(const ActionVar& x, string strDes = "",int iIndex = -1) 
    {
        ActVar = x;
        strVardesc = strDes;
        iVarIndex = iIndex;
    }

    ActionVarInfo(ActionVar& x, string strDes = "",int iIndex = -1) 
    {
        ActVar = x;
        strVardesc = strDes;
        iVarIndex = iIndex;
    }

    template<class Type> ActionVarInfo(const Type& x, string strDes = "",int iIndex = -1) 
    {
        ActVar = x;
        strVardesc = strDes;
        iVarIndex = iIndex;
    }


    ActionVarInfo& operator=(const ActionVarInfo& rhs) 
    {
        ActVar = rhs.ActVar;
        strVardesc = rhs.strVardesc;
        iVarIndex = rhs.iVarIndex;
        return (*this);
    }

    const string TypeName()
    {
        return ActVar.strVarTypename;
    }

    template<class Type> 
    bool IsType()
    {
        if (typeid(Type).name() == ActVar.strVarTypename)
        {
            return true;
        }
        return false;
    }

    template<class Type>
    const Type getValuePoint()
    {
        if (typeid(Type).name() == ActVar.strVarTypename)
        {
            return (Type)ActVar.pVarPoint;
        }
        return NULL;
    }
};

#endif