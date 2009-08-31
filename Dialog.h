

#ifndef HWX_DIALOG_H
#define HWX_DIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif 
#include "Window.h"
#include <windows.h>
#include <windef.h>
class _Dialog  : public _Window
{
protected:	
	_Dialog();	
public:
	virtual ~_Dialog();	
public:	
	virtual LRESULT DestroyWindow();
	BOOL CheckDlgButton(UINT, UINT);
	UINT IsDlgButtonChecked(UINT);
	int GetItemWindowText(LPTSTR, int); 
	BOOL EnableItemWindow( UINT, BOOL);
	BOOL SetItemText(UINT, LPCTSTR);
	int  GetItemWindowText(UINT, LPTSTR, int);
	HWND GetDlgItem( UINT);
	BOOL HWCreateDialog(HINSTANCE, LPCTSTR, HWND);
	//////////////////////////////////////////////////////////////////////////
	
protected:	
	virtual LRESULT _DialogProc(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnCommand(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnInitDialog(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnDestroy(HWND, UINT, WPARAM, LPARAM);
protected:
	static BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
protected:	
};

#endif //HWX_HWDIALOG_H
