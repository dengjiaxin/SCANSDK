/***********************************************************************

	Copyright (c) Seiko Epson Corporation
 	All rights reserved.

	Date		Author			Reason
 ***********************************************************************/
#pragma once

typedef BOOL WINAPI tfeslInit(DWORD* pInstance, LPCSTR pszKey, DWORD* pErrCode);
typedef BOOL WINAPI tfeslFree(const DWORD Instance, DWORD* pErrCode);
typedef BOOL WINAPI tfeslOpenScanner(const DWORD Instance, const ConnectInfo* pScanner, DWORD* pErrCode);
typedef BOOL WINAPI tfeslCtrlParameter(const DWORD Instance, const eParamOpType OpType, EslParamHdr* pParam, DWORD* pErrCode);
typedef BOOL WINAPI tfeslExecute(const DWORD Instance, const eExecType ExecType, peslCallback pCbFunc, PVOID pPrivate, DWORD* pErrCode);
typedef BOOL WINAPI tfeslCloseScanner(const DWORD Instance, DWORD* pErrCode);

/*****************************************************************************
 * CEslStub class.
 *****************************************************************************/
class CEslStub {

public:
	CEslStub(LPCTSTR pszLibModule);
	~CEslStub();

	void	Exit(void);
	
	BOOL	eslInit(DWORD* pInstance, LPCSTR pszKey, DWORD* pErrCode);
	BOOL	eslFree(const DWORD Instance, DWORD* pErrCode);
	BOOL	eslOpenScanner(const DWORD Instance, const ConnectInfo* pScanner, DWORD* pErrCode);
	BOOL	eslCtrlParameter(const DWORD Instance, const eParamOpType OpType, EslParamHdr* pParam, DWORD* pErrCode);
	BOOL	eslExecute(const DWORD Instance, const eExecType ExecType, peslCallback pCbFunc, PVOID pPrivate, DWORD* pErrCode);
	BOOL	eslCloseScanner(const DWORD Instance, DWORD* pErrCode);

protected:

	HMODULE				m_hModule;

	tfeslInit*			m_peslInit;
	tfeslFree*			m_peslFree;
	tfeslOpenScanner*	m_peslOpenScanner;
	tfeslCtrlParameter*	m_peslCtrlParameter;
	tfeslExecute*		m_peslExecute;
	tfeslCloseScanner*	m_peslCloseScanner;

	BOOL		loadEslLibrary(LPCTSTR pszLibModule);
};
