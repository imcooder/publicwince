

#ifndef HWX_HWDEVICE_H
#define HWX_HWDEVICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windef.h>


void WINAPI BacklightForce(BOOL);
void WINAPI BacklightLight();
LONG WINAPI XUE_GetSDID(LPTSTR pszID, LONG nLen);
BOOL WINAPI XUE_GetSDID2(UINT& uSDID);
BOOL WINAPI XUE_GetSDID3(UINT& uSDID);
#endif //HWX_HWDEVICE_H
