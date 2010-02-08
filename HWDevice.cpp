
#include "stdafx.h"
#include "HWDevice.h"
#include "Rect.h"


#ifdef WINCE
#include <pm.h>
#endif

void WINAPI BacklightForce( BOOL blOpen)
{
#ifdef WINCE
	if (blOpen)
	{
		SetDevicePower(TEXT("BKL1:"), (DWORD)(0x00000001), D0); 			
	}
	else
	{
		SetDevicePower(TEXT("BKL1:"), (DWORD)(0x00000001), PwrDeviceUnspecified); 		
	}
#endif
}				

void WINAPI BacklightLight()
{
#ifdef WINCE
	HKEY hKey = NULL;
	HANDLE hEventWakeup = NULL;
	TCHAR szEventName[MAX_NAME] = {0};

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCTSTR)TEXT("SYSTEM\\GWE"), 0, 0, &hKey) && hKey)
	{					
		DWORD dwSize = sizeof(szEventName);
		DWORD dwType = REG_SZ;
		RegQueryValueEx(hKey, TEXT("ActivityEvent"), NULL, &dwType, (LPBYTE)szEventName, &dwSize);	
		RegCloseKey(hKey);
		hKey = NULL;		
	}	
	if (szEventName[0])
	{
		hEventWakeup = CreateEvent(NULL, FALSE, FALSE, szEventName);
		if (hEventWakeup)
		{
			for (UINT nIdx = 0; nIdx < 5; nIdx ++)
			{
				SetEvent(hEventWakeup);
			}
			CloseHandle(hEventWakeup);
			hEventWakeup = NULL;
		}
	}		
#endif
	return;	
}

#ifdef WINCE
typedef   struct   _STORAGE_IDENTIFICATION   
{    
	DWORD   dwSize;    
	DWORD   dwFlags;    
	DWORD   dwManufactureIDOffset;    
	DWORD   dwSerialNumOffset;    
}   STORAGE_IDENTIFICATION,   *PSTORAGE_IDENTIFICATION; 
#ifndef FILE_DEVICE_DISK
#define FILE_DEVICE_DISK 0x00000007
#endif
#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED                 0
#endif
#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS 0
#endif
#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
	((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
	)
#endif

#ifndef FILE_DEVICE_DISK
#define FILE_DEVICE_DISK 0x00000007
#endif
#ifndef METHOD_BUFFERED
#define METHOD_BUFFERED                 0
#endif
#ifndef FILE_ANY_ACCESS
#define FILE_ANY_ACCESS 0
#endif
#ifndef CTL_CODE
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
	((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
	)
#endif
LONG GetID(LPCTSTR pszDevice, LPTSTR pszID, LONG nLen)
{
	TCHAR szDevice[128] = {0};
	if (!pszDevice)
	{
		return 0;
	}
	StringCchCopy(szDevice, _countof(szDevice), pszDevice);
	TCHAR   tzSDCardID[22] = {0}; 
	HANDLE   hCard = CreateFile(szDevice, GENERIC_READ|GENERIC_WRITE,	0, NULL, OPEN_EXISTING, 0,  NULL); 
	if(hCard) 
	{ 
		int  i  =  0; 
		PSTORAGE_IDENTIFICATION  pStoreInfo;    
		pStoreInfo=(STORAGE_IDENTIFICATION *)LocalAlloc(LMEM_ZEROINIT, 3000);    

		DWORD   dwGetBytes; 
		if   (DeviceIoControl(hCard, 	CTL_CODE(FILE_DEVICE_DISK,   0x0709, METHOD_BUFFERED,  FILE_ANY_ACCESS), NULL, 0, (LPVOID)pStoreInfo, 3000, &dwGetBytes, NULL))    
		{    
			if(dwGetBytes > 0) 
			{ 
				i  =  0; 
				int  j = 0; 
				if(pStoreInfo->dwSerialNumOffset  <  dwGetBytes) 
				{ 
					BYTE*  SerialNo=(((BYTE*)pStoreInfo) + pStoreInfo->dwSerialNumOffset);    
					while(SerialNo[i] && i < (int)(dwGetBytes-pStoreInfo->dwSerialNumOffset) &&  i < 22)    
					{    
						if(SerialNo[i] < '0' ||  SerialNo[i] >  'z') 
						{ 
							j ++; 
						} 
						else 
						{ 
							tzSDCardID[i - j]=(TCHAR)SerialNo[i];    
						} 
						i ++;    
					}    
				} 
				tzSDCardID[i - j]=0;   
				StringCchCopy(pszID, nLen, tzSDCardID);
			} 
		} 		
		LocalFree(pStoreInfo);    
		CloseHandle(hCard); 		
	}
	return _tcslen(pszID);
}
#endif

LONG WINAPI XUE_GetSDID(LPTSTR pszID, LONG nLen)
{
	TCHAR		szID[1024] = {0};
	HKEY		hKeyActive = NULL;	
	HKEY		hRegKey = NULL;	
	HKEY		hGuidKey = NULL;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Drivers\\Active"), 0, KEY_ALL_ACCESS, &hKeyActive))
	{		
		return 0;
	}			
	DWORD dwSubKeyCount = 0;
	if(ERROR_SUCCESS != RegQueryInfoKey(hKeyActive, 0, 0, 0, &dwSubKeyCount, 0, 0, 0, 0, 0, 0, 0))   
	{   	
		RegCloseKey(hKeyActive);
		hKeyActive = NULL;		
		return 0;
	}   
	TCHAR szItemID[128] = {0};
	DWORD dwValueSize = 0;
	DWORD dwValueType = 0;
	TCHAR szKName[512] = {0};   
	DWORD cbKName = sizeof(szKName); 
	TCHAR szDeviceName[512] = {0};
	for (DWORD dwIdx = 0; dwIdx < dwSubKeyCount; dwIdx ++)
	{
		ZeroMemory(szKName, sizeof(szKName));
		cbKName = sizeof(szKName);
		if (ERROR_SUCCESS == RegEnumKeyEx(hKeyActive, dwIdx, szKName, &cbKName, NULL, NULL, NULL, NULL))
		{
			HKEY hSubKey = NULL;
			if (ERROR_SUCCESS == RegOpenKeyEx (hKeyActive, szKName, 0L, KEY_READ, &hSubKey))
			{
				if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("Name"), NULL,	&dwValueType, NULL, &dwValueSize) && REG_SZ == dwValueType)
				{		
					dwValueSize = sizeof(szDeviceName);
					if (ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("Name"), NULL,	&dwValueType, (LPBYTE)&szDeviceName, &dwValueSize) && szDeviceName[0])
					{
						szItemID[0] = 0;
#ifdef WINCE
						GetID(szDeviceName, szItemID, _countof(szItemID));
#endif
						if (szItemID[0])
						{
							StringCchCat(szID, _countof(szID), szItemID);
							StringCchCat(szID, _countof(szID), TEXT(" "));
						}						
					}					
				}
				RegCloseKey(hSubKey);
				hSubKey = NULL;
			}			
		}		
	}	
	RegCloseKey(hKeyActive);
	hKeyActive = NULL;	

	if (!pszID && !nLen)
	{
		return _tcslen(szID);
	}
	else if (pszID && nLen)
	{
		StringCchCopy(pszID, nLen, szID);
		return _tcslen(pszID);
	}
	return 0;
}

