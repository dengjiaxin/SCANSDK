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

#include "stdafx.h"
#include "DebugLog.h"

//#define _LOG_FILE
#define LOG_NAME			TEXT("DebugLog")

#define MAX_STR_SIZE 512
#define MAX_DUMPSIZE 1024

static bool g_bOutputPath = false;

void MyOutputDebugString(LPCTSTR pszString)
{
	TCHAR Buf[MAX_STR_SIZE];
	StringCbPrintf(Buf, sizeof(Buf), TEXT("%s"), pszString);
	OutputMessage(Buf);
}

void MyOutputDebugString2(LPCTSTR pszFormat, ...)
{
	va_list	argp;
	TCHAR Buf[MAX_STR_SIZE];
	va_start(argp, pszFormat);
	StringCbVPrintf(Buf, sizeof(Buf), pszFormat, argp);
	va_end(argp);
	OutputMessage(Buf);
}

void MyOutputDumpData(LPCTSTR pszLabel, LPBYTE lpData, DWORD dwLength)
{
	LPTSTR pszBuf = NULL;
	
	if (dwLength < MAX_DUMPSIZE) {
		DWORD count = lstrlen(pszLabel) + dwLength * 3 + 16;
		pszBuf = new TCHAR[count];
		if (pszBuf != NULL) {
			StringCchCopy(pszBuf, count, pszLabel);
			for (DWORD i = 0; i < dwLength; i++) {
				TCHAR szTmp[8] = {0};
				StringCbPrintf(szTmp, sizeof(szTmp), TEXT(" %02X"), lpData[i]);
				StringCchCat(pszBuf, count, szTmp);
			}
			StringCchCat(pszBuf, count, TEXT("\n"));
			OutputMessage(pszBuf);
			delete[] pszBuf;
		}
	} else {
		MyOutputDebugString2(TEXT("%s(%d bytes)\n"), pszLabel, dwLength);
	}
}

void MyOutputDumpData2(LPCTSTR pszLabel, LPBYTE lpData, DWORD dwLength)
{
	LPTSTR pszBuf = NULL;
	
	if (dwLength < MAX_DUMPSIZE) {
		DWORD count = lstrlen(pszLabel) + dwLength + 16;
		pszBuf = new TCHAR[count];
		if (pszBuf != NULL) {
			StringCchCopy(pszBuf, count, pszLabel);
			for (DWORD i = 0; i < dwLength; i++) {
				TCHAR szTmp[8] = {0};
				if (lpData[i] < 0x20) {
					StringCbCopy(szTmp, sizeof(szTmp), TEXT(" "));
				} else {
					StringCbPrintf(szTmp, sizeof(szTmp), TEXT("%C"), lpData[i]);
				}
				StringCchCat(pszBuf, count, szTmp);
			}
			StringCchCat(pszBuf, count, TEXT("\n"));
			OutputMessage(pszBuf);
			delete[] pszBuf;
		}
	} else {
		MyOutputDebugString2(TEXT("%s(%d bytes)\n"), pszLabel, dwLength);
	}

}

void OutputMessage(LPCTSTR pszMsg)
{
#if defined(_LOG_FILE)
	// It is output by a log file.
	TCHAR szLogName[MAX_PATH];
	if (GetTempPath(MAX_PATH, szLogName) > 0) {
		StringCchCat(szLogName, MAX_PATH, LOG_NAME);
		GetLogFileName(szLogName, MAX_PATH);
		if (!g_bOutputPath) {
			OutputDebugString(szLogName);
			g_bOutputPath = true;
		}
	}

	TCHAR szTimeStamp[32];
	GetTimeStamp(szTimeStamp, 32);

	FILE* pf = _tfopen(szLogName, TEXT("a+t"));
	if (pf) {
		_fputts(szTimeStamp, pf);
		_fputts(pszMsg, pf);
		fclose(pf);
	}
#else
	OutputDebugString(pszMsg);
#endif
}

void GetLogFileName(LPTSTR pszLog, long lSizeCount)
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	StringCchPrintf(pszLog, lSizeCount, TEXT("%s_%04d%02d%02d.log"), pszLog, sys_time.wYear, sys_time.wMonth, sys_time.wDay);
}

void GetTimeStamp(LPTSTR pszTime, long lSizeCount)
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	StringCbPrintf(pszTime, lSizeCount, TEXT("%02d:%02d:%02d.%04d: "), sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);
}

