#ifndef _SCRIPT_CORE_H_
#define _SCRIPT_CORE_H_

#include "dllexport.h"
#include <QObject>
#include <QMap>

#ifdef SCRIPT_CORE_EXPORTS
#define SCRIPT_CORE_API Q_DECL_EXPORT
#else
#define SCRIPT_CORE_API Q_DECL_IMPORT
#endif

class IScriptObjWrap;
class SCRIPT_CORE_API ScriptCore: public QObject
{
    Q_OBJECT
private:
    ScriptCore();
    ~ScriptCore();
public:
    static ScriptCore* GetScriptCore();
    static void ReleaseScriptCore();

    void AddModule(const QString& strDisplay,QObject* pObj);
    bool EvalFile(const char* pScriptFile);
    bool BuildFile(const char* pScriptFile);
    bool ClearBuildFile(const char* pScriptFile);
    bool EvalString(const char* pString);
    bool EvalFileWithDebug(const char* pScriptFile);
    void RegisterDebugCallBack(void* pfun);
    void QuitScriptRun(const char* pString);

    QStringList GetVariableList(const QString& strModuleName);
    QStringList GetClassList(const QString& strModuleName);
    QStringList GetModuleList(const QString& strModuleName);
    QStringList GetFunctionList(const QString& strModuleName);
    QStringList GetAllList( const QString& strModuleName );
    QString GetFunDesc(const QString& strModuleName,const QString& strFunName );



private:
    static ScriptCore* m_pSelf;
    bool m_binit;
    bool InitScriptCore();
    bool InitScriptUIModule();

    QMap<QString,IScriptObjWrap*> m_pMapObjs;

signals:
    void ScriptRunOut(const QString&);
    void ScriptRunErr(const QString&);
};

#endif //_SCRIPT_CORE_H_
