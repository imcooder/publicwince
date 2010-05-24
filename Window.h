

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
	//
	BOOL IsVisible() const;
	static BOOL IsVisible(HWND);
	//
	BOOL IsWindow() const;	
	static BOOL IsWindow(HWND);
	//
	virtual BOOL ShowWindow(DWORD);
	static BOOL ShowWindow(HWND, DWORD);
	//
	virtual BOOL SetWindowPos(HWND, int, int, int,int,UINT);
	static BOOL SetWindowPos(HWND, HWND, int, int, int,int,UINT);
	//
	BOOL InvalidateRect(CONST RECT*,BOOL);
	virtual BOOL UpdateWindow();	
	//
	virtual BOOL	MoveWindow(	int,int,int,int,BOOL);
	static BOOL MoveWindow(HWND, int,int,int,int,BOOL);
	//
	BOOL ResizeWindow(SIZE);
	HWND GetParent() const;
	static HWND GetParent(HWND);
	HWND GetSafeHwnd() const;
	//
	HWND GetOwner() const;
	static HWND GetOwner(HWND);
	//
	BOOL GetWindowRect(RECT*) const;
	static BOOL GetWindowRect(HWND, RECT*);
	//
	BOOL GetClientRect(RECT*) const;
	static BOOL GetClientRect(HWND, RECT*);
	//
	BOOL EnableWindow(BOOL);
	static BOOL EnableWindow(HWND, BOOL);
	//
	BOOL EnableItemWindow(UINT, BOOL);
	HINSTANCE GetInstance() const;
	//////////////////////////////////////////////////////////////////////////
	BOOL SetWindowText( LPCTSTR);	
	static BOOL SetWindowText(HWND, LPCTSTR);
	//////////////////////////////////////////////////////////////////////////
	BOOL ClientToScreen(LPRECT);	
	static BOOL ClientToScreen(HWND, LPRECT);
	// 
	BOOL ScreenToClient(LPRECT);
	static BOOL ScreenToClient(HWND, LPRECT);
	//
	LONG SetID(LONG);
	static LONG SetID(HWND, LONG);
	//
	LONG GetID() const;
	static LONG GetID(HWND);
	//
	HWND GetCreator() const;
	//////////////////////////////////////////////////////////////////////////	
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	//////////////////////////////////////////////////////////////////////////
	LRESULT SendMessage( UINT,	WPARAM, LPARAM);	
	static LRESULT SendMessage(HWND, UINT,	WPARAM, LPARAM);
	//
	BOOL PostMessage(UINT, WPARAM,LPARAM);
	static BOOL PostMessage(HWND, UINT, WPARAM,LPARAM);
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
protected:
	_Window();	
protected:
	virtual LRESULT OnCommand(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnInitDialog(HWND, UINT, WPARAM, LPARAM) = 0;	
	virtual LRESULT OnSize(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnMove(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnDestroy(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnEraseBackground(HWND, UINT, WPARAM, LPARAM) = 0;	
	virtual LRESULT OnSetCursor(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnLButtonDown(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnLButtonDbClk(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnLButtonUp(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnMouseMove(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnShowWindow(HWND, UINT, WPARAM, LPARAM) = 0;	
	virtual LRESULT OnTimer(HWND, UINT, WPARAM, LPARAM) = 0;	
	virtual LRESULT OnHScroll(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnVScroll(HWND, UINT, WPARAM, LPARAM) = 0;
	//////////////////////////////////////////////////////////////////////////	
protected:
	HWND m_hWnd;	
	HINSTANCE m_hInstance;	
	HWND m_hWndCreator;
};

#endif //HWX_WINDOW_H
