#pragma once


// CSnDlg dialog

class CSnDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnDlg)

public:
	CSnDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnDlg();

// Dialog Data
	enum { IDD = IDD_SN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	TCHAR  m_szTempDir[MAX_PATH];
	wchar_t m_wszCpuInfo[49];
	BOOL m_bIsx64;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedUpdate();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();
};
