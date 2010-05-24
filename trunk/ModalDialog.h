

#ifndef HWX_HWDIALOG_H
#define HWX_HWDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Window.h"
#include <Dialog.h>
#include <windef.h>
class ModalDialog  : public CXUE_Dialog
{
protected:	
	ModalDialog();	
public:
	virtual ~ModalDialog();	
public:
	virtual LRESULT Create(HINSTANCE, LPCTSTR, HWND);	
	virtual LRESULT DestroyWindow();
	
protected:	
	virtual LRESULT _DialogProc(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnInitDialog(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnClose(HWND, UINT, WPARAM, LPARAM);	
	virtual LRESULT OnActivate(HWND, UINT, WPARAM, LPARAM);
	virtual LRESULT OnSettingChange(HWND, UINT, WPARAM, LPARAM);
	
protected:	
};

#endif //HWX_HWDIALOG_H
