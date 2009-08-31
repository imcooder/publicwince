
#include "stdafx.h"
#include "Ext_type.h"
#include <windows.h>
#include "ModalDialog.h"
#include <HWDebug.h>


ModalDialog::ModalDialog()
{

}
ModalDialog::~ModalDialog()
{

}

LRESULT ModalDialog::Create( HINSTANCE hInstance, LPCTSTR lpName, HWND hWndParent)
{
	m_hInstance = hInstance;
	return  DialogBoxParam(hInstance, lpName, hWndParent, (DLGPROC)DialogProc, (LONG)this);		
}

LRESULT ModalDialog::_DialogProc(HWND hDlg, UINT  message, WPARAM  wParam, LPARAM  lParam)
{
	switch (message)
	{	
	case WM_CLOSE:
		{
			return OnClose(hDlg, message, wParam, lParam);
		}
	default:
		{			
			return __super::_DialogProc(hDlg, message, wParam, lParam);
		}
		break;
	}
	return FALSE;
}


LRESULT ModalDialog::OnClose( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	EndDialog(hDlg, LOWORD(wParam));
	return 0;//DefWindowProc(hDlg, message, wParam, lParam);;
}

LRESULT ModalDialog::DestroyWindow()
{
	if (IsWindow())
	{
		return EndDialog(GetSafeHwnd(), 0);
	}
	return FALSE;
}

