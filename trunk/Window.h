

#ifndef HWX_WINDOW_H
#define HWX_WINDOW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windef.h>
class _Window  
{
public:	
	virtual ~_Window();
	void DestroyWindow(void);
	BOOL IsVisible() const;
	BOOL IsWindow() const;
	BOOL PostMessage(UINT, WPARAM,LPARAM);
	LRESULT SendMessage( UINT,	WPARAM, LPARAM);	
	virtual BOOL ShowWindow(DWORD);
	virtual BOOL SetWindowPos(HWND, int, int, int,int,UINT);
	BOOL InvalidateRect(CONST RECT*,BOOL);
	virtual BOOL UpdateWindow();	
	virtual BOOL	MoveWindow(	int,int,int,int,BOOL);
	BOOL ResizeWindow(SIZE);
	HWND GetParent() const;
	HWND GetSafeHwnd() const;
	BOOL GetWindowRect(RECT*) const;
	BOOL GetClientRect(RECT*) const;
	BOOL EnableWindow(BOOL);
	BOOL EnableItemWindow(UINT, BOOL);
	HINSTANCE GetInstance() const;
	//////////////////////////////////////////////////////////////////////////
	BOOL SetWindowText( LPCTSTR);	
	static BOOL ClientToScreen(HWND, LPRECT);

	//////////////////////////////////////////////////////////////////////////	
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	//////////////////////////////////////////////////////////////////////////
	static LRESULT SendMessage(HWND, UINT,	WPARAM, LPARAM);
	static BOOL PostMessage(HWND, UINT, WPARAM,LPARAM);
	//////////////////////////////////////////////////////////////////////////
protected:
	_Window();	
	//////////////////////////////////////////////////////////////////////////	
protected:
	HWND m_hWnd;	
	HINSTANCE m_hInstance;	
};

#endif //HWX_WINDOW_H
