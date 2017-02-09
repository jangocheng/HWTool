// HWToolDlg.h : header file
//

#pragma once
#include "BiosDlg.h"
#include "SnDlg.h"
#include "DpkDlg.h"
#include "CpuInfo.h"
#include "Oa3Dlg.h"

#pragma warning(disable:4996)

class CAmiTool
{
public:
	CAmiTool();
	virtual ~CAmiTool();
private:
	HANDLE m_hMutex;
	BOOL   m_bExist;
};


class CBytTool
{
public:
	CBytTool();
	virtual ~CBytTool();
private:
	HANDLE m_hMutex;
	BOOL   m_bExist;
};

class CChtTool86
{
public:
	CChtTool86();
	virtual ~CChtTool86();
private:
	HANDLE m_hMutex;
	BOOL   m_bExist;
};

class CChtTool64
{
public:
	CChtTool64();
	virtual ~CChtTool64();
private:
	HANDLE m_hMutex;
	BOOL   m_bExist;
};

class CAplTool64
{
public:
	CAplTool64();
	virtual ~CAplTool64();
private:
	HANDLE m_hMutex;
	BOOL   m_bExist;
};

// CHWToolDlg dialog
class CHWToolDlg : public CDialog
{
// Construction
public:
	CHWToolDlg(CWnd* pParent = NULL);	// standard constructor
	CAmiTool* m_AmiTool;
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
	CBiosDlg m_BiosDlg;
	CDpkDlg	m_DpkDlg;
	CSnDlg	m_SnDlg;
	COa3Dlg m_OaDlg;

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
