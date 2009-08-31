/********************************************************************
Copyright (c) 2002-2003 汉王科技有限公司. 版权所有.
文件名称: 	BitmapEx.h					
文件内容:			Bitmap简单处理
版本历史:	1.0
作者:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/

#ifndef HWX_BITMAP_H
#define HWX_BITMAP_H

#ifdef UNICODE

#define LoadBitmapFromFile		LoadBitmapFromFileW

#else 

#define LoadBitmapFromFile		LoadBitmapFromFileA

#endif


HBITMAP WINAPI LoadBitmapFromFileW(LPCWSTR);	
HBITMAP WINAPI LoadBitmapFromFileA(LPCSTR);


#endif //HWX_BITMAP_H