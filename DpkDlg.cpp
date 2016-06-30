// DpkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "DpkDlg.h"
#include "HWToolDlg.h"


// CDpkDlg dialog

IMPLEMENT_DYNAMIC(CDpkDlg, CDialog)

CDpkDlg::CDpkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDpkDlg::IDD, pParent)
{
	//m_pAdoDB = new CAdoDB();
}

CDpkDlg::~CDpkDlg()
{
	//delete m_pAdoDB;
}

void CDpkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDpkDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_QUERY, &CDpkDlg::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_INJECT, &CDpkDlg::OnBnClickedInject)
END_MESSAGE_MAP()


// CDpkDlg message handlers

void CDpkDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CDpkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect m_rDataBox(40,160,420,180);
	CFont m_LogoFont;
	CString sLogoString = m_wszCpuInfo;
	sLogoString += m_bIsx64 ? TEXT(" | 64bits") : TEXT(" | 32bits");
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

BOOL CDpkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	/*
	CAdoDB db;
	int ret = db.Connect(TEXT("Provider=SQLOLEDB;Data Source=112.95.232.223;Initial Catalog=DISConfigurationCloud;User ID=sa;Password=!Flex123"),TEXT(""),TEXT(""),adModeUnknown);
	if (ret == 0)
	{
		ret = db.Open(TEXT("Configurations"));
	}
	if (ret == 0)
	{
	}
	*/
	_tcscpy(m_szTempDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(m_szTempDir,TEXT("\\Temp"));
	CCpuInfo ci;
	mbstowcs(m_wszCpuInfo,ci.GetBrand(),49);
	IsWow64Process(GetCurrentProcess(),&m_bIsx64);
	CHWToolDlg* pParent = (CHWToolDlg*)GetParent();
	CBiosInfo* pInfo = pParent->m_BiosInfo;
	SetDlgItemText(IDC_IBV,pInfo->m_BiosInfoW.m_wszIVN);
	SetDlgItemText(IDC_BIOSVER,pInfo->m_BiosInfoW.m_wszIV);
	SetDlgItemText(IDC_BIOSDATE,pInfo->m_BiosInfoW.m_wszID);
	SetDlgItemText(IDC_BIOSHASH,pInfo->m_BiosInfoW.m_wszSK);
	SetDlgItemText(IDC_PID,pInfo->m_BiosInfoW.m_wszSM);
	SetDlgItemText(IDC_MODEL,pInfo->m_BiosInfoW.m_wszSP);
	SetDlgItemText(IDC_SERIALNUM,pInfo->m_BiosInfoW.m_wszSS);
	SetDlgItemText(IDC_MBPID,pInfo->m_BiosInfoW.m_wszBM);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDpkDlg::OnBnClickedQuery()
{
	// TODO: Add your control notification handler code here
		CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)GetKeyThread,this,0,NULL));
}

void CDpkDlg::OnBnClickedInject()
{
	// TODO: Add your control notification handler code here
	char szKey[32]={0};

	SetCurrentDirectory(m_szTempDir);
	GetDlgItemTextA(m_hWnd,IDC_DPKSAVE,szKey,32);
	if (strlen(szKey)!=29)
	{
		MessageBox(TEXT("密钥长度不正确！"),TEXT("密钥工具"),MB_ICONERROR);
		return;
	}
	char szVerify[32]={0};
	strcpy(szVerify,szKey);
	char* p=strtok(szVerify,"-");
	char* p2 = p;
	int i=5;
	while (i--)
	{
		p2=p;
		if (strlen(p2) != 5)
		{
			MessageBox(TEXT("密钥格式不正确！"),TEXT("密钥工具"),MB_ICONERROR);
			return;
		}
		while (*p2)
		{
			if (!isalnum(*p2))
			{
				MessageBox(TEXT("密钥中含有非法字符！"),TEXT("密钥工具"),MB_ICONERROR);
				return;
			}
			p2++;
		}
		p = strtok(NULL,"-");
	}
	char szDPK[20]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x1d,0x00,0x00,0x00};
	CFile fp;
	if (!fp.Open(TEXT("tmp.bin"),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		MessageBox(TEXT("创建密钥文件失败！"),TEXT("密钥工具"),MB_ICONERROR);
		return;
	}
	fp.Write(szDPK,20);
	fp.Write(szKey,(UINT)strlen(szKey));
	fp.Close();
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)PutKeyThread,this,0,NULL));
}

void CDpkDlg::QueryProductKey()
{
	BOOL retval;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	HANDLE hReadPipe,hWritePipe;
	DWORD retcode = -1;
	char szKeyID[32]={0};
	CFile fp;
	DWORD dwLen,dwRead;

	sa.bInheritHandle=TRUE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	SetCurrentDirectory(m_szTempDir);
	if(retval)
	{
		si.cb=sizeof STARTUPINFOA;
		si.wShowWindow=SW_HIDE;
		si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
		si.hStdOutput=si.hStdError=hWritePipe;


		retval=CreateProcessA(NULL,"cmd.exe /c Check.exe",&sa,&sa,TRUE,0,NULL,0,&si,&pi);
		if(retval)
		{
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			GetExitCodeProcess(pi.hProcess,&retcode);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (retcode != 0)
			{
				dwLen=GetFileSize(hReadPipe,NULL);
				char *buff=new char [dwLen+1];
				memset(buff,0,dwLen+1);
				ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
				delete buff;
				retval=CreateProcessA(NULL,"powershell.exe (get-wmiobject win32_operatingsystem).serialnumber",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
				if(retval)
				{
					WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
					dwLen=GetFileSize(hReadPipe,NULL);
					retval=ReadFile(hReadPipe,szKeyID,dwLen,&dwRead,NULL);
					CStringA str(szKeyID);
					str.Remove('-');
					str = str.Mid(2);
					str = str.Left(13);
					memset(szKeyID,0,sizeof(szKeyID));
					strcpy(szKeyID,(LPCSTR)str);
					str.ReleaseBuffer();
				}
				SetDlgItemText(IDC_DPK,TEXT("无效OEM密钥"));
				SetDlgItemTextA(m_hWnd,IDC_PKID,szKeyID);
				goto end;
			}
			dwLen=GetFileSize(hReadPipe,NULL);
			char *buff=new char [dwLen+1];
			char dpk[30]={0};
			wchar_t wdpk[30]={0};
			char* vptr,*token="Product key:       ";
			memset(buff,0,dwLen+1);
			retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
			vptr=strstr(buff+700,token);
			if (vptr)
			{
				vptr +=strlen(token);
				strncpy(dpk,vptr,29);
			}
			mbstowcs(wdpk,dpk,29);
			SetDlgItemText(IDC_DPK,wdpk);
			SetDlgItemText(IDC_DPKSAVE,TEXT(""));
			delete buff;
		}
		retval=CreateProcessA(NULL,"cmd.exe /c oa3tool.exe /report /configfile=oa3toolfile.cfg",&sa,&sa,TRUE,0,NULL,0,&si,&pi);
		if(retval)
		{
			DWORD dwLen;
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			GetExitCodeProcess(pi.hProcess,&retcode);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (retcode)
			{
				SetDlgItemText(IDC_PKID,TEXT(""));
				goto end;
			}
			if (!fp.Open(TEXT("oa3.xml"),CFile::modeRead|CFile::typeBinary))
			{
				SetDlgItemText(IDC_PKID,TEXT(""));
				goto end;
			}
			dwLen=(DWORD)fp.GetLength();
			char* fBuff = new char[dwLen];
			char pkid[14]={0};
			wchar_t wpkid[14]={0};
			fp.Read(fBuff,dwLen);
			fp.Close();
			char* dpk=strstr(fBuff,"<ProductKeyID>");
			if (dpk)
			{
				strncpy(pkid,dpk+14,13);
				mbstowcs(wpkid,pkid,13);
				SetDlgItemText(IDC_PKID,wpkid);
			}
			delete fBuff;
		}
end:
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
	}
	else
	{
		SetDlgItemText(IDC_DPK,TEXT("无效OEM密钥"));
		SetDlgItemText(IDC_PKID,TEXT(""));
	}
}

void CDpkDlg::WriteProductKey()
{
	BOOL retval;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};

	SetCurrentDirectory(m_szTempDir);
	sa.bInheritHandle=FALSE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.hStdOutput=si.hStdError=NULL;
	retval=CreateProcessA(NULL,"cmd.exe /c afuwin.exe /oad",&sa,&sa,0,0,NULL,0,&si,&pi);
	if(retval)
	{
		WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	retval=CreateProcessA(NULL,"cmd.exe /c afuwin.exe /atmp.bin",&sa,&sa,0,0,NULL,0,&si,&pi);
	if(retval)
	{
		WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

UINT CDpkDlg::GetKeyThread(LPVOID lp)
{
	CDpkDlg* p = (CDpkDlg*)lp;
	p->GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow(0);
	p->GetDlgItem(IDC_QUERY)->EnableWindow(0);
	p->GetDlgItem(IDC_INJECT)->EnableWindow(0);
	EnableMenuItem(::GetSystemMenu(p->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_DISABLED);
	p->QueryProductKey();
	p->GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
	p->GetDlgItem(IDC_QUERY)->EnableWindow();
	p->GetDlgItem(IDC_INJECT)->EnableWindow();
	EnableMenuItem(::GetSystemMenu(p->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
	return 0L;
}

UINT CDpkDlg::PutKeyThread(LPVOID lp)
{
	CDpkDlg* p = (CDpkDlg*)lp;
	p->GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow(0);
	p->GetDlgItem(IDC_QUERY)->EnableWindow(0);
	p->GetDlgItem(IDC_INJECT)->EnableWindow(0);
	EnableMenuItem(::GetSystemMenu(p->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_DISABLED);
	p->WriteProductKey();
	p->QueryProductKey();
	p->GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
	p->GetDlgItem(IDC_QUERY)->EnableWindow();
	p->GetDlgItem(IDC_INJECT)->EnableWindow();
	EnableMenuItem(::GetSystemMenu(p->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
	return 0L;
}