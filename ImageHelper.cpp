

#include "StdAfx.h"
#include "ImageHelper.h"
#define INITGUID
#include <initguid.h>
#include <imgguids.h>
#include <Imaging.h>


CXUE_ImageHelper::CXUE_ImageHelper(void)
: m_pbBuffer(NULL),
m_cbBuffer(0),
m_pImage(NULL)
{
	m_szFileName[0] = 0;
}


CXUE_ImageHelper::~CXUE_ImageHelper(void)
{
	ReleaseImage();
}

void CXUE_ImageHelper::ReleaseImage()
{	
	if (m_pImage)
	{
		m_pImage->Release();
		m_pImage = NULL;
		UpdateImageInfo();
	}
	if (m_pbBuffer)
	{
		LocalFree (m_pbBuffer);
		m_pbBuffer = NULL;
	}
	m_cbBuffer = 0;
	m_szFileName[0] = 0;
}


BOOL CXUE_ImageHelper::LoadImage(LPCTSTR pszFullName)
{
	BOOL blRet = TRUE;	
	HANDLE hFile = NULL;
	ReleaseImage();
	StringCchCopy (m_szFileName, _countof(m_szFileName), pszFullName);
	hFile = CreateFile(m_szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);	
	if (!hFile)
	{
		blRet = FALSE;
	}		
	if (blRet)
	{
		m_cbBuffer = GetFileSize (hFile, NULL);
		if (m_cbBuffer != 0xFFFFFFFF)
		{
			m_pbBuffer = static_cast<BYTE *>(LocalAlloc (LMEM_FIXED, m_cbBuffer));
			if (m_pbBuffer)
			{
				DWORD cbRead;
				if (!ReadFile (hFile, m_pbBuffer, m_cbBuffer, &cbRead, NULL) || (m_cbBuffer != cbRead))
				{
					LocalFree (m_pbBuffer);
					m_pbBuffer = NULL;
					blRet = FALSE;
				}						
			}
		}
	}	
	if (hFile)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}
	if (blRet)
	{
		blRet = CreateIImage();
	}	
	return blRet;
}

BOOL CXUE_ImageHelper::LoadImageFromRes( HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType )
{
	BOOL blRet = TRUE;
	HRSRC hRes = NULL;
	DWORD dwSize = 0;
	HGLOBAL hData = NULL;
	void*  pvRes = NULL;	
	ReleaseImage();

	hRes = FindResource(hModule, lpName, lpType);
	if (!hRes)
	{
		blRet = FALSE;
	}
	if (blRet)
	{
		dwSize = SizeofResource(hModule, hRes);
		hData = LoadResource(hModule, hRes);
		pvRes = (void*)LockResource(hData);		
		if (dwSize != 0xFFFFFFFF && dwSize)
		{
			m_cbBuffer = dwSize;
			m_pbBuffer = static_cast<BYTE *>(LocalAlloc (LMEM_FIXED, m_cbBuffer));
			if (m_pbBuffer)
			{
				memcpy(m_pbBuffer, pvRes, m_cbBuffer);					
			}
			else
			{
				m_pbBuffer = NULL;
				m_cbBuffer = NULL;
				blRet = FALSE;
			}		
		}	
	}	
	
	if (blRet)
	{
		blRet = CreateIImage();
	}	
	return blRet;	
}

BOOL CXUE_ImageHelper::LoadImageFromBuffer( LPVOID pvBuffer, DWORD dwSize )
{
	BOOL blRet = TRUE;
	if (!pvBuffer || !dwSize )
	{
		blRet = FALSE;
	}	
	ReleaseImage();
	if (blRet)
	{
		m_cbBuffer = dwSize;
		m_pbBuffer = static_cast<BYTE *>(LocalAlloc (LMEM_FIXED, m_cbBuffer));
		if (m_pbBuffer)
		{
			memcpy(m_pbBuffer, pvBuffer, m_cbBuffer);					
		}
		else
		{
			m_pbBuffer = NULL;
			m_cbBuffer = NULL;
			blRet = FALSE;
		}			
	}
	if (blRet)
	{
		blRet = CreateIImage();
	}
	return blRet;
}

BOOL CXUE_ImageHelper::CreateIImage()
{
	BOOL blRet = TRUE;
	IImagingFactory	*pImgFactory = NULL;
	if (!m_pbBuffer)
	{
		blRet = FALSE;
	}
	if (blRet)
	{
		if (!m_pImage)
		{	
			HRESULT hr = CoCreateInstance (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void **)&pImgFactory);
			if (SUCCEEDED (hr))
			{
				hr = pImgFactory->CreateImageFromBuffer (m_pbBuffer, m_cbBuffer, DISPOSAL_NONE, &m_pImage);
				pImgFactory->Release();
				UpdateImageInfo();				
			}		
			else
			{
				blRet = FALSE;
			}
		}
	}	
	return blRet;
}
BOOL CXUE_ImageHelper::Draw (HDC hDC, const RECT* dstRect,const RECT* srcRect)
{
	BOOL blRet = TRUE;
	if (!m_pImage)
	{
		blRet = FALSE;
	}
	if (blRet && m_pImage)
	{
		HRESULT hr = S_OK;		
		if (srcRect)
		{
			RECT rtImage = *srcRect;
			double dDotPermmX = m_imgInfo.Xdpi / 25.4;
			double dDotPermmY = m_imgInfo.Ydpi / 25.4;
			rtImage.left = (LONG)(rtImage.left * 2540.0 / m_imgInfo.Xdpi);
			rtImage.right = (LONG)(rtImage.right * 2540.0 / m_imgInfo.Xdpi);
			rtImage.top = (LONG)(rtImage.top * 2540.0 / m_imgInfo.Ydpi);
			rtImage.bottom = (LONG)(rtImage.bottom * 2540.0 / m_imgInfo.Ydpi);
			hr = m_pImage->Draw (hDC, dstRect, &rtImage);
		}
		else
		{
			hr = m_pImage->Draw (hDC, dstRect, NULL);
		}
		if (FAILED(hr))
		{
			blRet = FALSE;
		}			
	}
	return blRet;
}

SIZE CXUE_ImageHelper::GetImageSize()
{
	SIZE sz = {m_imgInfo.Width, m_imgInfo.Height};
	return sz;
}

BOOL CXUE_ImageHelper::UpdateImageInfo()
{
	BOOL blRet = TRUE;
	memset(&m_imgInfo, 0, sizeof(m_imgInfo));
	if (!m_pImage)
	{		
		blRet = FALSE;	
	}	
	if (blRet)
	{
		if (FAILED(m_pImage->GetImageInfo (&m_imgInfo)))
		{
			blRet = FALSE;	
		}		
	}
	return blRet;
}

LONG CXUE_ImageHelper::GetImageWidth()
{
	return m_imgInfo.Width;
}

LONG CXUE_ImageHelper::GetImageHeight()
{
	return m_imgInfo.Height;
}