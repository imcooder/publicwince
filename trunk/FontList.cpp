

#include "stdafx.h"
#include "FontList.h"
#include <algorithm>

BOOL LogfontCmp (LPLOGFONT pFontA, LPLOGFONT pFontB )
{
	if (!pFontA || !pFontB)
	{
		return FALSE;
	}
	return 0 >= _tcsicmp(pFontA->lfFaceName, pFontB->lfFaceName);
}


CXUE_FontList::CXUE_FontList()
{	

}

CXUE_FontList::~CXUE_FontList()
{
	FreeFonts();
}

BOOL CALLBACK CXUE_FontList::EnumFontFamExProc(ENUMLOGFONTEX* pelf,NEWTEXTMETRICEX* lpntm,  DWORD dwFontType, LPARAM lParam)
{
  CXUE_FontList* pFontEnum = (CXUE_FontList*)lParam;
  if (pFontEnum)
  {
    if (pFontEnum->AddFont(&pelf->elfLogFont, dwFontType))
    {
      
    }
  }  
  return TRUE;
}


void CXUE_FontList::Create(BYTE nCharSet)
{
	FreeFonts(); 
	LOGFONT logfont;
	logfont.lfFaceName[0] = 0;
	logfont.lfCharSet = nCharSet;
	HDC hDC = GetDC(NULL);
	::EnumFontFamiliesEx(hDC, &logfont, (FONTENUMPROC)EnumFontFamExProc, (LPARAM)this, 0); 
	ReleaseDC(NULL, hDC);
	hDC = NULL;
	m_listpFonts.sort(LogfontCmp);	
	//std::sort(m_listpFonts.begin(), m_listpFonts.end()/*, LogfontCmp*/);
}


BOOL CXUE_FontList::AddFont(const LOGFONT* pLF, DWORD dwType)
{    
  if (!pLF)
  {
		return FALSE;
  }
  if (DoesFontExist(pLF->lfFaceName))
  {
    return FALSE;
  } 
	LPLOGFONT pLogfont = new LOGFONT;
	if (pLogfont)
	{
		memcpy(pLogfont, pLF, sizeof(*pLF));
	}
	
  m_listpFonts.push_back(pLogfont);
  
  return TRUE;
}

BOOL CXUE_FontList::DoesFontExist(LPCTSTR pszFaceName)
{	
	if (!pszFaceName)
	{
		return FALSE;
	}
	LPLOGFONT pLogfont = GetLogFont(pszFaceName);
	return !!pLogfont;
}

LPLOGFONT CXUE_FontList::GetLogFont(LPCTSTR pszFaceName)
{
	std::list<LPLOGFONT>::iterator itlstTravel = m_listpFonts.begin();
  while(itlstTravel != m_listpFonts.end())
  {
    if (0 == _tcsicmp(pszFaceName, (*itlstTravel)->lfFaceName))
    {
			break;      
    }
    itlstTravel ++;
  }		
	if (itlstTravel != m_listpFonts.end())
	{
		return *itlstTravel;
	}
  return NULL;
}

void CXUE_FontList::FreeFonts()
{
	std::list<LPLOGFONT>::iterator itlstTravel = m_listpFonts.begin();
	while (itlstTravel != m_listpFonts.end())
	{
		SAFE_DELETE(*itlstTravel);
		itlstTravel ++;
	}		
	m_listpFonts.clear();
}

UINT CXUE_FontList::GetCount() const
{
	return m_listpFonts.size();
}

LPLOGFONT CXUE_FontList::GetFontByIndex( UINT nIdx)
{
	std::list<LPLOGFONT>::iterator itlstTravel = m_listpFonts.begin();
	while (itlstTravel != m_listpFonts.end())
	{		
		if (nIdx <= 0)
		{
			break;
		}
		itlstTravel ++;
		nIdx --;
	}		
	if (itlstTravel != m_listpFonts.end())
	{
		return *itlstTravel;
	}
	return NULL;
}