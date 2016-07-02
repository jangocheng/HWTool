// BytBiosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "BiosDlg.h"
#include "HWToolDlg.h"


// CBiosDlg dialog

IMPLEMENT_DYNAMIC(CBiosDlg, CDialog)

CBiosDlg::CBiosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBiosDlg::IDD, pParent)
{
	m_nType = -1;
}

CBiosDlg::~CBiosDlg()
{
}

void CBiosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBiosDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BROWSE, &CBiosDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_UPDATE, &CBiosDlg::OnBnClickedUpdate)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CBiosDlg message handlers

void CBiosDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect m_rDataBox(40,180,380,201);
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

void CBiosDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	SetCurrentDirectory(m_curPath);
	CFileDialog dlg(TRUE,0,0,6,TEXT("BIOS Firmware|*.bin||"),this);
	dlg.m_ofn.lpstrInitialDir = m_curPath;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	m_szPath = dlg.GetPathName();
	SetDlgItemText(IDC_FW,m_szPath);
	GetCurrentDirectory(2048,m_curPath);
}

void CBiosDlg::OnBnClickedUpdate()
{
	// TODO: Add your control notification handler code here
	//GUID CORM  = { 0x0ff0a55a, 0x0003, 0x0204, { 0x06, 0x02, 0x10, 0x15, 0x20, 0x01, 0x21, 0x00 } };
	GUID BYTCR = { 0x0ff0a55a, 0x0003, 0x0204, { 0x06, 0x02, 0x10, 0x0b, 0x20, 0x00, 0x21, 0x00 } };
	GUID CHT3  = { 0x0ff0a55a, 0x0003, 0x0204, { 0x06, 0x02, 0x10, 0x0e, 0x20, 0x00, 0x21, 0x00 } };
	GUID* pFw=NULL;

	m_nType = -1;
	char buff[16]={0};
	if (wcsstr(m_wszCpuInfo,L"Z83") || 
		wcsstr(m_wszCpuInfo,L"Z85") ||
		wcsstr(m_wszCpuInfo,L"Z87")
		)
	{
		if (m_bIsx64)
		{
			m_nType = 3;
		}
		else
		{
			m_nType = 1;
		}
	}
	else if (wcsstr(m_wszCpuInfo,L"Z37"))
	{
		m_nType = 2;
	}

	if (m_nType == 1 || m_nType == 3)
	{
		pFw = &CHT3;
	}
	else if (m_nType == 2)
	{
		pFw = &BYTCR;
	}
	if (pFw == NULL)
	{
		MessageBox(TEXT("不支持当前平台！"),TEXT("错误"),MB_ICONERROR);
		EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
		GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
		GetDlgItem(IDC_SNCHECK)->EnableWindow();
		GetDlgItem(IDC_UPDATE)->EnableWindow();
		GetDlgItem(IDC_BROWSE)->EnableWindow();
		return;
	}

	m_bExistKey = FALSE;
	CString szBios;
	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_DISABLED);
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow(0);
	GetDlgItem(IDC_SNCHECK)->EnableWindow(0);
	GetDlgItem(IDC_UPDATE)->EnableWindow(0);
	GetDlgItem(IDC_BROWSE)->EnableWindow(0);
	if (m_szPath.GetLength()<=0) 
	{
		MessageBox(TEXT("未发现BIOS文件！"),TEXT("错误"),MB_ICONSTOP);
		EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
		GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
		GetDlgItem(IDC_SNCHECK)->EnableWindow();
		GetDlgItem(IDC_UPDATE)->EnableWindow();
		GetDlgItem(IDC_BROWSE)->EnableWindow();
		return;
	}
	CFile fp;
	if (!fp.Open(m_szPath,CFile::modeRead|CFile::typeBinary))
	{
		MessageBox(TEXT("未发现BIOS文件"),TEXT("错误"),MB_ICONERROR);
		EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
		GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
		GetDlgItem(IDC_SNCHECK)->EnableWindow();
		GetDlgItem(IDC_UPDATE)->EnableWindow();
		GetDlgItem(IDC_BROWSE)->EnableWindow();
		return;
	}
	ULONG len=(ULONG)fp.GetLength();
	if (len != 8388608)
	{
		MessageBox(TEXT("无效的BIOS文件"),TEXT("错误"),MB_ICONERROR);
		EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
		GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
		GetDlgItem(IDC_SNCHECK)->EnableWindow();
		GetDlgItem(IDC_UPDATE)->EnableWindow();
		GetDlgItem(IDC_BROWSE)->EnableWindow();
		fp.Close();
		return;
	}
	fp.Seek(0x10,SEEK_SET);
	fp.Read(buff,16);
	if (memcmp(buff,(char*)pFw,16))
	{
		MessageBox(TEXT("无效的BIOS文件"),TEXT("错误"),MB_ICONERROR);
		EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
		GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
		GetDlgItem(IDC_SNCHECK)->EnableWindow();
		GetDlgItem(IDC_UPDATE)->EnableWindow();
		GetDlgItem(IDC_BROWSE)->EnableWindow();
		fp.Close();
		return;
	}
	fp.Seek(0x42,SEEK_SET);
	fp.Read(&m_nBiosSize,4);
	m_nBiosSize >>= 16;
	m_nBiosSize <<= 12;
	m_nBiosSize = 0x800000 - m_nBiosSize;
	fp.Close();
	SetCurrentDirectory(m_szTempDir);
	CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)UpdateThread,this,0,NULL));
}


UINT CBiosDlg::UpdateThread(LPVOID lp)
{
	CBiosDlg* p = (CBiosDlg*)lp;
	return p->UpdateBios();
}

void CBiosDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR szPath[2048]={0};
	int len = DragQueryFile(hDropInfo, 0, szPath, sizeof(szPath));
	if (len)
	{
		WIN32_FIND_DATA wfd;
		BOOL bValue = FALSE;
		HANDLE hFind = FindFirstFile(szPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//SetDlgItemText(IDC_FW,"");
			//m_szPath="";
		}
		else if (hFind != INVALID_HANDLE_VALUE)
		{
			m_szPath=szPath;
			SetDlgItemText(IDC_FW,m_szPath);
		}
		FindClose(hFind);
	}
	else
	{
		SetDlgItemText(IDC_FW,TEXT(""));
		m_szPath=TEXT("");
	}

	CDialog::OnDropFiles(hDropInfo);
}

BOOL CBiosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	_tcscpy(m_szTempDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(m_szTempDir,TEXT("\\Temp"));
	GetCurrentDirectory(2048,m_curPath);
	typedef BOOL (WINAPI* ChangeWindowMessageFilterFn)( UINT, DWORD );
	HMODULE hUserMod = NULL;
	BOOL bResult = FALSE;
	hUserMod = LoadLibrary(TEXT("user32.dll"));
	if (hUserMod)
	{
		ChangeWindowMessageFilterFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterFn)GetProcAddress( hUserMod, "ChangeWindowMessageFilter" );
		if (pfnChangeWindowMessageFilter)
		{
			pfnChangeWindowMessageFilter(WM_DROPFILES, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
			pfnChangeWindowMessageFilter(0x0049, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
		}

		FreeLibrary(hUserMod);
	}
	m_nBiosSize = 0;
	m_bExistKey = FALSE;
	m_strSSN = TEXT("");
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

int CBiosDlg::UpdateBios(void)
{
	BOOL retval;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	HANDLE hReadPipe,hWritePipe;
	sa.bInheritHandle=TRUE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.hStdOutput=si.hStdError=hWritePipe;
	DWORD dwLen,dwRead,retCode1=-1,retCode2=-1,retDet=-1;
	UINT nActualBiosSize=0,nLock=0;
	char buff[256] = {0};
	TCHAR szErrMsg[256]={0};
	CFile fp;
	CFile fp1,fp2;
	DWORD fLen;
	BYTE* fBuff;
	CStringA cmd;
	CBytTool* t1=0;
	CChtTool86* t2=0;
	CChtTool64* t3=0;
	if (m_nType == 1)
	{
		t2 = new CChtTool86();
	}
	else if (m_nType == 2)
	{
		t1 = new CBytTool();
	}
	else if (m_nType == 3)
	{
		t3 = new CChtTool64();
	}
	SetCurrentDirectory(m_szTempDir);
	m_nSN = ((CButton*)GetDlgItem(IDC_SNCHECK))->GetCheck();
	retval=CreateProcessA(NULL,"cmd.exe /c fptw.exe -i",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
	if(retval)
	{
		WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
		GetExitCodeProcess(pi.hProcess,&retDet);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		if (retDet)
		{
			_tcscpy(szErrMsg,TEXT("不支持当前平台！"));
			goto end;
		}
		dwLen=GetFileSize(hReadPipe,NULL);
		char* szRegion = new char[dwLen];
		ReadFile(hReadPipe,szRegion,dwLen,&dwRead,NULL);
		if (dwRead)
		{
			char* szToken=strstr(szRegion,"BIOS       - Base: ");
			char szLen[9]={0};
			if (szToken)
			{
				strncpy(szLen,szToken+19,8);
				sscanf(szLen,"%x",&nActualBiosSize);
				nActualBiosSize = 0x800000 - nActualBiosSize;
			}
		}
		delete szRegion;
	}
	if (m_nBiosSize != nActualBiosSize)
	{
		wsprintf(szErrMsg,TEXT("当前BIOS分区大小与机器里的不一致, 机器中的大小为：0x%X, 文件中的大小为：0x%X!"),nActualBiosSize,m_nBiosSize);
		goto end;
	}

	retval=CreateProcessA(NULL,"cmd.exe /c fptw.exe -dumplock",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
	if(retval)
	{
		WaitForSingleObject(pi.hThread,INFINITE);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		dwLen=GetFileSize(hReadPipe,NULL);
		char* szRegion = new char[dwLen];
		ReadFile(hReadPipe,szRegion,dwLen,&dwRead,NULL);
		if (dwRead)
		{
			char* szToken=strstr(szRegion,"Host CPU master:    ");
			int a,b,c,d,nCPU=0,nTXE=0;
			if (szToken)
			{
				szToken += 20;
				if (4 == sscanf(szToken,"%x %x %x %x",&a,&b,&c,&d))
				{
					nCPU = (a<<24) + (b<<16) + (c<<8) + d;
				}
			}
			szToken=strstr(szRegion,"TXE region master:  ");
			if (szToken)
			{
				szToken += 20;
				if (4 == sscanf(szToken,"%x %x %x %x",&a,&b,&c,&d))
				{
					nTXE = (a<<24) + (b<<16) + (c<<8) + d;
				}
			}
			if (nCPU == 0x0B0A && nTXE == 0x0D0C)
			{
				nLock = 1;
			}
		}
		SetDlgItemText(IDC_STATUS,nLock?TEXT("TXE/ME 已锁定"):TEXT("TXE/ME 未锁定"));
		delete szRegion;
	}
	Sleep(2000);
	//////////////////////////////////////////////////////////////////////
	retval=CreateProcessA(NULL,"cmd.exe /c Check.exe",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
	if(retval)
	{
		WaitForSingleObject(pi.hThread,INFINITE);
		GetExitCodeProcess(pi.hProcess,&retCode1);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		if (retCode1 != 0)
		{
			m_bExistKey = FALSE;
			SetDlgItemText(IDC_STATUS,TEXT("未发现微软密钥"));
		}
		else
		{
			SetDlgItemText(IDC_STATUS,TEXT("发现微软密钥，保存中......"));
			dwLen=GetFileSize(hReadPipe,NULL);
			char *buff=new char [dwLen+1];
			char dpk[30]={0};
			char* vptr,*token="Product key:       ";
			memset(buff,0,dwLen+1);
			retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
			vptr=strstr(buff+700,token);
			if (vptr)
			{
				vptr +=strlen(token);
				strncpy(dpk,vptr,29);
				char szDPK[20]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x1d,0x00,0x00,0x00};
				fp.Open(TEXT("key.bin"),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				fp.Write(szDPK,20);
				fp.Write(dpk,29);
				fp.Close();
				m_bExistKey = TRUE;
			}
			delete buff;
		}
	}

	if (!fp1.Open(m_szPath,CFile::modeRead|CFile::typeBinary))
	{
		_tcscpy(szErrMsg,TEXT("未发现BIOS文件，请确认其是否存在！"));
		goto end;
	}
	fLen=(DWORD)fp1.GetLength();
	fBuff = new BYTE[fLen];
	fp1.Read(fBuff,fLen);
	fp1.Close();
	if (!fp2.Open(TEXT("fw.bin"),CFile::modeCreate|CFile::modeReadWrite))
	{
		delete fBuff;
		_tcscpy(szErrMsg,TEXT("打开BIOS文件失败！"));
		goto end;
	}
	fp2.Write(fBuff,fLen);
	fp2.Close();
	delete fBuff;

	if (nLock == 0)
	{
		cmd="cmd.exe /c fptw.exe -f fw.bin";
	}
	else
	{
		cmd="cmd.exe /c fptw.exe -f fw.bin -bios";
	}
	Sleep(2000);
	SetDlgItemText(IDC_STATUS,TEXT("正在刷写BIOS......"));
	sa.bInheritHandle=0;
	si.wShowWindow=SW_SHOW;
	retval=CreateProcessA(NULL,(LPSTR)(LPCSTR)cmd,&sa,&sa,0,0,NULL,NULL,&si,&pi);
	WaitForSingleObject(pi.hThread,INFINITE);
	GetExitCodeProcess(pi.hProcess,&retCode1);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	DeleteFile(TEXT("fw.bin"));
	if (retCode1 == 0)
	{
		SetDlgItemText(IDC_STATUS,TEXT("升级完成！"));
		Sleep(1000);
	}
	else
	{
		SetDlgItemText(IDC_STATUS,TEXT("升级失败！"));
		_tcscpy(szErrMsg,TEXT("BIOS升级失败！"));
		goto end;
	}

	si.wShowWindow=SW_HIDE;
	if (m_nSN == BST_CHECKED)
	{
		if (m_strSSN.GetLength())
		{
			Sleep(2000);
			SetDlgItemText(IDC_STATUS,TEXT("正在刷写保留的序列号......"));
			CBiosInfo* pInfo = ((CHWToolDlg*)GetParent())->m_BiosInfo;
			strcpy(buff,"cmd.exe /c amidewin.exe /ss \"");
			strcat(buff,pInfo->m_BiosInfoA.m_szSS);
			strcat(buff,"\"");
			retval=CreateProcessA(NULL,buff,&sa,&sa,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}

	if (m_bExistKey)
	{
		if (fp.Open(TEXT("key.bin"),CFile::modeRead))
		{
			fp.Close();
			Sleep(2000);
			SetDlgItemText(IDC_STATUS,TEXT("正在重新写入机器中的微软密钥......"));
			CreateProcessA(NULL,"cmd.exe /c afuwin.exe /oad",&sa,&sa,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CreateProcessA(NULL,"cmd.exe /c afuwin.exe /akey.bin",&sa,&sa,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);
			GetExitCodeProcess(pi.hProcess,&retCode2);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (retCode2 == 0)
			{
				SetDlgItemText(IDC_STATUS,TEXT("刷KEY成功！"));
			}
			else
			{
				SetDlgItemText(IDC_STATUS,TEXT("刷KEY失败！"));
			}
			DeleteFile(TEXT("key.bin"));
		}
	}
end:
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	if (t1) delete t1;
	if (t2) delete t2;
	if (t3) delete t3;
	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
	GetDlgItem(IDC_SNCHECK)->EnableWindow();
	GetDlgItem(IDC_UPDATE)->EnableWindow();
	GetDlgItem(IDC_BROWSE)->EnableWindow();
	if (retCode1 == 0)
	{
		HANDLE hToken;
		TOKEN_PRIVILEGES tkp;
		TCHAR szTmpDir[2048]={0};

		_tcscpy(szTmpDir,_tgetenv(TEXT("SystemRoot")));
		_tcscat(szTmpDir,TEXT("\\Temp"));
		SetCurrentDirectory(szTmpDir);
		DeleteFile(TEXT("Check.exe"));
		DeleteFile(TEXT("afuwin.exe"));
		DeleteFile(TEXT("amidewin.exe"));
		DeleteFile(TEXT("amifldrv32.sys"));
		DeleteFile(TEXT("amifldrv64.sys"));
		Sleep(1000);
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
		LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,0);
		ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
		CloseHandle(hToken);
	}
	else
	{
		MessageBox(szErrMsg,TEXT("升级错误"),MB_ICONERROR);
	}

	return 0;
}
