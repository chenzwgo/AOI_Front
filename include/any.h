#ifndef _RUNTIME_ANY_H_
#define _RUNTIME_ANY_H_

#ifdef _MSC_VER || WIN32 || _WIN64 || __WIN32__ || WINVER
#include "any_win.h"
#else
#include "any_mac.h"
#endif

#endif