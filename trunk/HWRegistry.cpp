#include "stdafx.h"
#include "HWRegistry.h"
#include <tchar.h>
#include <assert.h>


DWORD WINAPI HWRegQueryValueDWORD( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData )
{
	DWORD dwValue = dwData;
	if (!hKey)
	{
		return dwValue;
	}
	DWORD dwValueSize = 0, dwValueType = 0;
	if (pszSubKey)
	{				
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx (hKey, pszSubKey, 0L, KEY_READ, &hSubKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
			{		
				dwValueSize = sizeof(dwValue);
				if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, (LPBYTE)&dwValue, &dwValueSize))
				{
					dwValue = dwData;
				}					
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{
		if(ERROR_SUCCESS == RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_DWORD == dwValueType)
		{		
			dwValueSize = sizeof(dwValue);
			if (ERROR_SUCCESS != RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, (LPBYTE)&dwValue, &dwValueSize))
			{
				dwValue = dwData;
			}					
		}
	}	
	return dwValue;
}

BOOL WINAPI HWRegSetValueDWORD( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData )
{	
	BOOL blRet = FALSE;
	if (!hKey)
	{
		return blRet;
	}	
	if (pszSubKey)
	{			
		HKEY hSubKey = NULL;
		DWORD dwTmp = 0;
		if (ERROR_SUCCESS == RegCreateKeyEx(hKey, pszSubKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 	NULL, &hSubKey, &dwTmp))
		{			
			if(ERROR_SUCCESS == ::RegSetValueEx (hSubKey, pszValue, 0L, REG_DWORD, (LPBYTE) &dwData, sizeof(dwData)))
			{
				blRet = TRUE;
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{		
		if(ERROR_SUCCESS == ::RegSetValueEx (hKey, pszValue, 0L, REG_DWORD, (LPBYTE) &dwData, sizeof(dwData)))		
		{
			blRet = TRUE;
		}
	}	
	return blRet;
}

BOOL		WINAPI HWRegSetValueString( HKEY hKey , LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszData )
{
	BOOL blRet = FALSE;
	if (!hKey || !pszData)
	{
		return blRet;
	}	
	if (pszSubKey)
	{			
		HKEY hSubKey = NULL;
		DWORD dwTmp = 0;
		if (ERROR_SUCCESS == RegCreateKeyEx(hKey, pszSubKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 	NULL, &hSubKey, &dwTmp))
		{
			DWORD dwSize = sizeof(_tcslen(pszData) + 1) * sizeof(TCHAR);
			if(ERROR_SUCCESS == ::RegSetValueEx (hSubKey, pszValue, 0L, REG_SZ, (LPBYTE)pszData, dwSize))
			{
				blRet = TRUE;
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{
		DWORD dwSize = sizeof(_tcslen(pszData) + 1) * sizeof(TCHAR);
		if(ERROR_SUCCESS == ::RegSetValueEx (hKey, pszValue, 0L, REG_DWORD, (LPBYTE) pszData, dwSize))		
		{
			blRet = TRUE;
		}
	}	
	return blRet;
}

DWORD		WINAPI HWRegQueryValueString( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszDefData, LPTSTR pszData, LONG nSize )
{	
	TCHAR szData[512] = {0};
	LPTSTR pszQueryData = NULL;
	if (!hKey || !pszData || nSize <= 0)
	{
		return 0;
	}
	DWORD dwValueSize = 0, dwValueType = 0;
	if (pszSubKey)
	{				
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx (hKey, pszSubKey, 0L, KEY_READ, &hSubKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_SZ == dwValueType)
			{		
				if (dwValueSize > sizeof(szData))
				{
					pszQueryData = new TCHAR[dwValueSize / 2 + 1];
					if (pszQueryData)
					{
						if (ERROR_SUCCESS != RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, (LPBYTE)&dwValue, &dwValueSize))
						{
							dwValue = dwData;
						}			
					}
					else
					{

					}
				}
						
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{
		if(ERROR_SUCCESS == RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_SZ == dwValueType)
		{		
			dwValueSize = sizeof(dwValue);
			if (ERROR_SUCCESS != RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, (LPBYTE)&dwValue, &dwValueSize))
			{
				dwValue = dwData;
			}					
		}
	}	
	return dwValue;
}