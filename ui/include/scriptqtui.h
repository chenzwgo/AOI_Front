#ifndef _SCRIPT_QT_UI_H
#define _SCRIPT_QT_UI_H

#ifdef WIN32
#ifdef SCRIPT_QTALL_EXPORTS
#define SCRIPT_QTALL_EXPORT __declspec(dllexport)
#else
#define SCRIPT_QTALL_EXPORT __declspec(dllimport)
#endif
#else
#define SCRIPT_QTALL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

  SCRIPT_QTALL_EXPORT void InitScriptUI();

#ifdef __cplusplus
}
#endif

#endif
