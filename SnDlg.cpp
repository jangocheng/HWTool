// SnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "SnDlg.h"
#include "HWToolDlg.h"


// CSnDlg dialog

IMPLEMENT_DYNAMIC(CSnDlg, CDialog)

CSnDlg::CSnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnDlg::IDD, pParent)
{

}

CSnDlg::~CSnDlg()
{
}

void CSnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSnDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_UPDATE, &CSnDlg::OnBnClickedUpdate)
END_MESSAGE_MAP()


// CSnDlg message handlers

void CSnDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect m_rDataBox(40,160,420,180);
	CFont m_LogoFont;
	CString sLogoString = m_wszCpuInfo;
	sLogoString += m_bIsx64 ? TEXT(" - 64bits") : TEXT(" - 32bits");
  	m_LogoFont.CreateFont(18, 0, 0, 0, FW_BOLD, 1, FALSE, FALSE,
	  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	  FIXED_PITCH | FF_ROMAN, TEXT("Times New Roman"));

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	CFont* oldFont = dc.SelectObject(&m_LogoFont);
	CSize sz = dc.GetTextExtent(sLogoString, sLogoString.GetLength());
	// GetTextExtent calculates the size of the displayed logo
	// which depends on the device context....
	// Calculate the box size by subtracting the text width and height from the
	// window size.  Also subtract 20% of the average character size to keep the
	// logo from printing into the borders...
	m_rDataBox.left = m_rDataBox.right  - sz.cx - tm.tmAveCharWidth/2;
	m_rDataBox.top  = m_rDataBox.bottom - sz.cy - tm.tmHeight/5;
	dc.SetBkMode(TRANSPARENT);
	// shift logo box right, and print black...
	COLORREF oldColor = dc.SetTextColor(RGB(255,255,255));
	dc.DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// shift logo box left and print white
	m_rDataBox.left -= 3*tm.tmAveCharWidth/5;
	dc.SetTextColor(RGB(0,0,0));
	dc.DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// Restore original location and print in the button face color
	m_rDataBox.left += tm.tmAveCharWidth/5;
	dc.SetTextColor(RGB(224,64,64));
	dc.DrawText(sLogoString, sLogoString.GetLength(), &m_rDataBox, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	// restore the original properties and release resources...
	dc.SelectObject(oldFont);
	dc.SetTextColor(oldColor); 


}
void CSnDlg::OnBnClickedUpdate()
{
	// TODO: Add your control notification handler code here
	BOOL retval;
	char buff[256]={0},snbuff[32]={0};
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	CHWToolDlg* pParent = (CHWToolDlg*)((CHWToolApp*)AfxGetApp())->m_pMainWnd;
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW;

	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_DISABLED);
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow(0);
	GetDlgItem(IDC_UPDATE)->EnableWindow(0);
	GetDlgItem(IDC_FW)->EnableWindow(0);

	GetDlgItemTextA(m_hWnd,IDC_FW,snbuff,32);
	SetCurrentDirectory(m_szTempDir);
	if (strlen(snbuff))
	{
		strcpy(buff,"cmd.exe /c amidewin.exe /ss \"");
		strcat(buff,snbuff);
		strcat(buff,"\"");
		retval=CreateProcessA(NULL,buff,&sa,&sa,0,0,NULL,NULL,&si,&pi);
		WaitForSingleObject(pi.hThread,INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else
	{
		MessageBox(TEXT("要刷写的序列号不能为空！"),TEXT("序列号刷写"),MB_ICONERROR);
		goto end;
	}
	if (1)
	{
		char buff[256] = {0};
		CBiosInfo* pInfo = ((CHWToolApp*)AfxGetApp())->m_BiosInfo;
		if (strncmp(pInfo->m_BiosInfoA.m_szSU,"00020003000400050006000700080009",32) == 0)
		{
			strcpy(buff,"cmd.exe /c amidewin.exe /su \"");
			GUID guid;
			CoCreateGuid(&guid);
			memset(pInfo->m_BiosInfoA.m_szSU,0,sizeof(pInfo->m_BiosInfoA.m_szSU));
			sprintf(pInfo->m_BiosInfoA.m_szSU,"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",guid.Data1,guid.Data2,guid.Data3,guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]);
			mbstowcs(pInfo->m_BiosInfoW.m_wszSU,pInfo->m_BiosInfoA.m_szSU,64);
			pParent->m_pDlg[1]->SetDlgItemText(IDC_UUID,pInfo->m_BiosInfoW.m_wszSU);
			strcat(buff,pInfo->m_BiosInfoA.m_szSU);
			strcat(buff,"\"");
			retval=CreateProcessA(NULL,buff,&sa,&sa,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	Sleep(500);
	CBiosInfo* pInfo = ((CHWToolApp*)AfxGetApp())->m_BiosInfo;
	pInfo->RebuildInfo();
	SetDlgItemText(IDC_IBV,pInfo->m_BiosInfoW.m_wszIVN);
	SetDlgItemText(IDC_BIOSVER,pInfo->m_BiosInfoW.m_wszIV);
	SetDlgItemText(IDC_BIOSDATE,pInfo->m_BiosInfoW.m_wszID);
	SetDlgItemText(IDC_BIOSHASH,pInfo->m_BiosInfoW.m_wszSK);
	SetDlgItemText(IDC_PID,pInfo->m_BiosInfoW.m_wszSM);
	SetDlgItemText(IDC_MODEL,pInfo->m_BiosInfoW.m_wszSP);
	SetDlgItemText(IDC_SERIALNUM,pInfo->m_BiosInfoW.m_wszSS);
	SetDlgItemText(IDC_MBPID,pInfo->m_BiosInfoW.m_wszBM);
	SetDlgItemText(IDC_UUID,pInfo->m_BiosInfoW.m_wszSU);
	pParent->m_pDlg[0]->SetDlgItemText(IDC_SERIALNUM,pInfo->m_BiosInfoW.m_wszSS);
	pParent->m_pDlg[2]->SetDlgItemText(IDC_SERIALNUM,pInfo->m_BiosInfoW.m_wszSS);
end:
	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
	GetDlgItem(IDC_UPDATE)->EnableWindow();
	GetDlgItem(IDC_FW)->EnableWindow();
	GetDlgItem(IDC_FW)->SetFocus();
}

BOOL CSnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	_tcscpy(m_szTempDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(m_szTempDir,TEXT("\\Temp"));
	CCpuInfo ci;
	mbstowcs(m_wszCpuInfo,ci.GetBrand(),49);
	IsWow64Process(GetCurrentProcess(),&m_bIsx64);
	CBiosInfo* pInfo = ((CHWToolApp*)AfxGetApp())->m_BiosInfo;
	SetDlgItemText(IDC_IBV,pInfo->m_BiosInfoW.m_wszIVN);
	SetDlgItemText(IDC_BIOSVER,pInfo->m_BiosInfoW.m_wszIV);
	SetDlgItemText(IDC_BIOSDATE,pInfo->m_BiosInfoW.m_wszID);
	SetDlgItemText(IDC_BIOSHASH,pInfo->m_BiosInfoW.m_wszSK);
	SetDlgItemText(IDC_PID,pInfo->m_BiosInfoW.m_wszSM);
	SetDlgItemText(IDC_MODEL,pInfo->m_BiosInfoW.m_wszSP);
	SetDlgItemText(IDC_SERIALNUM,pInfo->m_BiosInfoW.m_wszSS);
	SetDlgItemText(IDC_MBPID,pInfo->m_BiosInfoW.m_wszBM);
	SetDlgItemText(IDC_UUID,pInfo->m_BiosInfoW.m_wszSU);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSnDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnCancel();
}

void CSnDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}
