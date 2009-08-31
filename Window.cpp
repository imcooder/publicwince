
#include "stdafx.h"
#include "Ext_type.h"
#include "Window.h"
#include <HWDebug.h>

_Window::_Window()
{
	m_hInstance = NULL;
	m_hWnd = NULL;	
}

_Window::~_Window()
{	
	DestroyWindow();	
}

BOOL _Window::IsVisible() const
{
	HWTRACE(TEXT("_Window::IsVisible %d %d\n"), GetSafeHwnd(), ::IsWindowVisible(GetSafeHwnd()));	
	return ::IsWindowVisible(GetSafeHwnd());
}

BOOL _Window::IsWindow() const
{
	return ::IsWindow(GetSafeHwnd());
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
		return ::ShowWindow(m_hWnd, dwCmdShow);
	}	
	else
	{
		return FALSE;
	}
}
BOOL _Window::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx,int cy,UINT uFlags)
{
	if (IsWindow())
	{
		return ::SetWindowPos(m_hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
	}
	else
	{
		return FALSE;
	}
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
		return ::MoveWindow(m_hWnd, X, Y, nWidth, nHeight, bRepaint);
	} 
	else
	{
		return FALSE;
	}
}

BOOL _Window::ResizeWindow( SIZE sz)
{
	return SetWindowPos(NULL, 0, 0, sz.cx, sz.cy,	SWP_NOZORDER | SWP_NOMOVE);
}

HWND _Window::GetParent() const
{
	return ::GetParent(m_hWnd);
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
	return ::GetWindowRect(m_hWnd, pRect);
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
	return ::GetClientRect(m_hWnd, pRect);
}

void _Window::DestroyWindow( void )
{	
	::DestroyWindow(GetSafeHwnd());
	m_hWnd = NULL;
}


BOOL _Window::SetWindowText( LPCTSTR pszTitle)
{
	if (IsWindow())
	{
		return ::SetWindowText(GetSafeHwnd(), pszTitle);
	}
	return FALSE;
}

BOOL _Window::EnableWindow( BOOL bEnable )
{
	if (IsWindow())
	{
		return ::EnableWindow(GetSafeHwnd(), bEnable);
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