#pragma once


// CWaitDlg dialog

class CWaitDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitDlg)

public:
	CWaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaitDlg();

// Dialog Data
	enum { IDD = IDD_WAITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
