
#include "stdafx.h"
#include "Dialog.h"
#include "Ext_type.h"
#include <windows.h>
#include <HWDebug.h>

CXUE_Dialog::CXUE_Dialog()
{
	m_bModalless = FALSE;
}

CXUE_Dialog::~CXUE_Dialog()
{
	m_bModalless = FALSE;
}


BOOL CALLBACK CXUE_Dialog::DialogProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{	
	CXUE_Dialog *pDialog = NULL; 

	if (WM_INITDIALOG == message)
	{		
		pDialog = (CXUE_Dialog *)lparam;
		SetWindowLong(hwnd, DWL_USER, (LONG)pDialog);
	}
	else 
	{
		pDialog = (CXUE_Dialog *) GetWindowLong(hwnd, DWL_USER); 
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

LRESULT CXUE_Dialog::_DialogProc(HWND hWnd, UINT  message, WPARAM  wParam, LPARAM  lParam)
{
	switch (message)
	{
	case  WM_INITDIALOG:
		{
			m_hWnd = hWnd;
			return OnInitDialog(hWnd, message, wParam, lParam);		
		}
		break;
	case WM_DESTROY:
		{
			return OnDestroy(hWnd, message, wParam, lParam);
		}
		break;	
	case  WM_COMMAND:
		{
			return OnCommand(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		{
			return OnSize(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ERASEBKGND:
		{
			return OnEraseBackground(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SETCURSOR:
		{
			return OnSetCursor(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOVE:
		{
			return OnMove(hWnd, message,wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			return OnLButtonDown(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			return OnLButtonDbClk(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		{
			return OnMouseMove(hWnd, message, wParam, lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			return OnLButtonUp(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SHOWWINDOW:
		{
			return OnShowWindow(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		{
			return OnTimer(hWnd, message, wParam, lParam);
		}
		break;
	case WM_HSCROLL:
		{
			return OnHScroll(hWnd, message, wParam, lParam);
		}
		break;
	case WM_VSCROLL:
		{
			return OnVScroll(hWnd, message, wParam, lParam);
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

LRESULT CXUE_Dialog::OnInitDialog( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);;
}

LRESULT CXUE_Dialog::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

LRESULT CXUE_Dialog::OnDestroy(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

LRESULT CXUE_Dialog::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;	
}

LRESULT CXUE_Dialog::OnEraseBackground(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnLButtonDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CXUE_Dialog::OnLButtonDbClk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnLButtonUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnShowWindow(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CXUE_Dialog::OnSetCursor(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

LRESULT CXUE_Dialog::OnTimer(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CXUE_Dialog::OnHScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CXUE_Dialog::OnVScroll(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
BOOL CXUE_Dialog::CheckDlgButton( UINT nID, UINT nCheck)
{
	if (IsWindow())
	{
		return ::CheckDlgButton(GetSafeHwnd(), nID, nCheck);
	}
	return FALSE;
}

UINT CXUE_Dialog::IsDlgButtonChecked( UINT nID)
{
	if (IsWindow())
	{
		return ::IsDlgButtonChecked(GetSafeHwnd(), nID);
	}
	return BST_UNCHECKED;
}

int CXUE_Dialog::GetItemWindowText(UINT nID, LPTSTR pszStr, int nMax)
{
	HWND hItem = GetDlgItem(nID);
	if (hItem)
	{
		return GetWindowText(hItem, pszStr, nMax);
	}
	return -1;
}

BOOL CXUE_Dialog::SetItemText(UINT nID, LPCTSTR pszStr)
{
	HWND hCtrl = GetDlgItem(nID);
	if (hCtrl)
	{
		return ::SetWindowText(hCtrl, pszStr);
	}
	return FALSE;
}


BOOL CXUE_Dialog::EnableItemWindow( UINT nID, BOOL bEnable)
{
	HWND hCtrl = ::GetDlgItem(GetSafeHwnd(), nID);
	if (hCtrl)
	{
		return ::EnableWindow(hCtrl, bEnable);
	}
	return FALSE;
}

HWND CXUE_Dialog::GetDlgItem( UINT nID)
{
	if (IsWindow())
	{
		return GetDlgItem(GetSafeHwnd(), nID);
	}
	return NULL;
}

HWND CXUE_Dialog::GetDlgItem(HWND hDlg, UINT nID)
{
	return ::GetDlgItem(hDlg, nID);
}
LRESULT CXUE_Dialog::DestroyWindow()
{
	if (IsWindow())
	{
		if (m_bModalless)
		{
			return ::DestroyWindow(GetSafeHwnd()) ? ERROR_SUCCESS : ERROR_INVALID_DATA;
		}
		else
		{
			return ::EndDialog(GetSafeHwnd(), 0)  ? ERROR_SUCCESS : ERROR_INVALID_DATA;
		}
	}	
	return ERROR_INVALID_DATA;
}

LRESULT CXUE_Dialog::CreateModalDialog(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent)
{
	if (!lpTemplate)
	{
		return ERROR_INVALID_PARAMETER;
	}
	if (IsWindow())
	{
		return ERROR_SUCCESS;
	}
	m_hInstance = hInstance;
	m_bModalless = FALSE;
	m_hWndCreator = hWndParent;
	return  DialogBoxParam(hInstance, lpTemplate, hWndParent, (DLGPROC)DialogProc, (LONG)this);	
}

LRESULT CXUE_Dialog::CreateModalDialog(HINSTANCE hInstance, UINT nTemplate, HWND hWndParent)
{
	return CreateModalDialog(hInstance, MAKEINTRESOURCE(nTemplate), hWndParent);
}
LRESULT CXUE_Dialog::CreateModallessDialog(HINSTANCE hInstance, LPCTSTR lpTemplate, HWND hWndParent)
{
	if (!lpTemplate)
	{
		return ERROR_INVALID_PARAMETER;
	}
	if (IsWindow())
	{
		return ERROR_SUCCESS;
	}
	m_hInstance = hInstance;
	m_bModalless = TRUE;
	m_hWndCreator = hWndParent;
	::CreateDialogParam(hInstance, lpTemplate, hWndParent, DialogProc, (LPARAM)this);	
	return IsWindow()? ERROR_SUCCESS : ERROR_INVALID_DATA;
}

LRESULT CXUE_Dialog::CreateModallessDialog( HINSTANCE hInstance, UINT nTemplate, HWND hWndParent)
{
	return CreateModallessDialog(hInstance, MAKEINTRESOURCE(nTemplate), hWndParent);
}
UINT CXUE_Dialog::GetDlgItemText( int nIDDlgItem, LPTSTR lpString, int nMaxCount )
{	
	return GetDlgItemText(GetSafeHwnd(), nIDDlgItem, lpString, nMaxCount);
}
UINT CXUE_Dialog::GetDlgItemText( HWND hDlg, int nIDDlgItem, LPTSTR lpString, int nMaxCount )
{
	return ::GetDlgItemText(hDlg, nIDDlgItem, lpString, nMaxCount);
}
BOOL CXUE_Dialog::SetDlgItemText( int nIDDlgItem, LPCTSTR lpString)
{
	return SetDlgItemText(GetSafeHwnd(), nIDDlgItem, lpString);
}
BOOL CXUE_Dialog::SetDlgItemText( HWND hDlg, int nIDDlgItem, LPCTSTR lpString)
{
	return ::SetDlgItemText(hDlg, nIDDlgItem, lpString);
}

