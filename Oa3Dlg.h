#pragma once
#include "DisConfigDlg.h"
//#define __MAC
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
// COa3Dlg dialog
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

class COa3Dlg : public CDialog
{
	DECLARE_DYNAMIC(COa3Dlg)

public:
	COa3Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COa3Dlg();

// Dialog Data
	enum { IDD = IDD_OADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	DWORD CRC32(DWORD crc,BYTE *buffer, DWORD size);
	void init_crc_table();
	BOOL GetProductKey();
	DWORD CRC_Table[256];
	TCHAR  m_szTempDir[MAX_PATH];
	SOCKET m_socket;
	ProductKeyInfo m_pkInfo;
	KeyInfo m_KeyInfo;

private:
	DpkCfg m_cfg;
	char m_szIP[32],m_szPara[32],m_szParaValue[32],m_szSKU[32],m_szType[32],m_szSubType[32],m_szScreenSize[32],m_szTouch[32],m_szSN[32],m_szCloud[40];
public:
	virtual BOOL OnInitDialog();
	int m_nIndex;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ProcessKeyInjection();
	static UINT KeyThread(LPVOID lpv);
	int Reboot();
	static UINT KeyEraseThread(LPVOID lpv);
	void EraseKey();
#ifdef __MAC
	BOOL GetDeviceAddress();
	BOOL GetIMEI();
#endif
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedInject();
	afx_msg void OnBnClickedErase();
};
