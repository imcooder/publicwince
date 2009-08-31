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

#define LoadBitmapFromFile		LoadBitmapFromFileW

#else 

#define LoadBitmapFromFile		LoadBitmapFromFileA

#endif


HBITMAP WINAPI LoadBitmapFromFileW(LPCWSTR);	
HBITMAP WINAPI LoadBitmapFromFileA(LPCSTR);


#endif //HWX_BITMAP_H