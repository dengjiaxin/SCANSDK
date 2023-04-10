/***********************************************************************

	Copyright (c) Seiko Epson Corporation
 	All rights reserved.

	Date		Author			Reason
 ***********************************************************************/
#include "stdafx.h"
#include "EslStub.h"
#include <Shlobj.h>

#define	SCANSDK_DLL	TEXT("\\EPSON\\SCANSDK_V10\\BIN\\ESCANLIB.DLL")

CEslStub::CEslStub(LPCTSTR pszLibModule) :
m_hModule(NULL),
m_peslInit(NULL),
m_peslFree(NULL),
m_peslOpenScanner(NULL),
m_peslCtrlParameter(NULL),
m_peslExecute(NULL),
m_peslCloseScanner(NULL)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::CEslStub"));

	if (!loadEslLibrary(pszLibModule)) {
		return;
	}	

	m_peslInit = (tfeslInit*)GetProcAddress(m_hModule, "eslInit");
	m_peslFree = (tfeslFree*)GetProcAddress(m_hModule, "eslFree");
	m_peslOpenScanner = (tfeslOpenScanner*)GetProcAddress(m_hModule, "eslOpenScanner");
	m_peslCtrlParameter = (tfeslCtrlParameter*)GetProcAddress(m_hModule, "eslCtrlParameter");
	m_peslExecute = (tfeslExecute*)GetProcAddress(m_hModule, "eslExecute");
	m_peslCloseScanner = (tfeslCloseScanner*)GetProcAddress(m_hModule, "eslCloseScanner");

	if (m_peslInit && m_peslFree && m_peslOpenScanner && m_peslCtrlParameter
		&& m_peslExecute && m_peslCloseScanner) {
		ES_TRACE(TEXT("[INFO]Required function pointers READY\n"));
		return;
	} else {
		ES_TRACE(TEXT("[ERROR]Required function pointers NOT READY\n"));
		Exit();
	}
}

CEslStub::~CEslStub()
{
//	ES_TRACE_FUNC(TEXT("CEslStub::~CEslStub"));
	
	Exit();
}

void CEslStub::Exit()
{ 
//	ES_TRACE_FUNC(TEXT("CEslStub::Exit"));

	if (m_hModule) {
		FreeLibrary(m_hModule);
		m_hModule = NULL;

		m_peslInit = NULL;
		m_peslFree = NULL;
		m_peslOpenScanner = NULL;
		m_peslCtrlParameter = NULL;
		m_peslExecute = NULL;
		m_peslCloseScanner = NULL;
	}
}

BOOL CEslStub::eslInit(DWORD* pInstance, LPCSTR pszKey, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslInit"));

	if (NULL == m_peslInit) {
		ES_TRACE(TEXT("[ERROR]m_peslInit is NULL\n"));
		return FALSE;
	}
	return m_peslInit(pInstance, pszKey, pErrCode);
}

BOOL CEslStub::eslFree(const DWORD Instance, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslFree"));

	if (NULL == m_peslFree) {
		ES_TRACE(TEXT("[ERROR]m_peslFree is NULL\n"));
		return FALSE;
	}
	return m_peslFree(Instance, pErrCode);
}

BOOL CEslStub::eslOpenScanner(const DWORD Instance, const ConnectInfo* pScanner, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslOpenScanner"));

	if (NULL == m_peslOpenScanner) {
		ES_TRACE(TEXT("[ERROR]m_peslOpenScanner is NULL\n"));
		return FALSE;
	}
	return m_peslOpenScanner(Instance, pScanner, pErrCode);
}

BOOL CEslStub::eslCtrlParameter(const DWORD Instance, const eParamOpType OpType, EslParamHdr* pParam, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslCtrlParameter"));

	if (NULL == m_peslCtrlParameter) {
		ES_TRACE(TEXT("[ERROR]m_peslCtrlParameter is NULL\n"));
		return FALSE;
	}
	return m_peslCtrlParameter(Instance, OpType, pParam, pErrCode);
}

BOOL CEslStub::eslExecute(const DWORD Instance, const eExecType ExecType, peslCallback pCbFunc, PVOID pPrivate, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslExecute"));

	if (NULL == m_peslExecute) {
		ES_TRACE(TEXT("[ERROR]m_peslExecute is NULL\n"));
		return FALSE;
	}
	return m_peslExecute(Instance, ExecType, pCbFunc, pPrivate, pErrCode);
}

BOOL CEslStub::eslCloseScanner(const DWORD Instance, DWORD* pErrCode)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::eslCloseScanner"));

	if (NULL == m_peslCloseScanner) {
		ES_TRACE(TEXT("[ERROR]m_peslCloseScanner is NULL\n"));
		return FALSE;
	}
	return m_peslCloseScanner(Instance, pErrCode);
}

BOOL CEslStub::loadEslLibrary(LPCTSTR pszLibModule)
{
//	ES_TRACE_FUNC(TEXT("CEslStub::loadEslLibrary"));
	
	TCHAR szLibPath[MAX_PATH];
	if (pszLibModule == NULL) {
		if (SUCCEEDED(::SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, szLibPath))) {
			::PathAppend(szLibPath, SCANSDK_DLL);
		}
	} else {
		::StringCbCopy(szLibPath, sizeof(szLibPath), TEXT("ESCANLIB.DLL"));
	}

	if (m_hModule == NULL) {
		m_hModule = LoadLibrary(szLibPath);	
		if (m_hModule == NULL) {
			ES_TRACE1(TEXT("[ERROR]LoadLibrary failed: %x\n"), ::GetLastError());
			return FALSE;
		}
	}

	return TRUE;
}
