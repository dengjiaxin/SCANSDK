/***********************************************************************

	Copyright (c) Seiko Epson Corporation

	This document contains information that is the proprietary property of 
	Seiko EPSON Corporation and may not be copied, published, summarized, or 
	disclosed to others, or used for any  purpose other than review, without 
	the express written authorization of an officer of Seiko Epson Corp.  
	All information contained herein shall be held in the strict confidence 
	and in trust for the sole and exclusive benefit of Seiko Epson Corp.

 	All rights reserved.

	Date		Author	Reason
************************************************************************/
#pragma once

// Debugging functions and variables
#if defined(_DEBUG) || defined(DEBUG)
#define ES_TRACE(x)					MyOutputDebugString(x)
#define ES_TRACE0(x)				MyOutputDebugString(x)
#define ES_TRACE1(x, a)				MyOutputDebugString2(x, a)
#define ES_TRACE2(x, a, b)			MyOutputDebugString2(x, a, b)
#define ES_TRACE3(x, a, b, c)		MyOutputDebugString2(x, a, b, c)
#define ES_TRACE4(x, a, b, c, d)	MyOutputDebugString2(x, a, b, c, d)
#define ES_TRACE_FUNC(x)			CMyFuncTrace __myFuncTrace(x)
#define ASSERT(x)					if (!(x)) {MyOutputDebugString2(TEXT("[ERROR]ASSERT failed:%s(%d)\n"), __FILE__, __LINE__); DebugBreak();}
#define VERIFY(x)					ASSERT(x)
#else
#define ES_TRACE(x)
#define ES_TRACE0(x)
#define ES_TRACE1(x, a)
#define ES_TRACE2(x, a, b)
#define ES_TRACE3(x, a, b, c)
#define ES_TRACE4(x, a, b, c, d)
#define ES_TRACE_FUNC(x)
#define ASSERT(x)
#define VERIFY(x)	x
#endif

// Macros to dump command and data
#if defined(_DUMP)
#define DUMP(l, x, a)			MyOutputDumpData(l, x, a)
#else
#define DUMP(l, x, a)
#endif

// Macros to dump command and data
#if defined(_DUMP2)
#define DUMP2(l, x, a)			MyOutputDumpData2(l, x, a)
#else
#define DUMP2(l, x, a)
#endif

void MyOutputDebugString(LPCTSTR pszString);
void MyOutputDebugString2(LPCTSTR pszFormat, ...);
void MyOutputDumpData(LPCTSTR pszLabel, LPBYTE lpData, DWORD dwLength);
void MyOutputDumpData2(LPCTSTR pszLabel, LPBYTE lpData, DWORD dwLength);
void OutputMessage(LPCTSTR pszMsg);
void GetLogFileName(LPTSTR pszLog, long lSizeCount);
void GetTimeStamp(LPTSTR pszTime, long lSizeCount);

class CMyFuncTrace {
private:
	TCHAR	m_szFuncName[MAX_PATH];

public:
	inline CMyFuncTrace(LPCTSTR pszMsg) {
		StringCbCopy(m_szFuncName, sizeof(m_szFuncName), pszMsg);
		MyOutputDebugString2(TEXT("[FUNCTION] %s, entering\n"), m_szFuncName);
	}

	inline ~CMyFuncTrace() {
		MyOutputDebugString2(TEXT("[FUNCTION] %s, leaving\n"), m_szFuncName);
	}
};
