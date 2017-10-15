#pragma once
#include "afxwin.h"
#include "waitdlg.h"

typedef struct {
	wchar_t database[32];
	wchar_t business[32];
	wchar_t ip[32];
	wchar_t ip2[32];
	wchar_t password[32];
	int idx;
	wchar_t para[32];
	wchar_t sku[32];
	wchar_t maintype[32];
	wchar_t subtype[32];
	wchar_t hastouch[32];
	wchar_t screensize[32];
	wchar_t Id[40];
	int nUip;
	int nOAType;

}DpkCfg;

typedef struct
{
	wchar_t Name[40];
	wchar_t Id[8];
}Configuration;

typedef struct
{
	wchar_t Name[40];
	wchar_t Parameter[64];
}ParaIndex;

// CDisConfigDlg dialog

class CDisConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CDisConfigDlg)

public:
	CDisConfigDlg(DpkCfg* pCfg, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisConfigDlg();

// Dialog Data
	enum { IDD = IDD_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	int m_nIndex;
	HICON m_hIcon;
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedOk();
private:
	static UINT ProcessThread(LPVOID lpv);
	_ConnectionPtr m_pConn;
	_RecordsetPtr m_pRds;
	DpkCfg* m_pCfg;
	CList <Configuration, Configuration&> m_ConfigurationList;
	CList <ParaIndex, ParaIndex&> m_ParaLicList,m_ParaOemPOList,m_ParaOemPartList;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	int BuildConfiguration(void);
	int BuildParameter();
	afx_msg void OnCbnSelchangeBusiness();
	afx_msg void OnCbnSelchangeType();
};
