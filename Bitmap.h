/********************************************************************
Copyright (c) 2002-2003 �����Ƽ����޹�˾. ��Ȩ����.
�ļ�����: 	BitmapEx.h					
�ļ�����:			Bitmap�򵥴���
�汾��ʷ:	1.0
����:		xuejuntao xuejuntao@hanwang.com.cn 2008/08/04
*********************************************************************/

#ifndef HWX_BITMAP_H
#define HWX_BITMAP_H

#ifdef UNICODE

#define LoadBitmapFromFile		XUE_LoadBitmapFromFileW

#else 

#define LoadBitmapFromFile		XUE_LoadBitmapFromFileA

#endif


HBITMAP WINAPI XUE_LoadBitmapFromFileW(LPCWSTR);	
HBITMAP WINAPI XUE_LoadBitmapFromFileA(LPCSTR);
void*		WINAPI XUE_LoadResource(HMODULE, LPCTSTR lpName, LPCTSTR lpType, LPDWORD);


#endif //HWX_BITMAP_H