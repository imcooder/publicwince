#include "stdafx.h"
#include "HWShell.h"
#include <ShlObj.h>


#ifdef UNDER_CE
#pragma comment(lib, "Ceshell.lib")
#endif

//代替系统中SHGetSpecialFolderPath
LONG WINAPI HWSHGetSpecialFolderPath(LPWSTR pszPath, LONG nMax, LONG nFolder)
{
	TCHAR szPath[MAX_PATH] = {0};
	IMalloc * pShellMalloc = NULL; 	
	LPITEMIDLIST pidlItem = NULL; 	 
	HRESULT hRes = SHGetMalloc(&pShellMalloc);
	if (FAILED(hRes))
	{
		return -1;
	}
	hRes = SHGetSpecialFolderLocation(NULL,	nFolder, &pidlItem);
	if (SUCCEEDED(hRes))
	{		
		if (!SHGetPathFromIDList(pidlItem, szPath))
		{
				return -1;	
		}			
	}	
	if (pidlItem)
	{
		pShellMalloc->Free(pidlItem);
		pidlItem = NULL;
	}
	pShellMalloc->Release();
	pShellMalloc = NULL;
	if (pszPath && nMax > 0)
	{
		StringCchCopyW(pszPath, nMax, szPath);
		return wcslen(pszPath);
	}
	else
	{
		return wcslen(szPath);
	}
}