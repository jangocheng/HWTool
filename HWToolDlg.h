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

// CHWToolDlg dialog
class CHWToolDlg : public CDialog
{
// Construction
public:
	CHWToolDlg(CWnd* pParent = NULL);	// standard constructor
	CBiosInfo *m_BiosInfo;
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
