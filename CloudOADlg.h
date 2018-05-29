#pragma once
#include "DisConfigDlg.h"

// CCloudOADlg dialog

class CCloudOADlg : public CDialog
{
	DECLARE_DYNAMIC(CCloudOADlg)

public:
	CCloudOADlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCloudOADlg();

// Dialog Data
	enum { IDD = IDD_CLOUDOADLG };

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
	char m_szOATool[32];

private:
	DpkCfg m_cfg;
	char m_szIP[32],m_szPara[32],m_szParaValue[32],m_szSKU[32],m_szType[32],m_szSubType[32],m_szScreenSize[32],m_szTouch[32],m_szTrackInfo[32],m_szSN[32],m_szCloud[40];
public:
	virtual BOOL OnInitDialog();
	int m_nIndex, m_nCheckIp;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ProcessKeyInjection();
	static UINT KeyThread(LPVOID lpv);
	int Reboot();
	static UINT KeyEraseThread(LPVOID lpv);
	void EraseKey();
	BOOL GetOA3Parameter(char* szCmd, int nIdx);
#ifdef __MAC
	BOOL GetDeviceAddress();
	BOOL GetIMEI();
#endif
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedInject();
	afx_msg void OnBnClickedErase();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnBnClickedGip();
	afx_msg void OnIpnFieldchangedServip2(NMHDR *pNMHDR, LRESULT *pResult);
};
