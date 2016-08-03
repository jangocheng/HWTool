// HWToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "HWToolDlg.h"
#include "xDisplayDevice.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAmiTool::CAmiTool()
{
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("AmiTool"));
	if (m_hMutex)
	{
		m_bExist = TRUE;
		return;
	}
	m_bExist = FALSE;
	m_hMutex = CreateMutex(NULL,FALSE,TEXT("AmiTool"));

	TCHAR szTmpDir[2048]={0};
	CFile fp;
	HRSRC hSrc;
	HGLOBAL hGl;
	LPBYTE lpBuf;
	DWORD dwLen;
	BOOL bRet;

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);
	//CHECK
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_CHECK),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("Check.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//AFUWIN_EXE
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_AFUWIN),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("afuwin.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//AMIDEWIN_EXE
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_AMIDEWIN),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("amidewin.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//AMIFLDRV32_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_AMIFLDRV32),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("amifldrv32.sys"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//AMIFLDRV64_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_AMIFLDRV64),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("amifldrv64.sys"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//OA3ToolFile.cfg
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_OACFG),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("OA3ToolFile.cfg"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//OA3Tool.exe
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_OATOOL),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("oa3tool.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
}

CAmiTool::~CAmiTool()
{
	if (m_bExist)
	{
		return;
	}

	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);
	DeleteFile(TEXT("Check.exe"));
	DeleteFile(TEXT("afuwin.exe"));
	DeleteFile(TEXT("amidewin.exe"));
	DeleteFile(TEXT("amifldrv32.sys"));
	DeleteFile(TEXT("amifldrv64.sys"));
	DeleteFile(TEXT("oa3toolfile.cfg"));
	DeleteFile(TEXT("oa3tool.exe"));
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}


CBytTool::CBytTool()
{
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("BytTool"));
	if (m_hMutex)
	{
		m_bExist = TRUE;
		return;
	}
	m_bExist = FALSE;
	m_hMutex = CreateMutex(NULL,FALSE,TEXT("BytTool"));

	CFile fp;
	HRSRC hSrc;
	HGLOBAL hGl;
	LPBYTE lpBuf;
	DWORD dwLen;
	BOOL bRet;

	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);

	//FPTW_EXE
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPTW1),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fptw.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//IDRV_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_IDRVDLL),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("idrvdll.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//PMX_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_PMXDLL),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("pmxdll.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//FPARTS_TXT
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPARTS1),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fparts.txt"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
}

CBytTool::~CBytTool()
{
	if (m_bExist)
	{
		return;
	}
	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);
	DeleteFile(TEXT("fptw.exe"));
	DeleteFile(TEXT("idrvdll.dll"));
	DeleteFile(TEXT("pmxdll.dll"));
	DeleteFile(TEXT("fparts.txt"));
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}

CChtTool86::CChtTool86()
{
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("ChtTool86"));
	if (m_hMutex)
	{
		m_bExist = TRUE;
		return;
	}
	m_bExist = FALSE;
	m_hMutex = CreateMutex(NULL,FALSE,TEXT("ChtTool86"));

	CFile fp;
	HRSRC hSrc;
	HGLOBAL hGl;
	LPBYTE lpBuf;
	DWORD dwLen;
	BOOL bRet;

	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);

	//FPTW_EXE
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPTW2),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fptw.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//IDRV_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_IDRVDLL),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("idrvdll.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//PMX_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_PMXDLL),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("pmxdll.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//FPARTS_TXT
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPARTS2),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fparts.txt"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
}

CChtTool86::~CChtTool86()
{
	if (m_bExist)
	{
		return;
	}
	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);
	DeleteFile(TEXT("fptw.exe"));
	DeleteFile(TEXT("idrvdll.dll"));
	DeleteFile(TEXT("pmxdll.dll"));
	DeleteFile(TEXT("fparts.txt"));
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}

CChtTool64::CChtTool64()
{
	m_hMutex = OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("ChtTool64"));
	if (m_hMutex)
	{
		m_bExist = TRUE;
		return;
	}
	m_bExist = FALSE;
	m_hMutex = CreateMutex(NULL,FALSE,TEXT("ChtTool64"));

	CFile fp;
	HRSRC hSrc;
	HGLOBAL hGl;
	LPBYTE lpBuf;
	DWORD dwLen;
	BOOL bRet;

	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);

	//FPTW_EXE
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPTW3),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fptw.exe"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//IDRV_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_IDRVDLL32E),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("idrvdll32e.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//PMX_DLL
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_PMXDLL32E),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("pmxdll32e.dll"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
	//FPARTS_TXT
	hSrc = FindResource(NULL,MAKEINTRESOURCE(IDR_FPARTS1),TEXT("DATA"));
	hGl = LoadResource(NULL,hSrc);
	dwLen = SizeofResource(NULL,hSrc);
	lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(TEXT("fparts.txt"),CFile::modeCreate|CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf,dwLen);
	fp.Close();
}

CChtTool64::~CChtTool64()
{
	if (m_bExist)
	{
		return;
	}
	TCHAR szTmpDir[2048]={0};

	_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(szTmpDir,TEXT("\\Temp"));
	SetCurrentDirectory(szTmpDir);
	DeleteFile(TEXT("fptw.exe"));
	DeleteFile(TEXT("idrvdll32e.dll"));
	DeleteFile(TEXT("pmxdll32e.dll"));
	DeleteFile(TEXT("fparts.txt"));
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}
// CHWToolDlg dialog




CHWToolDlg::CHWToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHWToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_AmiTool = new CAmiTool();
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
	RECT rc;
	m_hIconFw = AfxGetApp()->LoadIcon(IDR_BIOS);
	m_hIconSn = AfxGetApp()->LoadIcon(IDR_SN);
	m_hIconKey = AfxGetApp()->LoadIcon(IDR_KEY);
	m_hIconOa = AfxGetApp()->LoadIcon(IDR_OA);
	m_ImgList.Create(16,16,ILC_COLOR32,0,0);
	m_ImgList.Add(m_hIconFw);
	m_ImgList.Add(m_hIconSn);
	m_ImgList.Add(m_hIconKey);
	m_ImgList.Add(m_hIconOa);
	m_pTab = (CTabCtrl*)GetDlgItem(IDC_TAB1);
	m_pTab->SetImageList(&m_ImgList);
	m_pTab->InsertItem(0,TEXT("BIOS工具"),0);
	m_pTab->InsertItem(1,TEXT("序列号刷写"),1);
	m_pTab->InsertItem(2,TEXT("微软密钥"),2);
	//m_pTab->InsertItem(3,TEXT("刷Key工具"),3);
	m_pTab->GetClientRect(&rc);

	m_BiosDlg.Create(IDD_BIOS,this);
	m_DpkDlg.Create(IDD_DPK,this);
	m_SnDlg.Create(IDD_SN,this);
	m_OaDlg.Create(IDD_OADLG,this);
	m_pDlg[0]=&m_BiosDlg;
	m_pDlg[1]=&m_SnDlg;
	m_pDlg[2]=&m_DpkDlg;
	//m_pDlg[3]=&m_OaDlg;
	for (int i=0; i< 3; i++)
	{
		m_pDlg[i]->SetWindowPos(NULL,0,rc.bottom,0,0,SWP_NOSIZE);
	}
	int sel = m_pTab->GetCurSel();
	m_pDlg[sel]->ShowWindow(SW_SHOW);
	//DWORD dwWidth,dwHeight;
	//XDD_GetActiveMonitorPhysicalSize(dwWidth,dwHeight);
    //int nScreenWidth, nScreenHeight; 
	//HDC hdcScreen = ::GetDC(NULL);   //获取屏幕的HDC 
    //nScreenWidth = GetDeviceCaps(hdcScreen, HORZSIZE); 
    //nScreenHeight = GetDeviceCaps(hdcScreen, VERTSIZE); 
	//::ReleaseDC(NULL, hdcScreen); 
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
	for (int i=0; i< 3; i++)
	{
		m_pDlg[i]->ShowWindow(SW_HIDE);
	}
	m_pDlg[sel]->ShowWindow(SW_SHOW);
	m_pDlg[sel]->SetFocus();
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
	delete m_AmiTool;
	DestroyIcon(m_hIconFw);
	DestroyIcon(m_hIconSn);
	DestroyIcon(m_hIconKey);
	DestroyIcon(m_hIconOa);
	m_ImgList.DeleteImageList();
	// TODO: Add your message handler code here
}
