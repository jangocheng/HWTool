// HWTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


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



typedef struct 
{
	char m_szIVN[64];	//BIOS vendor name.
	char m_szIV[64];	//BIOS version
	char m_szID[64];	//BIOS release date
	char m_szSM[64];	//System manufacture
	char m_szSP[64];	//System product
	char m_szSV[64];	//System version
	char m_szSS[64];	//System Serial number
	char m_szSU[64];	//System UUID
	char m_szSK[64];	//System SKU number
	char m_szSF[64];	//System Family
	char m_szBM[64];	//Baseboard manufacture
	char m_szBP[64];	//Baseboard product
	char m_szBV[64];	//Baseboard version
	char m_szBS[64];	//Baseboard Serial number
}SmBiosInfoA;

typedef struct 
{
	wchar_t m_wszIVN[64];	//BIOS vendor name.
	wchar_t m_wszIV[64];	//BIOS version
	wchar_t m_wszID[64];	//BIOS release date
	wchar_t m_wszSM[64];	//System manufacture
	wchar_t m_wszSP[64];	//System product
	wchar_t m_wszSV[64];	//System version
	wchar_t m_wszSS[64];	//System Serial number
	wchar_t m_wszSU[64];	//System UUID		
	wchar_t m_wszSK[64];	//System SKU number
	wchar_t m_wszSF[64];	//System Family
	wchar_t m_wszBM[64];	//Baseboard manufacture
	wchar_t m_wszBP[64];	//Baseboard product
	wchar_t m_wszBV[64];	//Baseboard version
	wchar_t m_wszBS[64];	//Baseboard Serial number
}SmBiosInfoW;


class CBiosInfo
{
public:
	CBiosInfo();
	virtual ~CBiosInfo();
	void RebuildInfo();
	SmBiosInfoA m_BiosInfoA;
	SmBiosInfoW m_BiosInfoW;
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

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CHWToolApp theApp;