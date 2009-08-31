
#include "stdafx.h"
#include "Dialog.h"
#include "Ext_type.h"
#include <windows.h>
#include <HWDebug.h>

_Dialog::_Dialog()
{

}

_Dialog::~_Dialog()
{

}


BOOL CALLBACK _Dialog::DialogProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{	
	_Dialog *pDialog = NULL; 

	if (WM_INITDIALOG == message)
	{		
		pDialog = (_Dialog *)lparam;
		SetWindowLong(hwnd, DWL_USER, (LONG)pDialog);
	}
	else 
	{
		pDialog = (_Dialog *) GetWindowLong(hwnd, DWL_USER); 
	}

	if (!pDialog)
	{
		return 0;//DefWindowProc(hwnd, message, wparam, lparam);
	}
	else 
	{
		return pDialog->_DialogProc(hwnd, message, wparam, lparam);
	}
}

LRESULT _Dialog::_DialogProc(HWND hDlg, UINT  message, WPARAM  wParam, LPARAM  lParam)
{
	switch (message)
	{
	case  WM_INITDIALOG:
		{
			m_hWnd = hDlg;
			return OnInitDialog(hDlg, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		{
			return OnDestroy(hDlg, message, wParam, lParam);
		}
		break;	
	case  WM_COMMAND:
		{
			return OnCommand(hDlg, message, wParam, lParam);
		}
		break;

	default:
		{			
			return 0;//DefWindowProc(hDlg, message, wParam, lParam);
		}
		break;
	}
	return FALSE;
}

LRESULT _Dialog::OnInitDialog( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);;
}



LRESULT _Dialog::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

LRESULT _Dialog::OnDestroy(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

BOOL _Dialog::CheckDlgButton( UINT nID, UINT nCheck)
{
	if (IsWindow())
	{
		return ::CheckDlgButton(GetSafeHwnd(), nID, nCheck);
	}
	return FALSE;
}

UINT _Dialog::IsDlgButtonChecked( UINT nID)
{
	if (IsWindow())
	{
		return ::IsDlgButtonChecked(GetSafeHwnd(), nID);
	}
	return BST_UNCHECKED;
}

int _Dialog::GetItemWindowText(UINT nID, LPTSTR pszStr, int nMax)
{
	HWND hItem = GetDlgItem(nID);
	if (hItem)
	{
		return GetWindowText(hItem, pszStr, nMax);
	}
	return -1;
}

BOOL _Dialog::SetItemText(UINT nID, LPCTSTR pszStr)
{
	HWND hCtrl = GetDlgItem(nID);
	if (hCtrl)
	{
		return ::SetWindowText(hCtrl, pszStr);
	}
	return FALSE;
}


BOOL _Dialog::EnableItemWindow( UINT nID, BOOL bEnable)
{
	HWND hCtrl = ::GetDlgItem(GetSafeHwnd(), nID);
	if (hCtrl)
	{
		return ::EnableWindow(hCtrl, bEnable);
	}
	return FALSE;
}

HWND _Dialog::GetDlgItem( UINT nID)
{
	if (IsWindow())
	{
		return ::GetDlgItem(GetSafeHwnd(), nID);
	}
	return NULL;
}

LRESULT _Dialog::DestroyWindow()
{
	return ERROR_SUCCESS;
}

BOOL _Dialog::HWCreateDialog(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent)
{
	if (!lpTemplate)
	{
		return FALSE;
	}
	return !!::CreateDialogParam(hInstance, lpTemplate, hWndParent, DialogProc, (LPARAM)this);
}



