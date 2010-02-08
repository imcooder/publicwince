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
	virtual ~CXUE_FontLock();	
	void SetColor(COLORREF clrTextColor);		
	void SetFontSize(LONG);
	void SetFontFaceName(LPCTSTR);
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


class CXUE_BkModeLock
{
public:	
	CXUE_BkModeLock(HDC hDC, BOOL);	
	virtual ~CXUE_BkModeLock();	
	void SetBkMode(BOOL);
protected:	
	HDC    m_hDC;
	LONG	 m_nBkModeOld; 	
};


class CXUE_CompatibleDCLock
{
public:		
	CXUE_CompatibleDCLock(HDC, HBITMAP hBitmap); 	
	virtual ~CXUE_CompatibleDCLock();
	HDC GetHDC();
protected:
	HBITMAP m_hOldBitmap; 
	HDC m_hMemDC;
};


class CXUE_MemDCLock
{
public:
	CXUE_MemDCLock(HDC hDestDC, const LPRECT prcPaint);
	virtual ~CXUE_MemDCLock();
	HDC	GetSafeHdc();
	BOOL Clear(COLORREF);
	BOOL SwapBuffer();
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
	HDC			m_hDC;
	HDC     m_hDestDC;   
	HBITMAP m_hBitmap;    
	RECT		m_rtPaint;      
	HGDIOBJ m_hOldBitmap;
	HRGN		m_hClipRgn;
	HRGN		m_hClipRgnOld;
};

class CXUE_PaintDCLock
{
public:		
	CXUE_PaintDCLock(HWND hWnd);
	virtual ~CXUE_PaintDCLock();
	HDC GetSafeHdc();
protected:
	HWND m_hWnd;
	PAINTSTRUCT m_ps;
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



#endif //HWX_HDCEX_H