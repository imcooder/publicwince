
#include "stdafx.h"
#include "Ext_type.h"
#include "Window.h"
#include <HWDebug.h>

_Window::_Window()
{
	m_hInstance = NULL;
	m_hWnd = NULL;	
	m_hWndCreator = NULL;
}

_Window::~_Window()
{	
	DestroyWindow();	
	m_hWndCreator = NULL;
}

BOOL _Window::IsVisible() const
{		
	return ::IsWindowVisible(GetSafeHwnd());
}

BOOL _Window::IsVisible(HWND hWnd)
{
	return ::IsWindowVisible(hWnd);
}
BOOL _Window::IsWindow() const
{
	return ::IsWindow(GetSafeHwnd());
}

BOOL _Window::IsWindow( HWND hWnd)
{
	return ::IsWindow(hWnd);
}

BOOL _Window::UpdateWindow()
{
	if (IsWindow()) 
	{
		return ::UpdateWindow(GetSafeHwnd());
	}
	else
	{
		return FALSE;
	}
}


HWND _Window::GetSafeHwnd() const
{
	return m_hWnd;
}

BOOL _Window::ShowWindow( DWORD dwCmdShow)
{
	if (IsWindow())
	{
		return ::ShowWindow(GetSafeHwnd(), dwCmdShow);
	}	
	else
	{
		return FALSE;
	}
}

BOOL _Window::ShowWindow( HWND hWnd, DWORD dwCmdShow)
{
	return ::ShowWindow(hWnd, dwCmdShow);
}
BOOL _Window::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx,int cy,UINT uFlags)
{
	if (IsWindow())
	{
		return ::SetWindowPos(GetSafeHwnd(), hWndInsertAfter, X, Y, cx, cy, uFlags);
	}
	else
	{
		return FALSE;
	}
}

BOOL _Window::SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx,int cy,UINT uFlags )
{
	return ::SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
}
BOOL _Window::InvalidateRect( CONST RECT* lpRect,BOOL bErase )
{
	if (IsWindow())
	{
		return ::InvalidateRect(m_hWnd, lpRect, bErase);
	}
	else
	{
		return FALSE;
	}
}

BOOL _Window::MoveWindow( int X,int Y,int nWidth,int nHeight,BOOL bRepaint )
{
	if (IsWindow())
	{
		return MoveWindow(GetSafeHwnd(), X, Y, nWidth, nHeight, bRepaint);
	} 
	else
	{
		return FALSE;
	}
}

BOOL _Window::MoveWindow( HWND hWnd, int X,int Y,int nWidth,int nHeight,BOOL bRepaint)
{
	return ::MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);
}
BOOL _Window::ResizeWindow( SIZE sz)
{
	return SetWindowPos(NULL, 0, 0, sz.cx, sz.cy,	SWP_NOZORDER | SWP_NOMOVE);
}

HWND _Window::GetParent() const
{
	return GetParent(m_hWnd);
}

HWND _Window::GetParent( HWND hWnd)
{
	return ::GetParent(hWnd);
}

BOOL _Window::PostMessage( UINT Msg, WPARAM wParam,LPARAM lParam )
{
	return ::PostMessage(GetSafeHwnd(), Msg, wParam, lParam);
}

BOOL _Window::PostMessage( HWND hWnd, UINT Msg, WPARAM wParam,LPARAM lParam )
{
	return ::PostMessage(hWnd, Msg, wParam, lParam);
}
LRESULT _Window::SendMessage( UINT Msg,	WPARAM wParam, LPARAM lParam)
{
	return SendMessage(GetSafeHwnd(), Msg, wParam, lParam);
}

LRESULT _Window::SendMessage( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	//HWTRACE(TEXT("_Window::SendMessage hWnd %d %d\n"), hWnd, ::IsWindow(hWnd));
	return ::SendMessage(hWnd, Msg, wParam, lParam);
}

BOOL _Window::GetWindowRect( RECT* pRect) const
{
	if (!pRect)
	{
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(pRect));
	if (!IsWindow())
	{
		return FALSE;
	}
	return ::GetWindowRect(GetSafeHwnd(), pRect);
}

BOOL _Window::GetWindowRect(HWND hWnd, RECT* pRect)
{
	if (!pRect || !::IsWindow(hWnd))
	{
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(pRect));	
	return ::GetWindowRect(hWnd, pRect);
}
BOOL _Window::GetClientRect( RECT* pRect) const
{
	if (!pRect)
	{
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(pRect));
	if (!IsWindow())
	{
		return FALSE;
	}
	return ::GetClientRect(GetSafeHwnd(), pRect);
}

BOOL _Window::GetClientRect( HWND hWnd, RECT* pRect)
{
	if (!pRect || !::IsWindow(hWnd))
	{
		return FALSE;
	}
	ZeroMemory(pRect, sizeof(pRect));	
	return ::GetClientRect(hWnd, pRect);
}
void _Window::DestroyWindow( void )
{	
	::DestroyWindow(GetSafeHwnd());
	m_hWnd = NULL;
}


BOOL _Window::SetWindowText( LPCTSTR pszTitle)
{
	if (!pszTitle)
	{
		return FALSE;
	}
	if (IsWindow())
	{
		return ::SetWindowText(GetSafeHwnd(), pszTitle);
	}
	return FALSE;
}

BOOL _Window::SetWindowText(HWND hWnd, LPCTSTR pszTitle)
{
	if (!pszTitle)
	{
		return FALSE;
	}
	if (::IsWindow(hWnd))
	{
		return ::SetWindowText(hWnd, pszTitle);
	}
	return FALSE;
}
BOOL _Window:: ClientToScreen( LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}	
	return ClientToScreen(GetSafeHwnd(), pRect);
}
BOOL _Window:: ClientToScreen( HWND hWnd, LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}
	POINT point = {0, 0};
	point.x = pRect->left;
	point.y = pRect->top;
	::ClientToScreen(hWnd, &point);
	pRect->left= point.x;
	pRect->top = point.y;

	point.x = pRect->right;
	point.y = pRect->bottom;
	::ClientToScreen(hWnd, &point);
	pRect->right= point.x;
	pRect->bottom = point.y;
	return TRUE;
}


BOOL _Window::ScreenToClient( LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}	
	return ScreenToClient(GetSafeHwnd(), pRect);
}

BOOL _Window::ScreenToClient( HWND hWnd, LPRECT pRect)
{
	if (!pRect)
	{
		return FALSE;
	}
	POINT point = {0, 0};
	point.x = pRect->left;
	point.y = pRect->top;
	::ScreenToClient(hWnd, &point);
	pRect->left= point.x;
	pRect->top = point.y;

	point.x = pRect->right;
	point.y = pRect->bottom;
	::ScreenToClient(hWnd, &point);
	pRect->right= point.x;
	pRect->bottom = point.y;
	return TRUE;
}


BOOL _Window::EnableWindow( BOOL bEnable )
{
	if (IsWindow())
	{
		return ::EnableWindow(GetSafeHwnd(), bEnable);
	}
	return FALSE;	
}

BOOL _Window::EnableWindow(HWND hWnd, BOOL bEnable)
{
	if (::IsWindow(hWnd))
	{
		return ::EnableWindow(hWnd, bEnable);
	}
	return FALSE;	
}


BOOL _Window::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	HWND hWnd = GetSafeHwnd();
	if (!::IsWindow(hWnd))
	{
		return FALSE;
	}
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle != dwNewStyle)
	{
		::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
		if (nFlags != 0)
		{
			::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
	}	
	return TRUE;
}

BOOL _Window::ModifyStyleEx( DWORD dwRemove, DWORD dwAdd, UINT nFlags )
{
	HWND hWnd = GetSafeHwnd();
	if (!::IsWindow(hWnd))
	{
		return FALSE;
	}
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
	if (dwStyle != dwNewStyle)
	{
		::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewStyle);
		if (nFlags != 0)
		{
			::SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
	}	
	return TRUE;
}

HINSTANCE _Window::GetInstance() const
{
	return m_hInstance;
}

LONG _Window::SetID( LONG nID)
{
	return SetID(GetSafeHwnd(), nID);
}

LONG _Window::SetID( HWND hWnd, LONG nID)
{
	if (!IsWindow(hWnd))
	{
		return 0;
	}
	return SetWindowLong(hWnd, GWL_ID, nID);
}
LONG _Window::GetID() const
{
	return GetID(GetSafeHwnd());
}

LONG _Window::GetID(HWND hWnd)
{
	if (!IsWindow(hWnd))
	{
		return 0;
	}
	return GetWindowLong(hWnd, GWL_ID);	
}

HWND _Window::GetOwner() const
{
	return GetOwner(GetSafeHwnd());
}

HWND _Window::GetOwner( HWND hWnd)
{
	HWND hOwner = NULL;		;
	if (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
	{
		hOwner = GetParent(hWnd);
	}
	else
	{
		hOwner = GetWindow(hWnd, GW_OWNER);
	}
	return hOwner;
}

HWND _Window::GetCreator() const
{
	return m_hWndCreator;
}