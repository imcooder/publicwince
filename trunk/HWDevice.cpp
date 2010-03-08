
#include "stdafx.h"
#include "HWDevice.h"
#include "Rect.h"
#include <projects.h>
#pragma comment(lib, "note_prj.lib")

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

#define IOCTL_DISK_BASE           FILE_DEVICE_DISK 
#define IOCTL_DISK_GET_STORAGEID  CTL_CODE(IOCTL_DISK_BASE, 0x709,METHOD_BUFFERED, FILE_ANY_ACCESS)

const UINT MINIMUM_CARD_SIZE = 63 * (1024 * 1024); 
BOOL WINAPI isCardAvailable(TCHAR *szPath) 
{ //Î´¿¼ÂÇ¶à¸ö´æ´¢¿¨
	BOOL res = FALSE; 
	HANDLE hFlashCard = NULL; 
	WIN32_FIND_DATA find; 
	BOOL loop = TRUE; 

	ULARGE_INTEGER freeBytesAvailableToCaller; 
	ULARGE_INTEGER totalNumberOfBytes; 
	ULARGE_INTEGER totalNumberOfFreeBytes; 

	memset(&find, 0, sizeof(WIN32_FIND_DATA)); 

	hFlashCard = ::FindFirstFlashCard(&find); 

	if(INVALID_HANDLE_VALUE != hFlashCard) 
	{  
		if(((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) && 
			((find.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY)) 
		{ 
			res = ::GetDiskFreeSpaceEx(find.cFileName, &freeBytesAvailableToCaller, 
				&totalNumberOfBytes, 
				&totalNumberOfFreeBytes); 

			// Only count the card if it is the correct size 
			if((res == TRUE) && (totalNumberOfBytes.QuadPart >= MINIMUM_CARD_SIZE)) 
			{ 
				// Save the name of the flash card 
				_tcsncpy(szPath, find.cFileName, MAX_PATH); 

			} 
		} 

		::FindClose (hFlashCard); 
	} 

	return res; 
} 


BOOL WINAPI XUE_GetSDID2(UINT& uSDID)
{
	TCHAR szPath[MAX_PATH] = {0};

	if (!isCardAvailable(szPath))
	{
		return FALSE;
	}
	StringCchCat(szPath, MAX_PATH, TEXT("\\Vol:"));
	HANDLE theVolumeHandle = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	BOOL success = FALSE;
	if( theVolumeHandle != INVALID_HANDLE_VALUE )
	{
		// Allocate the storage info
		DWORD theBytesReturned = 0;

		int theSize = sizeof(STORAGE_IDENTIFICATION) + 33;
		STORAGE_IDENTIFICATION *theStorageInfo = (STORAGE_IDENTIFICATION*) malloc( theSize );
		memset( theStorageInfo, 0, theSize );

		success = DeviceIoControl(
			theVolumeHandle,
			IOCTL_DISK_GET_STORAGEID,
			NULL,
			0,
			theStorageInfo,
			theSize - 1,
			&theBytesReturned,
			NULL);

		if( success )
		{
			success = FALSE;

			if( theStorageInfo->dwManufactureIDOffset > 0 )
			{
				// Not used.
			}

			if( theStorageInfo->dwSerialNumOffset > 0 && theStorageInfo->dwSerialNumOffset < theStorageInfo->dwSize )
			{
				char* theCardId = ((char*) theStorageInfo ) + theStorageInfo->dwSerialNumOffset;

				// Convert string to long.
				uSDID = strtoul( theCardId, NULL, 16 );
				if( uSDID == 0 || uSDID == ULONG_MAX )
				{
					uSDID = strtoul( theCardId, NULL, 10 );
				}

				if( uSDID != 0 && uSDID != ULONG_MAX )
				{
					success = TRUE;

				}
			}
		}

		// Cleanup.

		free( theStorageInfo );

		CloseHandle( theVolumeHandle );
	}
	return success;

}

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

BOOL WINAPI XUE_GetSDID3(UINT& uSDID)
{
	BOOL blRet = FALSE;
	uSDID = 0;
	HANDLE theVolumeHandle = CreateFile(TEXT("\\Storage Card\\Vol:"), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);	
	if( theVolumeHandle != INVALID_HANDLE_VALUE )
	{
		// Allocate the storage info
		int theSize = sizeof(STORAGE_IDENTIFICATION) + 33;
		STORAGE_IDENTIFICATION* theStorageInfo = (STORAGE_IDENTIFICATION*) malloc( theSize );
		memset( theStorageInfo, 0, theSize );

		DWORD theBytesReturned;
		char* theCardId = NULL;
		long unit_id;
		BOOL success = DeviceIoControl(theVolumeHandle,	IOCTL_DISK_GET_STORAGEID,	NULL,	0,theStorageInfo,theSize - 1,	&theBytesReturned,NULL);
		if( success )
		{
			success = FALSE;
			if( theStorageInfo->dwManufactureIDOffset > 0 )
			{
				// Not used.
			}
			if( theStorageInfo->dwSerialNumOffset > 0 && theStorageInfo->dwSerialNumOffset < theStorageInfo->dwSize )
			{
				theCardId = ((char*) theStorageInfo ) + theStorageInfo->dwSerialNumOffset;
				// Convert string to long.
				unit_id = strtoul( theCardId, NULL, 16 );
				if( unit_id == 0 || unit_id == ULONG_MAX )
				{
					unit_id = strtoul( theCardId, NULL, 10 );
				}
				if( unit_id != 0 && unit_id != ULONG_MAX )
				{
					success = TRUE;					
					uSDID = unit_id;
					blRet = TRUE;
				}
			}
		}

		// Cleanup.
		free( theStorageInfo );
		theStorageInfo = NULL;
		CloseHandle( theVolumeHandle );
		theVolumeHandle = NULL;
	}
	return blRet;
}