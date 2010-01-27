

#ifndef HWX_RECT_H
#define HWX_RECT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windef.h>

BOOL WINAPI IsRectNULL(const RECT *);
BOOL WINAPI MoveRectTo(LPRECT, int, int);
void WINAPI NormalizeRect(LPRECT);
BOOL WINAPI SetRect (LPRECT ,POINT, SIZE);
BOOL WINAPI SetRect (LPRECT,POINT, POINT);
LONG WINAPI RectWidth(const LPRECT);
LONG WINAPI RectHeight(const LPRECT);
POINT WINAPI RectCenterPoint(const LPRECT);
void WINAPI RectSwapLeftRight(LPRECT);
void WINAPI RectSwapTopBottom(LPRECT);
BOOL WINAPI IsRectEmpty(const LPRECT);
BOOL WINAPI IsRectEqual(const RECT*, const RECT*);
BOOL WINAPI PtInRect(const LPRECT, POINT);
void WINAPI InflateRect(LPRECT, SIZE);
void WINAPI InflateRect(LPRECT, int, int, int, int);
void WINAPI DeflateRect(LPRECT, int, int) ;
void WINAPI DeflateRect(LPRECT, SIZE) ;
void WINAPI OffsetRect(LPRECT, POINT);
void WINAPI OffsetRect(LPRECT, SIZE);
void WINAPI RectMoveTo(LPRECT, int, int);
void WINAPI RectMoveTo(LPRECT, POINT);
void WINAPI RectCenterMoveTo(LPRECT, int, int);
void WINAPI RectCenterMoveTo(LPRECT, POINT);
void WINAPI ZoomRect(LPRECT, float, float);
#endif //HWX_RECT_H
