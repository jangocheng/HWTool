// HWTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HWTool.h"
#include "HWToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
		HWND hWnd=FindWindow(NULL,TEXT("HUIWEITool"));
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
	return CWinApp::ExitInstance();
}
