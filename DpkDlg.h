#pragma once
#include "AdoDB.h"


// CDpkDlg dialog

class CDpkDlg : public CDialog
{
	DECLARE_DYNAMIC(CDpkDlg)

public:
	CDpkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDpkDlg();

// Dialog Data
	enum { IDD = IDD_DPK };
private:
	CAdoDB* m_pAdoDB;
	_RecordsetPtr m_pRds;
	TCHAR  m_szTempDir[MAX_PATH];
	wchar_t m_wszCpuInfo[49];
	BOOL m_bIsx64;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedQuery();
	afx_msg void OnBnClickedInject();
	void QueryProductKey();
	void WriteProductKey();
	static UINT GetKeyThread(LPVOID lp);
	static UINT PutKeyThread(LPVOID lp);
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
