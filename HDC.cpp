
#include "stdafx.h"
#include "HDC.h"
#include "Rect.h"
BOOL WINAPI FillSolidRect(HDC hDC, const RECT* pRC, COLORREF crColor)
{
	HBRUSH hBrush = NULL, hOldBrush = NULL;
	if (!hDC || !pRC)
	{
		return FALSE;
	}	
	return FillSolidRect(hDC, pRC->left, pRC->top, pRC->right - pRC->left, pRC->bottom - pRC->top, crColor);
}
BOOL WINAPI FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr)
{	
	if (!hDC)
	{
		return FALSE;
	}
	RECT rect = {x, y, x + cx, y + cy};	
	COLORREF crOldBkColor = ::SetBkColor(hDC, clr);	
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	::SetBkColor(hDC, crOldBkColor);	
	return TRUE;
}
BOOL WINAPI Draw3dRect(HDC hDC, LPRECT lpRect,	COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (!lpRect || !hDC)
	{
		return FALSE;
	}
	return Draw3dRect(hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left,	lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}

BOOL WINAPI Draw3dRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (!hDC)
	{
		return FALSE;
	}
	return FillSolidRect(hDC, x, y, cx - 1, 1, clrTopLeft) 	&& FillSolidRect(hDC, x, y, 1, cy - 1, clrTopLeft)	&& FillSolidRect(hDC, x + cx, y, -1, cy, clrBottomRight)	&& FillSolidRect(hDC, x, y + cy, cx, -1, clrBottomRight);
}

BOOL WINAPI MoveTo(HDC hDC, int x, int y)
{
	if (!hDC)
	{
		return FALSE;
	}	
	return ::MoveToEx(hDC, x, y, NULL);
}

DWORD WINAPI GetFontCell( HDC hDC)
{
	if (!hDC)
	{
		return FALSE;
	}
	TEXTMETRIC tm;	
	DWORD dwRet = 0;
	if (GetTextMetrics(hDC, &tm))
	{		
		dwRet = MAKELONG(tm.tmHeight, tm.tmHeight);
	}
	else
	{
		dwRet = MAKELONG(24, 24);
	}	
	return dwRet;
}

BOOL WINAPI SetPoint( POINT* ptPoint, LONG x, LONG y)
{
	if (!ptPoint)
	{
		return FALSE;
	}
	ptPoint->x = x;
	ptPoint->y = y;
	return TRUE;
}

BOOL WINAPI IsPointNull( const POINT* ptPoint)
{
	if (!ptPoint)
	{
		return FALSE;
	}
	return !ptPoint->x && !ptPoint->y;
}

COLORREF WINAPI DarkenColor(COLORREF crColor, double dFactor)
{
	if (dFactor > 0.0 && dFactor <= 1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(crColor);
		green = GetGValue(crColor);
		blue = GetBValue(crColor);
		lightred = (BYTE)(red - (dFactor * red));
		lightgreen = (BYTE)(green - (dFactor * green));
		lightblue = (BYTE)(blue - (dFactor * blue));
		crColor = RGB(lightred, lightgreen, lightblue);
	} 
	return crColor;
}

BOOL WINAPI HDCDarker(HDC hDC, const LPRECT pRect, double dFactor, COLORREF crTransparent)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return FALSE;
	}	
	COLORREF	crPixel = 0;
	RECT rtArea = *pRect;	
	NormalizeRect(&rtArea);
	for (DWORD dwLoopX = (DWORD)rtArea.left; dwLoopX <= (DWORD)rtArea.right; dwLoopX ++)
	{
		for (DWORD dwLoopY = (DWORD)rtArea.top; dwLoopY <= (DWORD)rtArea.bottom; dwLoopY ++)
		{
			crPixel = ::GetPixel(hDC, dwLoopX, dwLoopY);
			if (crPixel != crTransparent)
			{
				::SetPixel(hDC, dwLoopX, dwLoopY, DarkenColor(crPixel, dFactor));		
			}				
		} 
	} 
	return TRUE;
} 

BOOL WINAPI HDCGray( HDC hDC, const LPRECT pRect)
{
	ASSERT(pRect);
	if (!pRect)
	{
		return FALSE;
	}	
	COLORREF	crPixel = 0, crNew = 0;
	RECT rtArea = *pRect;	
	NormalizeRect(&rtArea);
	for (DWORD dwLoopX = rtArea.left;  dwLoopX <= rtArea.right; dwLoopX ++)
	{
		for (DWORD dwLoopY = rtArea.top; dwLoopY <= rtArea.bottom; dwLoopY ++)
		{
			crPixel = ::GetPixel(hDC, dwLoopX, dwLoopY);
			crNew = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));
			::SetPixel(hDC, dwLoopX, dwLoopY, crNew);			
		} 
	} 
	return TRUE;
}


void WINAPI MovingBoarder(const RECT* pRect)
{
	if (!pRect)
	{
		return;
	}
	HDC hDC = GetDC(NULL);
	DrawFocusRect(hDC, pRect);	
	if (hDC)
	{
		ReleaseDC(NULL, hDC);
		hDC = NULL;
	}	
	return;
}

BOOL WINAPI DrawRectangles( HDC hDC, const LPRECT pRects, LONG nCount)
{
	if (!pRects || nCount < 1)
	{
		return FALSE;
	}
	for (LONG nIdx = 0; nIdx < nCount; nIdx ++)
	{
		POINT aPoints[5] = 
		{
			{(pRects + nIdx)->left, (pRects + nIdx)->top},			
			{(pRects + nIdx)->right, (pRects + nIdx)->top},
			{(pRects + nIdx)->right, (pRects + nIdx)->bottom},
			{(pRects + nIdx)->left, (pRects + nIdx)->bottom},	
			{(pRects + nIdx)->left, (pRects + nIdx)->top}		
		};
		Polyline(hDC, aPoints, 5);
	}
	return TRUE;
}


void WINAPI XUE_DrawSolidLine(HDC hDC, int x0,int y0,int x1,int y1, LONG nWdith, DWORD dwColor)
{
	HPEN	hNewPen = NULL,hOldPen = NULL;
	POINT	pBuf[2];

	// 定义新的画笔
	hNewPen = ::CreatePen( PS_SOLID, nWdith, dwColor);// 创建新的画笔
	if (hNewPen)
	{
		hOldPen = (HPEN)SelectObject( hDC, hNewPen );
	}
	// 采用新的画笔进行画线	
	pBuf[0].x = x0;
	pBuf[0].y = y0;
	pBuf[1].x = x1;
	pBuf[1].y = y1;
	Polyline( hDC, pBuf, 2 );

	// 恢复原来的画笔
	if (hOldPen)
	{
		SelectObject(hDC, hOldPen );
		hOldPen = NULL;
	}
	if (hNewPen)
	{
		DeleteObject( hNewPen );
		hNewPen = NULL;
	}
	return;
}


void WINAPI XUE_DrawAntiAliasLine( HDC hDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine)
{
	/* Make sure the line runs top to bottom */
	if (Y0 > Y1)
	{
		int Temp = Y0; Y0 = Y1; Y1 = Temp;
		Temp = X0; X0 = X1; X1 = Temp;
	}

	/* Draw the initial pixel, which is always exactly intersected by
	the line and so needs no weighting */
	SetPixel(hDC, X0, Y0, clrLine );

	int XDir, DeltaX = X1 - X0;
	if( DeltaX >= 0 )
	{
		XDir = 1;
	}
	else
	{
		XDir   = -1;
		DeltaX = 0 - DeltaX; /* make DeltaX positive */
	}

	/* Special-case horizontal, vertical, and diagonal lines, which
	require no weighting because they go right through the center of
	every pixel */
	int DeltaY = Y1 - Y0;
	if (DeltaY == 0)
	{
		/* Horizontal line */
		while (DeltaX-- != 0)
		{
			X0 += XDir;
			SetPixel(hDC, X0, Y0, clrLine );
		}
		return;
	}
	if (DeltaX == 0)
	{
		/* Vertical line */
		do
		{
			Y0 ++;
			SetPixel(hDC, X0, Y0, clrLine );
		} 
		while (--DeltaY != 0);
		return;
	}

	if (DeltaX == DeltaY)
	{
		/* Diagonal line */
		do
		{
			X0 += XDir;
			Y0 ++;
			SetPixel(hDC, X0, Y0, clrLine );
		} 
		while (--DeltaY != 0);
		return;
	}

	unsigned short ErrorAdj;
	unsigned short ErrorAccTemp, Weighting;

	/* Line is not horizontal, diagonal, or vertical */
	unsigned short ErrorAcc = 0;  /* initialize the line error accumulator to 0 */

	BYTE rl = GetRValue( clrLine );
	BYTE gl = GetGValue( clrLine );
	BYTE bl = GetBValue( clrLine );
	double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;

	/* Is this an X-major or Y-major line? */
	if (DeltaY > DeltaX)
	{
		/* Y-major line; calculate 16-bit fixed-point fractional part of a
		pixel that X advances each time Y advances 1 pixel, truncating the
		result so that we won't overrun the endpoint along the X axis */
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		/* Draw all pixels other than the first and last */
		while (-- DeltaY)
		{
			ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
			ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
			if (ErrorAcc <= ErrorAccTemp) 
			{
				/* The error accumulator turned over, so advance the X coord */
				X0 += XDir;
			}
			Y0++; /* Y-major, so always advance Y */
			/* The IntensityBits most significant bits of ErrorAcc give us the
			intensity weighting for this pixel, and the complement of the
			weighting for the paired pixel */
			Weighting = ErrorAcc >> 8;
			ASSERT( Weighting < 256 );
			ASSERT( ( Weighting ^ 255 ) < 256 );

			COLORREF clrBackGround = ::GetPixel( hDC, X0, Y0 );
			BYTE rb = GetRValue( clrBackGround );
			BYTE gb = GetGValue( clrBackGround );
			BYTE bb = GetBValue( clrBackGround );
			double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

			BYTE rr = ( rb > rl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rl - rb ) + rb ) ) );
			BYTE gr = ( gb > gl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gl - gb ) + gb ) ) );
			BYTE br = ( bb > bl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bl - bb ) + bb ) ) );
			SetPixel(hDC, X0, Y0, RGB( rr, gr, br ) );

			clrBackGround = ::GetPixel( hDC, X0 + XDir, Y0 );
			rb = GetRValue( clrBackGround );
			gb = GetGValue( clrBackGround );
			bb = GetBValue( clrBackGround );
			grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

			rr = ( rb > rl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rl - rb ) + rb ) ) );
			gr = ( gb > gl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gl - gb ) + gb ) ) );
			br = ( bb > bl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bl - bb ) + bb ) ) );
			SetPixel(hDC, X0 + XDir, Y0, RGB( rr, gr, br ) );
		}
		/* Draw the final pixel, which is always exactly intersected by the line
		and so needs no weighting */
		SetPixel(hDC, X1, Y1, clrLine );
		return;
	}
	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
	pixel that Y advances each time X advances 1 pixel, truncating the
	result to avoid overrunning the endpoint along the X axis */
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	/* Draw all pixels other than the first and last */
	while (--DeltaX) 
	{
		ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
		ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
		if (ErrorAcc <= ErrorAccTemp)
		{
			/* The error accumulator turned over, so advance the Y coord */
			Y0++;
		}
		X0 += XDir; /* X-major, so always advance X */
		/* The IntensityBits most significant bits of ErrorAcc give us the
		intensity weighting for this pixel, and the complement of the
		weighting for the paired pixel */
		Weighting = ErrorAcc >> 8;
		ASSERT( Weighting < 256 );
		ASSERT( ( Weighting ^ 255 ) < 256 );

		COLORREF clrBackGround = ::GetPixel( hDC, X0, Y0 );
		BYTE rb = GetRValue( clrBackGround );
		BYTE gb = GetGValue( clrBackGround );
		BYTE bb = GetBValue( clrBackGround );
		double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

		BYTE rr = ( rb > rl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( rl - rb ) + rb ) ) );
		BYTE gr = ( gb > gl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( gl - gb ) + gb ) ) );
		BYTE br = ( bb > bl ? ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?Weighting:(Weighting ^ 255)) ) / 255.0 * ( bl - bb ) + bb ) ) );

		SetPixel(hDC, X0, Y0, RGB( rr, gr, br ) );

		clrBackGround = ::GetPixel( hDC, X0, Y0 + 1 );
		rb = GetRValue( clrBackGround );
		gb = GetGValue( clrBackGround );
		bb = GetBValue( clrBackGround );
		grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

		rr = ( rb > rl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rb - rl ) + rl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( rl - rb ) + rb ) ) );
		gr = ( gb > gl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gb - gl ) + gl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( gl - gb ) + gb ) ) );
		br = ( bb > bl ? ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bb - bl ) + bl ) ) : ( ( BYTE )( ( ( double )( grayl<grayb?(Weighting ^ 255):Weighting) ) / 255.0 * ( bl - bb ) + bb ) ) );

		SetPixel(hDC, X0, Y0 + 1, RGB( rr, gr, br ) );
	}

	/* Draw the final pixel, which is always exactly intersected by the line
	and so needs no weighting */
	SetPixel(hDC, X1, Y1, clrLine );
}
#ifdef UNDER_CE

BOOL WINAPI XUE_LineDDA( int x0, int y0, int x1, int y1, LINEDDAPROC lpLineFunc, LPARAM lpData )
{
	if (!lpLineFunc)
	{
		return FALSE;
	}
	int i = 0, xSize = 0, ySize = 0, nCtrl = 0, xSign = 0, ySign = 0;
	BOOL isInterchange = FALSE;

	xSize = (x1 >= x0) ? (x1-x0) : (x0-x1);
	ySize = (y1 >= y0) ? (y1-y0) : (y0-y1);	
	xSign = (x1 >= x0) ? 1 : -1;
	ySign = (y1 >= y0) ? 1 : -1;
	if (!xSize)
	{//竖线
		if(y0 < y1 )
		{
			for(i = y0; i <= y1; i ++)
			{
				(*lpLineFunc)( x0, i, lpData);				
			}
		}
		else
		{
			for(i = y0; i >= y1; i --)
			{
				(*lpLineFunc)( x0, i, lpData);	
			}
		}
	}
	else if (!ySize)
	{//横线
		if(x0 < x1 )
		{
			for(i = x0; i <= x1; i ++)
			{
				(*lpLineFunc)( i, y0, lpData);				
			}
		}
		else
		{
			for(i = x0; i >= x1; i --)
			{
				(*lpLineFunc)( i, y0, lpData);	
			}
		}
	}
	else
	{//斜线
		if(ySize > xSize)
		{// 交换数据
			nCtrl = xSize;
			xSize = ySize;
			ySize = nCtrl;
			isInterchange = TRUE;
		}
		else
		{
			isInterchange = FALSE;
		}

		nCtrl = (ySize << 1) - xSize;// 控制变量
		for( i = 0; i <= xSize; i++ )
		{
			(*lpLineFunc)( x0, y0,  lpData);
			if(nCtrl >= 0)
			{
				if(isInterchange)
				{
					x0 += xSign;
				}
				else
				{
					y0 += ySign;
				}
				nCtrl -= (xSize << 1);
			}

			if(isInterchange)
			{
				y0 += ySign;
			}
			else
			{
				x0 += xSign;
			}

			nCtrl += (ySize << 1);
		}
	}	
	return TRUE;
}
#endif


BOOL WINAPI XUE_StretchDraw( HDC hdcDest, const RECT* prtDest, HDC hdcSrc, const RECT* prtSrc, DWORD dwRop, DWORD dwStretchMode )
{
	if (!hdcDest || !hdcSrc || !prtDest || !prtSrc)
	{
		return FALSE;
	}
	RECT rtDest = *prtDest;
	RECT rtSrc = *prtSrc;
	if (IsRectEmpty(&rtDest) || IsRectEmpty(&rtSrc))
	{
		return TRUE;
	}
	if (IsRectEqual(&rtDest, &rtSrc))
	{
		return ::BitBlt(hdcDest, rtDest.left, rtDest.top, rtDest.right - rtDest.left, rtDest.bottom - rtDest.top, hdcSrc, rtSrc.left, rtSrc.top, dwRop);
	}
	else
	{				
		if (dwStretchMode & XUE_STRETCHDRAW_NORMAL)
		{
			return ::StretchBlt(hdcDest, rtDest.left, rtDest.top, rtDest.right - rtDest.left, rtDest.bottom - rtDest.top, hdcSrc, rtSrc.left, rtSrc.top, rtSrc.right - rtSrc.left, rtSrc.bottom - rtSrc.top, dwRop);
		}
		else if(dwStretchMode & XUE_STRETCHDRAW_NOSHAPE)
		{
			RECT rtTmpDest = rtDest;
			POINT pointCenterDest = {(rtDest.left + rtDest.right) >> 1, (rtDest.top + rtDest.bottom) >> 1};
			POINT pointCenterSrc	= {(rtSrc.left + rtSrc.right) >> 1, (rtSrc.top + rtSrc.bottom) >> 1};
			LONG nAngleDest = ((rtDest.right - rtDest.left) * (rtSrc.bottom - rtSrc.top));
			LONG nAngleSrc = ((rtSrc.right - rtSrc.left) * (rtDest.bottom - rtDest.top));
			if (_abs(nAngleDest) >=  _abs(nAngleSrc))
			{
				rtTmpDest.top = 0;
				rtTmpDest.bottom = rtDest.bottom - rtDest.top;
				LONG nWidth = (rtSrc.right - rtSrc.left) * (rtTmpDest.bottom - rtTmpDest.top) / (rtSrc.bottom - rtSrc.top);
				rtTmpDest.left = 0;
				rtTmpDest.right = nWidth;
			}
			else
			{
				rtTmpDest.left = 0;
				rtTmpDest.right = rtDest.right - rtDest.left;
				LONG nHeight = (rtSrc.bottom - rtSrc.top) * (rtTmpDest.right - rtTmpDest.left) / (rtSrc.right - rtSrc.left);
				rtTmpDest.top = 0;
				rtTmpDest.bottom = nHeight;
			}		
			RectCenterMoveTo(&rtTmpDest, (rtDest.right + rtDest.left) / 2, (rtDest.bottom + rtDest.top) / 2);
			return ::StretchBlt(hdcDest, rtTmpDest.left, rtTmpDest.top, rtTmpDest.right - rtTmpDest.left, rtTmpDest.bottom - rtTmpDest.top, hdcSrc, rtSrc.left, rtSrc.top, rtSrc.right - rtSrc.left, rtSrc.bottom - rtSrc.top, dwRop);
		}	
	}	
	return TRUE;
}



BOOL WINAPI XUE_DrawTransparent(HDC hDC, int x, int y, HBITMAP	hBitmap, COLORREF crColour)
{
	if (!hDC || !hBitmap)
	{
		return FALSE;
	}
	COLORREF crOldBack = SetBkColor(hDC, RGB(255, 255, 255));

	COLORREF crOldText = SetTextColor(hDC, RGB(0, 0, 0));

	HDC hDCImage = NULL, hDCTrans = NULL;

	hDCImage = CreateCompatibleDC(hDC);

	hDCTrans = CreateCompatibleDC(hDC);


	// Select the image into the appropriate dc

	HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDCImage, hBitmap);


	// Create the mask hBmpInfo

	BITMAP hBmpInfo;

	GetObject(hBitmap, sizeof(BITMAP), &hBmpInfo);

	HBITMAP hBmpTrans = CreateBitmap(hBmpInfo.bmWidth, hBmpInfo.bmHeight, 1, 1, NULL);
	HBITMAP pOldBitmapTrans = (HBITMAP)SelectObject(hDCTrans, hBmpTrans);

	SetBkColor(hDCImage, crColour);

	BitBlt(hDCTrans, 0, 0, hBmpInfo.bmWidth, hBmpInfo.bmHeight, hDCImage, 0, 0, SRCCOPY);

	BitBlt(hDC, x, y, hBmpInfo.bmWidth, hBmpInfo.bmHeight, hDCImage, 0, 0, SRCINVERT);

	BitBlt(hDC, x, y, hBmpInfo.bmWidth, hBmpInfo.bmHeight, hDCTrans, 0, 0, SRCAND);

	BitBlt(hDC, x, y, hBmpInfo.bmWidth, hBmpInfo.bmHeight, hDCImage, 0, 0, SRCINVERT);


	// Restore settings
	SelectObject(hDCImage, pOldBitmapImage);
	pOldBitmapImage = NULL;
	SelectObject(hDCTrans, pOldBitmapTrans);
	pOldBitmapTrans = NULL;
	SetBkColor(hDC, crOldBack);
	crOldBack = 0;
	SetTextColor(hDC, crOldText);
	crOldText = 0;
	return TRUE;

}

BOOL WINAPI StretchBlt( HDC hdcDest, const RECT* pDest, HDC hdcSrc, const RECT* pSrc, DWORD dwRop)
{	
	switch(dwRop)
	{
	case DSTINVERT:
		{
			assert(pDest);
			if (pDest)
			{
				if(pSrc)	
				{
					return ::StretchBlt(hdcDest, pDest->left, pDest->top, pDest->right - pDest->left, pDest->bottom - pDest->top, NULL, pSrc->left, pSrc->top, pSrc->right - pSrc->left, pSrc->bottom - pSrc->top, dwRop);
				}
				else
				{
					return ::StretchBlt(hdcDest, pDest->left, pDest->top, pDest->right - pDest->left, pDest->bottom - pDest->top, NULL, 0, 0, 1, 1, dwRop);
				}
			}
			else
			{
				return FALSE;
			}
		}
		break;
	default:
		{
			if (pDest && pSrc)
			{
				return ::StretchBlt(hdcDest, pDest->left, pDest->top, pDest->right - pDest->left, pDest->bottom - pDest->top, hdcSrc, pSrc->left, pSrc->top, pSrc->right - pSrc->left, pSrc->bottom - pSrc->top, dwRop);
			}
			else
			{
				return FALSE;
			}
		}
		break;
	}	
	return FALSE;
}

BOOL WINAPI AlphaBlend( HDC hdcDest, const RECT* pDest, HDC hdcSrc, const RECT* pSrc, BLENDFUNCTION blendFunc)
{
	if (!pDest || !pSrc)
	{
		return FALSE;
	}
	return ::AlphaBlend(hdcDest, pDest->left, pDest->top, pDest->right - pDest->left, pDest->bottom - pDest->top, hdcSrc, pSrc->left, pSrc->top, pSrc->right - pSrc->left, pSrc->bottom - pSrc->top, blendFunc);
}