
#ifndef XUE_FILE_TOOLBAR_H
#define XUE_FILE_TOOLBAR_H

#include <windows.h>

BOOL WINAPI XUE_IsToolbar(HWND);
LONG WINAPI XUE_ToolbarIndexToCommand(HWND, LONG);
LONG WINAPI XUE_ToolbarCommandToIndex(HWND, LONG);
BOOL WINAPI XUE_ToolbarHideButton(HWND, LONG, BOOL blByIndex = TRUE, BOOL blHide = TRUE);
BOOL WINAPI XUE_ToolbarSetButtonText( HWND, LONG, BOOL blByIndex, LPCTSTR);
BOOL WINAPI XUE_ToolbarSetButtonCommand(HWND, LONG, LONG);

#endif // XUE_FILE_TOOLBAR_H
