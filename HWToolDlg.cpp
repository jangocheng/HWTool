// HWToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "HWToolDlg.h"
#include "xDisplayDevice.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHWToolDlg dialog




CHWToolDlg::CHWToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHWToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_OA);
}

void CHWToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHWToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CHWToolDlg::OnTcnSelchangeTab1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHWToolDlg message handlers

BOOL CHWToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	wchar_t filepath[MAX_PATH] = {0};
	GetModuleFileName(NULL,filepath,MAX_PATH);
	(wcsrchr(filepath,TEXT('\\')))[1] = 0;
	_tcscpy(m_szINI,filepath);
	_tcscat(m_szINI,TEXT("cloud.ini"));

	CRect rc;
	m_pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	//m_pTab->SetImageList(&m_ImgList);
	//m_pTab->InsertItem(0,TEXT("DIS3.0"),0);
	//m_pTab->InsertItem(1,TEXT("MDOS"),1);
	m_pTab->GetClientRect(&rc);
	m_pTab->ShowWindow(SW_HIDE);

	m_OaDlg.Create(IDD_OADLG,this);
	//m_OaDlg.SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE);
	m_CloudOADlg.Create(IDD_CLOUDOADLG,this);
	//m_CloudOADlg.SetWindowPos(NULL,0,0,0,0,SWP_NOSIZE);

	m_pDlg[0]=&m_OaDlg;
	m_pDlg[1]=&m_CloudOADlg;
	for (int i=0; i< 2; i++)
	{
		m_pDlg[i]->SetWindowPos(NULL,0,rc.bottom,0,0,SWP_NOSIZE);
	}
	int sel = GetPrivateProfileInt(TEXT("CLOUD"),TEXT("SELECT"),1,m_szINI);
	m_pTab->SetCurSel(sel);
	m_pDlg[sel]->ShowWindow(SW_SHOW);
	//DWORD dwWidth,dwHeight;
	//XDD_GetActiveMonitorPhysicalSize(dwWidth,dwHeight);
    //int nScreenWidth, nScreenHeight; 
	//HDC hdcScreen = ::GetDC(NULL);   //获取屏幕的HDC 
    //nScreenWidth = GetDeviceCaps(hdcScreen, HORZSIZE); 
    //nScreenHeight = GetDeviceCaps(hdcScreen, VERTSIZE); 
	//::ReleaseDC(NULL, hdcScreen); 
	BOOL retval,result=FALSE;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	HANDLE hReadPipe,hWritePipe;
	DWORD retcode = -1;
	char szVer[32] = {0};
	char szOSTitle[256] = { 0 };
	int iVer;
	char szCmdTool[32] = {"oa3tool_"};
	char szOSInfo[255];
	BOOL bIsx64;

	sa.bInheritHandle=TRUE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);

	si.cb=sizeof STARTUPINFO;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.hStdOutput=si.hStdError=hWritePipe;

	retval=CreateProcessA(NULL,"powershell.exe (Get-WmiObject Win32_OperatingSystem).Caption",&sa,&sa,TRUE,0,NULL,0,&si,&pi);
	if(retval)
	{
		DWORD dwLen,dwRead;
		WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
		GetExitCodeProcess(pi.hProcess,&retcode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		dwLen=GetFileSize(hReadPipe,NULL);
		char *buff=new char [dwLen+1];
		memset(buff,0,dwLen+1);
		retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
		if (buff)
		{
			strncpy(szOSTitle, buff,strlen(buff)-2);
		}
		delete buff;
	}

	retval=CreateProcessA(NULL,"powershell.exe (Get-WmiObject Win32_OperatingSystem).BuildNumber",&sa,&sa,TRUE,0,NULL,0,&si,&pi);
	if(retval)
	{
		DWORD dwLen,dwRead;
		WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
		GetExitCodeProcess(pi.hProcess,&retcode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		dwLen=GetFileSize(hReadPipe,NULL);
		char *buff=new char [dwLen+1];
		memset(buff,0,dwLen+1);
		retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
		if (buff)
		{
			strncpy(szVer, buff,strlen(buff)-2);
		}
		delete buff;
	}

	IsWow64Process(GetCurrentProcess(),&bIsx64);
	iVer = atoi(szVer);
	if (iVer > 16299)
	{
		MessageBox(TEXT("操作系统版本过旧，请更新系统至RS3或以上版本！"), TEXT("错误"), MB_ICONERROR);
		PostQuitMessage(0);
		return FALSE;
	}

	sprintf(szOSInfo, "%s(%s)", szOSTitle, szVer);
	SetWindowText(m_szTitle);
	SetDlgItemTextA(m_hWnd,IDC_OSVER, szOSInfo);

	if (bIsx64)
	{
		strcat(szCmdTool,"x64.exe");
	}
	else
	{
		strcat(szCmdTool,"x86.exe");
	}

	strcpy(m_OaDlg.m_szOATool,szCmdTool);
	strcpy(m_CloudOADlg.m_szOATool,szCmdTool);

	if (_access(szCmdTool,0) == -1)
	{
		MessageBox(TEXT("缺少OA3刷KEY文件，请联系软件提供商提供此文件！"),TEXT("错误"),MB_ICONERROR);
		PostQuitMessage(0);
	}
	DWORD ver=0;
	WCHAR wszCmdTool[32] = { 0 };
	mbstowcs(wszCmdTool, szCmdTool, 32);
	((CHWToolApp*)AfxGetApp())->GetVersion(wszCmdTool, &ver);
	if (ver < 16299)
	{
		MessageBox(TEXT("oa3tool.exe版本过旧，请更新系统至RS3或以上版本！"),TEXT("错误"),MB_ICONERROR);
		PostQuitMessage(0);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHWToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHWToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHWToolDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int sel = m_pTab->GetCurSel();
	for (int i=0; i< 2; i++)
	{
		m_pDlg[i]->ShowWindow(SW_HIDE);
	}
	m_pDlg[sel]->ShowWindow(SW_SHOW);
	m_pDlg[sel]->SetFocus();
	CString szSel;
	szSel.Format(TEXT("%d"),sel);
	WritePrivateProfileString(TEXT("CLOUD"),TEXT("SELECT"),szSel,m_szINI);
}

BOOL CHWToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CHWToolDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}
