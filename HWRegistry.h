#ifndef HWX_HWREGISTRY_H
#define HWX_HWREGISTRY_H

#include "Ext_Type.h"

DWORD		WINAPI HWRegQueryValueDWORD(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwDefData);
BOOL		WINAPI HWRegSetValueDWORD(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, DWORD dwData);
BOOL		WINAPI HWRegSetValueString(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszData);
DWORD		WINAPI HWRegQueryValueString(HKEY hKey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszDefData, LPTSTR pszData, LONG nSize);
#endif//HWX_HWREGISTRY_H
