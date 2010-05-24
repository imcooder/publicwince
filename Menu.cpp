
#include "stdafx.h"
#include "Menu.h"


BOOL WINAPI XUE_IsMenu(HMENU hMenu)
{
	MENUITEMINFO mii;
	ZeroMemory(&mii, sizeof(mii));
	mii.cbSize = sizeof(mii);

	SetLastError(0);
	if (::GetMenuItemInfo(hMenu, 0, TRUE, &mii))
	{
		return TRUE;
	}
	else
	{
		return (::GetLastError() != ERROR_INVALID_MENU_HANDLE);
	}
}
LONG WINAPI XUE_GetMenuItemCount(HMENU hMenu)
{
	if(!XUE_IsMenu(hMenu))
	{
		::SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}

	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);

	UINT i = 0;

	while(true)
	{
		if(0 == ::GetMenuItemInfo(hMenu, i, TRUE, &mii))
		{
			break;
		}
		++i;
	}

	return i;
}

UINT WINAPI XUE_GetMenuItemID(HMENU hMenu, int nPos)
{
	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii); 
	mii.fMask  = MIIM_ID; 

	if((0 == ::GetMenuItemInfo(hMenu, nPos, TRUE, &mii)) || (mii.hSubMenu != NULL))
	{
		return 0xFFFFFFFF;
	}

	return mii.wID; 
}



HMENU WINAPI XUE_GetMenu(HWND hWnd)
{
	HMENU hMenu = NULL;
#ifdef UNDER_CE
	#if defined(SHELLSDK_MODULES_AYGSHELL) || defined(SHELLW_MODULES_AYGSHELL)
	HWND hWndMB = ::SHFindMenuBar(hWnd);
	if(hWndMB != NULL)
	{
		return SHGetMenu(hWndMB);
	}
#endif

#else
	::GetMenu(hWnd);
#endif
	return hMenu;
}
