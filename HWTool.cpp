// HWTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HWTool.h"
#include "HWToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CBiosInfo::CBiosInfo()
{
	RebuildInfo();
}

void CBiosInfo::RebuildInfo()
{
	memset(&m_BiosInfoA,0,sizeof(m_BiosInfoA));
	memset(&m_BiosInfoW,0,sizeof(m_BiosInfoW));
	_tcscpy(m_szTempDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(m_szTempDir,TEXT("\\Temp"));
//*************************************************
	BOOL retval;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	HANDLE hReadPipe,hWritePipe;
	//HANDLE hReadInPipe,hWriteInPipe;
	sa.bInheritHandle=TRUE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	//retval = CreatePipe(&hReadInPipe,&hWriteInPipe,&sa,0);
	//SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);//communication with console, like kbd input.
	//SetHandleInformation(hWriteInPipe, HANDLE_FLAG_INHERIT, 0);//communication with console, like kbd input.
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.hStdOutput=si.hStdError=hWritePipe;
	//si.hStdInput = hReadInPipe;
	DWORD dwLen,dwRead,retCode=-1;
	char *buff;
	CStringA cmd;
	SetCurrentDirectory(m_szTempDir);
	//////////////////////////////////////////////////////////////////////
	retval=CreateProcessA(NULL,"cmd.exe /c amidewin.exe /IVN /IV /ID /SM /SP /SV /SS /SU /SK /SF /BM /BP /BV /BS",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
	//retval=CreateProcessA(NULL,"cmd.exe /c fptw.exe",&sa,&sa,TRUE,CREATE_NEW_CONSOLE,0,0,&si,&pi);
	//WaitForSingleObject(pi.hThread,1000);
	WaitForSingleObject(pi.hThread,INFINITE);
	TerminateThread(pi.hThread,0);
	TerminateProcess(pi.hProcess,0);
	GetExitCodeProcess(pi.hProcess,&retCode);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	dwLen=GetFileSize(hReadPipe,NULL);
	buff=new char [dwLen+1];
	memset(buff,0,dwLen+1);
	retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	//CloseHandle(hWriteInPipe);
	//CloseHandle(hReadInPipe);
//*************************************************
	char *szToken,*p1,*p2;
	szToken = strtok(buff,"\r\n");
	while (szToken)
	{
		if (strncmp(szToken,"(/IVN",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szIVN,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/IV)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szIV,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/ID)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szID,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SM)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSM,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SP)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSP,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SV)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSV,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SS)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSS,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SU)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSU,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SK)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSK,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/SF)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szSF,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/BM)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szBM,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/BP)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szBP,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/BV)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szBV,64,p1+1);
			}
		}
		else if (strncmp(szToken,"(/BS)",5) == 0)
		{
			p1 = strchr(szToken,'"');
			p2 = strrchr(szToken,'"');
			if (p1 && p2)
			{
				*p2 = 0;
				strcpy_s(m_BiosInfoA.m_szBS,64,p1+1);
			}
		}
		szToken = strtok(NULL,"\r\n");
	}
	delete buff;
	mbstowcs(m_BiosInfoW.m_wszIVN,m_BiosInfoA.m_szIVN,64);
	mbstowcs(m_BiosInfoW.m_wszIV,m_BiosInfoA.m_szIV,64);
	mbstowcs(m_BiosInfoW.m_wszID,m_BiosInfoA.m_szID,64);
	mbstowcs(m_BiosInfoW.m_wszSM,m_BiosInfoA.m_szSM,64);
	mbstowcs(m_BiosInfoW.m_wszSP,m_BiosInfoA.m_szSP,64);
	mbstowcs(m_BiosInfoW.m_wszSV,m_BiosInfoA.m_szSV,64);
	mbstowcs(m_BiosInfoW.m_wszSS,m_BiosInfoA.m_szSS,64);
	mbstowcs(m_BiosInfoW.m_wszSU,m_BiosInfoA.m_szSU,64);
	mbstowcs(m_BiosInfoW.m_wszSK,m_BiosInfoA.m_szSK,64);
	mbstowcs(m_BiosInfoW.m_wszSF,m_BiosInfoA.m_szSF,64);
	mbstowcs(m_BiosInfoW.m_wszBM,m_BiosInfoA.m_szBM,64);
	mbstowcs(m_BiosInfoW.m_wszBP,m_BiosInfoA.m_szBP,64);
	mbstowcs(m_BiosInfoW.m_wszBV,m_BiosInfoA.m_szBV,64);
	mbstowcs(m_BiosInfoW.m_wszBS,m_BiosInfoA.m_szBS,64);
}

CBiosInfo::~CBiosInfo()
{
}


// CHWToolApp

BEGIN_MESSAGE_MAP(CHWToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHWToolApp construction

CHWToolApp::CHWToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_hMutex = NULL;
	m_BiosInfo = NULL;
}


// The one and only CHWToolApp object

CHWToolApp theApp;


// CHWToolApp initialization

BOOL CHWToolApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	m_hMutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("HUIWEITool"));
	if(m_hMutex)
	{
		HWND hWnd=FindWindow(NULL,TEXT("HuiWeiTool - OA3.0(Ver:06)"));
		if (hWnd)
		{
			if(!IsWindowVisible(hWnd))
				ShowWindow(hWnd,SW_SHOW);
			SetForegroundWindow(hWnd);
		}
		return FALSE;
	}
	else
	{
		m_hMutex=CreateMutex(NULL,FALSE,TEXT("HUIWEITool"));
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("HUIWEITool"));
	CoInitialize(NULL);

	CHWToolDlg dlg;
	m_BiosInfo = new CBiosInfo();
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	CoUninitialize();
	return FALSE;
}

int CHWToolApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	if(m_hMutex)
		CloseHandle(m_hMutex);
	m_hMutex=NULL;
	if (m_BiosInfo) delete m_BiosInfo;
	return CWinApp::ExitInstance();
}
