
#include "stdafx.h"
#include "Rect.h"
#include "Ext_Type.h"

BOOL WINAPI IsRectNULL( const RECT * pRect)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return FALSE;
	}
	return (!pRect->left && !pRect->right && !pRect->top && !pRect->bottom);
}

BOOL WINAPI MoveRectTo( LPRECT pRect, int x, int y)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return FALSE;
	}
	LONG nWidth = pRect->right - pRect->left, nHeight = pRect->bottom - pRect->top;
	return SetRect(pRect, x, y, x + nWidth, y + nHeight);
}

void WINAPI NormalizeRect(LPRECT pRect) 
{
	ASSERT(pRect);
	if (!pRect)
	{		
		return;
	}	
	if (pRect->left > pRect->right)
	{
		_swap(pRect->left, pRect->right);
	}
	if (pRect->top > pRect->bottom)
	{
		_swap(pRect->top, pRect->bottom);
	}
}
BOOL WINAPI SetRect (LPRECT lprc,POINT point, SIZE size)
{
	ASSERT(lprc);
	if (!lprc)
	{		
		return FALSE;
	}
	lprc->right = (lprc->left = point.x) + size.cx;
	lprc->bottom = (lprc->top = point.y) + size.cy;
	return TRUE;
}
BOOL WINAPI SetRect (LPRECT lprc,POINT topLeft, POINT bottomRight)
{
	ASSERT(lprc);
	if (!lprc)
	{		
		return FALSE;
	}
	lprc->left = topLeft.x; 
	lprc->top = topLeft.y;
	lprc->right = bottomRight.x; 
	lprc->bottom = bottomRight.y; 
	return TRUE;
}


LONG WINAPI RectWidth(const LPRECT lprc) 
{
	ASSERT(lprc);
	if (!lprc)
	{		
		return 0;
	}
	return lprc->right - lprc->left; 
}
LONG WINAPI RectHeight(const LPRECT lprc)
{ 
	ASSERT(lprc);
	if (!lprc)
	{		
		return 0;
	}
	return lprc->bottom - lprc->top;
}



POINT WINAPI RectCenterPoint(const LPRECT lprc)
{ 
	ASSERT(lprc);
	POINT point = {0, 0};
	if (!lprc)
	{		
		return point;
	}	
	point.x = (lprc->left + lprc->right) / 2;
	point.y = (lprc->top + lprc->bottom) / 2;
	return point;
}
void WINAPI RectSwapLeftRight(LPRECT lpRect)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	_swap(lpRect->left, lpRect->right);
}
void WINAPI RectSwapTopBottom(LPRECT lpRect)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	_swap(lpRect->left, lpRect->bottom);
}

BOOL WINAPI IsRectEmpty(const LPRECT lpRect) 
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return FALSE;
	}
	return lpRect->right <= lpRect->left || lpRect->bottom <= lpRect->top; 
}

BOOL WINAPI PtInRect(const LPRECT lpRect, POINT point)
{
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return FALSE;
	}
	return lpRect->left <= point.x && point.x <= lpRect->right && lpRect->top <= point.y && point.y <= lpRect->bottom;
}


void WINAPI InflateRect(LPRECT lpRect, SIZE size)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	InflateRect(lpRect, size.cx, size.cy);	
}

void WINAPI InflateRect( LPRECT lpRect, int x, int y, int xx, int yy)
{
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	lpRect->left += x;
	lpRect->top += y;
	lpRect->right += xx;
	lpRect->bottom += yy;
}
void WINAPI DeflateRect(LPRECT lpRect, int x, int y) 
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	InflateRect(lpRect, -x, -y); 
}
void WINAPI DeflateRect(LPRECT lpRect, SIZE size) 
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	InflateRect(lpRect, -size.cx, -size.cy); 
}

void WINAPI OffsetRect(LPRECT lpRect, POINT point) 
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	::OffsetRect(lpRect, point.x, point.y);
}

void WINAPI OffsetRect(LPRECT lpRect, SIZE size)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	::OffsetRect(lpRect, size.cx, size.cy); 
}
void WINAPI RectMoveTo(LPRECT lpRect, int x, int y)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	lpRect->right = lpRect->right - lpRect->left + x;
	lpRect->left = x;
	lpRect->bottom = lpRect->bottom - lpRect->top + y;
	lpRect->top = y;
}
void WINAPI RectMoveTo(LPRECT lpRect, POINT pt)
{ 
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	RectMoveTo(lpRect, pt.x, pt.y);
}
void WINAPI RectCenterMoveTo( LPRECT pRect, int x, int y)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return;
	}	
	LONG nWidth = pRect->right - pRect->left, nHeight = pRect->bottom - pRect->top;
	SetRect(pRect, x - nWidth / 2, y - nHeight / 2, x + nWidth / 2, y + nHeight / 2);
	return;
}

void WINAPI RectCenterMoveTo( LPRECT pRect, POINT pointCenter)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return;
	}		
	RectCenterMoveTo(pRect, pointCenter.x, pointCenter.y);
	return;
}
void WINAPI ZoomRect( LPRECT lpRect, float fx, float fy)
{
	ASSERT(lpRect);
	if (!lpRect)
	{		
		return;
	}
	lpRect->left *= fx;
	lpRect->top *= fy;
	lpRect->right *= fx;
	lpRect->bottom *= fy;
	return;
}

BOOL WINAPI IsRectEqual( const RECT* pRectA, const RECT* pRectB)
{
	ASSERT(pRectA);
	ASSERT(pRectB);
	if (!pRectA || !pRectB)
	{
		return FALSE;
	}
	return pRectA->left == pRectB->left && pRectA->top == pRectB->top && pRectA->right == pRectB->right && pRectA->bottom == pRectB->bottom;
}

