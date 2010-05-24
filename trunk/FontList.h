

#pragma once

#include <windef.h>
#include <Windows.h>

#include <list>

class CXUE_FontList 
{
public:
	CXUE_FontList();
	virtual ~CXUE_FontList();
	void Create(BYTE = DEFAULT_CHARSET);
	UINT GetCount() const;
	LPLOGFONT GetFontByIndex(UINT);
	LPLOGFONT GetLogFont(LPCTSTR);
public:		
protected:	
  static BOOL CALLBACK EnumFontFamExProc(ENUMLOGFONTEX*,   NEWTEXTMETRICEX*,	DWORD, LPARAM);  
  BOOL AddFont(const LOGFONT* pLF, DWORD dwType);  
  BOOL DoesFontExist(LPCTSTR);  
	void FreeFonts();
protected:	
	std::list<LPLOGFONT> m_listpFonts;		
};

//////////////////////////////////////////////////////////////////////

