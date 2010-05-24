

#ifndef HWX_MENU_H
#define HWX_MENU_H

BOOL		WINAPI XUE_IsMenu(HMENU hMenu);
LONG		WINAPI XUE_GetMenuItemCount(HMENU hMenu);
UINT		WINAPI XUE_GetMenuItemID(HMENU hMenu, int nPos);
HMENU		WINAPI XUE_GetMenu(HWND hWnd);

#endif //HWX_MENU_H
