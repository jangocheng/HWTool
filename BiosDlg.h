#pragma once


// CBiosDlg dialog

class CBiosDlg : public CDialog
{
	DECLARE_DYNAMIC(CBiosDlg)

public:
	CBiosDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBiosDlg();

// Dialog Data
	enum { IDD = IDD_BIOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL  m_bExistKey;
	UINT  m_nBiosSize;
	CString m_szPath,m_strSSN;
	int   m_nSN;
	TCHAR  m_szTempDir[MAX_PATH],m_curPath[2048];
	wchar_t m_wszCpuInfo[49];
	int   m_nType;
	BOOL m_bIsx64;
	SYSTEM_POWER_STATUS Sps;
	DECLARE_MESSAGE_MAP()
public:
	static UINT UpdateThread(LPVOID lp);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL OnInitDialog();
	int UpdateBios(void);
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
