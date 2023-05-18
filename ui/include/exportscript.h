#ifndef EXPORTSCRIPT_H
#define EXPORTSCRIPT_H


#ifdef WIN32
#ifdef SCRIPT_EXPORTS
#define SCRIPT_EXPORT __declspec(dllexport)
#else
#define SCRIPT_EXPORT __declspec(dllimport)
#endif
#else
#define SCRIPT_EXPORT
#endif


#include <QObject>
#include <QString>
#include <QStringList>

typedef QObject* CreatorFunctionCB();

template<class T> QObject* CreateObject() { return new T(); };


class SCRIPT_EXPORT ExportScript : public QObject
{
    Q_OBJECT
public:

    enum IntrospectionType {
      Class,
      Function,
      Variable,
      Module,
      Anything
    };
    static ExportScript* self();

    bool ClearVariables();
    bool EvalFile(const char *pScriptFile);
    bool parseFile(const char *pScriptFile);
    bool evalString(const char *pstring);

    bool EvalFileWithDebug(const char * pScriptFile);
    void RegisterDebugCallBack( void* pfun );

    void registerCPPClass(const char* typeName, const char* parentTypeName = NULL, const char* package = NULL,CreatorFunctionCB* wrapperCreator = NULL);
    void addDecorators(QObject* o);

    QStringList introspection(const char* pModuleName, IntrospectionType type);

    void InitScriptUIModule();
private:
    ExportScript();
    ~ExportScript();
    static ExportScript* m_pSelf;

signals:
    void ScriptStdOut(const QString&);
    void ScriptStdErr(const QString&);
};



#endif // EXPORTSCRIPT_H
