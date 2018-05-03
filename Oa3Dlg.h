#pragma once
#include "DisConfigDlg.h"

// COa3Dlg dialog
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
	char m_szOATool[32];

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
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
