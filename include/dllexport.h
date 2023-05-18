#ifndef DLL_EXPORT_H
#define DLL_EXPORT_H

#ifndef Q_DECL_EXPORT
    #ifdef Q_OS_WIN
        #define Q_DECL_EXPORT __declspec(dllexport)
    #else
        #ifdef _WINDOWS
            #define Q_DECL_EXPORT __declspec(dllexport)
        #else
            #define Q_DECL_EXPORT
        #endif
    #endif
#endif

#ifndef Q_DECL_IMPORT
    #if defined(Q_OS_WIN)
        #define Q_DECL_IMPORT __declspec(dllimport)
    #else
        #ifdef _WINDOWS
            #define Q_DECL_IMPORT __declspec(dllimport)
        #else
            #define Q_DECL_IMPORT
        #endif
    #endif
#endif

#ifdef RT_EXPORTS
    #define RT_API Q_DECL_EXPORT
#else
    #define RT_API Q_DECL_IMPORT
#endif


#endif
