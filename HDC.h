

#ifndef HWX_HDC_H
#define HWX_HDC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windef.h>

BOOL WINAPI FillSolidRect(HDC hDC, const RECT* pRC, COLORREF crColor);
BOOL WINAPI FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr);
BOOL WINAPI Draw3dRect(HDC hDC, LPRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
BOOL WINAPI Draw3dRect(HDC hDC, int x, int y, int cx, int cy,	COLORREF clrTopLeft, COLORREF clrBottomRight);
BOOL WINAPI MoveTo(HDC hDC, int x, int y);
DWORD WINAPI GetFontCell(HDC);


BOOL WINAPI SetPoint(POINT*, LONG, LONG);
BOOL WINAPI IsPointNull(const POINT*);
BOOL WINAPI HDCDarker(HDC, const LPRECT, double, COLORREF crTransparent = RGB(0, 0, 0));
BOOL WINAPI HDCGray(HDC, const LPRECT);
void WINAPI MovingBoarder(const RECT*);
BOOL WINAPI DrawRectangles(HDC, const LPRECT, LONG);

void WINAPI XUE_DrawSolidLine(HDC hDC, int x0,int y0,int x1,int y1, LONG nWdith, DWORD dwColor);
void WINAPI XUE_DrawAntiAliasLine( HDC, int X0, int Y0, int X1, int Y1, COLORREF);

#endif //HWX_HDC_H
