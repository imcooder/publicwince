#include "stdafx.h"
#include "HWSip.h"
#include "HWDebug.h"
#include "HWStrSafe.h"

#ifdef WINCE
#include <SipApi.h>
#endif


typedef BOOL (*LPSipGetCurrentIM)( CLSID*);
typedef BOOL (*LPSipSetCurrentIM)( CLSID*);

LONG WINAPI HWSipSwitch( LPCWSTR pszGUID)
{
	if (!pszGUID)
	{
		return -1;
	}
	//为了兼容WinCE设备 采用动态加载DLL
	LPSipGetCurrentIM pSipGetCurrentIM = NULL;
	LPSipSetCurrentIM	pSipSetCurrentIM = NULL;
	HMODULE hModule = LoadLibrary(TEXT("Coredll.dll"));
	HWTRACEEX(!hModule, TEXT("LoadLibrary Coredll.dll failed %08X"), GetLastError());
	if (hModule)
	{
		pSipGetCurrentIM = (LPSipGetCurrentIM)GetProcAddress(hModule, L"SipGetCurrentIM");
		HWTRACEEX(!pSipGetCurrentIM, TEXT("GetProcAddress SipGetCurrentIM failed %08X"), GetLastError());
		pSipSetCurrentIM = (LPSipSetCurrentIM)GetProcAddress(hModule, L"SipSetCurrentIM");
		HWTRACEEX(!pSipSetCurrentIM, TEXT("GetProcAddress SipSetCurrentIM failed %08X"), GetLastError());		
	}	
	if (!pSipSetCurrentIM || !pSipGetCurrentIM)
	{
		FreeLibrary(hModule);
		hModule = NULL;
		return -1;
	}
	CLSID clsSip;
	ZeroMemory(&clsSip, sizeof(clsSip));		
	pSipGetCurrentIM(&clsSip);
	TCHAR szCLSIDSip[64] = {0};
	StringCchFromGuid(&clsSip, szCLSIDSip, _countof(szCLSIDSip));
	if (_tcsicmp(szCLSIDSip, pszGUID))
	{
		StringCchToGuid(pszGUID, _tcslen(pszGUID), &clsSip);
		pSipSetCurrentIM(&clsSip);
	}
	pSipGetCurrentIM = NULL;
	pSipSetCurrentIM = NULL;
	if (hModule)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
}