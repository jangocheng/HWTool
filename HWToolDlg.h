// HWToolDlg.h : header file
//

#pragma once
#include "CpuInfo.h"
#include "CloudOaDlg.h"

#pragma warning(disable:4996)


// CHWToolDlg dialog
class CHWToolDlg : public CDialog
{
// Construction
public:
	CHWToolDlg(CWnd* pParent = NULL);	// standard constructor
	CDialog* m_pDlg[4];
	CString m_szTitle;
// Dialog Data
	enum { IDD = IDD_HWTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CFont m_cFont;
	CImageList m_ImgList;
	HICON m_hIconFw,m_hIconSn,m_hIconKey,m_hIconOa;
	CCloudOADlg m_CloudOADlg;
	TCHAR  m_szINI[MAX_PATH];
	int m_iOSVer, m_iToolVer;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL IsAllowPerformed();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
