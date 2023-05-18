#ifndef CREATE_DUMP_H
#define CREATE_DUMP_H

#include <windows.h>
#include <DbgHelp.h>
#include <stdlib.h>
#include <tchar.h>
#pragma comment(lib, "dbghelp.lib")
 
inline bool IsDataSectionNeeded(const WCHAR* pModuleName)
{
#ifdef Q_OS_WIN

	if(pModuleName == 0)
	{
		return false;
	}
 
	WCHAR szFileName[_MAX_FNAME] = L"";
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);
 
 
	if(wcsicmp(szFileName, L"ntdll") == 0)
		return true;
 
#endif 
	return false;
}
 
 
inline bool CALLBACK MiniDumpCallback(PVOID                            pParam,
	const PMINIDUMP_CALLBACK_INPUT   pInput,
	PMINIDUMP_CALLBACK_OUTPUT        pOutput)
{
#ifdef Q_OS_WIN

	if(pInput == 0 || pOutput == 0)
		return false;
 
 
	switch(pInput->CallbackType)
	{
	case ModuleCallback:
		if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
			if(!IsDataSectionNeeded(pInput->Module.FullPath))
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
	case IncludeModuleCallback:
	case IncludeThreadCallback:
	case ThreadCallback:
	case ThreadExCallback:
		return true;
	default:;
	}
 
#endif 
	return false;
}
 
 
inline void CreateMiniDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName)
{
#ifdef Q_OS_WIN

	HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 
 
	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION mdei;
		mdei.ThreadId           = GetCurrentThreadId();
		mdei.ExceptionPointers  = pep;
		mdei.ClientPointers     = NULL;
 
 
		MINIDUMP_CALLBACK_INFORMATION mci;
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mci.CallbackParam       = 0;
 
 
		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, (pep != 0) ? &mdei : 0, NULL, &mci);
 
 
		CloseHandle(hFile);
	}
#endif
}
 
 
LONG __stdcall MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
#ifdef Q_OS_WIN
	SYSTEMTIME sys;
	wchar_t buffer[128] = { 0 };
	GetLocalTime(&sys);
    swprintf(buffer, L"%4d-%02d-%02d_%02d%-02d-%02d_core.dmp", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	CreateMiniDump(pExceptionInfo, buffer);
 
 
	return EXCEPTION_EXECUTE_HANDLER;
#endif
}
 
 
// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效
void DisableSetUnhandledExceptionFilter()
{
#ifdef Q_OS_WIN
	void* addr = (void*)GetProcAddress(LoadLibrary(L"kernel32.dll"),
		"SetUnhandledExceptionFilter");
 
 
	if (addr)
	{
		unsigned char code[16];
		int size = 0;
 
 
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;
 
 
		DWORD dwOldFlag, dwTempFlag;
		VirtualProtect(addr, size, PAGE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
#endif
}
 
 
void StartMinDump()
{
#ifdef Q_OS_WIN
	//注册异常处理函数
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	//使SetUnhandledExceptionFilter
	DisableSetUnhandledExceptionFilter();
#endif
}
 
 
#endif

