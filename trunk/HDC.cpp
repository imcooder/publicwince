
#include "stdafx.h"
#include "HDC.h"
#include "Rect.h"
BOOL FillSolidRect(HDC hDC, const RECT* pRC, COLORREF crColor)
{
	HBRUSH hBrush = NULL, hOldBrush = NULL;
	if (!hDC || !pRC)
	{
		return FALSE;
	}	
	return FillSolidRect(hDC, pRC->left, pRC->top, pRC->right - pRC->left, pRC->bottom - pRC->top, crColor);
}
BOOL FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr)
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
BOOL Draw3dRect(HDC hDC, LPRECT lpRect,
								COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (!lpRect || !hDC)
	{
		return FALSE;
	}
	return Draw3dRect(hDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left,
		lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}

BOOL Draw3dRect(HDC hDC, int x, int y, int cx, int cy,
								COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	if (!hDC)
	{
		return FALSE;
	}
	return FillSolidRect(hDC, x, y, cx - 1, 1, clrTopLeft) 
		&& FillSolidRect(hDC, x, y, 1, cy - 1, clrTopLeft)
		&& FillSolidRect(hDC, x + cx, y, -1, cy, clrBottomRight)
		&& FillSolidRect(hDC, x, y + cy, cx, -1, clrBottomRight);
}

BOOL MoveTo(HDC hDC, int x, int y)
{
	if (!hDC)
	{
		return FALSE;
	}	
	return ::MoveToEx(hDC, x, y, NULL);
}

DWORD GetFontCell( HDC hDC)
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

BOOL SetPoint( POINT* ptPoint, LONG x, LONG y)
{
	if (!ptPoint)
	{
		return FALSE;
	}
	ptPoint->x = x;
	ptPoint->y = y;
	return TRUE;
}

BOOL IsPointNull( const POINT* ptPoint)
{
	if (!ptPoint)
	{
		return FALSE;
	}
	return !ptPoint->x && !ptPoint->y;
}

COLORREF DarkenColor(COLORREF crColor, double dFactor)
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


void WINAPI DrawAntiAliasLine( HDC hDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine)
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