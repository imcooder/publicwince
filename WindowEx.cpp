

#include "stdafx.h"
#include "WindowEx.h"
#include <stdlib.h>
#include <stdio.h>
BOOL WINAPI InitDefaultFont(LOGFONT* pLogFont)
{
	if (!pLogFont)
	{
		return FALSE;
	}
#ifdef WINCE
	HFONT hFont =  (HFONT)::GetStockObject(SYSTEM_FONT);			
#else
	HFONT hFont =  (HFONT)::GetStockObject(DEFAULT_GUI_FONT);	
#endif
	GetObject(hFont, sizeof(*pLogFont), pLogFont);
	SAFE_DELETE_OBJECT(hFont);
	return TRUE;
}

BOOL WINAPI XUE_ClientToScreen( HWND hWnd, LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}
	POINT point = {0, 0};
	point.x = pRect->left;
	point.y = pRect->top;
	ClientToScreen(hWnd, &point);
	pRect->left= point.x;
	pRect->top = point.y;

	point.x = pRect->right;
	point.y = pRect->bottom;
	ClientToScreen(hWnd, &point);
	pRect->right= point.x;
	pRect->bottom = point.y;
	return TRUE;
}


BOOL WINAPI XUE_ScreenToClient( HWND hWnd, LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}
	POINT point = {0, 0};
	point.x = pRect->left;
	point.y = pRect->top;
	ScreenToClient(hWnd, &point);
	pRect->left= point.x;
	pRect->top = point.y;

	point.x = pRect->right;
	point.y = pRect->bottom;
	ScreenToClient(hWnd, &point);
	pRect->right= point.x;
	pRect->bottom = point.y;
	return TRUE;
}


BOOL WINAPI XUE_ModifyStyle(HWND hWnd, LONG_PTR dwRemove, LONG_PTR dwAdd, UINT nFlags)
{	
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_ModifyStyle Parameter Error\n"));
	if (!::IsWindow(hWnd))
	{
		return FALSE;
	}
	DWORD dwStyle = XUE_GetWindowStyle(hWnd);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle != dwNewStyle)
	{
		::XUE_SetWindowStyle(hWnd, dwNewStyle);
		if (nFlags != 0)
		{
			::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
	}	
	return TRUE;
}

BOOL WINAPI XUE_ModifyStyleEx(HWND hWnd, LONG_PTR dwRemove, LONG_PTR dwAdd, UINT nFlags )
{	
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_ModifyStyleEx Parameter Error\n"));
	if (!::IsWindow(hWnd))
	{
		return FALSE;
	}	
	DWORD dwStyle = XUE_GetWindowExStyle(hWnd);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle != dwNewStyle)
	{
		XUE_SetWindowExStyle(hWnd, dwNewStyle);		
		if (nFlags != 0)
		{
			::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
	}	
	return TRUE;
}

DLLXEXPORT LONG_PTR WINAPI XUE_GetWindowID( HWND hWnd)
{
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_GetWindowID Parameter Error\n"));
	return XUE_GetWindowLong(hWnd, GWL_ID);
}

DLLXEXPORT LONG_PTR WINAPI XUE_SetWindowID( HWND hWnd, LONG_PTR nID)
{	
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_SetWindowID Parameter Error\n"));
	return XUE_SetWindowLong(hWnd, GWL_ID, nID);
}

DLLXEXPORT LONG_PTR WINAPI XUE_GetWindowStyle( HWND hWnd )
{	
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_GetWindowStyle Parameter Error\n"));
	return XUE_GetWindowLong(hWnd, GWL_STYLE);
}

DLLXEXPORT LONG_PTR WINAPI XUE_GetWindowExStyle( HWND hWnd )
{	
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_GetWindowExStyle Parameter Error\n"));
	return XUE_GetWindowLong(hWnd, GWL_EXSTYLE);
}

DLLXEXPORT LONG_PTR		WINAPI XUE_SetWindowStyle( HWND hWnd, LONG_PTR nStyle )
{
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_SetWindowStyle Parameter Error\n"));
	return XUE_SetWindowLong(hWnd, GWL_STYLE, nStyle);
}

DLLXEXPORT LONG_PTR WINAPI XUE_SetWindowExStyle( HWND hWnd, LONG_PTR nExStyle)
{
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_SetWindowExStyle Parameter Error\n"));
	return XUE_SetWindowLong(hWnd, GWL_EXSTYLE, nExStyle);
}


DLLXEXPORT LONG_PTR WINAPI XUE_SetWindowLong( HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
#ifdef UNDER_CE
	return ::SetWindowLong(hWnd, nIndex, dwNewLong);
#else
	return ::SetWindowLongPtr(hWnd, nIndex, dwNewLong);
#endif
}

DLLXEXPORT LONG_PTR			WINAPI XUE_GetWindowLong( HWND hWnd, int nIndex)
{
#ifdef UNDER_CE
	return ::GetWindowLong(hWnd, nIndex);
#else
	return ::GetWindowLongPtr(hWnd, nIndex);
#endif
}

DLLXEXPORT BOOL WINAPI XUE_CenterWindow(HWND hWnd, HWND hWndCenter)
{
	HWTRACEEX(!IsWindow(hWnd), TEXT("XUE_CenterWindow Parameter Error\n"));	
	if (!::IsWindow(hWnd))
	{
		return FALSE;
	}
	// determine owner window to center against
	DWORD dwStyle = XUE_GetWindowStyle(hWnd);
	if(!hWndCenter)
	{
		if(dwStyle & WS_CHILD)
		{
			hWndCenter = ::GetParent(hWnd);
		}
		else
		{
			hWndCenter = ::GetWindow(hWnd, GW_OWNER);
		}
	}
	
	RECT rcWindow = {0, 0, 0, 0};
	::GetWindowRect(hWnd, &rcWindow);
	RECT rcArea = {0, 0, 0, 0};
	RECT rcCenter = {0, 0, 0, 0};
	HWND hWndParent = NULL;
	if(!(dwStyle & WS_CHILD))
	{		
		if(!hWndCenter)
		{
			DWORD dwStyleCenter = XUE_GetWindowStyle(hWndCenter);
			if(!(dwStyleCenter & WS_VISIBLE)
#ifndef UNDER_CE
				|| (dwStyleCenter & WS_MINIMIZE)
#endif
				)
			{
				hWndCenter = NULL;
			}
		}

		// center within screen coordinates
#if WINVER < 0x0500
		::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
#else
		HMONITOR hMonitor = NULL;
		if(hWndCenter != NULL)
		{
			hMonitor = ::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST);
		}
		else
		{
			hMonitor = ::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
		}
		ENSURE_RETURN_VAL(hMonitor, FALSE);
		MONITORINFO minfo;
		minfo.cbSize = sizeof(MONITORINFO);
		BOOL bResult = ::GetMonitorInfo(hMonitor, &minfo);
		ENSURE_RETURN_VAL(bResult, FALSE);

		rcArea = minfo.rcWork;
#endif
		if(!hWndCenter)
		{
			rcCenter = rcArea;
		}
		else
		{
			::GetWindowRect(hWndCenter, &rcCenter);
		}
	}
	else
	{		
		hWndParent = ::GetParent(hWnd);
		::GetClientRect(hWndParent, &rcArea);	
		::GetClientRect(hWndCenter, &rcCenter);
		::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
	}

	int DlgWidth = rcWindow.right - rcWindow.left;
	int DlgHeight = rcWindow.bottom - rcWindow.top;

	// find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// if the dialog is outside the screen, move it inside
	if(xLeft + DlgWidth > rcArea.right)
	{
		xLeft = rcArea.right - DlgWidth;
	}
	if(xLeft < rcArea.left)
	{
		xLeft = rcArea.left;
	}

	if(yTop + DlgHeight > rcArea.bottom)
	{
		yTop = rcArea.bottom - DlgHeight;
	}
	if(yTop < rcArea.top)
	{
		yTop = rcArea.top;
	}	
	return ::SetWindowPos(hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}