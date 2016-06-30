// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "WaitDlg.h"


// CWaitDlg dialog

IMPLEMENT_DYNAMIC(CWaitDlg, CDialog)

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
END_MESSAGE_MAP()


// CWaitDlg message handlers
