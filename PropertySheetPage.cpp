
#include "stdafx.h"
#include "PropertySheetPage.h"
#include "Ext_type.h"
#include <windows.h>
#include <HWDebug.h>


PropertySheetPage::PropertySheetPage(HINSTANCE hInstance, LPCTSTR lpName)
{	
	m_hInstance = hInstance;

	ZeroMemory(&m_psp, sizeof(m_psp));
	m_psp.dwSize = sizeof(m_psp);
		
	m_psp.dwFlags = PSP_DEFAULT|PSP_USETITLE|PSP_USECALLBACK;
	m_psp.hInstance = m_hInstance;	
	m_psp.pfnDlgProc = DialogProc;
	m_psp.pcRefParent = NULL;
	m_psp.hIcon = NULL;
	m_psp.pszTemplate = lpName;
	m_psp.lParam = (LONG)this;
	m_psp.pszTitle = NULL;	
}

PropertySheetPage::~PropertySheetPage()
{

}

BOOL CALLBACK PropertySheetPage::DialogProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	PropertySheetPage *pPage = NULL; 
	DWORD dwData = GetWindowLong(hwnd, DWL_USER); 
	if (WM_INITDIALOG == message)
	{		
		LPPROPSHEETPAGE pProperty = (LPPROPSHEETPAGE)lparam;	
		pPage = (PropertySheetPage*)pProperty->lParam;
		SetWindowLong(hwnd, DWL_USER, (LONG)pPage);
	}
	else 
	{
		pPage = (PropertySheetPage *) GetWindowLong(hwnd, DWL_USER); 
	}

	if (!pPage)
	{
		return 0;//DefWindowProc(hwnd, message, wparam, lparam);
	}
	else 
	{
		return pPage->_DialogProc(hwnd, message, wparam, lparam);
	}
}
LRESULT PropertySheetPage::_DialogProc(HWND hDlg, UINT  message, WPARAM  wParam, LPARAM  lParam)
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

LRESULT PropertySheetPage::OnInitDialog( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_hWnd = hDlg;
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);;
}



LRESULT PropertySheetPage::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

LRESULT PropertySheetPage::OnDestroy(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

const PROPSHEETPAGE& PropertySheetPage::GetPSP() const
{
	return m_psp;
}

LRESULT PropertySheetPage::DestroyWindow()
{
	EndDialog(GetSafeHwnd(), IDOK);
	return ERROR_SUCCESS;
}

HPROPSHEETPAGE  PropertySheetPage::Construct()
{
	return ::CreatePropertySheetPage(&m_psp);
}

HPROPSHEETPAGE  PropertySheetPage::Create()
{
	return Construct();
}

HWND PropertySheetPage::GetSafeHwnd() const
{
	return m_hWnd;
}

HWND PropertySheetPage::GetDlgItem( UINT nID)
{
	if (IsWindow(GetSafeHwnd()))
	{
		return ::GetDlgItem(GetSafeHwnd(), nID);
	}
	return NULL;
}