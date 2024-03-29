#ifndef HWX_XTYPEEX_H
#define HWX_XTYPEEX_H

#include "Ext_Type.h"


#define SelectFont(hDC, hFont) \
   ((HFONT) ::SelectObject((hDC), (HGDIOBJ) (HFONT) (hFont)))


#ifndef HANDLE_MSG
#define HANDLE_MSG(m_hWnd, message, fn) \
   case (message): \
      return HANDLE_##message((m_hWnd), (wParam), (lParam), (fn))
#endif

#ifndef HANDLE_WM_COMMAND
#define HANDLE_WM_COMMAND(m_hWnd, wParam, lParam, fn) \
   ( (fn) ((m_hWnd), (int) (LOWORD(wParam)), (HWND)(lParam), (UINT) HIWORD(wParam)), 0L)
#endif



#ifdef _XSTRING_ //include <string>
#ifndef _TSTRING_DEFINED
#define _TSTRING_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring		TSTRING;
#else
typedef std::string		TSTRING;
#endif
#endif
#endif

#ifdef _SSTREAM_ //include <sstream>
#ifndef _TSTRINGSTREAM_DEFINED
#define _TSTRINGSTREAM_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstringstream		TSTRINGSTREAM;
#else
typedef std::stringstream		TSTRINGSTREAM;
#endif
#endif
#endif

#ifdef _IOSTREAM_ //include <iostream>
#ifndef _TCOUT_DEFINED
#define _TCOUT_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
#define TCOUT						std::wcout			
#else
#define TCOUT						std::cin	
#endif
#endif

#ifndef _TCIN_DEFINED
#define _TCIN_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
#define  TCIN						std::wcin	
#else
#define  TCIN						std::cin
#endif
#endif


#ifndef _TCLOG_DEFINED
#define _TCLOG_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
#define  TCLOG				std::wclog
#else
#define  TCLOG				std::clog
#endif
#endif

#ifndef _TCERR_DEFINED
#define _TCERR_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
#define TCERR				  std::wcerr
#else
#define TCERR				  std::cerr
#endif
#endif

#endif
#endif//HWXUE_EXT_TYPE_H_INC

