/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDC.cpp					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/

#include "stdafx.h"
#include "HDCEx.h"

#define COLORREF_NULL   (COLORREF)-1


CXUE_BitmapLock::CXUE_BitmapLock(HDC hDC, HBITMAP hBitmap)
: m_hDC(hDC)
{
	m_hOldBitmap = SelectObject(m_hDC, hBitmap);
}

CXUE_BitmapLock::~CXUE_BitmapLock()
{
	if (m_hOldBitmap)
	{
		::SelectObject(m_hDC, m_hOldBitmap);
		m_hOldBitmap = NULL;
	}	
}

void CXUE_BitmapLock::SetBitmap(HBITMAP hBitmap)
{
	if (m_hOldBitmap)
	{
		::SelectObject(m_hDC, m_hOldBitmap);
		m_hOldBitmap = NULL;
	}
	assert(m_hDC);
	if (hBitmap)
	{
		m_hOldBitmap = (HBITMAP)::SelectObject(m_hDC, hBitmap);
	}	
}

CXUE_FontLock::CXUE_FontLock(HDC hDC, HFONT hFont)
: m_hDC(hDC)
, m_hFont(NULL)
, m_hFontOld(NULL)
, m_crOldTextColor(COLORREF_NULL)
{
	assert(m_hDC);	
	if (hFont)
	{
		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);		
	}
}

CXUE_FontLock::CXUE_FontLock(HDC hDC, HFONT hFont, COLORREF clrTextColor)
: m_hDC(hDC)
, m_hFont(NULL)
, m_hFontOld(NULL)
, m_crOldTextColor(COLORREF_NULL)
{
	assert(m_hDC);
	if (hFont)
	{	
		m_hFontOld = (HFONT)::SelectObject(m_hDC, hFont);		
	}
	if (COLORREF_NULL != clrTextColor)
	{
		m_crOldTextColor = ::SetTextColor(m_hDC, clrTextColor);	
	}	
}

CXUE_FontLock::~CXUE_FontLock()
{
	if (m_hFontOld)
	{
		SelectObject(m_hDC, m_hFontOld);
		m_hFontOld = NULL;
	}
	if (m_hFont)
	{
		::DeleteObject(m_hFont);
		m_hFont = NULL;
	}
	if (COLORREF_NULL != m_crOldTextColor)
	{
		::SetTextColor(m_hDC, m_crOldTextColor);
		m_crOldTextColor = COLORREF_NULL;
	}
}

void CXUE_FontLock::SetColor(COLORREF clrTextColor)
{
	assert(clrTextColor != COLORREF_NULL);
	assert(m_hDC);

	if (m_hDC && clrTextColor != COLORREF_NULL)
	{
		if (m_crOldTextColor != COLORREF_NULL)
		{
			::SetTextColor(m_hDC, m_crOldTextColor);
			m_crOldTextColor = COLORREF_NULL;
		}
		m_crOldTextColor = ::SetTextColor(m_hDC, clrTextColor);	
	}
}

void CXUE_FontLock::ReplaceCurrentFont( const LOGFONT*  pLogFont)
{
	if (!pLogFont || !m_hDC)
	{
		return;
	}
	HFONT hFontNew = NULL;
	VERIFY(hFontNew = ::CreateFontIndirect (pLogFont));
	if (hFontNew)
	{
		if (m_hFontOld)
		{
			::SelectObject (m_hDC, m_hFontOld);
			m_hFontOld = NULL;
		}

		if (m_hFont)
		{
			::DeleteObject(m_hFont);
			m_hFont = NULL;
		}	
		m_hFont = hFontNew;
		m_hFontOld = (HFONT)::SelectObject (m_hDC, m_hFont);
	}			
	return;
}

void CXUE_FontLock::SetFontFaceName( LPCTSTR pszFaceName)
{
	if (!pszFaceName || !m_hDC)
	{
		return;
	}
	LOGFONT logFont;
	HFONT hFontCurrent = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
	if (!hFontCurrent)
	{
		return;
	}
	GetObject(hFontCurrent, sizeof(logFont), &logFont);
	hFontCurrent = NULL;
	StringCchCopy(logFont.lfFaceName, _countof(logFont.lfFaceName), pszFaceName);	
	ReplaceCurrentFont(&logFont);	
}

void CXUE_FontLock::SetFontSize( LONG nSize)
{
	if (!m_hDC)
	{
		return;
	}
	LOGFONT logFont;
	HFONT hFontCurrent = (HFONT)::GetCurrentObject(m_hDC, OBJ_FONT);
	if (!hFontCurrent)
	{
		return;
	}
	GetObject(hFontCurrent, sizeof(logFont), &logFont);
	hFontCurrent = NULL;
	logFont.lfHeight = nSize;
	ReplaceCurrentFont(&logFont);	
}
CXUE_PenLock::CXUE_PenLock(HDC hDC, HPEN hPen)
: m_hDC(hDC)
, m_hPen(NULL)
, m_hOldPen(NULL)
{
	assert(hDC);
	assert(hPen);
	if (hPen)
	{
		m_hOldPen = (HPEN)::SelectObject(m_hDC, hPen);
	}	
}

CXUE_PenLock::CXUE_PenLock(HDC hDC, INT fnPenStyle, INT nWidth,	COLORREF crColor)
: m_hDC (hDC)
, m_hPen(NULL)
, m_hOldPen(NULL)
{
	LOGPEN logPen = {fnPenStyle, {nWidth, 1}, crColor};
	ReplaceCurrentPen(&logPen);	
}

CXUE_PenLock::~CXUE_PenLock ()
{
	if (m_hOldPen)
	{
		::SelectObject (m_hDC, m_hOldPen);
		m_hOldPen = NULL;
	}	
	if (m_hPen)
	{
		::DeleteObject(m_hPen);
		m_hPen = NULL;
	}	
}
void CXUE_PenLock::ReplaceCurrentPen(const LOGPEN* pLogPen)
{	
	if (!pLogPen)
	{
		return;
	}
	HPEN hPenNew = NULL;
	VERIFY(hPenNew = ::CreatePenIndirect (pLogPen));
	if (hPenNew)
	{
		if (m_hOldPen)
		{
			::SelectObject (m_hDC, m_hOldPen);
			m_hOldPen = NULL;
		}

		if (m_hPen)
		{
			::DeleteObject(m_hPen);
			m_hPen = NULL;
		}	
		m_hPen = hPenNew;
		m_hOldPen = (HPEN)::SelectObject (m_hDC, m_hPen);
	}			
	return;
}
void CXUE_PenLock::SetPenColor(COLORREF crColor)
{
	LOGPEN logPen = {PS_SOLID, {1, 1}, RGB(0, 0, 0)};
	HPEN hPenCurrent = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
	if (hPenCurrent)
	{
		GetObject(hPenCurrent, sizeof(logPen), &logPen);
		hPenCurrent = NULL;
	}	
	logPen.lopnColor = crColor;
	ReplaceCurrentPen(&logPen);	
	return;
}

void CXUE_PenLock::SetPenStyle( INT nStyle)
{		
	LOGPEN logPen = {PS_SOLID, {1, 1}, RGB(0, 0, 0)};
	HPEN hPenCurrent = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
	if (hPenCurrent)
	{
		GetObject(hPenCurrent, sizeof(logPen), &logPen);
		hPenCurrent = NULL;
	}	
	logPen.lopnStyle = nStyle;
	ReplaceCurrentPen(&logPen);	
	return;	
}

void CXUE_PenLock::SetPenWidth( INT nWidth)
{
	LOGPEN logPen = {PS_SOLID, {1, 1}, RGB(0, 0, 0)};
	HPEN hPenCurrent = (HPEN)::GetCurrentObject(m_hDC, OBJ_PEN);
	if (hPenCurrent)
	{
		GetObject(hPenCurrent, sizeof(logPen), &logPen);
		hPenCurrent = NULL;
	}	
	logPen.lopnWidth.x = nWidth;
	ReplaceCurrentPen(&logPen);	
	return;	
}

CXUE_SolidBrushLock::CXUE_SolidBrushLock( HDC hDC, HBRUSH hBrush)
: m_hDC (hDC)
, m_hBrush(NULL)
, m_hOldBrush(NULL)
{
	assert(hDC);
	assert(hBrush);
	if (hBrush)
	{
		m_hOldBrush = (HBRUSH)::SelectObject(m_hDC, hBrush);
	}	
}

CXUE_SolidBrushLock::CXUE_SolidBrushLock(HDC hDC, COLORREF crColor)
: m_hDC (hDC)
, m_hBrush(NULL)
, m_hOldBrush(NULL)
{
	VERIFY(m_hBrush = CreateSolidBrush (crColor));
	if (m_hBrush)
	{
		m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_hBrush);
	}	
}


CXUE_SolidBrushLock::~CXUE_SolidBrushLock()
{
	if (m_hOldBrush)
	{
		::SelectObject(m_hDC, m_hOldBrush);
		m_hOldBrush = NULL;
	}
	if (m_hBrush)
	{
		::DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}	
}

void CXUE_SolidBrushLock::SetBrushColor(COLORREF crColor)
{
	ReplaceCurrentBrush(crColor);
}

void CXUE_SolidBrushLock::ReplaceCurrentBrush( COLORREF crColor)
{
	HBRUSH hBrushNew = NULL;
	VERIFY(hBrushNew = CreateSolidBrush(crColor));
	if (hBrushNew)
	{
		if (m_hOldBrush)
		{
			::SelectObject(m_hDC, m_hOldBrush);
			m_hOldBrush = NULL;
		}
		if (m_hBrush)
		{
			::DeleteObject(m_hBrush);
			m_hBrush = NULL;
		}
		m_hBrush = hBrushNew;
		m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_hBrush);
	}					
}

CXUE_BkModeLock::CXUE_BkModeLock( HDC hDC, BOOL blTRANSPARENT)
: m_hDC(hDC)
, m_nBkModeOld(0)
{
	SetBkMode(blTRANSPARENT);
}

CXUE_BkModeLock::~CXUE_BkModeLock()
{
	if (m_hDC)
	{
		if (m_nBkModeOld)
		{
			::SetBkMode(m_hDC, m_nBkModeOld);
			m_nBkModeOld = 0;
		}
	}	
}

void CXUE_BkModeLock::SetBkMode(BOOL blTRANSPARENT)
{
	if (!m_hDC)
	{
		return;
	}
	if (m_nBkModeOld)
	{
		::SetBkMode(m_hDC, m_nBkModeOld);
		m_nBkModeOld = 0;
	}
	if (blTRANSPARENT)
	{
		m_nBkModeOld = ::SetBkMode(m_hDC, TRANSPARENT);
	}
	else
	{
		m_nBkModeOld = ::SetBkMode(m_hDC, OPAQUE);
	}
	return;
}



CXUE_CompatibleDCLock::CXUE_CompatibleDCLock(HDC hDC, HBITMAP hBitmap)
{
	m_hMemDC = CreateCompatibleDC(hDC);
	m_hOldBitmap = (HBITMAP)::SelectObject(m_hMemDC, hBitmap);
}

CXUE_CompatibleDCLock::~CXUE_CompatibleDCLock()
{
	if (m_hOldBitmap)
	{
		::SelectObject(m_hMemDC, m_hOldBitmap);
	}	
	if (m_hMemDC)
	{
		::DeleteDC(m_hMemDC);
		m_hMemDC = NULL;
	}	
}

HDC CXUE_CompatibleDCLock::GetHDC()
{
	return m_hMemDC;
}



CXUE_MemDCLock::CXUE_MemDCLock(HDC hDestDC, const LPRECT prcPaint) 
: m_hDestDC (hDestDC)
, m_hBitmap(NULL)
, m_hOldBitmap(NULL)
, m_hClipRgn(NULL)
, m_hClipRgnOld(NULL)
{
	if (hDestDC || prcPaint)
	{
		m_rtPaint = *prcPaint;
		m_hDC = ::CreateCompatibleDC (m_hDestDC);
		if (m_hDC)
		{
			m_hBitmap = ::CreateCompatibleBitmap(m_hDestDC, _abs(m_rtPaint.right - m_rtPaint.left), _abs(m_rtPaint.bottom - m_rtPaint.top));
			if (m_hBitmap)
			{
				m_hOldBitmap = (HBITMAP)::SelectObject (m_hDC, m_hBitmap);
				//////////////////////////////////////////////////////////////////////////
				::SetViewportOrgEx(m_hDC, -m_rtPaint.left, -m_rtPaint.top, NULL);
				//////////////////////////////////////////////////////////////////////////
				m_hClipRgn = ::CreateRectRgn(0, 0, 0, 0);
				if (1 == ::GetClipRgn(m_hDC, m_hClipRgn))
				{
					m_hClipRgnOld = (HRGN)::SelectClipRgn(m_hDC, m_hClipRgn);
				}							
			}			
		}		
	}
}

CXUE_MemDCLock::~CXUE_MemDCLock()
{
	if (!m_hDC)
	{
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	::SetViewportOrgEx(m_hDC, 0, 0, NULL);		
	//////////////////////////////////////////////////////////////////////////
	if (m_hOldBitmap)
	{
		::SelectObject (m_hDC, m_hOldBitmap);
		m_hOldBitmap = NULL;
	}
	if (m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_hClipRgnOld)
	{
		::SelectClipRgn(m_hDC, m_hClipRgnOld);
		m_hClipRgnOld = NULL;
	}
	if (m_hClipRgn)
	{
		::DeleteObject(m_hClipRgn);
		m_hClipRgn = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_hDC)
	{
		::DeleteDC(m_hDC);
		m_hDC = NULL;	
	}	
}

HDC CXUE_MemDCLock::GetSafeHdc()
{
	return m_hDC;
}

BOOL CXUE_MemDCLock::SwapBuffer( )
{
	if (!m_hDestDC)
	{
		return FALSE;
	}
	::BitBlt (m_hDestDC, m_rtPaint.left, m_rtPaint.top, _abs(m_rtPaint.right - m_rtPaint.left), _abs(m_rtPaint.bottom - m_rtPaint.top), m_hDC, m_rtPaint.left, m_rtPaint.top, SRCCOPY);
	return TRUE;
}

BOOL CXUE_MemDCLock::Clear( COLORREF crColor)
{
	if (!m_hDC)
	{
		return FALSE;
	}
	HBRUSH hBrush = ::CreateSolidBrush(crColor);	
	FillRect(m_hDC, &m_rtPaint, hBrush);
	DeleteObject(hBrush);
	hBrush = NULL;
	return TRUE;
}


HBRUSH CXUE_MemDCLock::GetHalftoneBrush()
{
	HBRUSH halftoneBrush = NULL;
	WORD grayPattern[8];
	for(int i = 0; i < 8; i++)
	{
		grayPattern[i] = (WORD)(0x5555 << (i & 1));
	}
	HBITMAP grayBitmap = ::CreateBitmap(8, 8, 1, 1, &grayPattern);
	if(grayBitmap != NULL)
	{
		halftoneBrush = ::CreatePatternBrush(grayBitmap);
		DeleteObject(grayBitmap);
	}
	return halftoneBrush;
}

void CXUE_MemDCLock::DrawDragRect(const LPRECT lpRect, SIZE size, const LPRECT lpRectLast, SIZE sizeLast, HBRUSH pBrush, HBRUSH pBrushLast)
{
	if (!GetSafeHdc())
	{
		return;
	}	
	// first, determine the update region and select it
	HRGN rgnNew = NULL;
	HRGN rgnOutside = NULL, rgnInside = NULL;
	rgnOutside = ::CreateRectRgnIndirect(lpRect);
	RECT rect = *lpRect;
	InflateRect(&rect, -size.cx, -size.cy);
	IntersectRect(&rect, &rect, lpRect);
	rgnInside = ::CreateRectRgnIndirect(&rect);
	rgnNew = ::CreateRectRgn(0, 0, 0, 0);
	::CombineRgn(rgnNew, rgnOutside, rgnInside, RGN_XOR);

	HBRUSH hBrushOld = NULL;
	if (pBrush == NULL)
	{
		pBrush = GetHalftoneBrush();
	}	

	if (pBrushLast == NULL)
	{
		pBrushLast = pBrush;
	}

	HRGN rgnLast = NULL, rgnUpdate = NULL;
	if (lpRectLast != NULL)
	{
		// find difference between new region and old region
		rgnLast = ::CreateRectRgn(0, 0, 0, 0);
		::SetRectRgn(rgnOutside, lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->bottom);
		rect = *lpRectLast;
		InflateRect(&rect, -sizeLast.cx, -sizeLast.cy);
		IntersectRect(&rect, &rect, lpRectLast);
		::SetRectRgn(rgnInside, rect.left, rect.top, rect.right, rect.bottom);
		::CombineRgn(rgnLast, rgnOutside, rgnInside, RGN_XOR);

		// only diff them if brushes are the same
		if (pBrush == pBrushLast)
		{
			rgnUpdate = ::CreateRectRgn( 0, 0, 0, 0);
			::CombineRgn(rgnUpdate, rgnLast, rgnNew, RGN_XOR);
		}
	}
	if (pBrush != pBrushLast && lpRectLast != NULL)
	{
		// brushes are different -- erase old region first
		::SelectClipRgn(GetSafeHdc(), rgnLast);
		::GetClipBox(GetSafeHdc(), &rect);
		hBrushOld = (HBRUSH)SelectObject(GetSafeHdc(), pBrushLast);
		::PatBlt(GetSafeHdc(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
		::SelectObject(GetSafeHdc(), hBrushOld);
		hBrushOld = NULL;
	}

	// draw into the update/new region
	::SelectClipRgn(GetSafeHdc(), rgnUpdate != NULL ? rgnUpdate : rgnNew);
	::GetClipBox(GetSafeHdc(), &rect);
	hBrushOld = (HBRUSH)SelectObject(GetSafeHdc(), pBrush);
	::PatBlt(GetSafeHdc(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);

	
	if (hBrushOld != NULL)
	{
		::SelectObject(GetSafeHdc(), hBrushOld);
		hBrushOld = NULL;
	}
	::SelectClipRgn(GetSafeHdc(), NULL);
}

void CXUE_MemDCLock::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{	
	if (!GetSafeHdc())
	{
		return;
	}	
	if (lpRect)
	{	
		::SetBkColor(GetSafeHdc(), clr);
		::ExtTextOut(GetSafeHdc(), 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
	}	
}

void CXUE_MemDCLock::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	if (!GetSafeHdc())
	{
		return;
	}	
	::SetBkColor(GetSafeHdc(), clr);
	RECT rect = {x, y, x + cx, y + cy};
	::ExtTextOut(GetSafeHdc(), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void CXUE_MemDCLock::Draw3dRect(LPCRECT lpRect,COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (lpRect)
	{
		Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
	}	
}

void CXUE_MemDCLock::Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
}

CXUE_PaintDCLock::CXUE_PaintDCLock( HWND hWnd )
{
	assert(::IsWindow(hWnd));
	m_hWnd = hWnd;
	m_hDC = ::BeginPaint(hWnd, &m_ps);
}

CXUE_PaintDCLock::~CXUE_PaintDCLock()
{
	assert(m_hDC);
	assert(::IsWindow(m_hWnd));
	::EndPaint(m_hWnd, &m_ps);
	m_hWnd = NULL;
	m_hDC = NULL;
}

HDC CXUE_PaintDCLock::GetSafeHdc()
{
	return m_hDC;
}

CXUE_SetViewportOrgExLock::CXUE_SetViewportOrgExLock( HDC hDC, POINT point)
: m_hDC(hDC)
{
	m_PointOld.x = 0;
	m_PointOld.y = 0;
	SetViewportOrgEx(point);
}

void CXUE_SetViewportOrgExLock::SetViewportOrgEx( POINT point)
{
	if (!m_hDC)
	{
		return;
	}
	if (m_PointOld.x != 0 || m_PointOld.y != 0)
	{
		::SetViewportOrgEx(m_hDC, m_PointOld.x, m_PointOld.y, NULL);
	}	
	::SetViewportOrgEx(m_hDC, point.x, point.y, &m_PointOld);
	return;
}

CXUE_SetViewportOrgExLock::~CXUE_SetViewportOrgExLock()
{
	if (m_PointOld.x != 0 || m_PointOld.y != 0)
	{
		::SetViewportOrgEx(m_hDC, m_PointOld.x, m_PointOld.y, NULL);
	}	
}