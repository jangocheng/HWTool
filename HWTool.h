// HWTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "dminfo.h"
#pragma warning(disable:4101)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
// CHWToolApp:
// See HWTool.cpp for the implementation of this class
//
#define __MAC
#ifdef __MAC
#define _WIN32_DCOM
#include <comdef.h>
#include <atlbase.h>
#include <rpcsal.h>
// headers needed to use Mobile Broadband APIs 
#pragma comment(lib, "mbnapi_uuid.lib")
#include "mbnapi.h"
#pragma comment(lib, "IPHLPAPI.lib")
#include <iphlpapi.h>
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#endif

typedef struct _tagKeyInfo
{
	char BSN[32];
	char SSN[32];
	char PKID[32];
	char KEY[32];
	char WIFI[32];
	char BT[32];
	char IMEI[32];
	DWORD CRC;
}KeyInfo,*PKeyInfo;

typedef struct
{
	char pkid[32];
	char key[32];
}ProductKeyInfo;



class CBiosInfo
{
public:
	CBiosInfo();
	virtual ~CBiosInfo();
	void RebuildInfo();
	dm_infoA m_InfoA;
	dm_infoW m_InfoW;
	TCHAR  m_szTempDir[MAX_PATH];
};


class CHWToolApp : public CWinApp
{
public:
	CHWToolApp();
	HANDLE m_hMutex;
	CBiosInfo *m_BiosInfo;
// Overrides
	public:
	virtual BOOL InitInstance();
	void GetVersion(LPCTSTR szPEFile, CString &ver);
	void GetVersion(LPCTSTR szPEFile, LPDWORD pVer);

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CHWToolApp theApp;