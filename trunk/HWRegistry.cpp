#include "stdafx.h"
#include "HWRegistry.h"
#include <tchar.h>
#include <assert.h>


DWORD WINAPI HWXUE_RegQueryValueDWORD( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData )
{
	DWORD dwValue = dwData;
	if (!hKey)
	{
		return dwValue;
	}
	DWORD dwValueSize = 0, dwValueType = 0;
	if (pszSubKey && _tcslen(pszSubKey))
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

BOOL WINAPI HWXUE_RegSetValueDWORD( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData )
{	
	BOOL blRet = FALSE;
	if (!hKey)
	{
		return blRet;
	}	
	if (pszSubKey && _tcslen(pszSubKey))
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

BOOL		WINAPI HWXUE_RegSetValueString( HKEY hKey , LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszData )
{
	BOOL blRet = FALSE;
	if (!hKey || !pszData)
	{
		return blRet;
	}	
	if (pszSubKey && _tcslen(pszSubKey))
	{			
		HKEY hSubKey = NULL;
		DWORD dwTmp = 0;
		if (ERROR_SUCCESS == RegCreateKeyEx(hKey, pszSubKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,	NULL, &hSubKey, &dwTmp))
		{
			DWORD dwSize = (_tcslen(pszData) + 1) * sizeof(*pszData);
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
		DWORD dwSize = (_tcslen(pszData) + 1) * sizeof(*pszData);
		if(ERROR_SUCCESS == ::RegSetValueEx (hKey, pszValue, 0L, REG_SZ, (LPBYTE) pszData, dwSize))		
		{
			blRet = TRUE;
		}
	}	
	return blRet;
}

DWORD		WINAPI HWXUE_RegQueryValueString( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszDefData, LPTSTR pszData, DWORD dwCount )
{	
	//内部最多支持512个字符 否则不能读取真实的值
	TCHAR szData[512] = {0};		
	if (!hKey || !pszData || dwCount <= 0)
	{
		return 0;
	}
	if (pszDefData)
	{
		StringCchCopy(szData, _countof(szData), pszDefData);
	}
	
	DWORD dwValueSize = 0, dwValueType = 0;
	if (pszSubKey && _tcslen(pszSubKey))
	{				
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx (hKey, pszSubKey, 0L, KEY_READ, &hSubKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_SZ == dwValueType)
			{		
				if (dwValueSize <= sizeof(szData))
				{					
					dwValueSize = sizeof(szData);
					RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, (LPBYTE)szData, &dwValueSize);					
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
			if (dwValueSize <= sizeof(szData))
			{					
				dwValueSize = sizeof(szData);
				RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, (LPBYTE)szData, &dwValueSize);					
			}						
		}
	}	
	StringCchCopy(pszData, dwCount, szData);
	return _tcslen(pszData);
}

BOOL		WINAPI HWXUE_RegSetValueBinary( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, const BYTE* pbData, DWORD dwByte )
{
	BOOL blRet = FALSE;
	if (!hKey || !pbData || dwByte <= 0)
	{
		return blRet;
	}	
	if (pszSubKey && _tcslen(pszSubKey))
	{			
		HKEY hSubKey = NULL;
		DWORD dwTmp = 0;
		if (ERROR_SUCCESS == RegCreateKeyEx(hKey, pszSubKey, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,	NULL, &hSubKey, &dwTmp))
		{			
			if(ERROR_SUCCESS == ::RegSetValueEx (hSubKey, pszValue, 0L, REG_BINARY, (LPBYTE)pbData, dwByte))
			{
				blRet = TRUE;
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{		
		if(ERROR_SUCCESS == ::RegSetValueEx (hKey, pszValue, 0L, REG_BINARY, (LPBYTE) pbData, dwByte))		
		{
			blRet = TRUE;
		}
	}	
	return blRet;
}

DWORD		WINAPI HWXUE_RegQueryValueBinary( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, const BYTE* pbDefData, DWORD dwDefSize, BYTE* pbData, DWORD dwCount )
{
	//内部最多支持1024Byte 否则不能读取真实的值
	BYTE abData[1024] = {0};	
	ZeroMemory(abData, sizeof(abData));
	DWORD dwRet = 0;
	if (!hKey || !pbData || dwCount <= 0)
	{
		return dwRet;
	}
	if (pbDefData && dwDefSize > 0)
	{
		dwRet = _min(dwDefSize, sizeof(abData));
		memcpy(abData, pbDefData, dwRet);
	}

	DWORD dwValueSize = 0, dwValueType = 0;
	if (pszSubKey && _tcslen(pszSubKey))
	{				
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx (hKey, pszSubKey, 0L, KEY_READ, &hSubKey))
		{
			if(ERROR_SUCCESS == RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_BINARY == dwValueType)
			{		
				if (dwValueSize <= sizeof(abData))
				{					
					dwValueSize = sizeof(abData);
					RegQueryValueEx (hSubKey, pszValue, NULL,	&dwValueType, (LPBYTE)abData, &dwValueSize);		
					dwRet = dwValueSize;
				}						
			}
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{
		if(ERROR_SUCCESS == RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, NULL, &dwValueSize) && REG_BINARY == dwValueType)
		{		
			if (dwValueSize <= sizeof(abData))
			{					
				dwValueSize = sizeof(abData);
				RegQueryValueEx (hKey, pszValue, NULL,	&dwValueType, (LPBYTE)abData, &dwValueSize);		
				dwRet = dwValueSize;
			}						
		}
	}	
	dwRet = _min(dwRet, dwCount);
	memcpy(pbData, abData, dwRet);
	return dwRet;
}

BOOL		WINAPI HWXUE_RegDeleteValue( HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue )
{	
	BOOL blRet = TRUE;
	if (pszSubKey && _tcslen(pszSubKey))
	{				
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx (hKey, pszSubKey, 0L, KEY_ALL_ACCESS, &hSubKey))
		{
			if (ERROR_SUCCESS != RegDeleteValue(hSubKey, pszValue))
			{
				blRet = FALSE;				
			}				
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}			
	}
	else
	{
		if (ERROR_SUCCESS != RegDeleteValue(hKey, pszValue))
		{
			blRet = FALSE;			
		}	
	}	
	return blRet;
}