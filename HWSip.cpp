#include "stdafx.h"
#include "HWSip.h"
#include "HWDebug.h"
#include "HWStrSafe.h"

#ifdef WINCE
#include <SipApi.h>
#endif


typedef BOOL (*LPSipGetCurrentIM)( CLSID*);
typedef BOOL (*LPSipSetCurrentIM)( CLSID*);
typedef BOOL (*LPSipShowIM)( DWORD );


LONG WINAPI XUE_SipSwitch( LPCWSTR pszGUID)
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
	return 0;
}

LONG WINAPI XUE_SipEnable( LPCWSTR pszGUID, BOOL blEnable)
{
	if (!pszGUID)
	{
		return -1;
	}
	TCHAR szEnable[8] = {0};
	StringCchPrintf(szEnable, _countof(szEnable), TEXT("%d"), blEnable ? 1 : 0);
	HKEY hSubKeyIsSip = NULL;
	TCHAR szKeyName[128] = {0};
	StringCchPrintf(szKeyName, _countof(szKeyName), TEXT("CLSID\\%s\\IsSIPInputMethod"), pszGUID);
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, szKeyName, 0, KEY_ALL_ACCESS, &hSubKeyIsSip))
	{		
		return -1;
	}			
	if (ERROR_SUCCESS != RegSetValueEx(hSubKeyIsSip, NULL, NULL, REG_SZ, (LPBYTE)szEnable, (_tcslen(szEnable) + 1) * sizeof(*szEnable)))
	{
		SAFE_REG_CLOSEKEY(hSubKeyIsSip);		
		return -1;
	}
	SAFE_REG_CLOSEKEY(hSubKeyIsSip);
	return 0;
}

BOOL WINAPI XUE_SipShowIM( DWORD dwFlag )
{
	BOOL blRet = FALSE;
	//为了兼容WinCE设备 采用动态加载DLL
	LPSipShowIM	 pSipShowIM = NULL;	
	HMODULE hModule = LoadLibrary(TEXT("Coredll.dll"));
	HWTRACEEX(!hModule, TEXT("LoadLibrary Coredll.dll failed %08X"), GetLastError());
	if (hModule)
	{
		pSipShowIM = (LPSipShowIM)GetProcAddress(hModule, L"SipShowIM");
		HWTRACEEX(!pSipShowIM, TEXT("GetProcAddress SipShowIM failed %08X"), GetLastError());				
	}	
	if (!pSipShowIM)
	{
		FreeLibrary(hModule);
		hModule = NULL;
		return blRet;
	}

	blRet = (*pSipShowIM)(dwFlag);



	pSipShowIM = NULL;	
	if (hModule)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
	return blRet;
}