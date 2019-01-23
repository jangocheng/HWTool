// HWTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HWTool.h"
#include "HWToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "version.lib")

CBiosInfo::CBiosInfo()
{
	memset(&m_InfoA,0,sizeof(m_InfoA));
	memset(&m_InfoW,0,sizeof(m_InfoW));
	RebuildInfo();
}

void CBiosInfo::RebuildInfo()
{
	///////////////////////////////////////////////////////////////////
	CDminfo dminfo;
	dminfo.GetDMInfo(&m_InfoA);
	mbstowcs(m_InfoW.dmi_bios_vendor,m_InfoA.dmi_bios_vendor,64);
	mbstowcs(m_InfoW.dmi_bios_version,m_InfoA.dmi_bios_version,64);
	mbstowcs(m_InfoW.dmi_bios_release_date,m_InfoA.dmi_bios_release_date,64);
	mbstowcs(m_InfoW.dmi_sys_manufacture,m_InfoA.dmi_sys_manufacture,64);
	mbstowcs(m_InfoW.dmi_sys_product,m_InfoA.dmi_sys_product,64);
	mbstowcs(m_InfoW.dmi_sys_version,m_InfoA.dmi_sys_version,64);
	mbstowcs(m_InfoW.dmi_sys_serial_number,m_InfoA.dmi_sys_serial_number,64);
	mbstowcs(m_InfoW.dmi_sys_uuid,m_InfoA.dmi_sys_uuid,64);
	mbstowcs(m_InfoW.dmi_sys_sku,m_InfoA.dmi_sys_sku,64);
	mbstowcs(m_InfoW.dmi_sys_family,m_InfoA.dmi_sys_family,64);
	mbstowcs(m_InfoW.dmi_bsbrd_manufacture,m_InfoA.dmi_bsbrd_manufacture,64);
	mbstowcs(m_InfoW.dmi_bsbrd_product,m_InfoA.dmi_bsbrd_product,64);
	mbstowcs(m_InfoW.dmi_bsbrd_version,m_InfoA.dmi_bsbrd_version,64);
	mbstowcs(m_InfoW.dmi_bsbrd_serial,m_InfoA.dmi_bsbrd_serial,64);
	mbstowcs(m_InfoW.dmi_chassis_type,m_InfoA.dmi_chassis_type,64);
	mbstowcs(m_InfoW.dmi_oem_table_id,m_InfoA.dmi_oem_table_id,64);
	mbstowcs(m_InfoW.dmi_oem_id,m_InfoA.dmi_oem_id,64);
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


void CHWToolApp::GetVersion(LPCTSTR szPEFile, CString &ver)
{  
    DWORD dwInfoSize = 0;  
    TCHAR exePath[MAX_PATH];  
    memset(exePath, 0, sizeof(exePath));  
  
    ver.Format(_T("V1.00"));  
	if (szPEFile == NULL)
	{
		// 得到程序的自身路径  
		GetModuleFileName(NULL, exePath, sizeof(exePath)/sizeof(TCHAR));  
	}
	else
	{
		wcscpy_s(exePath, szPEFile);
	}
  
	if (_waccess(exePath, 0) == -1)
	{
		return;
	}
    // 判断是否能获取版本号  
    dwInfoSize = GetFileVersionInfoSize(exePath, NULL);  
  
    if (dwInfoSize == 0)  
    {  
        ::OutputDebugString(L"GetFileVersionInfoSize fail\r\n");  
    }  
    else  
    {  
        BYTE* pData = new BYTE[dwInfoSize];  
  
        // 获取版本信息  
        if (!GetFileVersionInfo(exePath, NULL, dwInfoSize, pData))  
        {  
            ::OutputDebugString(L"GetFileVersionInfo fail\r\n");  
        }  
        else  
        {  
            // 查询版本信息中的具体键值  
            LPVOID lpBuffer;  
            UINT uLength;  
            if (!::VerQueryValue((LPCVOID)pData, _T("\\"), &lpBuffer, &uLength))  
            {  
                ::OutputDebugString(L"GetFileVersionInfo fail\r\n");  
            }  
            else  
            {  
                DWORD dwVerMS;  
                DWORD dwVerLS;  
                dwVerMS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionMS;    
                dwVerLS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionLS;  
                ver.Format(_T("V%d.%d.%d.%d"), (dwVerMS >> 16), (dwVerMS & 0xFFFF), (dwVerLS >> 16), (dwVerLS & 0xFFFF));  
            }  
        }  
  
        delete pData;  
    }  
}  

void CHWToolApp::GetVersion(LPCTSTR szPEFile, LPDWORD pVer)
{
	DWORD dwInfoSize = 0;
	TCHAR exePath[MAX_PATH];
	memset(exePath, 0, sizeof(exePath));

	if (szPEFile == NULL)
	{
		// 得到程序的自身路径  
		GetModuleFileName(NULL, exePath, sizeof(exePath) / sizeof(TCHAR));
	}
	else
	{
		wcscpy_s(exePath, szPEFile);
	}

	if (_waccess(exePath, 0) == -1)
	{
		return;
	}
	if (pVer == NULL)
	{
		return;
	}
	// 判断是否能获取版本号  
	dwInfoSize = GetFileVersionInfoSize(exePath, NULL);

	if (dwInfoSize == 0)
	{
		::OutputDebugString(L"GetFileVersionInfoSize fail\r\n");
	}
	else
	{
		BYTE* pData = new BYTE[dwInfoSize];

		// 获取版本信息  
		if (!GetFileVersionInfo(exePath, NULL, dwInfoSize, pData))
		{
			::OutputDebugString(L"GetFileVersionInfo fail\r\n");
		}
		else
		{
			// 查询版本信息中的具体键值  
			LPVOID lpBuffer;
			UINT uLength;
			if (!::VerQueryValue((LPCVOID)pData, _T("\\"), &lpBuffer, &uLength))
			{
				::OutputDebugString(L"GetFileVersionInfo fail\r\n");
			}
			else
			{
				DWORD dwVerMS;
				DWORD dwVerLS;
				dwVerMS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionMS;
				dwVerLS = ((VS_FIXEDFILEINFO*)lpBuffer)->dwProductVersionLS;
				*pVer = (dwVerLS >> 16);
			}
		}

		delete pData;
	}
}


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

	///////////////////////////////////////////////////////////////////

	CString ver,szTitle;
	GetVersion(0,ver);
	szTitle.Format(TEXT("MDOSTool-%s"),ver);
	AfxEnableControlContainer();
	m_hMutex=OpenMutex(MUTEX_ALL_ACCESS,FALSE,TEXT("HUIWEITool"));
	if(m_hMutex)
	{
		HWND hWnd=FindWindow(NULL,szTitle);
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
	dlg.m_szTitle = szTitle;
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
