/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	BitmapEx.cpp			
文件内容:			Bitmap简单处理
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/
#include "StdAfx.h"
#include "Bitmap.h"
#ifdef WINCE
	
#endif

HBITMAP WINAPI XUE_LoadBitmapFromFileW( LPCWSTR pszFile)
{
	HBITMAP hBitmap = NULL;
	if (!pszFile)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return hBitmap;
	}
#ifdef WINCE
	{
		hBitmap = SHLoadDIBitmap(pszFile);
	}
#else
	{
		hBitmap = (HBITMAP)LoadImage(NULL, pszFile, IMAGE_BITMAP,	0, 0, LR_LOADFROMFILE);
	}
#endif	
	return hBitmap;
}


HBITMAP WINAPI XUE_LoadBitmapFromFileA( LPCSTR pszFile)
{
	HBITMAP hBitmap = NULL;
	if (!pszFile)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return hBitmap;
	}
	WCHAR szPath[MAX_PATH] = {0};
	mbstowcs(szPath, pszFile, strlen(pszFile));
	return XUE_LoadBitmapFromFileW(szPath);
}

void* WINAPI XUE_LoadResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType,  LPDWORD pvSize)
{	
	HRSRC hRes = NULL;
	DWORD dwSize = 0;
	HGLOBAL hData = NULL;
	void*  pvRes = NULL;	
	hRes = FindResource(hModule, lpName, lpType);
	if (hRes)
	{
		dwSize = SizeofResource(hModule, hRes);
		hData = LoadResource(hModule, hRes);
		pvRes = (void*)LockResource(hData);
	}
	if (pvSize)
	{
		*pvSize = dwSize;
	}
	return pvRes;	
}