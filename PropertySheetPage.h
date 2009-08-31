

#ifndef HWX_PROPERTYSHEETPAGE_H
#define HWX_PROPERTYSHEETPAGE_H

#if _MSC_VER > 1000
#pragma once
#endif 
#include "Window.h"
#include <windows.h>
#include <windef.h>
#include "Dialog.h"

class PropertySheetPage
{
protected:		
public:
	PropertySheetPage(HINSTANCE, LPCTSTR);	
	virtual ~PropertySheetPage();	
public:		
	HPROPSHEETPAGE  Create();
	const PROPSHEETPAGE& GetPSP() const;
	virtual LRESULT DestroyWindow();
	HWND GetSafeHwnd() const;
	HWND GetDlgItem( UINT);
protected:	
	virtual LRESULT _DialogProc(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnCommand(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnInitDialog(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnDestroy(HWND, UINT, WPARAM, LPARAM);
	static BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
	/*
	virtual BOOL OnApply();
		virtual void OnReset();
		virtual void OnOK();
		virtual void OnCancel();
		virtual BOOL OnSetActive();
		virtual BOOL OnKillActive();
		virtual BOOL OnQueryCancel();
	
		virtual LRESULT OnWizardBack();
		virtual LRESULT OnWizardNext();
		virtual BOOL OnWizardFinish();	
		virtual HWND OnWizardFinishEx();*/
	HPROPSHEETPAGE Construct();

protected:
	HINSTANCE m_hInstance;
	HWND	m_hWnd;
public:
	PROPSHEETPAGE m_psp;	

};

#endif //HWX_PROPERTYSHEETPAGE_H
