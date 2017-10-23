// HWToolDlg.h : header file
//

#pragma once
#include "CpuInfo.h"
#include "Oa3Dlg.h"
#include "CloudOaDlg.h"

#pragma warning(disable:4996)


// CHWToolDlg dialog
class CHWToolDlg : public CDialog
{
// Construction
public:
	CHWToolDlg(CWnd* pParent = NULL);	// standard constructor
	CDialog* m_pDlg[4];
// Dialog Data
	enum { IDD = IDD_HWTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CTabCtrl* m_pTab;
	CImageList m_ImgList;
	HICON m_hIconFw,m_hIconSn,m_hIconKey,m_hIconOa;
	COa3Dlg m_OaDlg;
	CCloudOADlg m_CloudOADlg;
	TCHAR  m_szINI[MAX_PATH];

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
};
