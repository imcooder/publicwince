
#include "stdafx.h"
#include "HWDevice.h"
#include "Rect.h"


#ifdef WINCE
#include <pm.h>
#endif

void WINAPI ForceBacklight( BOOL blOpen)
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

void WINAPI LightBacklight()
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
			SetEvent(hEventWakeup);
			CloseHandle(hEventWakeup);
			hEventWakeup = NULL;
		}
	}		
#endif
	return;	
}
