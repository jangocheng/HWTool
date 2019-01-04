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

	m_CloudOADlg.Create(IDD_CLOUDOADLG,this);
	m_CloudOADlg.SetWindowPos(NULL,0,20,0,0,SWP_NOSIZE);
	int sel = GetPrivateProfileInt(TEXT("CLOUD"),TEXT("SELECT"),1,m_szINI);

	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);
	m_cFont.CreateFontIndirect(&lf);
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
	wchar_t wszOSInfo[256] = { 0 };
	char szCmdTool[32] = {"oa3tool_"};
	char szOSInfo[255];
	BOOL bIsx64;

	m_iOSVer = m_iToolVer = 0;
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
			m_iOSVer = atoi(szVer);
		}
		delete buff;
	}

	IsWow64Process(GetCurrentProcess(),&bIsx64);
	sprintf(szOSInfo, " 系统版本:%s[%s]", szOSTitle, szVer);
	SetWindowText(m_szTitle);
	MultiByteToWideChar(CP_ACP, 0, szOSInfo, -1, wszOSInfo, 256);
	GetDlgItem(IDC_OSVER)->SetFont(&m_cFont);
	GetDlgItem(IDC_TOOLVER)->SetFont(&m_cFont);
	SetDlgItemText(IDC_OSVER, wszOSInfo);

	if (bIsx64)
	{
		strcat(szCmdTool,"x64.exe");
	}
	else
	{
		strcat(szCmdTool,"x86.exe");
	}

	strcpy(m_CloudOADlg.m_szOATool,szCmdTool);

	if (_access(szCmdTool,0) == -1)
	{
		MessageBox(TEXT("缺少OA3刷KEY文件，请联系软件提供商提供此文件！"),TEXT("错误"),MB_ICONERROR);
		PostQuitMessage(0);
	}
	WCHAR wszCmdTool[32] = { 0 };
	mbstowcs(wszCmdTool, szCmdTool, 32);
	((CHWToolApp*)AfxGetApp())->GetVersion(wszCmdTool, (LPDWORD)&m_iToolVer);
	wsprintf(wszOSInfo, TEXT(" OA3工具版本:%d"), m_iToolVer);
	SetDlgItemText(IDC_TOOLVER, wszOSInfo);
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
	m_cFont.DeleteObject();
	// TODO: Add your message handler code here
}

BOOL CHWToolDlg::IsAllowPerformed()
{
	return (m_iOSVer == m_iToolVer);
}
