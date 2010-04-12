
#include "stdafx.h"
#include "ModuleEx.h"
#include "Ext_Type.h"
#include "HWDebug.h"

//////////////////////////////////////////////////////////////////////////

HWModule::HWModule() : m_hModule(NULL)
{		
	m_szModuleFilePath[0] = 0;
}
HWModule::~HWModule()
{
	FreeLibrary();
}

BOOL HWModule::LoadLibrary( LPCTSTR pszPath)
{
	BOOL blReturn = FALSE;
	FreeLibrary();
	if (m_hModule = ::LoadLibrary(pszPath))
	{		
		StringCchCopy(m_szModuleFilePath, _countof(m_szModuleFilePath), pszPath);
		blReturn = TRUE;
	}
	else
	{		
		HWTRACE(TEXT("LoadLibrary %s failed %08X\n"), pszPath, GetLastError());
	}	
	return blReturn;
}
void HWModule::FreeLibrary()
{		
	SAFE_FREE_LIBRARY(m_hModule);
	m_szModuleFilePath[0] = 0;
	m_blSucceed = FALSE;
}

BOOL HWModule::Succeed()
{
	return m_hModule != NULL;
}

HMODULE HWModule::GetSafeHModule()
{
	return m_hModule;
}

//////////////////////////////////////////////////////////////////////////