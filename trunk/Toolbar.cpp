#include "stdafx.h"
#include "Toolbar.h"


BOOL WINAPI XUE_IsToolbar( HWND hToolbar)
{
	BOOL blRet = FALSE;
	if (IsWindow(hToolbar))
	{
		TBBUTTONINFO toolbarInfo;
		ZeroMemory(&toolbarInfo, sizeof(toolbarInfo));
		toolbarInfo.cbSize = sizeof(toolbarInfo);
		toolbarInfo.dwMask = TBIF_COMMAND | TBIF_BYINDEX;		
		if (-1 != SendMessage(hToolbar, TB_GETBUTTONINFO, 0, (LPARAM)&toolbarInfo))
		{
			blRet = TRUE;
		}	
	}	
	return blRet;
}

LONG WINAPI XUE_ToolbarIndexToCommand( HWND hToolbar, LONG nIndex)
{
	LONG nRet = 0;
	if (IsWindow(hToolbar))
	{
		TBBUTTONINFO toolbarInfo;
		ZeroMemory(&toolbarInfo, sizeof(toolbarInfo));
		toolbarInfo.cbSize = sizeof(toolbarInfo);
		toolbarInfo.dwMask = TBIF_COMMAND | TBIF_BYINDEX;		
		if (-1 != SendMessage(hToolbar, TB_GETBUTTONINFO, nIndex, (LPARAM)&toolbarInfo))
		{
			nRet = toolbarInfo.idCommand;
		}	
	}	
	return nRet;
}
LONG WINAPI XUE_ToolbarCommandToIndex( HWND hToolbar, LONG nCmd)
{
	LONG nRet = 0;
	if (IsWindow(hToolbar))
	{
		nRet = SendMessage(hToolbar, TB_COMMANDTOINDEX, nCmd, 0);		
	}	
	return nRet;
}
BOOL WINAPI XUE_ToolbarHideButton( HWND hToolbar, LONG nIndexOrCmd, BOOL blByIndex /*= TRUE*/, BOOL blHide /*= TRUE*/ )
{
	BOOL blRet = FALSE;	
	if (IsWindow(hToolbar))
	{
		LONG nCmd = 0;
		if(blByIndex)
		{
			nCmd = XUE_ToolbarIndexToCommand(hToolbar, nIndexOrCmd);
		}
		else
		{
			nCmd = nIndexOrCmd;
		}
		if(nCmd)
		{
			if (SendMessage(hToolbar, TB_HIDEBUTTON, nCmd, blHide))
			{
				blRet = TRUE;
			}		
		}					
	}
	return blRet;
}

BOOL WINAPI XUE_ToolbarSetButtonText( HWND hToolbar, LONG nIndexOrCmd, BOOL blByIndex, LPCTSTR pszText)
{
	BOOL blRet = FALSE;	
	TCHAR szText[128] = {0};	
	if (IsWindow(hToolbar) && pszText)
	{
		StringCchCopy(szText, _countof(szText), pszText);
		TBBUTTONINFO toolbarInfo;
		ZeroMemory(&toolbarInfo, sizeof(toolbarInfo));
		toolbarInfo.cbSize = sizeof(toolbarInfo);
		toolbarInfo.dwMask = TBIF_TEXT;
		toolbarInfo.pszText = szText;

		if(blByIndex)
		{
			toolbarInfo.dwMask |= TBIF_BYINDEX;
		}	
		if (SendMessage(hToolbar, TB_SETBUTTONINFO, nIndexOrCmd, (LPARAM)&toolbarInfo))
		{
			blRet = TRUE;
		}		
	}
	return blRet;
}


BOOL XUE_ToolbarSetButtonCommand( HWND hToolbar, LONG nIndex, LONG nCmd)
{
	BOOL blRet = FALSE;
	if (nCmd)
	{
		TBBUTTONINFO toolbarInfo;
		ZeroMemory(&toolbarInfo, sizeof(toolbarInfo));
		toolbarInfo.cbSize = sizeof(toolbarInfo);
		toolbarInfo.dwMask = TBIF_COMMAND  | TBIF_BYINDEX;
		toolbarInfo.idCommand = nCmd;
		if (-1 != SendMessage(hToolbar, TB_SETBUTTONINFO, nIndex, (LPARAM)&toolbarInfo))
		{
			blRet = TRUE;
		}		
	}	
	return blRet;
}
