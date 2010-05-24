 /********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	XDC.h					
文件内容:	
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/19
*********************************************************************/

#ifndef HWX_HDCEX_H
#define HWX_HDCEX_H

class CXUE_BitmapLock
{
public:	
	CXUE_BitmapLock(HDC, HBITMAP hBitmap);  
	virtual ~CXUE_BitmapLock();
	void SetBitmap(HBITMAP);
protected:
	HDC     m_hDC;        
	HGDIOBJ m_hOldBitmap;
};

class CXUE_FontLock
{
public:
	CXUE_FontLock(HDC, HFONT = NULL);
	CXUE_FontLock(HDC, HFONT, COLORREF clrTextColor); 
	CXUE_FontLock(HDC, const LOGFONT*); 
	virtual ~CXUE_FontLock();	
	void SetColor(COLORREF clrTextColor);		
	void SetFontHeight(LONG);
	void SetFontFaceName(LPCTSTR);
	LONG GetFontHeight();
protected:
	void ReplaceCurrentFont(const LOGFONT*);
protected:
	HDC m_hDC;	
	HFONT m_hFont;
	HFONT m_hFontOld;
	COLORREF m_crOldTextColor; 
};
class CXUE_PenLock
{
public:
	CXUE_PenLock(HDC, HPEN);
	CXUE_PenLock(HDC hDC, INT fnPenStyle, INT nWidth,	COLORREF crColor); 
	virtual ~CXUE_PenLock();		
	void SetPenColor(COLORREF crColor); 
	void SetPenStyle(INT);
	void SetPenWidth(INT);
protected:
	void ReplaceCurrentPen(const LOGPEN*);
protected:	 
	HDC  m_hDC; 
	HPEN m_hPen;
	HPEN m_hOldPen; 
};

class CXUE_SolidBrushLock
{
public:	
	CXUE_SolidBrushLock(HDC hDC, HBRUSH);	
	CXUE_SolidBrushLock(HDC hDC, COLORREF crColor);	
	virtual ~CXUE_SolidBrushLock();	
	void SetBrushColor(COLORREF crColor);
protected:
	void ReplaceCurrentBrush(COLORREF);
protected:	
	HDC    m_hDC;
	HBRUSH m_hBrush; 
	HBRUSH m_hOldBrush; 
};

class CXUE_StockObjectLock
{
public:	
	CXUE_StockObjectLock(HDC hDC, int);	
	virtual ~CXUE_StockObjectLock();	
protected:	
	HDC    m_hDC;
	HGDIOBJ  m_hGdiObj; 
	HGDIOBJ m_hOldGdiObj; 
};

class CXUE_BkModeLock
{
public:	
	CXUE_BkModeLock(HDC hDC, int);	
	virtual ~CXUE_BkModeLock();	
	void SetBkMode(int);
protected:	
	HDC    m_hDC;
	LONG	 m_nBkModeOld; 	
};
class CXUE_SafeCompatibleBitmap
{
public:		
	CXUE_SafeCompatibleBitmap(HDC, INT, INT); 		
	virtual ~CXUE_SafeCompatibleBitmap();
	HBITMAP GetSafeHBitmap();
protected:	
protected:
	HBITMAP m_hBitmap;	
	HDC m_hDC;
};

class CXUE_SafeCompatibleDC
{
public:		
	CXUE_SafeCompatibleDC(HDC, HBITMAP hBitmap = NULL); 
	CXUE_SafeCompatibleDC(HDC, LPCTSTR);
	CXUE_SafeCompatibleDC(HDC, HINSTANCE, LONG); 
	virtual ~CXUE_SafeCompatibleDC();
	HDC GetSafeHdc();
	LONG GetHeight();
	LONG GetWidth();
protected:
	void Construct(HDC, HBITMAP);
	void Destruct();
protected:
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap; 
	HDC m_hMemDC;
	BITMAP  m_Bitmap;
};


class CXUE_SafeMemDC
{
public:	
	CXUE_SafeMemDC(HDC hDestDC, const LPRECT prcPaint = NULL);
	virtual ~CXUE_SafeMemDC();
	HDC	GetSafeHdc();
	BOOL Clear(COLORREF);
	BOOL SwapDC();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	void DrawDragRect(const LPRECT lpRect, SIZE size, const LPRECT lpRectLast, SIZE sizeLast, HBRUSH pBrush, HBRUSH pBrushLast);
	void FillSolidRect(LPCRECT lpRect, COLORREF clr);
	void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
	void Draw3dRect(LPCRECT lpRect,COLORREF clrTopLeft, COLORREF clrBottomRight);
	void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);
	//////////////////////////////////////////////////////////////////////////
	HBRUSH GetHalftoneBrush();
protected:
	void Construct(HDC hDestDC, const LPRECT);
protected:
	HDC			m_hDC;
	HDC     m_hDestDC;   
	HBITMAP m_hBitmap;    
	RECT		m_rtPaint;      
	HGDIOBJ m_hOldBitmap;	
};

class CXUE_SafePaintDC
{
public:		
	CXUE_SafePaintDC(HWND hWnd);
	virtual ~CXUE_SafePaintDC();
	HDC GetSafeHdc();
protected:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;
	HDC m_hDC;
};

class CXUE_SafeClientDC
{
public:		
	CXUE_SafeClientDC(HWND hWnd);
	virtual ~CXUE_SafeClientDC();
	HDC GetSafeHdc();
protected:
	HWND m_hWnd;
	HDC m_hDC;
};

class CXUE_SafeWindowDC
{
public:		
	CXUE_SafeWindowDC(HWND hWnd);
	virtual ~CXUE_SafeWindowDC();
	HDC GetSafeHdc();
protected:
	HWND m_hWnd;
	HDC m_hDC;
};
class CXUE_SetViewportOrgExLock
{
public:		
	CXUE_SetViewportOrgExLock(HDC, POINT); 	
	virtual ~CXUE_SetViewportOrgExLock();	
	void SetViewportOrgEx(POINT);
protected:	
	HDC m_hDC;
	POINT m_PointOld;
};
class CXUE_StretchBltModeLock
{
public:		
	CXUE_StretchBltModeLock(HDC, INT); 	
	virtual ~CXUE_StretchBltModeLock();	
	void SetStretchBltMode(INT);
protected:	
	HDC m_hDC;
	INT m_nStretchModeOld;
};

#endif //HWX_HDCEX_H