
/********************************************************************
created:	2009/11/20
created:	20:11:2009   10:24
filename: 	HWRegistry.h
file base:	HWRegistry
file ext:	h
author:		xuejuntao@hanwang.com.cn	
purpose:	简单注册表访问函数, 接口简单 效率低下 不适合大量注册表访问
*********************************************************************/

#ifndef HWX_HWREGISTRY_H
#define HWX_HWREGISTRY_H

#include "Ext_Type.h"

DWORD		WINAPI HWXUE_RegQueryValueDWORD(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwDefData);
BOOL		WINAPI HWXUE_RegSetValueDWORD(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData);
BOOL		WINAPI HWXUE_RegSetValueString(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszData);
DWORD		WINAPI HWXUE_RegQueryValueString(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszDefData, LPTSTR pszData, DWORD dwCount);
BOOL		WINAPI HWXUE_RegSetValueBinary(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, const BYTE* pbData, DWORD dwByte);
DWORD		WINAPI HWXUE_RegQueryValueBinary(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, const BYTE* pbDefData, DWORD dwDefSize, BYTE* pbData, DWORD dwCount);
BOOL		WINAPI HWXUE_RegDeleteValue(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue);

#endif//HWX_HWREGISTRY_H
