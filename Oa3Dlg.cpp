// Oa3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "Oa3Dlg.h"
#include "HWToolDlg.h"


// COa3Dlg dialog

IMPLEMENT_DYNAMIC(COa3Dlg, CDialog)
const char* szKeyString[3] = 
{
	"LicensablePartNumber",
	"OEMPONumber",
	"OEMPartNumber"
};

const char* strOATool = 
{
"<?xml version=\"1.0\"?>\r\n\
<OA3 xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n\
  <ServerBased>\r\n\
    <KeyProviderServerLocation>\r\n\
      <IPAddress>%s</IPAddress>\r\n\
      <ProtocolSequence>ncacn_ip_tcp</ProtocolSequence>\r\n\
      <EndPoint>8765</EndPoint>\r\n\
      <Options />\r\n\
    </KeyProviderServerLocation>\r\n\
    <Parameters>\r\n\
      <Parameter name=\"%s\" value=\"%s\" />\r\n\
      <OEMOptionalInfo>\r\n\
        <Field>\r\n\
          <Name>ZPC_MODEL_SKU</Name>\r\n\
          <Value>%s</Value>\r\n\
        </Field>\r\n\
        <Field>\r\n\
          <Name>ZFRM_FACTOR_CL1</Name>\r\n\
          <Value>%s</Value>\r\n\
        </Field>\r\n\
        <Field>\r\n\
          <Name>ZFRM_FACTOR_CL2</Name>\r\n\
          <Value>%s</Value>\r\n\
        </Field>\r\n\
        <Field>\r\n\
          <Name>ZSCREEN_SIZE</Name>\r\n\
          <Value>%s</Value>\r\n\
        </Field>\r\n\
        <Field>\r\n\
          <Name>ZTOUCH_SCREEN</Name>\r\n\
          <Value>%s</Value>\r\n\
        </Field>\r\n\
      </OEMOptionalInfo>\r\n\
      <SerialNumber>%s</SerialNumber>\r\n\
      <CloudConfigurationID>%s</CloudConfigurationID>\r\n\
    </Parameters>\r\n\
  </ServerBased>\r\n\
  <OutputData>\r\n\
    <AssembledBinaryFile>OA3.bin</AssembledBinaryFile>\r\n\
    <ReportedXMLFile>OA3.xml</ReportedXMLFile>\r\n\
  </OutputData>\r\n\
</OA3>\r\n"
};

#ifdef __MAC

BOOL COa3Dlg::GetDeviceAddress()
{
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
	BOOL bWIFI = FALSE,bBT=FALSE,bRet = TRUE;

    unsigned int i;

    /* variables used for GetIfTable and GetIfEntry */
    MIB_IFTABLE *pIfTable;
    MIB_IFROW *pIfRow;

    // Allocate memory for our pointers.
    pIfTable = (MIB_IFTABLE *) MALLOC(sizeof (MIB_IFTABLE));
    if (pIfTable == NULL) {
		//MessageBox("Error allocating memory needed to call GetIfTable","Error",MB_ICONERROR);
        return FALSE;
    }
    // Make an initial call to GetIfTable to get the
    // necessary size into dwSize
    dwSize = sizeof (MIB_IFTABLE);
    if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        FREE(pIfTable);
        pIfTable = (MIB_IFTABLE *) MALLOC(dwSize);
        if (pIfTable == NULL) {
			//MessageBox("Error allocating memory needed to call GetIfTable","Error",MB_ICONERROR); 
            return FALSE;
        }
    }

    // Make a second call to GetIfTable to get the actual
    // data we want.
    if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) 
	{
        for (i = 0; i < pIfTable->dwNumEntries; i++) {
            pIfRow = (MIB_IFROW *) & pIfTable->table[i];
            switch (pIfRow->dwType) {
            case IF_TYPE_ETHERNET_CSMACD:
				{
					if (strstr((char*)pIfRow->bDescr,"Bluetooth") && !bBT)
					{
						sprintf(m_KeyInfo.BT,"%02X-%02X-%02X-%02X-%02X-%02X",pIfRow->bPhysAddr[0],
							pIfRow->bPhysAddr[1],
							pIfRow->bPhysAddr[2],
							pIfRow->bPhysAddr[3],
							pIfRow->bPhysAddr[4],
							pIfRow->bPhysAddr[5]);
						bBT = TRUE;
					}
				}
                break;
            case IF_TYPE_IEEE80211:
				{
					if (strstr((char*)pIfRow->bDescr,"802.11") && !bWIFI)
					{
						sprintf(m_KeyInfo.WIFI,"%02X-%02X-%02X-%02X-%02X-%02X",pIfRow->bPhysAddr[0],
							pIfRow->bPhysAddr[1],
							pIfRow->bPhysAddr[2],
							pIfRow->bPhysAddr[3],
							pIfRow->bPhysAddr[4],
							pIfRow->bPhysAddr[5]);
						bWIFI = TRUE;
					}
				}
                break;
            default:
                //printf("Unknown type %ld\n", pIfRow->dwType);
                break;
            }
        }
    }

    if (pIfTable != NULL)
	{
        FREE(pIfTable);
        pIfTable = NULL;
    }

	return bRet;
}

BOOL COa3Dlg::GetIMEI()
{
    SAFEARRAY *psa = NULL;
	LONG lBound=0;
	BOOL bRet = FALSE;
	MBN_INTERFACE_CAPS InterfaceCaps;
	CComPtr<IMbnInterfaceManager>  pInterfaceMgr = NULL;
	CComPtr<IMbnInterface> pMbnInterface = NULL;
	HRESULT hr= CoCreateInstance(CLSID_MbnInterfaceManager,NULL,CLSCTX_ALL,IID_IMbnInterfaceManager,(void**)&pInterfaceMgr);
	if (FAILED(hr)) goto END;
	hr = pInterfaceMgr->GetInterfaces(&psa);
	if (FAILED(hr)) goto END;
	SafeArrayGetElement(psa, &lBound, &pMbnInterface);
	if (FAILED(hr)) goto END;
	hr = pMbnInterface->GetInterfaceCapability(&InterfaceCaps);
	if (FAILED(hr)) goto END;
	wchar_t* pBuf = InterfaceCaps.deviceID;
	wcstombs(m_KeyInfo.IMEI,pBuf,wcslen(pBuf));

    SysFreeString(InterfaceCaps.customDataClass);
    SysFreeString(InterfaceCaps.customBandClass);
    SysFreeString(InterfaceCaps.deviceID);
    SysFreeString(InterfaceCaps.manufacturer);
    SysFreeString(InterfaceCaps.model);
    SysFreeString(InterfaceCaps.firmwareInfo);

END:
	pInterfaceMgr = NULL;
	pMbnInterface = NULL;
	return bRet;
}


#endif
COa3Dlg::COa3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(COa3Dlg::IDD, pParent)
	, m_nIndex(0)
{
	memset(&m_cfg,0,sizeof(m_cfg));
}

COa3Dlg::~COa3Dlg()
{
}

void COa3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nIndex);
	DDV_MinMaxInt(pDX, m_nIndex, 0, 2);
}


BEGIN_MESSAGE_MAP(COa3Dlg, CDialog)
	ON_BN_CLICKED(IDC_CONNECT, &COa3Dlg::OnBnClickedConnect)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_INJECT, &COa3Dlg::OnBnClickedInject)
	ON_BN_CLICKED(IDC_ERASE, &COa3Dlg::OnBnClickedErase)
END_MESSAGE_MAP()


// COa3Dlg message handlers

void COa3Dlg::OnBnClickedConnect()
{
	// TODO: Add your control notification handler code here
	CIPAddressCtrl* ip = (CIPAddressCtrl*)GetDlgItem(IDC_SERVIP);
	BYTE szIP[4];
	char szCfg[2048] = {0};
	CWaitDlg wDlg;
	GetDlgItem(IDC_CONNECT)->EnableWindow(0);
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow(0);
	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_DISABLED);
	ip->GetAddress(szIP[0],szIP[1],szIP[2],szIP[3]);
	CDisConfigDlg Dlg(&m_cfg);
	if (szIP[0] == 0)
	{
		MessageBox(TEXT("请输入有效的IP地址"),TEXT("IP地址错误"),MB_ICONERROR);
		goto end;
	}
	GetDlgItemText(IDC_PWD,m_cfg.password,32);
	if (wcslen(m_cfg.password) == 0)
	{
		MessageBox(TEXT("请输入密码"),TEXT("错误"),MB_ICONERROR);
		goto end;
	}
	wsprintf(m_cfg.ip,TEXT("%u.%u.%u.%u"),szIP[0],szIP[1],szIP[2],szIP[3]);
	wDlg.Create(IDD_WAITDLG,this);
	wDlg.SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	wDlg.CenterWindow(this);
	if (Dlg.BuildConfiguration())
	{
		MessageBox(TEXT("连接数据库服务器失败"),TEXT("连接错误"),MB_ICONERROR);
		wDlg.DestroyWindow();
		goto end;
	}
	wDlg.DestroyWindow();
	if (Dlg.DoModal() == IDOK)
	{
		CFile fp;
		wchar_t filepath[MAX_PATH] = {0};
		GetModuleFileName(NULL,filepath,MAX_PATH);
		(wcsrchr(filepath,TEXT('.')))[1] = 0;
		wcscat(filepath,TEXT("cfg"));
		if(!fp.Open(filepath,CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(TEXT("保存配置文件失败，请确保磁盘可写"),TEXT("错误"),MB_ICONERROR);
			goto end;
		}
		fp.Write((LPBYTE)&m_cfg,sizeof(m_cfg));
		fp.Close();

		m_nIndex = m_cfg.idx;
		UpdateData(0);
		SetDlgItemText(IDC_SELNUMBER,m_cfg.para);
		SetDlgItemText(IDC_BUSINESS,m_cfg.business);
		SetDlgItemText(IDC_MODEL,m_cfg.sku);
		SetDlgItemText(IDC_TYPE,m_cfg.maintype);
		SetDlgItemText(IDC_SUBTYPE,m_cfg.subtype);
		SetDlgItemText(IDC_SCREEN,m_cfg.screensize);
		SetDlgItemText(IDC_TOUCH,m_cfg.hastouch);

		/////////////////////////////////////////////
		wcstombs(m_szIP,m_cfg.ip,32);
		strcpy(m_szPara,szKeyString[m_cfg.idx]);
		wcstombs(m_szParaValue,m_cfg.para,32);
		wcstombs(m_szSKU,m_cfg.sku,32);
		wcstombs(m_szType,m_cfg.maintype,32);
		wcstombs(m_szSubType,m_cfg.subtype,32);
		wcstombs(m_szScreenSize,m_cfg.screensize,32);
		wcstombs(m_szTouch,m_cfg.hastouch,32);
		strcpy(m_szSN,"00000000");
		wcstombs(m_szCloud,m_cfg.Id,40);

		(wcsrchr(filepath,TEXT('\\')))[1] = 0;
		wcscat(filepath,TEXT("oa3tool.cfg"));
		fp.Open(filepath,CFile::modeCreate|CFile::modeWrite);
		sprintf(szCfg,strOATool,m_szIP,m_szPara,m_szParaValue,m_szSKU,m_szType,m_szSubType,m_szScreenSize,m_szTouch,m_szSN,m_szCloud);
		fp.Write(szCfg,(UINT)strlen(szCfg));
		fp.Close();
		SetCurrentDirectory(m_szTempDir);
		fp.Open(TEXT("oa3tool.cfg"),CFile::modeCreate|CFile::modeWrite);
		fp.Write(szCfg,(UINT)strlen(szCfg));
		fp.Close();
	}
end:
	GetDlgItem(IDC_CONNECT)->EnableWindow();
	GetParent()->GetDlgItem(IDC_TAB1)->EnableWindow();
	EnableMenuItem(::GetSystemMenu(GetParent()->m_hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_ENABLED);
}

BOOL COa3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CFile fp;
	wchar_t filepath[MAX_PATH] = {0};
	char szCfg[2048] = {0};
	UINT szIP[4];
	CIPAddressCtrl* ip = (CIPAddressCtrl*)GetDlgItem(IDC_SERVIP);
	_tcscpy(m_szTempDir,_tgetenv(TEXT("SystemRoot")));
	_tcscat(m_szTempDir,TEXT("\\Temp"));

	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	memset(&m_KeyInfo,0,sizeof(KeyInfo));
	init_crc_table();

	memset(&m_cfg,0,sizeof(m_cfg));
	GetModuleFileName(NULL,filepath,MAX_PATH);
	(wcsrchr(filepath,TEXT('.')))[1] = 0;
	wcscat(filepath,TEXT("cfg"));
	if (fp.Open(filepath,CFile::modeRead))
	{
		if (sizeof(m_cfg) == fp.Read((LPBYTE)&m_cfg,sizeof(m_cfg)))
		{
			fp.Close();
			m_nIndex = m_cfg.idx;
			UpdateData(0);
			SetDlgItemText(IDC_SELNUMBER,m_cfg.para);
			SetDlgItemText(IDC_BUSINESS,m_cfg.business);
			SetDlgItemText(IDC_MODEL,m_cfg.sku);
			SetDlgItemText(IDC_TYPE,m_cfg.maintype);
			SetDlgItemText(IDC_SUBTYPE,m_cfg.subtype);
			SetDlgItemText(IDC_SCREEN,m_cfg.screensize);
			SetDlgItemText(IDC_TOUCH,m_cfg.hastouch);
			SetDlgItemText(IDC_PWD,m_cfg.password);
			swscanf(m_cfg.ip,TEXT("%u.%u.%u.%u"),&szIP[0],&szIP[1],&szIP[2],&szIP[3]);
			ip->SetAddress(szIP[0],szIP[1],szIP[2],szIP[3]);
			/////////////////////////////////////////////////
			wcstombs(m_szIP,m_cfg.ip,32);
			strcpy(m_szPara,szKeyString[m_cfg.idx]);
			wcstombs(m_szParaValue,m_cfg.para,32);
			wcstombs(m_szSKU,m_cfg.sku,32);
			wcstombs(m_szType,m_cfg.maintype,32);
			wcstombs(m_szSubType,m_cfg.subtype,32);
			wcstombs(m_szScreenSize,m_cfg.screensize,32);
			wcstombs(m_szTouch,m_cfg.hastouch,32);
			strcpy(m_szSN,"00000000");
			wcstombs(m_szCloud,m_cfg.Id,40);

			(wcsrchr(filepath,TEXT('\\')))[1] = 0;
			wcscat(filepath,TEXT("oa3tool.cfg"));
			fp.Open(filepath,CFile::modeCreate|CFile::modeWrite);
			sprintf(szCfg,strOATool,m_szIP,m_szPara,m_szParaValue,m_szSKU,m_szType,m_szSubType,m_szScreenSize,m_szTouch,m_szSN,m_szCloud);
			fp.Write(szCfg,(UINT)strlen(szCfg));
			fp.Close();
			SetCurrentDirectory(m_szTempDir);
			fp.Open(TEXT("oa3tool.cfg"),CFile::modeCreate|CFile::modeWrite);
			fp.Write(szCfg,(UINT)strlen(szCfg));
			fp.Close();
		}
		else
		{
			fp.Close();
			memset(&m_cfg,0,sizeof(m_cfg));
			(wcsrchr(filepath,TEXT('\\')))[1] = 0;
			wcscat(filepath,TEXT("oa3tool.cfg"));
			DeleteFile(filepath);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COa3Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(1);
		SetDlgItemText(IDC_STATUS,TEXT(""));
	}
	CDialog::OnTimer(nIDEvent);
}


void COa3Dlg::init_crc_table()  
{  
    DWORD c;  
    DWORD i, j;  
      
    for (i = 0; i < 256; i++) {  
        c = (DWORD)i;  
        for (j = 0; j < 8; j++) {  
            if (c & 1)  
                c = 0xedb88320L ^ (c >> 1);  
            else  
                c = c >> 1;  
        }  
        CRC_Table[i] = c;  
    }  
}  

DWORD COa3Dlg::CRC32(DWORD crc,BYTE *buffer, DWORD size)  
{  
    DWORD i;  
    for (i = 0; i < size; i++) {  
        crc = CRC_Table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);  
    }  
    return crc ;  
}  

BOOL COa3Dlg::GetProductKey()
{
	BOOL retval,result=FALSE;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	HANDLE hReadPipe,hWritePipe;
	DWORD retcode = -1;
	CFile fp;

	SetCurrentDirectory(m_szTempDir);
	memset(&m_pkInfo,0,sizeof(m_pkInfo));
	sa.bInheritHandle=TRUE;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	if(retval)
	{
		si.cb=sizeof STARTUPINFO;
		si.wShowWindow=SW_HIDE;
		si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
		si.hStdOutput=si.hStdError=hWritePipe;
		retval=CreateProcessA(NULL,"cmd.exe /c Check.exe",&sa,&sa,TRUE,0,NULL,0,&si,&pi);
		if(retval)
		{
			DWORD dwLen,dwRead;
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			GetExitCodeProcess(pi.hProcess,&retcode);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (retcode != 0)
			{
				goto end;
			}
			dwLen=GetFileSize(hReadPipe,NULL);
			char *buff=new char [dwLen+1];
			//memset(m_szKey,0,sizeof(m_szKey));
			char* vptr,*token="Product key:       ";
			memset(buff,0,dwLen+1);
			retval=ReadFile(hReadPipe,buff,dwLen,&dwRead,NULL);
			vptr=strstr(buff+700,token);
			if (vptr)
			{
				vptr +=strlen(token);
				strncpy(m_pkInfo.key,vptr,29);
				result = TRUE;
			}
			delete buff;
		}
		if (result == FALSE)
		{
			goto end;
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
				goto end;
			}
			if (!fp.Open(TEXT("oa3.xml"),CFile::modeRead|CFile::typeBinary))
			{
				goto end;
			}
			dwLen=(DWORD)fp.GetLength();
			char* fBuff = new char[dwLen];
			char pkid[14]={0};
			fp.Read(fBuff,dwLen);
			fp.Close();
			char* dpk=strstr(fBuff,"<ProductKeyID>");
			if (dpk)
			{
				strncpy(m_pkInfo.pkid,dpk+14,13);
			}
			delete fBuff;
		}
end:
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
	}
	return result;
}

void COa3Dlg::ProcessKeyInjection()
{

	CFile fp;
	CString szSN,szBSN;
	HANDLE hReadPipe,hWritePipe;
	char szTmp[1024]={0},szIP[32] = {0}, *szBuf=NULL, *pt=NULL;
	DWORD len,cnt,cnt2,retCode,dwLen,dwRead;
	BOOL bHasKey = FALSE,bHasCBR = FALSE,retval;
	int iCount,iVal;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	SECURITY_ATTRIBUTES sa={0};
	struct sockaddr_in  addr;//这是网络地址数据结构,服务端地址 
	struct timeval timeout={10,0};//3s
	CHWToolDlg* pParent = (CHWToolDlg*)((CHWToolApp*)AfxGetApp())->m_pMainWnd;

	sa.bInheritHandle=1;
	sa.nLength=sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor=NULL;
	retval=CreatePipe(&hReadPipe,&hWritePipe,&sa,0);
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.hStdOutput=si.hStdError=hWritePipe;

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SetCurrentDirectory(m_szTempDir);
	if (fp.Open(TEXT("oa3tool.cfg"),CFile::modeRead))
	{
		dwLen = (DWORD)fp.GetLength();
		szBuf = new char[dwLen];
		fp.Read(szBuf,dwLen);
		fp.Close();
		char *pt = strstr(szBuf,"<IPAddress>");
		char *pt2 = strstr(szBuf,"</IPAddress>");
		if (pt && pt2)
		{
			strncpy(szIP,pt+11,pt2-pt-11);
		}
		delete szBuf;
	}


	if (inet_addr(szIP) == INADDR_NONE)
	{
		MessageBox(TEXT("IP地址不合法，请检查oa3tool.cfg配置文件"),TEXT("IP错误"),MB_ICONERROR);
		goto __end;
	}

	SetDlgItemText(IDC_STATUS,TEXT("正在连接服务器......"));

	addr.sin_family = AF_INET; 
	addr.sin_port = htons(4000); 
	addr.sin_addr.S_un.S_addr = inet_addr(szIP);
	setsockopt(m_socket,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeout));
	setsockopt(m_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
	DWORD ul = 0;

	ioctlsocket(m_socket,FIONBIO,&ul);//blocking mode

	if(connect(m_socket,(sockaddr*)&addr,sizeof(addr)) != 0)
	{
		closesocket(m_socket);
		MessageBox(TEXT("连接服务器失败！"),TEXT("连接错误"),MB_ICONERROR);
		goto __end;
	}

	Sleep(500);
	iVal=send(m_socket,"handshake",(int)strlen("handshake"),0);
	Sleep(500);
	iVal=recv(m_socket,szTmp,1024,0);
	ul = GetLastError();
	if (iVal < 1 && strncmp(szTmp,"authorized",iVal))
	{
		MessageBox(TEXT("服务器授权失败，请确认软件版本是否正确"),TEXT("授权错误"),MB_ICONERROR);
		goto __end;
	}

	SetDlgItemText(IDC_STATUS,TEXT("正在查询机器的KEY......"));
	bHasKey = GetProductKey();

	if (!bHasKey)
	{
		SetDlgItemText(IDC_STATUS,TEXT("在正从服务器获取KEY......"));
		retval=CreateProcessA(NULL,"cmd.exe /c oa3tool.exe /assemble /configfile=oa3tool.cfg",&sa,&sa,0,0,NULL,NULL,&si,&pi);
		WaitForSingleObject(pi.hThread,INFINITE);
		GetExitCodeProcess(pi.hProcess,&retCode);
		if (retCode)
		{
			MessageBox(TEXT("从服务器获取KEY失败！"),TEXT("KEY错误"),MB_ICONERROR);
			goto __end;
		}
#ifdef __MAC
		GetDeviceAddress();
		GetIMEI();
#endif
		Sleep(500);
		if (fp.Open(TEXT("oa3.xml"),CFile::modeRead))
		{
			dwLen = (DWORD)fp.GetLength();
			szBuf = new char[dwLen];
			fp.Read(szBuf,dwLen);
			fp.Close();
			pt = strstr(szBuf,"<ProductKeyID>");
			if (pt)
			{
				strncpy(m_KeyInfo.PKID,pt+14,13);
			}
			pt = strstr(szBuf,"<ProductKey>");
			if (pt)
			{
				strncpy(m_KeyInfo.KEY,pt+12,29);
			}
			delete szBuf;
		}
		else
		{
			MessageBox(TEXT("打开oa3.xml文件失败，请确认是否存在！"),TEXT("错误"),MB_ICONERROR);
			goto __end;
		}

		SetDlgItemText(IDC_STATUS,TEXT("正在向机器注入KEY，请稍候......"));
		retval=CreateProcessA(NULL,"cmd.exe /c afuwin.exe /oad",0,0,0,0,NULL,NULL,&si,&pi);
		WaitForSingleObject(pi.hThread,INFINITE);
		CreateProcessA(NULL,"cmd.exe /c afuwin.exe /aoa3.bin",0,0,0,0,NULL,NULL,&si,&pi);
		WaitForSingleObject(pi.hThread,INFINITE);
		GetExitCodeProcess(pi.hProcess,&retCode);

		if (retCode)
		{
			MessageBox(TEXT("注入KEY失败！"),TEXT("错误"),MB_ICONERROR);
			goto __end;
		}
		//----------------------------------------------------------------------
#if 1
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
		retval=CreateProcessA(NULL,"cmd.exe /c amidewin.exe /bs",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
		if(retval)
		{
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			dwLen=GetFileSize(hReadPipe,NULL);
			char *buf=new char[dwLen+1];
			retval=ReadFile(hReadPipe,buf,dwLen,&dwRead,NULL);
			if (strlen(buf))
			{
				char* p1=strchr(buf,'"');
				if (p1)
				{
					p1++;
					char* p2=strchr(p1,'"');
					if (p2)
					{
						*p2 = 0;
						strcpy(m_KeyInfo.BSN,p1);
					}
				}
			}
			delete buf;
		}
		//----------------------------------------------------------------------
		retval=CreateProcessA(NULL,"cmd.exe /c amidewin.exe /ss",&sa,&sa,TRUE,0,NULL,NULL,&si,&pi);
		if(retval)
		{
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			dwLen=GetFileSize(hReadPipe,NULL);
			char *buf=new char[dwLen+1];
			retval=ReadFile(hReadPipe,buf,dwLen,&dwRead,NULL);
			if (strlen(buf))
			{
				char* p1=strchr(buf,'"');
				if (p1)
				{
					p1++;
					char* p2=strchr(p1,'"');
					if (p2)
					{
						*p2 = 0;
						strcpy(m_KeyInfo.SSN,p1);
					}
				}
			}
			delete buf;
		}
#endif
		//----------------------------------------------------------------------

		len=sizeof(KeyInfo) - 4;
		m_KeyInfo.CRC = CRC32(0xFFFFFFFF,(BYTE*)&m_KeyInfo,len);
		cnt = 1, cnt2 = 10;
		len = send(m_socket,(char*)&m_KeyInfo,sizeof(KeyInfo),0);
		Sleep(500);
		len = recv(m_socket,szTmp,1024,0);
		if (strncmp(szTmp,"techvision",len))
		{
			MessageBox(TEXT("上传CBR失败！"),TEXT("错误"),MB_ICONERROR);
			goto __end;
		}

		SetDlgItemText(IDC_STATUS,TEXT("正在对比CBR，请稍候......"));

		GetProductKey();

		if (strncmp(m_pkInfo.key,m_KeyInfo.KEY,strlen(m_KeyInfo.KEY)) == 0)//检查机器中的KEY是否和CBR中的一致
		{
			SetDlgItemText(IDC_STATUS,TEXT("正在上传CBR......"));
			iCount = 5;
			while (iCount-- > 0)
			{
				retval=CreateProcessA(NULL,"cmd.exe /c oa3tool.exe /report /configfile=oa3tool.cfg",&sa,&sa,0,0,NULL,NULL,&si,&pi);
				WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
				GetExitCodeProcess(pi.hProcess,&retCode);
				if (retCode == 0)
				{
					bHasCBR = TRUE;
				}
				if (retCode == 0xc0000134)
				{
					break;
				}
			}
			if (retCode == 0xc0000134)
			{
				if (bHasCBR)
				{
					SetDlgItemTextA(m_hWnd,IDC_PKID,m_KeyInfo.PKID);
					SetDlgItemText(IDC_STATUS,TEXT("CBR上传成功......"));
				}
				else
				{
					SetDlgItemText(IDC_STATUS,TEXT("CBR已经上传，请勿重复提交！"));
				}
				goto __end;
			}
			else
			{
				MessageBox(TEXT("CBR上传失败！"),TEXT("错误"),MB_ICONERROR);
				goto __end;
			}
		}
		else//刷完后两者不一致，需要重启才能生效
		{
			MessageBox(TEXT("需要重启机器才能上传CBR，按OK立即重启!"),TEXT("警告"),MB_ICONWARNING);
			Reboot();
			goto __end;
		}
	}
	else
	{
		SetDlgItemText(IDC_STATUS,TEXT("正在上传CBR......"));
		iCount = 5;
		while (iCount-- > 0)
		{
			retval=CreateProcessA(NULL,"cmd.exe /c oa3tool.exe /report /configfile=oa3tool.cfg",&sa,&sa,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);//等待命令行执行完毕
			GetExitCodeProcess(pi.hProcess,&retCode);
			if (retCode == 0)
			{
				bHasCBR = TRUE;
			}
			if (retCode == 0xc0000134)
			{
				break;
			}
		}
		if (retCode == 0xc0000134)
		{
			if (bHasCBR)
			{
				SetDlgItemTextA(m_hWnd,IDC_PKID,m_pkInfo.pkid);
				SetDlgItemText(IDC_STATUS,TEXT("CBR上传成功......"));
			}
			else
			{
				SetDlgItemTextA(m_hWnd,IDC_PKID,m_pkInfo.pkid);
				SetDlgItemText(IDC_STATUS,TEXT("CBR已经上传，请勿重复提交！"));
			}
			goto __end;
		}
		else
		{
			MessageBox(TEXT("CBR上传失败！"),TEXT("错误"),MB_ICONERROR);
			goto __end;
		}
	}

__end:
	SetTimer(1,8000,NULL);
	shutdown(m_socket,SD_BOTH);
	closesocket(m_socket);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	GetDlgItem(IDC_CONNECT)->EnableWindow();
	GetDlgItem(IDC_ERASE)->EnableWindow();
	GetDlgItem(IDC_INJECT)->EnableWindow();
	GetDlgItem(IDC_SERVIP)->EnableWindow();
	GetDlgItem(IDC_PWD)->EnableWindow();

}

UINT COa3Dlg::KeyThread(LPVOID lpv)
{
	COa3Dlg* p = (COa3Dlg*)lpv;
	p->ProcessKeyInjection();
	return 0;
}

void COa3Dlg::OnDestroy()
{
	CDialog::OnDestroy();
	SetCurrentDirectory(m_szTempDir);
	DeleteFile(TEXT("oa3tool.cfg"));
	WSACleanup();
	// TODO: Add your message handler code here
}

void COa3Dlg::OnBnClickedInject()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	GetDlgItem(IDC_CONNECT)->EnableWindow(0);
	GetDlgItem(IDC_ERASE)->EnableWindow(0);
	GetDlgItem(IDC_INJECT)->EnableWindow(0);
	GetDlgItem(IDC_SERVIP)->EnableWindow(0);
	GetDlgItem(IDC_PWD)->EnableWindow(0);
	CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)KeyThread,this,0,0));
}

void COa3Dlg::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_CONNECT)->EnableWindow(0);
	GetDlgItem(IDC_ERASE)->EnableWindow(0);
	GetDlgItem(IDC_INJECT)->EnableWindow(0);
	GetDlgItem(IDC_SERVIP)->EnableWindow(0);
	GetDlgItem(IDC_PWD)->EnableWindow(0);
	CloseHandle(CreateThread(0,0,(LPTHREAD_START_ROUTINE)KeyEraseThread,this,0,0));
}

int COa3Dlg::Reboot()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
	LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,0);
	ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
	CloseHandle(hToken);
	return 0;
}

void COa3Dlg::EraseKey()
{
	DWORD retCode;
	BOOL bHasKey = FALSE;
	PROCESS_INFORMATION pi={0};
	STARTUPINFOA si={0};
	si.cb=sizeof STARTUPINFOA;
	si.wShowWindow=SW_HIDE;
	si.dwFlags=STARTF_USESHOWWINDOW;

	bHasKey = GetProductKey();

	if (bHasKey)
	{
		if (IDYES == MessageBox(TEXT("确定要擦除此设备中的KEY吗？"),TEXT("警告"),MB_YESNO|MB_ICONWARNING))
		{
			SetDlgItemText(IDC_STATUS,TEXT("正在删除KEY码......"));
			CreateProcessA(NULL,"cmd.exe /c afuwin.exe /oad",0,0,0,0,NULL,NULL,&si,&pi);
			WaitForSingleObject(pi.hThread,INFINITE);
			GetExitCodeProcess(pi.hProcess,&retCode);
			if (retCode == 0)
			{
				MessageBox(TEXT("KEY擦除成功，按确认后系统将重启！"),TEXT("提示"),MB_ICONINFORMATION);
				Reboot();
			}
			else
			{
				MessageBox(TEXT("KEY擦除失败，可能需要重刷一次BIOS"),TEXT("错误"),MB_ICONERROR);
			}
			SetDlgItemText(IDC_STATUS,TEXT(""));
		}
	}
	else
	{
		MessageBox(TEXT("此机器中不存在KEY，无需擦除！"),TEXT("错误"),MB_ICONERROR);
		SetDlgItemText(IDC_STATUS,TEXT(""));
	}
	GetDlgItem(IDC_CONNECT)->EnableWindow();
	GetDlgItem(IDC_ERASE)->EnableWindow();
	GetDlgItem(IDC_INJECT)->EnableWindow();
	GetDlgItem(IDC_SERVIP)->EnableWindow();
	GetDlgItem(IDC_PWD)->EnableWindow();
}

UINT COa3Dlg::KeyEraseThread(LPVOID lpv)
{
	COa3Dlg* p = (COa3Dlg*)lpv;
	p->EraseKey();
	return 0;
}
