/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	HWStrSafe.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/06/21
*********************************************************************/
#ifndef HWX_HWSTRSAFE_H
#define HWX_HWSTRSAFE_H
#pragma once
#include <strsafe.h>
#include "Ext_Type.h"


#ifdef _UNICODE

#define StringToken			StringTokW	
#define StringMid				StringMidW
#define StringRight			StringRightW
#define StringLeft			StringLeftW
#define StringTrimLeft	StringTrimLeftW
#define StringTrimRight	StringTrimRightW
#define StringTrim			StringTrimW
#define StringUpper			StringUpperW
#define StringLower			StringLowerW
#define StringReverse		StringReverseW
#define StringCchToGuid	StringCchToGuidW
#define StringCchFromGuid	StringCchFromGuidW
#define StringCompare		StringCompareW
#define StringCompareNoCase				StringCompareNoCaseW
#define StringFirstBelongTo				StringFirstBelongToW
#define StringFirstNotBelongTo		StringFirstNotBelongToW
#define StringToLong							StringToLongW
#define StringToFloat							StringToFloatW
#define StringToDword							StringToDwordW
#define StringPathAppend					StringCchPathAppendW
#define StringChrChr							StringChrChrW
#define StringSplitPath						StringSplitPathW
#else

#define StringToken			StringTokA
#define StringMid				StringMidA
#define StringRight			StringRightA
#define StringLeft			StringLeftA
#define StringTrimLeft	StringTrimLeftA
#define StringTrimRight	StringTrimRightA
#define StringTrim			StringTrimA
#define StringUpper			StringUpperA
#define StringLower			StringLowerA
#define StringReverse		StringReverseA
#define StringCchToGuid	StringCchToGuidA
#define StringCchFromGuid	StringCchFromGuidA
#define StringCompare		StringCompareA
#define StringCompareNoCase				StringCompareNoCaseA
#define StringFirstBelongTo				StringFirstBelongToA
#define StringFirstNotBelongTo		StringFirstNotBelongToA
#define StringToLong							StringToLongA
#define StringToFloat							StringToFloatA
#define StringToDword							StringToDwordA
#define StringPathAppend					StringCchPathAppendA
#define StringChrChr							StringChrChrA
#define StringSplitPath						StringSplitPathA
#endif



#ifdef __cplusplus
extern "C"{
#endif

	CHAR*		WINAPI StringTokA(CHAR*,	const CHAR*,	CHAR **);
	WCHAR*	WINAPI StringTokW(WCHAR*,	const WCHAR*,	WCHAR **);
	LONG		WINAPI StringMidW(LPWSTR pszString, LONG nIndex, UINT nCount);
	LONG		WINAPI StringMidA(LPSTR pszString, LONG nIndex, UINT nCount);
	LONG		WINAPI StringRightW(LPWSTR pszString, UINT nCount);
	LONG		WINAPI StringRightA(LPSTR pszString, UINT nCount);
	LONG		WINAPI StringLeftW(LPWSTR pszString, UINT nCount);
	LONG		WINAPI StringLeftA(LPSTR pszString, UINT nCount);
	void		WINAPI StringTrimLeftW(LPWSTR pszString, LPCWSTR pwhTag);
	void		WINAPI StringTrimLeftA(LPSTR pszString, LPCSTR pszTag);
	void		WINAPI StringTrimRightW(LPWSTR pszString, LPCWSTR pszTag);	
	void		WINAPI StringTrimRightA(LPSTR pszString, LPCSTR pszTag);
	void		WINAPI StringTrimW(LPWSTR pszString, LPCWSTR pszTag);
	void		WINAPI StringTrimA(LPSTR pszString, LPCSTR pszTag);
	WCHAR*	WINAPI StringUpperW(LPWSTR pszString);
	CHAR*		WINAPI StringUpperA(LPSTR pszString);
	WCHAR*	WINAPI StringLowerW(LPWSTR pszString);
	CHAR*		WINAPI StringLowerA(LPSTR pszString);
	WCHAR*	WINAPI StringReverseW(LPWSTR pszString);
	CHAR*		WINAPI StringReverseA(LPSTR pszString);
	LONG		WINAPI StringWCharToChar(LPCWSTR pszStr, LONG nLen, LPSTR pszString, LONG nMaxLen);
	LONG		WINAPI StringCharToWChar(LPCSTR pszStr, LONG nLen, LPWSTR pszString, LONG nMaxLen);
	LONG		WINAPI StringCchToGuidW(LPCWSTR pszString, LONG nCount, GUID* pGUID);
	LONG		WINAPI StringCchToGuidA(LPCSTR pszString, LONG nCount, GUID* pGUID);
	LONG		WINAPI StringCchFromGuidW(const GUID* pGUID, LPWSTR pszString, LONG nCount);
	LONG		WINAPI StringCchFromGuidA(const GUID* pGUID, LPSTR pszString, LONG nCount);
	LONG		WINAPI StringCompareA(LPCSTR,	LPCSTR);
	LONG		WINAPI StringCompareW(LPCWSTR,	LPCWSTR);
	LONG		WINAPI StringCompareNoCaseA(LPCSTR,	LPCSTR);
	LONG		WINAPI StringCompareNoCaseW(LPCWSTR,	LPCWSTR);
	LONG		WINAPI StringFirstBelongToA(LPCSTR, LPCSTR);
	LONG		WINAPI StringFirstBelongToW(LPCWSTR, LPCWSTR);
	LONG		WINAPI StringFirstNotBelongToW( LPCWSTR, LPCWSTR);
	LONG		WINAPI StringFirstNotBelongToA( LPCSTR, LPCSTR);
	LONG		WINAPI StringToLongA(LPCSTR);
	LONG		WINAPI StringToLongW(LPCWSTR);
	float		WINAPI StringToFloatA(LPCSTR);
	float		WINAPI StringToFloatW(LPCWSTR);
	DWORD		WINAPI StringToDwordA(LPCSTR);
	DWORD		WINAPI StringToDwordW(LPCWSTR);
	BOOL		WINAPI StringCchPathAppendW( LPWSTR, DWORD, LPCWSTR);
	BOOL		WINAPI StringCchPathAppendA( LPSTR, DWORD, LPCSTR);
	BOOL		WINAPI StringChrChrW(LPWSTR, WCHAR, WCHAR);
	BOOL		WINAPI StringChrChrA(LPSTR, CHAR, CHAR);
	void		WINAPI StringSplitPathA(const CHAR *,  CHAR *pszDrive, size_t, CHAR *pszDir, size_t,  CHAR *pszFileName, size_t,  CHAR *pszExt, size_t);
	void		WINAPI StringSplitPathW(const WCHAR *,  WCHAR *pszDrive, size_t, WCHAR *pszDir, size_t,  WCHAR *pszFileName, size_t,  WCHAR *pszExt, size_t);
#ifdef __cplusplus
}
#endif


#endif //HWX_HWSTRSAFE_H