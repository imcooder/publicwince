/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	HWStrSafe.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/21
*********************************************************************/

#include "stdafx.h"
#include "HWStrSafe.h"
#include <String.h>
#include <wchar.h>

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

CHAR*		WINAPI StringTokA( CHAR* pszToken, const CHAR* pszDelimit, CHAR ** ppszContext)
{
	CHAR *token = NULL;
	const CHAR *pszCtrl = NULL;

	/* validation section */
	if (!ppszContext)
	{
		return NULL;
	}   
	if (!pszDelimit)
	{
		return NULL;
	}
	if (!(pszToken != NULL || *ppszContext != NULL))
	{
		return NULL;
	}    

	/* If pszString==NULL, continue with previous pszString */
	if (!pszToken)
	{
		pszToken = *ppszContext;
	}

	/* Find beginning of token (skip over leading delimiters). Note that
	* there is no token iff this loop sets pszString to point to the terminal null. */
	for ( ; *pszToken != 0 ; pszToken ++)
	{
		for (pszCtrl = pszDelimit; *pszCtrl != 0 && *pszCtrl != *pszToken; pszCtrl ++)
		{
			;
		}
		if (*pszCtrl == 0)
		{
			break;
		}
	}

	token = pszToken;

	/* Find the end of the token. If it is not the end of the pszString,
	* put a null there. */
	for ( ; *pszToken != 0 ; pszToken++)
	{
		for (pszCtrl = pszDelimit; *pszCtrl != 0 && *pszCtrl != *pszToken; pszCtrl++)
		{
			;
		}
		if (*pszCtrl != 0)
		{
			*pszToken ++ = 0;
			break;
		}
	}

	/* Update the context */
	*ppszContext = pszToken;

	/* Determine if a token has been found. */
	if (token == pszToken)
	{
		return NULL;
	}
	else
	{
		return token;
	}
}

WCHAR*	WINAPI StringTokW( WCHAR* pszToken, const WCHAR* pszDelimit, WCHAR ** ppszContext)
{
	WCHAR *token = NULL;
	const WCHAR *pszCtrl;

	/* validation section */
	if (!ppszContext)
	{
		return NULL;
	}   
	if (!pszDelimit)
	{
		return NULL;
	}
	if (!(pszToken != NULL || *ppszContext != NULL))
	{
		return NULL;
	}    

	/* If pszString==NULL, continue with previous pszString */
	if (!pszToken)
	{
		pszToken = *ppszContext;
	}

	/* Find beginning of token (skip over leading delimiters). Note that
	* there is no token iff this loop sets pszString to point to the terminal null. */
	for ( ; *pszToken != 0 ; pszToken ++)
	{
		for (pszCtrl = pszDelimit; *pszCtrl != 0 && *pszCtrl != *pszToken; pszCtrl ++)
		{
			;
		}
		if (*pszCtrl == 0)
		{
			break;
		}
	}

	token = pszToken;

	/* Find the end of the token. If it is not the end of the pszString,
	* put a null there. */
	for ( ; *pszToken != 0 ; pszToken++)
	{
		for (pszCtrl = pszDelimit; *pszCtrl != 0 && *pszCtrl != *pszToken; pszCtrl++)
		{
			;
		}
		if (*pszCtrl != 0)
		{
			*pszToken ++ = 0;
			break;
		}
	}

	/* Update the context */
	*ppszContext = pszToken;

	/* Determine if a token has been found. */
	if (token == pszToken)
	{
		return NULL;
	}
	else
	{
		return token;
	}
}



LONG WINAPI StringMidA(LPSTR pszString, LONG nIndex, UINT nCount)
{	 
	LONG nRet = -1;
	if (!pszString)
	{
		return nRet;
	}	
	LONG nLength = (LONG)strlen(pszString);
	assert(nIndex >= 0 && nIndex + (LONG)nCount <= nLength);	
	if (0 != nIndex && nCount > 0)
	{
		memmove(pszString, pszString + nIndex, nCount * sizeof(*pszString));
		pszString[nCount] = 0;  
#if defined(_DEBUG) || defined(DEBUG)
		{
			if (nIndex + nCount < (LONG)nLength)
			{
				ZeroMemory(pszString + nCount, (nLength - nCount) * sizeof(*pszString));
			}
		}  
#endif
		nRet = (LONG)strlen(pszString);
	}  
	return nRet;
}
LONG WINAPI StringRightA(LPSTR pszString, UINT nCount)
{
	if (!pszString)
	{
		return -1;
	}
	return StringMidA(pszString, (LONG)strlen(pszString) - nCount, nCount);
}
LONG WINAPI StringLeftA(LPSTR pszString, UINT nCount)
{
	if (!pszString)
	{
		return -1;
	}
	LONG nLength = (LONG)strlen(pszString);
	if (nLength >= (LONG)nCount)
	{
		pszString[nCount] = 0;
		return (LONG)strlen(pszString);
	}
	return -1;
}
void WINAPI StringTrimLeftA(LPSTR pszString, LPCSTR pszTag)
{	
	if (!pszTag || !pszString)
	{
		return;
	}   
	if (!pszString[0])
	{
		return;
	}	
	LONG nLength = (LONG)strlen(pszString);
	LONG nAmount = (LONG)strspn(pszString, pszTag);
	if (nAmount < nLength)
	{
		StringRightA(pszString, nLength - nAmount);
	}	
}

void WINAPI StringTrimRightA(LPSTR pszString, LPCSTR pszTag)
{
	if (!pszTag || !pszString)
	{
		return;
	}   
	LONG nAmount = 0;
	BOOL blContinue = TRUE;	
	LONG nLength = (LONG)strlen(pszString);
	for (LONG i = 0; i < nLength && blContinue; i++)
	{    
		if (NULL != strchr(pszTag, pszString[nLength - i - 1]))
		{
			nAmount ++;
		}
		else
		{
			blContinue = FALSE;
		}
	}  
	if (!blContinue || nAmount > 0)
	{
		StringLeftA(pszString, nLength - nAmount);
	}
}
void WINAPI StringTrimA(LPSTR pszString, LPCSTR pszTag)
{
	StringTrimLeftA(pszString, pszTag);
	StringTrimRightA(pszString, pszTag);
}

LONG WINAPI StringMidW(LPWSTR pszString, LONG nIndex, UINT nCount)
{	 
	LONG nRet = -1;
	if (!pszString)
	{
		return nRet;
	}	
	LONG nLength = (LONG)wcslen(pszString);
	assert(nIndex >= 0 && nIndex + (LONG)nCount <= nLength);	
	if (0 != nIndex && nCount > 0)
	{
		memmove(pszString, pszString + nIndex, nCount * sizeof(*pszString));
		pszString[nCount] = 0;  
#if defined(_DEBUG) || defined(DEBUG)
		{
			if (nIndex + (LONG)nCount < nLength)
			{
				ZeroMemory(pszString + nCount, (nLength - nCount) * sizeof(*pszString));
			}
		}  
#endif
		nRet = (LONG)wcslen(pszString);
	}  
	return nRet;
}
LONG WINAPI StringRightW(LPWSTR pszString, UINT nCount)
{
	if (!pszString)
	{
		return -1;
	}
	return StringMidW(pszString, (LONG)_tcslen(pszString) - nCount, nCount);
}
LONG WINAPI StringLeftW(LPWSTR pszString, UINT nCount)
{
	if (!pszString)
	{
		return -1;
	}
	LONG nLength = (LONG)wcslen(pszString);
	if (nLength >= (LONG)nCount)
	{
		pszString[nCount] = 0;
		return (LONG)wcslen(pszString);
	}
	return -1;
}
void WINAPI StringTrimLeftW(LPWSTR pszString, LPCWSTR pwhTag)
{	
	if (!pwhTag || !pszString)
	{
		return;
	}   
	if (!pszString[0])
	{
		return;
	}	
	LONG nLength = (LONG)wcslen(pszString);
	LONG nAmount = (LONG)wcsspn(pszString, pwhTag);
	if (nAmount < nLength)
	{
		StringRightW(pszString, nLength - nAmount);
	}	
}
void WINAPI StringTrimRightW(LPWSTR pszString, LPCWSTR pszTag)
{
	if (!pszTag || !pszString)
	{
		return;
	}   
	LONG nAmount = 0;
	BOOL blContinue = TRUE;	
	LONG nLength = (LONG)wcslen(pszString);
	for (LONG i = 0; i < nLength && blContinue; i++)
	{    
		if (NULL != wcschr(pszTag, pszString[nLength - i - 1]))
		{
			nAmount ++;
		}
		else
		{
			blContinue = FALSE;
		}
	}  
	if (!blContinue || nAmount > 0)
	{
		StringLeftW(pszString, nLength - nAmount);
	}
}
void WINAPI StringTrimW(LPWSTR pszString, LPCWSTR pszTag)
{
	StringTrimLeftW(pszString, pszTag);
	StringTrimRightW(pszString, pszTag);
}


WCHAR* WINAPI StringUpperW(LPWSTR pszString)
{
	assert(pszString);
	if (!pszString)
	{
		return NULL;
	}
	if (!pszString[0])
	{
		return NULL;
	}
	return wcsupr(pszString);
}

CHAR* WINAPI StringUpperA(LPSTR pszString)
{
	assert(pszString);
	if (!pszString)
	{
		return NULL;
	}
	if (!pszString[0])
	{
		return NULL;
	}
	return _strupr(pszString);
}

WCHAR* WINAPI StringLowerW(LPWSTR pszString)
{
	assert(pszString);
	if (!pszString)
	{
		return NULL;
	}
	if (!pszString[0])
	{
		return NULL;
	}
	return wcslwr(pszString);
}

CHAR* WINAPI StringLowerA(LPSTR pszString)
{
	assert(pszString);
	if (!pszString)
	{
		return NULL;
	}
	if (!pszString[0])
	{
		return NULL;
	}
	return _strlwr(pszString);
}
WCHAR* WINAPI StringReverseW(LPWSTR pszString)
{  
	assert(pszString);	
	if (!pszString)
	{
		return NULL;
	}
	WCHAR *start = pszString;
	WCHAR *left = pszString;
	WCHAR ch = 0;
	while (*pszString++)                 /* find end of pszString */
	{
		;
	}
	pszString -= 2;

	while (left < pszString)
	{
		ch = *left;
		*left++ = *pszString;
		*pszString -- = ch;
	}

	return(start);
}
CHAR* WINAPI StringReverseA(LPSTR pszString)
{  
	assert(pszString);	
	if (!pszString)
	{
		return NULL;
	}
	CHAR *start = pszString;
	CHAR *left = pszString;
	CHAR ch = 0;
	while (*pszString++)                 /* find end of pszString */
	{
		;
	}
	pszString -= 2;

	while (left < pszString)
	{
		ch = *left;
		*left++ = *pszString;
		*pszString -- = ch;
	}

	return(start);
}


LONG WINAPI StringWCharToChar(LPCWSTR pszStr, LONG nLen, LPSTR pszString, LONG nMaxLen)
{	
	return WideCharToMultiByte(CP_ACP, 0, pszStr, nLen,  pszString, nMaxLen, 0, 0);		
}

LONG WINAPI StringCharToWChar(LPCSTR pszStr, LONG nLen, LPWSTR pszString, LONG nMaxLen)
{ 
	return 	MultiByteToWideChar(CP_ACP, 0, pszStr, nLen,  pszString, nMaxLen);		
}




LONG WINAPI StringCchToGuidW(LPCWSTR pszString, LONG nCount, GUID* pGUID)
{
	if (!pszString || nCount < 0 || !pGUID)
	{
		return -1;
	}
	WCHAR szGUID[64] = {0};
	StringCchCopyW(szGUID, _countof(szGUID), pszString);
	StringTrimW(szGUID, L"{}");
	swscanf(szGUID, TEXT("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"), &pGUID->Data1, &pGUID->Data2, &pGUID->Data3, &pGUID->Data4[0], &pGUID->Data4[1], &pGUID->Data4[2], &pGUID->Data4[3], &pGUID->Data4[4], &pGUID->Data4[5], &pGUID->Data4[6], &pGUID->Data4[7]);
	return 0;	
}
LONG WINAPI StringCchToGuidA(LPCSTR pszString, LONG nCount, GUID* pGUID)
{
	WCHAR szGUID[64] = {0};
	if (!pszString || nCount < 0 || !pGUID)
	{
		return -1;
	}
	StringCharToWChar(pszString, nCount, szGUID, _countof(szGUID));	
	return StringCchToGuidW(szGUID, _countof(szGUID), pGUID);	
}

LONG WINAPI StringCchFromGuidW(const GUID* pGUID, LPWSTR pszString, LONG nCount)
{
	if (!pGUID)
	{
		return -1;
	}
	WCHAR szGUID[64] = {0};
	HRESULT  hRes = StringCchPrintfW(szGUID, _countof(szGUID), TEXT("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"), pGUID->Data1, pGUID->Data2, pGUID->Data3, pGUID->Data4[0], pGUID->Data4[1], pGUID->Data4[2], pGUID->Data4[3], pGUID->Data4[4], pGUID->Data4[5], pGUID->Data4[6], pGUID->Data4[7]);		
	if (hRes != S_OK)
	{	
		return -1;
	}
	if (pszString && nCount > 0)
	{
		StringCchCopyW(pszString, nCount, szGUID);
		return wcslen(pszString);
	}
	else if(!pszString && !nCount)
	{
		return wcslen(szGUID);
	}
	return -1;
}


LONG WINAPI StringCchFromGuidA(const GUID* pGUID, LPSTR pszString, LONG nCount)
{
	if (!pGUID)
	{
		return -1;
	}
	WCHAR szGUID[64] = {0};
	LONG nRet = StringCchFromGuidW(pGUID, szGUID, _countof(szGUID));
	if (!nRet && pszString && nCount > 0)
	{
		StringWCharToChar(szGUID, wcslen(szGUID), pszString, nCount);
	}
	return nRet;	
}

LONG WINAPI StringCompareA( LPCSTR pszA, LPCSTR pszB)
{	
	int ret = 0;
	while( ! (ret = *(BYTE *)pszA - *(BYTE *)pszB) && *pszB)
	{
		++ pszA;
		++ pszB;
	}

	if ( ret < 0 )
		ret = -1 ;
	else if ( ret > 0 )
		ret = 1 ;
	return( ret );
}

LONG WINAPI StringCompareW( LPCWSTR pszA, LPCWSTR pszB)
{
	int ret = 0;
	while( ! (ret = *(WORD  *)pszA - *(WORD *)pszB) && *pszB)
	{
		++ pszA;
		++ pszB;
	}

	if ( ret < 0 )
		ret = -1 ;
	else if ( ret > 0 )
		ret = 1 ;
	return( ret );
}

LONG WINAPI StringCompareNoCaseA( LPCSTR pszA, LPCSTR pszB )
{
	return _stricmp(pszA, pszB);
}

LONG WINAPI StringCompareNoCaseW( LPCWSTR pszA, LPCWSTR pszB )
{
	return wcsicmp(pszA, pszB);
}

LONG WINAPI StringFirstBelongToA( LPCSTR pszString, LPCSTR pszCharset)
{
	if (!pszString || !pszCharset)
	{
		return -1;
	}
	LPCSTR pszBegin = (LPCSTR) pszString;
	LPCSTR pszTravel;

	while (*pszBegin) 
	{
		for (pszTravel = (LPCSTR)pszCharset; *pszTravel != *pszBegin; pszTravel++)
		{
			if (*pszTravel == (BYTE)0) 
			{				
				return (LONG)(pszBegin - pszString);
			}
		}
		pszBegin ++;
	}	
	return (LONG)(pszBegin - pszString);	
}
LONG WINAPI StringFirstBelongToW( LPCWSTR pszString, LPCWSTR pszCharset)
{
	if (!pszString || !pszCharset)
	{
		return -1;
	}
	LPCWSTR pszBegin = (LPCWSTR) pszString;
	LPCWSTR pszTravel;

	while (*pszBegin) 
	{
		for (pszTravel = (LPCWSTR)pszCharset; *pszTravel != *pszBegin; pszTravel++)
		{
			if (*pszTravel == (WCHAR)0) 
			{				
				return (LONG)(pszBegin - pszString);
			}
		}
		pszBegin ++;
	}	
	return (LONG)(pszBegin - pszString);
}
LONG WINAPI StringFirstNotBelongToW( LPCWSTR pszString, LPCWSTR pszCharset)
{
	if (!pszString || !pszCharset)
	{
		return -1;
	}
	LPCWSTR pszTravel = (LPCWSTR) pszString;
	LPCWSTR pszCtrl;

	while (*pszTravel) 
	{
		for (pszCtrl = (LPCWSTR)pszCharset; *pszCtrl != *pszTravel; pszCtrl++)
		{
			if (*pszCtrl == *pszTravel) 
			{
				return (LONG)(pszTravel - pszString);
			}
			if (*pszCtrl == (WCHAR)0) 
			{				
				return (LONG)(pszTravel - pszString);
			}
		}
		pszTravel ++;
	}	
	return (LONG)(pszTravel - pszString);
}

LONG WINAPI StringFirstNotBelongToA( LPCSTR pszString, LPCSTR pszCharset)
{
	if (!pszString || !pszCharset)
	{
		return -1;
	}
	LPCSTR pszTravel = (LPCSTR) pszString;
	LPCSTR pszCtrl;

	while (*pszTravel) 
	{
		for (pszCtrl = (LPCSTR)pszCharset; *pszCtrl != *pszTravel; pszCtrl++)
		{
			if (*pszCtrl == *pszTravel) 
			{
				return (LONG)(pszTravel - pszString);
			}
			if (*pszCtrl == (CHAR)0) 
			{				
				return (LONG)(pszTravel - pszString);
			}
		}
		pszTravel ++;
	}	
	return (LONG)(pszTravel - pszString);
}


DWORD WINAPI StringToDwordW(LPCWSTR pszNum)
{
	DWORD dwValue = 0;
	if(!pszNum)
	{
		return dwValue;
	}	
	LPCWSTR pwhIndex = wcspbrk(pszNum, L"0123456789ABCDEFabcdef");
	if (pwhIndex) 
	{
		swscanf(pwhIndex, L"%x", &dwValue); 
	}
	return dwValue;
}

DWORD WINAPI StringToDwordA(LPCSTR pszNum)
{
	DWORD dwValue = 0;
	if(!pszNum)
	{
		return dwValue;
	}	
	LPCSTR pwhIndex = strpbrk(pszNum, "0123456789ABCDEFabcdef");
	if (pwhIndex) 
	{
		sscanf(pwhIndex, "%x", &dwValue); 
	}
	return dwValue;
}


float WINAPI StringToFloatW(LPCWSTR pszNum)
{
	float flValue = 0.0;
	if(!pszNum)
	{
		return flValue;
	}	
	LPCWSTR pszIndex = wcspbrk(pszNum, L"0123456789.-+");
	if (pszIndex) 
	{
		swscanf(pszIndex, L"%f", &flValue); 
	}
	return flValue;
}

float WINAPI StringToFloatA(LPCSTR pszNum)
{
	float flValue = 0.0;
	if(!pszNum)
	{
		return flValue;
	}	
	LPCSTR pszIndex = strpbrk(pszNum, "0123456789.-+");
	if (pszIndex) 
	{
		sscanf(pszIndex, "%f", &flValue); 
	}
	return flValue;
}

LONG WINAPI StringToLongW(LPCWSTR pszNum)
{
	LONG nValue = 0;
	if(!pszNum)
	{
		return nValue;
	}

	LPCWSTR pwhIndex = wcspbrk(pszNum, L"0123456789.-+");
	if (pwhIndex) 
	{
		swscanf(pwhIndex, L"%ld", &nValue);
	}
	return nValue;
}
LONG WINAPI StringToLongA(LPCSTR pszNum)
{
	LONG nValue = 0;
	if(!pszNum)
	{
		return nValue;
	}

	LPCSTR pwhIndex = strpbrk(pszNum, "0123456789.-+");
	if (pwhIndex) 
	{
		sscanf(pwhIndex, "%ld", &nValue);
	}
	return nValue;
}

BOOL WINAPI StringCchPathAppendW( LPWSTR pszPath,DWORD dwLen, LPCWSTR pszFile)
{
	if (!pszPath || !pszFile)
	{
		return FALSE;
	}
	WCHAR szPath[MAX_PATH] = {0};
	WCHAR szFile[MAX_PATH] = {0};
	StringCchCopyW(szPath, _countof(szPath), pszPath);
	StringTrimRightW(szPath, L" \t\\/");
	StringCchCopyW(szFile, _countof(szFile), pszFile);
	StringTrimLeftW(szFile, L" \t\\/");
	StringCchCatW(szPath, _countof(szPath), L"\\");
	StringCchCatW(szPath, _countof(szPath), szFile);
	StringCchCopyW(pszPath, dwLen, szPath);
	return TRUE;
}
BOOL WINAPI StringCchPathAppendA( LPSTR pszPath,DWORD dwLen, LPCSTR pszFile)
{
	if (!pszPath || !pszFile)
	{
		return FALSE;
	}
	CHAR szPath[MAX_PATH] = {0};
	CHAR szFile[MAX_PATH] = {0};
	StringCchCopyA(szPath, _countof(szPath), pszPath);
	StringTrimRightA(szPath, " \t\\/");
	StringCchCopyA(szFile, _countof(szFile), pszFile);
	StringTrimLeftA(szFile, " \t\\/");	
	StringCchCatA(szPath, _countof(szPath), "\\");
	StringCchCatA(szPath, _countof(szPath), szFile);
	StringCchCopyA(pszPath, dwLen, szPath);
	return TRUE;
}

BOOL	WINAPI StringChrChrW( LPWSTR pszString, WCHAR chA, WCHAR chB)
{
	if (!pszString)
	{
		return FALSE;
	}
	if (chA == chB)
	{
		return TRUE;
	}
	LPWSTR pszTravel = pszString;
	while (*pszTravel)
	{
		if (chA == *pszTravel)
		{
			*pszTravel = chB;
		}
		pszTravel ++;
	}
	return TRUE;
}

BOOL	WINAPI StringChrChrA( LPSTR pszString, CHAR chA, CHAR chB)
{
	if (!pszString)
	{
		return FALSE;
	}
	if (chA == chB)
	{
		return TRUE;
	}
	LPSTR pszTravel = pszString;
	while (*pszTravel)
	{
		if (chA == *pszTravel)
		{
			*pszTravel = chB;
		}
		pszTravel ++;
	}
	return TRUE;
}

void WINAPI StringSplitPathA(const CHAR *pszPath,  CHAR *pszDrive, size_t _DriveSize, CHAR *pszDir, size_t _DirSize,  CHAR *pszFileName, size_t _FilenameSize,  CHAR *pszExt, size_t _ExtSize)
{
	const CHAR *tmp = NULL;
	const CHAR *last_slash = NULL;
	const CHAR *dot = NULL;
	int drive_set = 0;
	size_t length = 0;
	int bEinval = 0;

	/* validation section */
	if (!pszPath
		|| ((!pszDrive && _DriveSize != 0) || (!pszDrive && _DriveSize == 0))
		|| ((!pszDir && _DirSize != 0) || (!pszDir && _DirSize == 0))
		|| ((!pszFileName && _FilenameSize != 0) || (!pszFileName && _FilenameSize == 0))
		|| ((!pszExt && _ExtSize != 0) || (!pszExt && _ExtSize == 0)))
	{
		if (pszDrive != NULL && _DriveSize > 0)
		{
			ZeroMemory(pszDrive, _DriveSize * sizeof(*pszDrive));
		}
		if (pszDir != NULL && _DirSize > 0)
		{
			ZeroMemory(pszDir, _DirSize * sizeof(*pszDir));
		}
		if (pszFileName != NULL && _FilenameSize > 0)
		{
			ZeroMemory(pszFileName, _FilenameSize * sizeof(*pszFileName));
		}
		if (pszExt != NULL && _ExtSize > 0)
		{
			ZeroMemory(pszExt, _ExtSize * sizeof(*pszExt));
		}		
	}    
	else
	{
		if (!drive_set)
		{
			size_t skip = _MAX_DRIVE - 2;
			tmp = pszPath;
			while (skip > 0 && *tmp != 0)
			{
				skip --;
				tmp ++;
			}
			if (*tmp == ':')
			{
				if (pszDrive != NULL)
				{					
					StringCchCopyNA(pszDrive, _DriveSize, pszPath, _MAX_DRIVE - 1);
				}
				pszPath = tmp + 1;
			}
			else
			{
				if (!pszDrive)
				{
					ZeroMemory(pszDrive, _DriveSize * sizeof(*pszDrive));
				}
			}
		}
	
		last_slash = NULL;
		dot = NULL;
		tmp = pszPath;
		for (; *tmp != 0; ++tmp)
		{
			if (*tmp == '/' || *tmp == '\\')
			{				
				last_slash = tmp + 1;
			}
			else if (*tmp == _T('.'))
			{
				dot = tmp;
			}
		}		

		if (last_slash != NULL)
		{			
			if (pszDir != NULL) 
			{
				length = (size_t)(last_slash - pszPath);				
				StringCchCopyNA(pszDir, _DirSize, pszPath, length);
			}
			pszPath = last_slash;
		}
		else
		{			
			if (pszDir != NULL)
			{
				ZeroMemory(pszDir, _DirSize * sizeof(*pszDir));
			}
		}
		
		if (dot != NULL && (dot >= pszPath))
		{			
			if (pszFileName)
			{
				length = (size_t)(dot - pszPath);			
				StringCchCopyNA(pszFileName, _FilenameSize, pszPath, length);
			}
			
			if (pszExt)
			{
				length = (size_t)(tmp - dot);				
				StringCchCopyNA(pszExt, _ExtSize, dot, length);
			}
		}
		else
		{			
			if (pszFileName)
			{
				length = (size_t)(tmp - pszPath);			
				StringCchCopyNA(pszFileName, _FilenameSize, pszPath, length);
			}
			if (pszExt)
			{
				ZeroMemory(pszExt, _ExtSize * sizeof(*pszExt));
			}
		}
	}	    
}



void WINAPI StringSplitPathW(const WCHAR *pszPath,  WCHAR *pszDrive, size_t _DriveSize, WCHAR *pszDir, size_t _DirSize,  WCHAR *pszFileName, size_t _FilenameSize,  WCHAR *pszExt, size_t _ExtSize)
{
	const WCHAR *tmp = NULL;
	const WCHAR *last_slash = NULL;
	const WCHAR *dot = NULL;
	int drive_set = 0;
	size_t length = 0;
	int bEinval = 0;

	/* validation section */
	if (!pszPath
		|| ((!pszDrive && _DriveSize != 0) || (!pszDrive && _DriveSize == 0))
		|| ((!pszDir && _DirSize != 0) || (!pszDir && _DirSize == 0))
		|| ((!pszFileName && _FilenameSize != 0) || (!pszFileName && _FilenameSize == 0))
		|| ((!pszExt && _ExtSize != 0) || (!pszExt && _ExtSize == 0)))
	{
		if (pszDrive != NULL && _DriveSize > 0)
		{
			ZeroMemory(pszDrive, _DriveSize * sizeof(*pszDrive));
		}
		if (pszDir != NULL && _DirSize > 0)
		{
			ZeroMemory(pszDir, _DirSize * sizeof(*pszDir));
		}
		if (pszFileName != NULL && _FilenameSize > 0)
		{
			ZeroMemory(pszFileName, _FilenameSize * sizeof(*pszFileName));
		}
		if (pszExt != NULL && _ExtSize > 0)
		{
			ZeroMemory(pszExt, _ExtSize * sizeof(*pszExt));
		}		
	}    
	else
	{
		if (!drive_set)
		{
			size_t skip = _MAX_DRIVE - 2;
			tmp = pszPath;
			while (skip > 0 && *tmp != 0)
			{
				skip --;
				tmp ++;
			}
			if (*tmp == L':')
			{
				if (pszDrive != NULL)
				{					
					StringCchCopyNW(pszDrive, _DriveSize, pszPath, _MAX_DRIVE - 1);
				}
				pszPath = tmp + 1;
			}
			else
			{
				if (!pszDrive)
				{
					ZeroMemory(pszDrive, _DriveSize * sizeof(*pszDrive));
				}
			}
		}

		last_slash = NULL;
		dot = NULL;
		tmp = pszPath;
		for (; *tmp != 0; ++tmp)
		{
			if (*tmp == '/' || *tmp == '\\')
			{				
				last_slash = tmp + 1;
			}
			else if (*tmp == '.')
			{
				dot = tmp;
			}
		}		

		if (last_slash != NULL)
		{			
			if (pszDir != NULL) 
			{
				length = (size_t)(last_slash - pszPath);				
				StringCchCopyNW(pszDir, _DirSize, pszPath, length);
			}
			pszPath = last_slash;
		}
		else
		{			
			if (pszDir != NULL)
			{
				ZeroMemory(pszDir, _DirSize * sizeof(*pszDir));
			}
		}

		if (dot != NULL && (dot >= pszPath))
		{			
			if (pszFileName)
			{
				length = (size_t)(dot - pszPath);			
				StringCchCopyNW(pszFileName, _FilenameSize, pszPath, length);
			}

			if (pszExt)
			{
				length = (size_t)(tmp - dot);				
				StringCchCopyNW(pszExt, _ExtSize, dot, length);
			}
		}
		else
		{			
			if (pszFileName)
			{
				length = (size_t)(tmp - pszPath);			
				StringCchCopyNW(pszFileName, _FilenameSize, pszPath, length);
			}
			if (pszExt)
			{
				ZeroMemory(pszExt, _ExtSize * sizeof(*pszExt));
			}
		}
	}	    
}
