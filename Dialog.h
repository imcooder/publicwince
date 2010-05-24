

#ifndef HWX_DIALOG_H
#define HWX_DIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif 
#include "Window.h"
#include <windows.h>
#include <windef.h>
class CXUE_Dialog  : public _Window
{
protected:	
	CXUE_Dialog();	
public:
	virtual ~CXUE_Dialog();	
public:	
	virtual LRESULT DestroyWindow();
	BOOL CheckDlgButton(UINT, UINT);
	UINT IsDlgButtonChecked(UINT);
	int GetItemWindowText(LPTSTR, int); 
	BOOL EnableItemWindow( UINT, BOOL);
	BOOL SetItemText(UINT, LPCTSTR);
	int  GetItemWindowText(UINT, LPTSTR, int);	
	//
	HWND GetDlgItem( UINT);
	static HWND GetDlgItem(HWND, UINT);
	//	
	BOOL SetDlgItemText(int nIDDlgItem, LPCTSTR);
	static BOOL SetDlgItemText(HWND, int nIDDlgItem, LPCTSTR); 
	UINT GetDlgItemText(int nIDDlgItem, LPTSTR, int); 
	static UINT GetDlgItemText( HWND hDlg, int nIDDlgItem, LPTSTR, int); 
	//
	//////////////////////////////////////////////////////////////////////////
	virtual LRESULT CreateModalDialog(HINSTANCE, LPCTSTR, HWND);	
	virtual LRESULT CreateModalDialog(HINSTANCE, UINT, HWND);	
	virtual LRESULT CreateModallessDialog(HINSTANCE, LPCTSTR, HWND);	
	virtual LRESULT CreateModallessDialog(HINSTANCE, UINT, HWND);
	//////////////////////////////////////////////////////////////////////////
	
protected:	
	virtual LRESULT _DialogProc(HWND, UINT, WPARAM, LPARAM);
	//////////////////////////////////////////////////////////////////////////
	virtual LRESULT OnCommand(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnInitDialog(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnSize(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnMove(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnDestroy(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnEraseBackground(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnSetCursor(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnLButtonDown(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnLButtonDbClk(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnLButtonUp(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnMouseMove(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnShowWindow(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnTimer(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnHScroll(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnVScroll(HWND, UINT, WPARAM, LPARAM);
protected:
	static BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
protected:	
	BOOL	m_bModalless;
};

#endif //HWX_HWDIALOG_H
