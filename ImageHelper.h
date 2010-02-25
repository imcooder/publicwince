#ifndef HW_IMAGEHELPER_H
#define HW_IMAGEHELPER_H
#pragma once

#include <Imaging.h>

class CImagingHelper
{
public:
	CImagingHelper();
	virtual ~CImagingHelper();
	//
	void ReleaseImage();	
	//Create Image
	BOOL LoadImage(LPCTSTR pszFullName);	
	virtual BOOL LoadImageFromRes(HMODULE, LPCTSTR lpName, LPCTSTR lpType);
	virtual BOOL LoadImageFromBuffer(LPVOID pBuffer, DWORD dwSize);
	//
	


	//Draw
	BOOL Draw(HDC, const RECT*,const RECT* = NULL);
	static void DrawImage(HDC hdcDst, const RECT *prcDst, LPCTSTR pszFilename, bool bStretch = false, bool bStretchScale = false);
	//
	SIZE	GetImageSize();	
	LONG GetImageWidth();	
	LONG GetImageHeight();
protected:	
	BOOL CreateIImage();
	BOOL UpdateImageInfo();
protected:		
	IImage *m_pImage;					// Pointer to the IImaging class that does all the display work		
	//
	BYTE   *m_pbBuffer;				// in-memory buffer holding image data
	DWORD		m_cbBuffer;				// Size of buffer	
	//Info
	ImageInfo m_imgInfo;
	//
	TCHAR	m_szFileName[MAX_PATH];	// Full path to file being displayed	
};



#endif//HW_IMAGEHELPER_H