/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	BitmapEx.cpp			
文件内容:			Bitmap简单处理
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/
#include "StdAfx.h"
#include "Bitmap.h"
#ifdef WINCE
	
#endif

HBITMAP WINAPI XUE_LoadBitmapFromFileW( LPCWSTR pszFile)
{
	HBITMAP hBitmap = NULL;
	if (!pszFile)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return hBitmap;
	}
#ifdef WINCE
	{
		hBitmap = SHLoadDIBitmap(pszFile);
	}
#else
	{
		hBitmap = (HBITMAP)LoadImage(NULL, pszFile, IMAGE_BITMAP,	0, 0, LR_LOADFROMFILE);
	}
#endif	
	return hBitmap;
}


HBITMAP WINAPI XUE_LoadBitmapFromFileA( LPCSTR pszFile)
{
	HBITMAP hBitmap = NULL;
	if (!pszFile)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return hBitmap;
	}
	WCHAR szPath[MAX_PATH] = {0};
	mbstowcs(szPath, pszFile, strlen(pszFile));
	return XUE_LoadBitmapFromFileW(szPath);
}

void* WINAPI XUE_LoadResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType,  LPDWORD pvSize)
{	
	HRSRC hRes = NULL;
	DWORD dwSize = 0;
	HGLOBAL hData = NULL;
	void*  pvRes = NULL;	
	hRes = FindResource(hModule, lpName, lpType);
	if (hRes)
	{
		dwSize = SizeofResource(hModule, hRes);
		hData = LoadResource(hModule, hRes);
		pvRes = (void*)LockResource(hData);
	}
	if (pvSize)
	{
		*pvSize = dwSize;
	}
	return pvRes;	
}


HBITMAP WINAPI XUE_CreateGrayBitmap(HBITMAP hBitmap, COLORREF crTrans)
{
	HBITMAP	hGrayBitmap = NULL;
	HDC	hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;

	BITMAP tBitmap;
	__try
	{
		if (!hBitmap)
		{
			__leave;
		}
		if (!(hMainDC = ::GetDC(NULL)))	
		{
			__leave;
		}
		if (!(hMemDC1 = ::CreateCompatibleDC(hMainDC)))
		{
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if
		if (!(hMemDC2 = ::CreateCompatibleDC(hMainDC)))
		{
			//::DeleteDC(hMemDC1);
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if		
		if (!GetObject(hBitmap, sizeof(tBitmap), &tBitmap))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight);
		if (hGrayBitmap)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);

			//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

			DWORD			dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE			byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);
					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));

					if (crPixel != crTrans)
					{
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
					}
					else
					{
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
					}
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);
		} // if
	}
	__finally
	{
		if (hMemDC1)
		{
			::DeleteDC(hMemDC1);
		}
		if (hMemDC2)
		{
			::DeleteDC(hMemDC2);
		}
		if (hMainDC)
		{
			::ReleaseDC(NULL, hMainDC);		
		}		
	}		
	return hGrayBitmap;
} 

static COLORREF DarkenColor(COLORREF crColor, double dFactor)
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
HBITMAP WINAPI XUE_CreateDarkerBitmap(HBITMAP hBitmap, COLORREF crTrans)
{
	HBITMAP		hGrayBitmap = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	__try
	{		
		if (!(hMainDC = ::GetDC(NULL)))	
		{
			__leave;
		}		
		if(!(hMemDC1 = ::CreateCompatibleDC(hMainDC)))
		{
			//::ReleaseDC(NULL, hMainDC);
			__leave;
		} // if
		if (!(hMemDC2 = ::CreateCompatibleDC(hMainDC)))
		{
			//::DeleteDC(hMemDC1);
			//::ReleaseDC(NULL, hMainDC);
			//return NULL;
			__leave;
		} // if
		BITMAP tBitmap;
		if (!(::GetObject(hBitmap, sizeof(tBitmap), &tBitmap)))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		if (hGrayBitmap = ::CreateCompatibleBitmap(hMainDC, dwWidth, dwHeight))
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, hGrayBitmap);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, hBitmap);
			//::BitBlt(hMemDC1, 0, 0, dwWidth, dwHeight, hMemDC2, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;

			for (dwLoopY = 0; dwLoopY < dwHeight; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < dwWidth; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

					if (crPixel != crTrans)
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, DarkenColor(crPixel, 0.25));
					else
						::SetPixel(hMemDC1, dwLoopX, dwLoopY, crPixel);
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);
		} // if
	}
	__finally
	{
		if (hMemDC1)
		{
			::DeleteDC(hMemDC1);
			hMemDC1 = NULL;
		}
		if (hMemDC2)
		{
			::DeleteDC(hMemDC2);
			hMemDC2 = NULL;
		}
		if (hMainDC)
		{
			::ReleaseDC(NULL, hMainDC);
		}		
	}	
	return hGrayBitmap;
} 

HBITMAP WINAPI XUE_CreateBitmapMask(HBITMAP hBitmap,  COLORREF crTransColor)
{
	HBITMAP		hMask			= NULL;
	HDC				hdcSrc		= NULL;
	HDC				hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	__try
	{
		BITMAP tBitmap;
		if (!(::GetObject(hBitmap, sizeof(tBitmap), &tBitmap)))
		{
			__leave;
		}
		DWORD dwWidth = tBitmap.bmWidth, dwHeight = tBitmap.bmHeight;
		if (dwWidth <= 0 || dwHeight <= 0)
		{
			__leave;
		}
		if (!(hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL)))
		{
			__leave;
		}

		hdcSrc	= ::CreateCompatibleDC(NULL);
		hdcDest	= ::CreateCompatibleDC(NULL);

		hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hBitmap);
		hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

		crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

		::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);
		//crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
		//::SetBkColor(hdcSrc, RGB(0, 0, 0));
		::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCINVERT);


		::SetBkColor(hdcSrc, crSaveBk);
		//::SetTextColor(hdcSrc, crSaveDestText);

		::SelectObject(hdcSrc, hbmSrcT);
		::SelectObject(hdcDest, hbmDestT);
	}
	__finally
	{
		if (hdcSrc)
		{
			::DeleteDC(hdcSrc);
			hdcSrc = NULL;
		}
		if (hdcDest)
		{
			::DeleteDC(hdcDest);
			hdcDest = NULL;
		}		
	}
	return hMask;
} 

#ifndef UNDER_CE

HANDLE WINAPI XUE_DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE hPal )
{
	/* Taken from http://www.codeguru.com/bitmap/ddb_to_dib.shtml */

	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwLen;
	HANDLE hDIB;
	HANDLE handle;
	HDC hDC;	

	ASSERT( bitmap);

	// The function has no arg for bitfields
	if ( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette	
	if (!hPal)
	{
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
	}

	// Get bitmap information
	::GetObject(bitmap, sizeof(bm),(LPVOID)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize               = sizeof(BITMAPINFOHEADER);
	bi.biWidth              = bm.bmWidth;
	bi.biHeight             = bm.bmHeight;
	bi.biPlanes             = 1;
	bi.biBitCount           = bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression        = dwCompression;
	bi.biSizeImage          = 0;
	bi.biXPelsPerMeter      = 0;
	bi.biYPelsPerMeter      = 0;
	bi.biClrUsed            = 0;
	bi.biClrImportant       = 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( bi.biBitCount > 8 ) // if ( nColors > 256 ) // by sswater
	{
		nColors = 0;
	}
	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB)
	{
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver
	// will calculate the biSizeImage field
	GetDIBits(hDC, bitmap, 0L, (DWORD)bi.biHeight,(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
			* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
		{
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
		}
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	{
		hDIB = handle;
	}
	else
	{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, bitmap, 0L,(DWORD)bi.biHeight, 	(LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)),	(LPBITMAPINFO)lpbi,(DWORD)DIB_RGB_COLORS);

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC, hPal,FALSE);
	::ReleaseDC(NULL, hDC);
	return hDIB;
}

#endif