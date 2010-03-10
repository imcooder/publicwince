#include "stdafx.h"
#include "HWDevice.h"
#include "Rect.h"
#include <Iphlpapi.h>

#ifdef UNDER_CE
#include <pm.h>
#include "tapi.h"
#include "pkfuncs.h"
#endif

#define DEVICEID_APPLICATION_DATA            TEXT("www.hwpen.net")


void WINAPI BacklightForce( BOOL blOpen)
{
#ifdef UNDER_CE
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
#ifdef UNDER_CE
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

#define TAPI_API_LOW_VERSION    0x00020000
#define TAPI_API_HIGH_VERSION   0x00020000
#define EXT_API_LOW_VERSION     0x00010000
#define EXT_API_HIGH_VERSION    0x00010000

#ifndef _ExTAPI_H_

typedef struct linegeneralinfo_tag {
	DWORD dwTotalSize;
	DWORD dwNeededSize;
	DWORD dwUsedSize;
	DWORD dwManufacturerSize;
	DWORD dwManufacturerOffset;
	DWORD dwModelSize;
	DWORD dwModelOffset;
	DWORD dwRevisionSize;
	DWORD dwRevisionOffset;
	DWORD dwSerialNumberSize;
	DWORD dwSerialNumberOffset;
	DWORD dwSubscriberNumberSize;
	DWORD dwSubscriberNumberOffset;
} LINEGENERALINFO, *LPLINEGENERALINFO;


#endif

#ifndef CELLTSP_PROVIDERINFO_STRING
#define CELLTSP_PROVIDERINFO_STRING (L"Cellular TAPI Service Provider")
#endif
#ifndef CELLTSP_LINENAME_STRING
#define CELLTSP_LINENAME_STRING (L"Cellular Line")
#endif
#ifndef CELLTSP_PHONENAME_STRING
#define CELLTSP_PHONENAME_STRING (L"Cellular Phone")
#endif
//////////////////////////////////////////////////////////////////////////
//CellCore.dll
typedef LONG (WINAPI*LPlineGetGeneralInfo)(HLINE, LPLINEGENERALINFO);
//CoreDll.dll
typedef LONG (WINAPI*LPlineShutdown)(HLINEAPP);
typedef LONG (WINAPI*LPlineClose)(HLINE);
typedef LONG (WINAPI*LPlineOpen)(HLINEAPP, DWORD, LPHLINE, DWORD, DWORD, DWORD, DWORD, DWORD, LPLINECALLPARAMS const);
typedef LONG (WINAPI*LPlineGetDevCaps)(HLINEAPP, DWORD, DWORD, DWORD, LPLINEDEVCAPS);
typedef LONG (WINAPI*LPlineNegotiateAPIVersion)(HLINEAPP, DWORD, DWORD, DWORD, LPDWORD, LPLINEEXTENSIONID);
typedef LONG (WINAPI *LPlineNegotiateExtVersion)(HLINEAPP, DWORD, DWORD, DWORD, DWORD, LPDWORD);
typedef LONG (WINAPI *LPlineInitializeEx)(LPHLINEAPP, HINSTANCE,LINECALLBACK,LPCWSTR,LPDWORD,LPDWORD,LPLINEINITIALIZEEXPARAMS);
typedef HRESULT (WINAPI*LPGetDeviceUniqueID)(LPBYTE , DWORD, DWORD, LPBYTE, DWORD *);

//////////////////////////////////////////////////////////////////////////
#define GETDEVICEUNIQUEID_V1                1
#define GETDEVICEUNIQUEID_V1_MIN_APPDATA    8
#define GETDEVICEUNIQUEID_V1_OUTPUT         20

LONG WINAPI XUE_GetIMEI(LPWSTR pszIMEI, LONG nCount)
{
	TCHAR szIMEI[256] = {0};
	DWORD dwNumDevs;
	DWORD dwAPIVersion = TAPI_API_HIGH_VERSION;
	LINEINITIALIZEEXPARAMS liep;
	HLINEAPP hLineApp = 0;
	HLINE hLine = 0;
	DWORD dwExtVersion;	
	BOOL bRetVal = FALSE;
	LPBYTE pLineGeneralInfoBytes = NULL;
	DWORD dwTAPILineDeviceID;
	const DWORD dwMediaMode = LINEMEDIAMODE_DATAMODEM | LINEMEDIAMODE_INTERACTIVEVOICE;
	LINEGENERALINFO lviGeneralInfo;
	LPLINEGENERALINFO plviGeneralInfo;
	LPTSTR tsManufacturer, tsModel, tsRevision, tsSerialNumber, tsSubscriberNumber;
	TCHAR szUnavailable[36] = {0}; 

	//////////////////////////////////////////////////////////////////////////
	LPlineGetGeneralInfo plineGetGeneralInfo = NULL;
	LPlineShutdown			 pLineShutdown = NULL;
	LPlineClose					 pLineClose = NULL;
	LPlineOpen					 pLineOpen = NULL;
	LPlineNegotiateExtVersion pLineNegotiateExtVersion = NULL;
	LPlineInitializeEx	 pLineInitializeEx = NULL;
	LPlineGetDevCaps	   plineGetDevCaps;
	LPlineNegotiateAPIVersion	plineNegotiateAPIVersion = NULL;
	HMODULE hModuleCoreDll = LoadLibrary(TEXT("CoreDll.dll"));
	LONG nFailedFun = 0;
	if (!hModuleCoreDll)
	{
		goto cleanup;
	}
	pLineShutdown = (LPlineShutdown)GetProcAddress(hModuleCoreDll, TEXT("lineShutdown"));
	if (!pLineShutdown)
	{
		nFailedFun ++;
	}
	pLineClose = (LPlineClose)GetProcAddress(hModuleCoreDll, TEXT("lineClose"));
	if (!pLineClose)
	{
		nFailedFun ++;
	}
	pLineOpen = (LPlineOpen)GetProcAddress(hModuleCoreDll, TEXT("lineOpen"));
	if (!pLineOpen)
	{
		nFailedFun ++;
	}
	pLineNegotiateExtVersion = (LPlineNegotiateExtVersion)GetProcAddress(hModuleCoreDll, TEXT("lineNegotiateExtVersion"));
	if (!pLineNegotiateExtVersion)
	{
		nFailedFun ++;
	}
	pLineInitializeEx = (LPlineInitializeEx)GetProcAddress(hModuleCoreDll, TEXT("lineInitializeEx"));
	if (!pLineInitializeEx)
	{
		nFailedFun ++;
	}
	plineNegotiateAPIVersion = (LPlineNegotiateAPIVersion)GetProcAddress(hModuleCoreDll, TEXT("lineNegotiateAPIVersion"));
	if (!plineNegotiateAPIVersion)
	{
		nFailedFun ++;
	}
	plineGetDevCaps = (LPlineGetDevCaps)GetProcAddress(hModuleCoreDll, TEXT("lineGetDevCaps"));
	if (!plineGetDevCaps)
	{
		nFailedFun ++;
	}
	//CellCore.Dll
	HMODULE hModuleCellCore = LoadLibrary(TEXT("CellCore.dll"));
	if (!hModuleCellCore)
	{
		goto cleanup;
	}
	plineGetGeneralInfo = (LPlineGetGeneralInfo)GetProcAddress(hModuleCellCore, TEXT("lineGetGeneralInfo"));
	if (!plineGetGeneralInfo)
	{
		nFailedFun ++;
	}



	// set the line init params
	liep.dwTotalSize = sizeof(liep);
	liep.dwOptions = LINEINITIALIZEEXOPTION_USEEVENT;

	if (pLineInitializeEx(&hLineApp, 0, 0, DEVICEID_APPLICATION_DATA, &dwNumDevs, &dwAPIVersion, &liep)) 
	{
		goto cleanup;
	}

	// get the device ID
	dwTAPILineDeviceID = 0xffffffff;
	for(DWORD dwCurrentDevID = 0 ; dwCurrentDevID < dwNumDevs ; dwCurrentDevID ++)
	{
		DWORD dwAPIVersion;
		LINEEXTENSIONID LineExtensionID;
		if(0 == plineNegotiateAPIVersion(hLineApp, dwCurrentDevID, TAPI_API_LOW_VERSION, TAPI_API_HIGH_VERSION, &dwAPIVersion, &LineExtensionID)) 
		{
			LINEDEVCAPS LineDevCaps;
			LineDevCaps.dwTotalSize = sizeof(LineDevCaps);
			if(0 == plineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, &LineDevCaps)) 
			{
				BYTE* pLineDevCapsBytes = new BYTE[LineDevCaps.dwNeededSize];
				if(0 != pLineDevCapsBytes) 
				{
					LINEDEVCAPS* pLineDevCaps = (LINEDEVCAPS*)pLineDevCapsBytes;
					pLineDevCaps->dwTotalSize = LineDevCaps.dwNeededSize;
					if(0 == plineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, pLineDevCaps)) 
					{
						if(0 == _tcscmp((TCHAR*)((BYTE*)pLineDevCaps+pLineDevCaps->dwLineNameOffset), CELLTSP_LINENAME_STRING)) 
						{
							dwTAPILineDeviceID = dwCurrentDevID;
							break;
						}
					}
					delete[]  pLineDevCapsBytes;
					pLineDevCapsBytes = NULL;
				}
			}
		}
	}		
	
	// error getting the line device ID?
	if (0xffffffff == dwTAPILineDeviceID) 
	{
		goto cleanup;
	}

	// now try and open the line
	if(pLineOpen(hLineApp, dwTAPILineDeviceID, &hLine, dwAPIVersion, 0, 0, LINECALLPRIVILEGE_OWNER, dwMediaMode, 0)) 
	{
		goto cleanup;
	}

	// set up ExTAPI
	if (pLineNegotiateExtVersion(hLineApp, dwTAPILineDeviceID, dwAPIVersion, EXT_API_LOW_VERSION, EXT_API_HIGH_VERSION, &dwExtVersion)) 
	{
		goto cleanup;
	}

	// try to get the general info
	lviGeneralInfo.dwTotalSize = sizeof(lviGeneralInfo);

	// step 1: find out how much space we need
	if (plineGetGeneralInfo(hLine, &lviGeneralInfo)) 
	{
		goto cleanup;
	}

	// step 2: malloc space for all the info we need
	pLineGeneralInfoBytes = new BYTE[lviGeneralInfo.dwNeededSize];
	plviGeneralInfo = (LPLINEGENERALINFO)pLineGeneralInfoBytes;

	// step 3: call lGGI again with the appropriately sized buffer
	if(NULL != pLineGeneralInfoBytes) 
	{
		plviGeneralInfo->dwTotalSize = lviGeneralInfo.dwNeededSize;
		if (plineGetGeneralInfo(hLine, plviGeneralInfo)) 
		{
			goto cleanup;
		}
	} 
	else 
	{
		goto cleanup;
	}

	// step 4: cast all the arguments to strings
	if(0 < plviGeneralInfo->dwManufacturerSize)
	{ 
		tsManufacturer = (WCHAR*)(((BYTE*)plviGeneralInfo)+plviGeneralInfo->dwManufacturerOffset);
	}
	else
	{
		tsManufacturer = szUnavailable;
	}

	if(0 < plviGeneralInfo->dwModelSize)
	{ 
		tsModel = (WCHAR*)(((BYTE*)plviGeneralInfo)+plviGeneralInfo->dwModelOffset);
	}
	else
	{
		tsModel = szUnavailable;
	}

	if(0 < plviGeneralInfo->dwRevisionSize)
	{
		tsRevision = (WCHAR*)(((BYTE*)plviGeneralInfo)+plviGeneralInfo->dwRevisionOffset);
	}
	else
	{
		tsRevision = szUnavailable;
	}

	if(0 < plviGeneralInfo->dwSerialNumberSize)
	{
		tsSerialNumber = (WCHAR*)(((BYTE*)plviGeneralInfo)+plviGeneralInfo->dwSerialNumberOffset);
	}
	else
	{
		tsSerialNumber = szUnavailable;
	}

	if(0 < plviGeneralInfo->dwSubscriberNumberSize)
	{
		tsSubscriberNumber = (WCHAR*)(((BYTE*)plviGeneralInfo)+plviGeneralInfo->dwSubscriberNumberOffset);
	}
	else
	{
		tsSubscriberNumber = szUnavailable;
	}
	// create the message box string.
	// 
	StringCchCopy(szIMEI, _countof(szIMEI), tsSerialNumber);

	bRetVal = TRUE;

cleanup:
	if (pLineGeneralInfoBytes) 
	{
		delete [] pLineGeneralInfoBytes;
		pLineGeneralInfoBytes = NULL;
	}
	if (hLine)
	{
		pLineClose(hLine);
		hLine = NULL;
	}
	if (hLineApp) 
	{
		pLineShutdown(hLineApp);
		hLineApp = NULL;
	}
	plineGetGeneralInfo = NULL;
	if (hModuleCellCore)
	{
		FreeLibrary(hModuleCellCore);
		hModuleCellCore = NULL;
	}
	pLineShutdown = NULL;
	pLineClose = NULL;
	pLineOpen = NULL;
	pLineNegotiateExtVersion = NULL;
	pLineInitializeEx = NULL;
	plineGetDevCaps;
	plineNegotiateAPIVersion = NULL;
	if (hModuleCoreDll)
	{
		FreeLibrary(hModuleCoreDll);
		hModuleCoreDll = NULL;
	}
	wcsupr(szIMEI);
	if (!pszIMEI && !nCount)
	{
		return _tcslen(szIMEI);
	}
	if (pszIMEI && nCount)
	{
		StringCchCopy(pszIMEI, nCount, szIMEI);
		return _tcslen(pszIMEI);
	}
	return -1;
}

//get device id 
LONG  WINAPI XUE_GetDevicePlatFormID(BYTE* pDeviceID, LONG cbSizeDeviceID, BYTE* pPlatFormID, LONG cbSizePlatFormID)
{
	// assume error 
	BOOL returnVal = FALSE;

    // save the device id
	DEVICE_ID * devId = NULL;

	DWORD devIdSize = 0;

	INT iLength = 0;
	/* 
	* The correct size of the devId, which is determined early in this
	* function 
	*/
	DWORD dwCorrectDevIdSizeBytes;

	/* 
	* we need a junk DEVICE_ID to set the dwSize to null to query for the
	* size 
	*/
	DEVICE_ID queryId;

	/* set the size part to zero for the query function */
	queryId.dwSize = 0;

	BOOL bRet;

	/* figure out how large the devId needs to be for a standard, correct call */
	bRet = KernelIoControl (IOCTL_HAL_GET_DEVICEID, NULL, 0, 	&queryId, sizeof (queryId), &dwCorrectDevIdSizeBytes);

	DWORD getLastErrorVal = GetLastError ();

	/* we want to get an ERROR_INSUFFICIENT_BUFFER and FALSE */
	if (!bRet && getLastErrorVal == ERROR_INSUFFICIENT_BUFFER)
	{
		/* what we want */
	}
	else
	{
		// something went wrong
		goto Exit;
	}

	dwCorrectDevIdSizeBytes = queryId.dwSize;

	/* allocate memory for the devId */
	devId = (DEVICE_ID *) _alloca(dwCorrectDevIdSizeBytes);

	if (!devId)
	{
		//Couldn't allocate %u bytes for the devId.
		goto Exit;
	}

	/* want to check that the call sets the right size on output */
	DWORD checkSize;

	/* try to get the DEVICE_ID */
	bRet = KernelIoControl (IOCTL_HAL_GET_DEVICEID, NULL, 0,  devId, dwCorrectDevIdSizeBytes, &checkSize);

	if (!bRet)
	{
		/* something went wrong... */
		goto Exit;
	}

	if (checkSize != dwCorrectDevIdSizeBytes)
	{
		goto Exit;
	}

	/* check the size */
	if (devId->dwSize != dwCorrectDevIdSizeBytes)
	{
		goto Exit;
	}

	/* print out the preset id if set */
	if (devId->dwPresetIDOffset != 0)
	{
		if (NULL == pPlatFormID)
		{
			goto next;
		}
		/* code below relies on this assertion */
		if (sizeof (BYTE) != 1)
		{
			goto Exit;
		}

		/* 
		* values are not guaranteed to be null terminated.  Get a buffer
		* one more so that we force them to be.
		*/
		/*
		BYTE * pbPreset = (BYTE *) malloc(devId->dwPresetIDBytes * sizeof (BYTE));

		if (pbPreset == NULL)
		{
    		//Couldn't allocate memory for szPreset
			goto Exit;
		}
		*/

		/* zero buffer first */
		memset (pPlatFormID, 0, cbSizePlatFormID * sizeof (BYTE));
		/* copy from the buffer into our larger buffer */
		memcpy (pPlatFormID, ((BYTE *) devId) + devId->dwPresetIDOffset, devId->dwPresetIDBytes);

//		free (pbPreset);
	}
next:
	/* print out the platform id if set */
	if (devId->dwPlatformIDOffset != 0)
	{
		/* code below relies on this assertion */
		if (sizeof (BYTE) != 1)
		{
			goto Exit;
		}

		/* 
		* values are not guaranteed to be null terminated.  Get a buffer
		* one more so that we force them to be.
		*/
		/* copy from the buffer into our larger buffer */
		memcpy (pDeviceID, ((BYTE *) devId) + devId->dwPlatformIDOffset, devId->dwPlatformIDBytes);

		iLength = devId->dwPlatformIDBytes;

		//Device platform id

	//	free (pbPlatform);
	}

	/* only if we got here did we succeed */
	returnVal = TRUE;

Exit:

	return iLength;
}

LONG WINAPI XUE_GetUserName(WCHAR* pszUserName, LONG nMax)
{
	TCHAR cKeyOwner[] = TEXT("ControlPanel\\Owner");
	HKEY hKey;
	DWORD dwSize;
	BOOL ReturnCode = FALSE;
	WCHAR szUserName[100] = {0};
	if (RegOpenKeyEx(HKEY_CURRENT_USER, cKeyOwner, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwKeyDataType = REG_DWORD;
		dwSize = sizeof(szUserName);
		RegQueryValueEx(hKey,	TEXT("Name"),	NULL,	&dwKeyDataType,	(LPBYTE)&szUserName, 	&dwSize);
	}
	if (hKey)	
	{
		RegCloseKey(hKey);
	}
	wcsupr(szUserName);
	if (!pszUserName && !nMax)
	{
		return wcslen(szUserName); 
	}	

	StringCchCopy(pszUserName, nMax, szUserName);
	return wcslen(pszUserName);		
}

LONG WINAPI XUE_GetDeviceID(LPWSTR pszDeviceID, LONG nCount)
{
	WCHAR szDeviceID[256] = {0};
	BYTE            abDeviceID[GETDEVICEUNIQUEID_V1_OUTPUT * 12] = {0};
	DWORD           cbDeviceId      = sizeof(abDeviceID);
	ZeroMemory(abDeviceID, sizeof(abDeviceID));
	LPGetDeviceUniqueID pGetDeviceUniqueID = NULL;
	HMODULE hModuleCoreDll = LoadLibrary(TEXT("CoreDll.dll"));
	LONG nFailedFun = 0;
	if (!hModuleCoreDll)
	{
		goto cleanup;
	}
	pGetDeviceUniqueID = (LPGetDeviceUniqueID)GetProcAddress(hModuleCoreDll, TEXT("GetDeviceUniqueID"));
	if (!pGetDeviceUniqueID)
	{
		nFailedFun ++;
	}
	if (nFailedFun)
	{
		//Old Method
		cbDeviceId = XUE_GetDevicePlatFormID((LPBYTE)abDeviceID, GETDEVICEUNIQUEID_V1_OUTPUT * 12, NULL, 0);
	}
	else
	{		
		pGetDeviceUniqueID(reinterpret_cast<PBYTE>(DEVICEID_APPLICATION_DATA),	wcslen(DEVICEID_APPLICATION_DATA) * sizeof(WCHAR),	GETDEVICEUNIQUEID_V1,	abDeviceID,	&cbDeviceId);
	}
	ZeroMemory(szDeviceID, sizeof(szDeviceID));
	for (LONG i = 0 ; i < cbDeviceId ; i ++)
	{
		TCHAR szCode[5] = {0};
		StringCchPrintf(szCode, _countof(szCode), TEXT("%02X"), (BYTE)abDeviceID[i]);
		StringCchCat(szDeviceID, _countof(szDeviceID), szCode);
	}

cleanup:
	pGetDeviceUniqueID = NULL;
	if (hModuleCoreDll) 
	{
		FreeLibrary(hModuleCoreDll);
		hModuleCoreDll = NULL;
	}	
	if (!pszDeviceID && !nCount)
	{
		return _tcslen(szDeviceID);
	}
	if (pszDeviceID && nCount)
	{
		StringCchCopy(pszDeviceID, nCount, szDeviceID);
		return wcslen(pszDeviceID);
	}
	return -1;
}


#ifdef UNDER_CE
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
static DWORD GetID(LPCWSTR pszDevice)
{
	DWORD dwID = ULONG_MAX;
	WCHAR szDevice[128] = {0};
	if (!pszDevice)
	{
		return dwID;
	}
	StringCchCopy(szDevice, _countof(szDevice), pszDevice);
	WCHAR		szElement[32] = {0};
	HANDLE   hCard = CreateFile(szDevice, GENERIC_READ|GENERIC_WRITE,	0, NULL, OPEN_EXISTING, 0,  NULL); 
	if(hCard) 
	{ 			  
		BYTE	abBuffer[3000] = {0};
		ZeroMemory(abBuffer, sizeof(abBuffer));
		PSTORAGE_IDENTIFICATION pStoreInfo = (STORAGE_IDENTIFICATION *)abBuffer;    

		DWORD   dwGetBytes; 
		if (DeviceIoControl(hCard, 	CTL_CODE(FILE_DEVICE_DISK,   0x0709, METHOD_BUFFERED,  FILE_ANY_ACCESS), NULL, 0, (LPVOID)pStoreInfo, 3000, &dwGetBytes, NULL))    
		{    
			if(dwGetBytes > 0) 
			{ 			
				if(pStoreInfo->dwSerialNumOffset  <  dwGetBytes) 
				{ 
					BYTE* SerialNo = (((BYTE*)pStoreInfo) + pStoreInfo->dwSerialNumOffset);  				   
					dwID = strtoul( (LPCSTR)SerialNo, NULL, 16);					
					if( !dwID || ULONG_MAX == dwID )
					{
						dwID = strtoul( (LPCSTR)SerialNo, NULL, 10);
					}	
					if ( !dwID)
					{
						dwID = ULONG_MAX;
					}
				}							
			} 
		} 			
		CloseHandle(hCard); 		
		hCard = NULL;
	}
	return dwID;
}
#endif

LONG WINAPI XUE_GetSDID(LPWSTR pszID, LONG nLen)
{
	WCHAR		szID[512] = {0};
	HKEY		hKeyActive = NULL;		
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
	WCHAR szItemID[128] = {0};
	DWORD dwValueSize = 0;
	DWORD dwValueType = 0;
	WCHAR szKName[512] = {0};   
	DWORD cbKName = sizeof(szKName); 
	WCHAR szDeviceName[512] = {0};
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
						DWORD dwID = GetID(szDeviceName);
						if ((DWORD)(-1) != dwID)
						{
							StringCchPrintf(szItemID, _countof(szItemID), TEXT("%08X"), dwID);
						}						
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NVIDIA_GUID TEXT("{1A3E09BE-1E45-494B-9174-D7385B45BBF5}")

#ifndef OID_802_3_PERMANENT_ADDRESS
#define OID_802_3_PERMANENT_ADDRESS             0x01010101
#endif

#ifndef IOCTL_NDIS_QUERY_GLOBAL_STATS
#define IOCTL_NDIS_QUERY_GLOBAL_STATS						0x00170002
#endif

typedef DWORD (WINAPI *LPGetAdaptersInfo)(PIP_ADAPTER_INFO,PULONG);

static LONG WINAPI GetNetAdapterPhysicalMAC(WCHAR* strDeviceName, WCHAR* pszMac, LONG nMax)
{
	WCHAR   szDriver[256] = {0};  
	BYTE		ucData[8];
	DWORD		ByteRet;	
	WCHAR  szPhysicalMAC[32] = {0};
	HANDLE hDriver = INVALID_HANDLE_VALUE;
	StringCchCopyW(szDriver, _countof(szDriver), L"\\\\.\\");
	StringCchCatW(szDriver, _countof(szDriver), strDeviceName);

	hDriver = CreateFileW(szDriver, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);  
	if(INVALID_HANDLE_VALUE == hDriver)  
	{  
		return -1;  
	}  
	ULONG dwID = OID_802_3_PERMANENT_ADDRESS;	
	ZeroMemory(ucData, sizeof(ucData));
	if(!DeviceIoControl(hDriver, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &ByteRet, NULL))
	{
		return -2;
	}
	else
	{		
		for (DWORD dwIdx = 0; dwIdx < 6; dwIdx ++)
		{
			WCHAR szCode[8] = {0};
			StringCchPrintfW(szCode, _countof(szCode), L"%02X", ucData[dwIdx]);
			StringCchCatW(szPhysicalMAC, _countof(szPhysicalMAC), szCode);
		}		
	}
	CloseHandle(hDriver);
	hDriver = NULL;

	if (!pszMac && !nMax)
	{
		return wcslen(szPhysicalMAC);
	}
	else if (pszMac && nMax)
	{
		StringCchCopyW(pszMac, nMax, szPhysicalMAC);	
		return wcslen(pszMac);
	}
	return 0;
}
LONG WINAPI XUE_GetMAC( LPWSTR pszMac, LONG nMax)
{
	BOOL blFind = FALSE;
	TCHAR szMac[256 + 10] = {0};
	HMODULE	hModuleIphlpapi = NULL;
	LPGetAdaptersInfo pGetAdapterInfo = NULL;

	ULONG nBufferLength = 0;
	BYTE* pBuffer = 0;
	hModuleIphlpapi = LoadLibrary(TEXT("Iphlpapi.dll"));
	if (!hModuleIphlpapi)
	{
		goto	__exit;
	}
#ifdef UNDER_CE
	pGetAdapterInfo = (LPGetAdaptersInfo)GetProcAddress(hModuleIphlpapi, L"GetAdaptersInfo");
#else
	pGetAdapterInfo = (LPGetAdaptersInfo)GetProcAddress(hModuleIphlpapi, "GetAdaptersInfo");
#endif
	if (!pGetAdapterInfo)
	{
		goto	__exit;
	}
	if( ERROR_BUFFER_OVERFLOW == pGetAdapterInfo( 0, &nBufferLength ))
	{		
		pBuffer = new BYTE[ nBufferLength ];
		ZeroMemory(pBuffer, sizeof(*pBuffer) * nBufferLength);
	}
	else
	{

	}

	// Get the Adapter Information.
	PIP_ADAPTER_INFO pAdapterInfo =	reinterpret_cast<PIP_ADAPTER_INFO>(pBuffer);
	if (ERROR_SUCCESS == pGetAdapterInfo( pAdapterInfo, &nBufferLength ))
	{
		while( pAdapterInfo && pAdapterInfo->Description[0])
		{		
			TCHAR szPhysicalMAC[20] = {0};
			TCHAR szLogicMAC[20] = {0};
			WCHAR szAdaptrName[256] = {0};
			StringCchPrintf(szLogicMAC, _countof(szLogicMAC), _T("%02X%02X%02X%02X%02X%02X"),	pAdapterInfo->Address[0],	pAdapterInfo->Address[1],	pAdapterInfo->Address[2],	pAdapterInfo->Address[3],	pAdapterInfo->Address[4],	pAdapterInfo->Address[5]);

			MultiByteToWideChar(CP_ACP, 0, pAdapterInfo->AdapterName, strlen(pAdapterInfo->AdapterName),  szAdaptrName, _countof(szAdaptrName));
			DWORD dwError = GetNetAdapterPhysicalMAC(szAdaptrName, szPhysicalMAC, _countof(szPhysicalMAC)); 
			if (dwError)
			{
				//HWTRACE(TEXT("GetPhysicalMAC ERROR %08X %d\n"), dwError, GetLastError());
			}
			if (!szPhysicalMAC[0])
			{
				StringCchCopy(szPhysicalMAC, _countof(szPhysicalMAC), szLogicMAC);
			}
			if (_tcscmp(szPhysicalMAC, TEXT("000000000000")))
			{
				StringCchCat(szMac, _countof(szMac), szPhysicalMAC);
				StringCchCat(szMac, _countof(szMac), TEXT(" "));
			}		

			pAdapterInfo = pAdapterInfo->Next;
		}
	}	

	if (pBuffer)
	{
		delete[] pBuffer;
		pBuffer	= NULL;
	}	
__exit:
	pGetAdapterInfo = NULL;
	FreeLibrary(hModuleIphlpapi);
	hModuleIphlpapi = NULL;

	wcsupr(szMac);
	if (!pszMac && !nMax)
	{
		return _tcslen(szMac);
	}
	else if (pszMac && nMax)
	{
		StringCchCopy(pszMac, nMax, szMac);	
		return _tcslen(pszMac);
	}
	return -1;	
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DWORD WINAPI XUE_GetLockState()
{
	DWORD dwLockState = 0;	
#ifdef UNDER_CE
	HKEY hSubKey = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, TEXT("System\\State"), 0L, KEY_READ, &hSubKey))
	{
		DWORD dwValueSize = 0, dwValueType = 0;	
		if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("Lock"), NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
		{		
			dwValueSize = sizeof(dwLockState);
			if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, TEXT("Lock"), NULL,	&dwValueType, (LPBYTE)&dwLockState, &dwValueSize))
			{
				dwLockState = 0;
			}					
		}
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}		
#endif
	return dwLockState;
}

BOOL WINAPI XUE_IsKeyboardLocked()
{
	BOOL bRes = FALSE;
#ifdef UNDER_CE
	DWORD dwLockState = XUE_GetLockState();	
	if (dwLockState & SN_LOCK_BITMASK_KEYLOCKED)
	{
		bRes = TRUE;
	}
#endif
	return bRes;
}



DWORD	WINAPI XUE_GetWiFiState()
{
	DWORD dwWiFiState = 0;	
#ifdef UNDER_CE
	HKEY hSubKey = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, TEXT("System\\State\\Hardware"), 0L, KEY_READ, &hSubKey))
	{
		DWORD dwValueSize = 0, dwValueType = 0;	
		if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("WiFi"), NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
		{		
			dwValueSize = sizeof(dwWiFiState);
			if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, TEXT("WiFi"), NULL,	&dwValueType, (LPBYTE)&dwWiFiState, &dwValueSize))
			{
				dwWiFiState = 0;
			}					
		}
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}		
#endif
	return dwWiFiState;
}

DWORD WINAPI XUE_GetBluetoothState()
{
	DWORD dwBluetoothState = 0;	
#ifdef UNDER_CE
	HKEY hSubKey = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, TEXT("System\\State\\Hardware"), 0L, KEY_READ, &hSubKey))
	{
		DWORD dwValueSize = 0, dwValueType = 0;	
		if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("Bluetooth"), NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
		{		
			dwValueSize = sizeof(dwBluetoothState);
			if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, TEXT("Bluetooth"), NULL,	&dwValueType, (LPBYTE)&dwBluetoothState, &dwValueSize))
			{
				dwBluetoothState = 0;
			}					
		}
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}		
#endif
	return dwBluetoothState;
}

DWORD	WINAPI XUE_GetPhoneState()
{
	DWORD dwPhoneState = 0;	
#ifdef UNDER_CE
	HKEY hSubKey = NULL;
	if (ERROR_SUCCESS == RegOpenKeyEx (HKEY_LOCAL_MACHINE, TEXT("System\\State\\Phone"), 0L, KEY_READ, &hSubKey))
	{
		DWORD dwValueSize = 0, dwValueType = 0;	
		if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, TEXT("Status"), NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
		{		
			dwValueSize = sizeof(dwPhoneState);
			if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, TEXT("Status"), NULL,	&dwValueType, (LPBYTE)&dwPhoneState, &dwValueSize))
			{
				dwPhoneState = 0;
			}					
		}
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}		
#endif
	return dwPhoneState;
}

BOOL WINAPI XUE_IsFlightModeOn()
{
	DWORD dwPhoneState = XUE_GetPhoneState();	
	return (dwPhoneState & SN_PHONERADIOOFF_BITMASK);	
}



