
#include "stdafx.h"
#include "PropertySheetPage.h"
#include "Ext_type.h"
#include <windows.h>
#include <HWDebug.h>


_PropertySheet::_PropertySheet()
{	
	ZeroMemory(&m_psh, sizeof(m_psh));
	m_psh.dwSize = sizeof(m_psh);

}

_PropertySheet::~_PropertySheet()
{

}

LRESULT _PropertySheet::_DialogProc(HWND hDlg, UINT  message, WPARAM  wParam, LPARAM  lParam)
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

LRESULT _PropertySheet::OnInitDialog( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);;
}



LRESULT _PropertySheet::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

LRESULT _PropertySheet::OnDestroy(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);
}

HPROPSHEETPAGE _PropertySheet::CreatePropertySheetPage( HINSTANCE hInstance, LPCTSTR lpName, HWND hParent)
{
	m_hInstance = hInstance;

	BOOL bReturn = FALSE;

	PROPSHEETHEADER psh;
	PROPSHEETPAGE   ppsp;
	HANDLE          hFile = NULL;

	
	HPROPSHEETPAGE  hpsp[2];
	INT nPages = 2;

	// Set all values for first property page
	ppsp.dwSize = sizeof(PROPSHEETPAGE)+sizeof(HWND);
	// Extra space at end of struct to tuck page's hwnd in when it's created
	ppsp.dwFlags = PSP_DEFAULT|PSP_USETITLE|PSP_USECALLBACK;
	ppsp.hInstance = g_hInstance;
	ppsp.pfnCallback = PropSheetPageProc;
	ppsp.pcRefParent = NULL;
	ppsp.hIcon = NULL;

	if (!g_AddWordListManage->m_ManageDic.IfAddPinYin())
	{
		ppsp.pszTitle = INIData_Content[0].ptcReturnedString;
		ppsp.pfnDlgProc = DicChinesePageProc;
		ppsp.lParam = (LONG)ID_BATT;
		ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_DIC_CHINESE);
	}
	else
	{
		ppsp.pszTitle = INIData_Content[2].ptcReturnedString;
		ppsp.pfnDlgProc = DicPYManagePageProc;
		ppsp.lParam = (LONG)ID_BATT;
		ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_DIC_PINYIN_MANAGE);
	}

	hpsp[0] = CreatePropertySheetPage(&ppsp);
	if (NULL == hpsp[0]) {
		//Fail out of function
		return bReturn;
	}

	// Set all values for second property page

	if (!g_AddWordListManage->m_ManageDic.IfAddPinYin())
	{
		ppsp.pszTitle = INIData_Content[1].ptcReturnedString;
		ppsp.pfnDlgProc = DicEnglishPageProc;//InkPageProc;
		ppsp.lParam = (LONG)ID_AC;
		ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_DIC_ENGLISH);
	}
	else
	{
		ppsp.pszTitle = INIData_Content[3].ptcReturnedString;
		ppsp.pfnDlgProc = DicPYEditPageProc;//InkPageProc;
		ppsp.lParam = (LONG)ID_AC;
		ppsp.pszTemplate = (LPTSTR)MAKEINTRESOURCE(IDD_DIC_PINYIN_EDIT);
	}

	hpsp[1] = CreatePropertySheetPage(&ppsp);
	if (NULL ==hpsp[1])
	{
		//Clean up the page we have created
		DestroyPropertySheetPage(hpsp[0]);
		//Fail out of function
		return bReturn;
	}

	psh.dwSize				= sizeof(psh);
	psh.dwFlags				= PSH_USECALLBACK|PSH_MAXIMIZE;
	psh.hwndParent			= hwndParent;
	psh.hInstance			= g_hInstance;
	psh.pszCaption			= NULL;
	psh.phpage				= hpsp;
	psh.nPages				= nPages;
	psh.nStartPage			= iApplet > (nPages-1)? (nPages-1): iApplet;
	psh.pfnCallback			= PropSheetProc;

	if(-1 != PropertySheet(&psh)) 
	{
		bReturn = TRUE;
	}
	else 
	{
		DestroyPropertySheetPage(hpsp[0]);
		DestroyPropertySheetPage(hpsp[1]);
	}

	return bReturn;
}

const PROPSHEETPAGE& _PropertySheet::GetPSP() const
{
	return m_psp;
}

LRESULT _PropertySheet::DestroyWindow()
{
	EndDialog(GetSafeHwnd(), IDOK);
	return ERROR_SUCCESS;
}