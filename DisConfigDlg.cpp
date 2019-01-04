// DisConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HWTool.h"
#include "DisConfigDlg.h"

// CDisConfigDlg dialog

IMPLEMENT_DYNAMIC(CDisConfigDlg, CDialog)
wchar_t* subTypes[3][3] = 
{
	{TEXT("Standard"),TEXT("AIO"),TEXT("")},
	{TEXT("Standard"),TEXT("Ultraslim"),TEXT("Convertible")},
	{TEXT("Standard"),TEXT(""),TEXT("")}
};

CDisConfigDlg::CDisConfigDlg(DpkCfg* pCfg, int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CDisConfigDlg::IDD, pParent)
	, m_nIndex(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_OA);
	m_nIndex = 0;
	m_pCfg = pCfg;
	m_nType = nType;
	m_ConfigurationList.RemoveAll();
	m_ParaLicList.RemoveAll();
	m_ParaOemPOList.RemoveAll();
	m_ParaOemPartList.RemoveAll();
	try
	{
		m_pConn.CreateInstance(__uuidof(Connection));
		m_pRds.CreateInstance(__uuidof(Recordset));
	}
	catch (_com_error e)
	{
		TRACE(e.ErrorMessage());
	}
}

CDisConfigDlg::~CDisConfigDlg()
{
}

void CDisConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nIndex);
	DDV_MinMaxInt(pDX, m_nIndex, 0, 2);
}


BEGIN_MESSAGE_MAP(CDisConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CDisConfigDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDisConfigDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &CDisConfigDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDOK, &CDisConfigDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_BUSINESS, &CDisConfigDlg::OnCbnSelchangeBusiness)
	ON_CBN_SELCHANGE(IDC_TYPE, &CDisConfigDlg::OnCbnSelchangeType)
END_MESSAGE_MAP()


// CDisConfigDlg message handlers

void CDisConfigDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	POSITION pos;
	int sel;
	CString lbText;
	ParaIndex paraIndex;
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_BUSINESS);
	CComboBox* pSelBox = (CComboBox*)GetDlgItem(IDC_SELNUMBER);
	pSelBox->ResetContent();
	sel = pBox->GetCurSel();
	pBox->GetLBText(sel,lbText);

	switch (m_nIndex)
	{
	case 0:
		pos = m_ParaLicList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaLicList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	case 1:
		pos = m_ParaOemPOList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaOemPOList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	case 2:
		pos = m_ParaOemPartList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaOemPartList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	default:
		break;
	}
}

void CDisConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	Configuration cfg;
	int nBusiness,nType,nSubType,nTouch,nSelNumber;
	wchar_t szScreenSize[32],szModel[32];
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_BUSINESS);
	CComboBox* pBoxType = (CComboBox*)GetDlgItem(IDC_TYPE);
	CComboBox* pBoxSubType = (CComboBox*)GetDlgItem(IDC_SUBTYPE);
	CComboBox* pBoxTouch = (CComboBox*)GetDlgItem(IDC_TOUCH);
	CComboBox* pSelBox = (CComboBox*)GetDlgItem(IDC_SELNUMBER);
	nBusiness = pBox->GetCurSel();
	nType = pBoxType->GetCurSel();
	nSubType = pBoxSubType->GetCurSel();
	nTouch = pBoxTouch->GetCurSel();
	nSelNumber = pSelBox->GetCurSel();
	GetDlgItemText(IDC_SCREEN,szScreenSize,32);
	GetDlgItemText(IDC_MODEL,szModel,32);
	UpdateData();
	if (nBusiness == -1 || nType == -1 || nSubType == -1 || nTouch == -1 || nSelNumber == -1 || wcslen(szScreenSize) == 0 || wcslen(szModel) == 0)
	{
		MessageBox(TEXT("����ɱ�������еĲ������ã�"),TEXT("����"),MB_ICONWARNING);
	}
	else
	{
		wchar_t lbText[32];
		pBox->GetLBText(nBusiness,lbText);
		wcscpy(m_pCfg->business,lbText);
		pos = m_ConfigurationList.GetHeadPosition();
		while (pos)
		{
			cfg = m_ConfigurationList.GetNext(pos);
			if (wcscmp(m_pCfg->business,cfg.SiteName) == 0)
			{
				wsprintf(m_pCfg->Id,TEXT("%d"),cfg.BusinessID);
				break;
			}
		}
		pBoxType->GetLBText(nType,lbText);
		wcscpy(m_pCfg->maintype,lbText);
		pBoxSubType->GetLBText(nSubType,lbText);
		wcscpy(m_pCfg->subtype,lbText);
		pBoxTouch->GetLBText(nTouch,lbText);
		wcscpy(m_pCfg->hastouch,lbText);
		pSelBox->GetLBText(nSelNumber,lbText);
		wcscpy(m_pCfg->para,lbText);
		m_pCfg->idx = m_nIndex;
		wcscpy(m_pCfg->sku,szModel);
		wcscpy(m_pCfg->screensize,szScreenSize);
	}

	OnOK();
}

BOOL CDisConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetIcon(m_hIcon,FALSE);
	SetIcon(m_hIcon,TRUE);
	//must call BuildConfiguration() before call this.
	BuildParameter();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDisConfigDlg::OnDestroy()
{
	CDialog::OnDestroy();
	try
	{
		if (m_pRds->GetState() & adStateOpen) 
		{
			m_pRds->Close();
		}
		if (m_pConn->GetState() & adStateOpen) 
		{
			m_pConn->Close();
		}
		m_pRds.Release();
		m_pConn.Release();
	}
	catch (_com_error e)
	{
		TRACE(e.ErrorMessage());
	}
	m_ConfigurationList.RemoveAll();
	m_ParaLicList.RemoveAll();
	m_ParaOemPOList.RemoveAll();
	m_ParaOemPartList.RemoveAll();
	// TODO: Add your message handler code here
}


int CDisConfigDlg::BuildConfiguration(void)
{
	//SELECT  LicensablePartNumber  FROM ProductKeyInfo group by LicensablePartNumber
	POSITION pos;
	CString connectionstr, strXmlDesc;

	if (m_nType == 1)
	{
		connectionstr.Format(TEXT("Provider=SQLOLEDB.1;Data Source=%s\\MDOS,1449;Initial Catalog=SmartClient;User ID=admin;Password=!Flex123"),m_pCfg->ip);
		try
		{
			if (m_pConn->GetState() & adStateOpen) 
			{
				m_pConn->Close();
			}
			//	Configuration cfg = {0};
			m_pConn->Open((LPTSTR)(LPCTSTR)connectionstr, TEXT(""),TEXT(""),adModeUnknown);

			//---------------------------------------read configuration from database-----------------------------------------------------
			if (m_pRds->GetState() & adStateOpen) 
			{
				m_pRds->Close();
			}
			//load profile table
			m_pRds->Open(TEXT("SELECT ProfileID,BusinessID,SiteID,IsActive,ClientID,SecretID,SiteName,BusinessName,EmailID FROM Profile"), m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
			while (!m_pRds->adoEOF)
			{
				Configuration cfg = { 0 };
				_variant_t var,index;
				index.vt = VT_I4;
				index.intVal=0;
				var = m_pRds->GetCollect(index);
				if(var.vt!=VT_NULL)
				{
					cfg.ProfileID = var.intVal;
				}
				index.intVal = 1;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					cfg.BusinessID = var.intVal;
				}
				index.intVal = 2;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					cfg.SiteID = var.intVal;
				}
				index.intVal = 3;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					cfg.IsActive = var.intVal;
				}
				index.intVal = 4;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					wcscpy(cfg.ClientID,var.bstrVal);
				}
				index.intVal = 5;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					wcscpy(cfg.SecretID, var.bstrVal);
				}
				index.intVal = 6;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					wcscpy(cfg.SiteName, var.bstrVal);
				}
				index.intVal = 7;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					wcscpy(cfg.BusinessName, var.bstrVal);
				}
				index.intVal = 8;
				var = m_pRds->GetCollect(index);
				if (var.vt != VT_NULL)
				{
					wcscpy(cfg.EmailID, var.bstrVal);
				}
				m_ConfigurationList.AddTail(cfg);
				m_pRds->MoveNext();
			}


			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			pos = m_ConfigurationList.GetHeadPosition();
			while (pos)
			{
				Configuration cfg = m_ConfigurationList.GetNext(pos);
				wchar_t wszSql[256]={0};
				if (m_pRds->GetState() & adStateOpen) 
				{
					m_pRds->Close();
				}
				//select distinct A.LicensablePartNumber from ProductKeyInfo as A, KeyInfoEx as B where A.ProductKeyID=B.ProductKeyID
				wsprintf(wszSql,TEXT("select distinct A.LicensablePartNumber from ProductKey as A, Profile as B where A.ProfileID=B.ProfileID and B.IsActive=1 and A.ProfileID=%d"),cfg.ProfileID);
				m_pRds->Open(wszSql, m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
				while (!m_pRds->adoEOF)
				{
					ParaIndex pdx = {0};
					_variant_t var,index;
					wcscpy(pdx.Name,cfg.SiteName);
					index.vt = VT_I4;
					index.intVal=0;
					var = m_pRds->GetCollect(index);
					if(var.vt!=VT_NULL)
					{
						wcscpy(pdx.Parameter,var.bstrVal);
					}
					m_ParaLicList.AddTail(pdx);
					m_pRds->MoveNext();
				}
				if (m_pRds->GetState() & adStateOpen) 
				{
					m_pRds->Close();
				}
				wsprintf(wszSql,TEXT("select distinct A.OEMPONumber from ProductKey as A, Profile as B where A.ProfileID=B.ProfileID and B.IsActive=1 and A.ProfileID=%d"), cfg.ProfileID);
				m_pRds->Open(wszSql, m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
				while (!m_pRds->adoEOF)
				{
					ParaIndex pdx = {0};
					_variant_t var,index;
					wcscpy(pdx.Name,cfg.SiteName);
					index.vt = VT_I4;
					index.intVal=0;
					var = m_pRds->GetCollect(index);
					if(var.vt!=VT_NULL)
					{
						wcscpy(pdx.Parameter,var.bstrVal);
					}
					m_ParaOemPOList.AddTail(pdx);
					m_pRds->MoveNext();
				}
				if (m_pRds->GetState() & adStateOpen) 
				{
					m_pRds->Close();
				}
				wsprintf(wszSql,TEXT("select distinct A.OEMPartNumber from ProductKey as A, Profile as B where A.ProfileID=B.ProfileID and B.IsActive=1 and A.ProfileID=%d"), cfg.ProfileID);
				m_pRds->Open(wszSql, m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
				while (!m_pRds->adoEOF)
				{
					ParaIndex pdx = {0};
					_variant_t var,index;
					wcscpy(pdx.Name,cfg.SiteName);
					index.vt = VT_I4;
					index.intVal=0;
					var = m_pRds->GetCollect(index);
					if(var.vt!=VT_NULL)
					{
						wcscpy(pdx.Parameter,var.bstrVal);
					}
					m_ParaOemPartList.AddTail(pdx);
					m_pRds->MoveNext();
				}
				if (m_pRds->GetState() & adStateOpen) 
				{
					m_pRds->Close();
				}
			}
		}
		catch (_com_error e)
		{
			_tcscpy(m_szDesc,e.Description());
			goto err;
		}
	}
	return 0;
err:
	try
	{
		if (m_pRds->GetState() & adStateOpen) 
		{
			m_pRds->Close();
		}
		if (m_pConn->GetState() & adStateOpen) 
		{
			m_pConn->Close();
		}
	}
	catch (_com_error e)
	{
		_tcscpy(m_szDesc,e.Description());
	}

	return -1;
}

int CDisConfigDlg::BuildParameter()
{
	POSITION pos = m_ConfigurationList.GetHeadPosition();
	int sel,count;
	CString lbText;
	ParaIndex paraIndex;
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_BUSINESS);
	CComboBox* pBoxType = (CComboBox*)GetDlgItem(IDC_TYPE);
	CComboBox* pBoxSubType = (CComboBox*)GetDlgItem(IDC_SUBTYPE);
	CComboBox* pBoxTouch = (CComboBox*)GetDlgItem(IDC_TOUCH);
	CComboBox* pSelBox = (CComboBox*)GetDlgItem(IDC_SELNUMBER);
	pSelBox->ResetContent();
	pBox->ResetContent();
	pBoxSubType->ResetContent();
	while(pos)
	{
		Configuration cfg = m_ConfigurationList.GetNext(pos);
		pBox->AddString(cfg.SiteName);
	}
	//business
	count = pBox->GetCount();
	pBox->SetCurSel(0);
	for (int i=0;i<count;i++)
	{
		pBox->GetLBText(i,lbText);
		if (lbText.Compare(m_pCfg->business) == 0)
		{
			pBox->SetCurSel(i);
			break;
		}
	}
	//form factor class
	count = pBoxType->GetCount();
	pBoxType->SetCurSel(0);
	for (int i=0;i<count;i++)
	{
		pBoxType->GetLBText(i,lbText);
		if (lbText.Compare(m_pCfg->maintype) == 0)
		{
			pBoxType->SetCurSel(i);
			break;
		}
	}
	//form factor subclass
	sel = pBoxType->GetCurSel();

	for (int i=0;i<3;i++)
	{
		if (wcslen(subTypes[sel][i]))
		{
			pBoxSubType->AddString(subTypes[sel][i]);
		}
	}
	count = pBoxSubType->GetCount();
	pBoxSubType->SetCurSel(0);
	for (int i=0;i<count;i++)
	{
		pBoxSubType->GetLBText(i,lbText);
		if (lbText.Compare(m_pCfg->subtype) == 0)
		{
			pBoxSubType->SetCurSel(i);
			break;
		}
	}
	//touch
	count = pBoxTouch->GetCount();
	pBoxTouch->SetCurSel(0);
	for (int i=0;i<count;i++)
	{
		pBoxTouch->GetLBText(i,lbText);
		if (lbText.Compare(m_pCfg->hastouch) == 0)
		{
			pBoxTouch->SetCurSel(i);
			break;
		}
	}

	SetDlgItemText(IDC_MODEL,m_pCfg->sku);
	SetDlgItemText(IDC_SCREEN,m_pCfg->screensize);
	m_nIndex = m_pCfg->idx;
	UpdateData(FALSE);
	sel = pBox->GetCurSel();

	pBox->GetLBText(sel,lbText);
	if (sel != -1)//business selection is valid
	{
		switch (m_pCfg->idx)
		{
		case 0:
			pos = m_ParaLicList.GetHeadPosition();
			while (pos)
			{
				paraIndex = m_ParaLicList.GetNext(pos);
				if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
				{
					pSelBox->AddString(paraIndex.Parameter);
				}
			}
			count = pSelBox->GetCount();
			pSelBox->SetCurSel(0);
			for (int i=0;i<count;i++)
			{
				pSelBox->GetLBText(i,lbText);
				if (lbText.Compare(m_pCfg->para) == 0)
				{
					pSelBox->SetCurSel(i);
					break;
				}
			}
			break;
		case 1:
			pos = m_ParaOemPOList.GetHeadPosition();
			while (pos)
			{
				paraIndex = m_ParaOemPOList.GetNext(pos);
				if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
				{
					pSelBox->AddString(paraIndex.Parameter);
				}
			}
			count = pSelBox->GetCount();
			pSelBox->SetCurSel(0);
			for (int i=0;i<count;i++)
			{
				pSelBox->GetLBText(i,lbText);
				if (lbText.Compare(m_pCfg->para) == 0)
				{
					pSelBox->SetCurSel(i);
					break;
				}
			}
			break;
		case 2:
			pos = m_ParaOemPartList.GetHeadPosition();
			while (pos)
			{
				paraIndex = m_ParaOemPartList.GetNext(pos);
				if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
				{
					pSelBox->AddString(paraIndex.Parameter);
				}
			}
			count = pSelBox->GetCount();
			pSelBox->SetCurSel(0);
			for (int i=0;i<count;i++)
			{
				pSelBox->GetLBText(i,lbText);
				if (lbText.Compare(m_pCfg->para) == 0)
				{
					pSelBox->SetCurSel(i);
					break;
				}
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
void CDisConfigDlg::OnCbnSelchangeBusiness()
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	int sel;
	CString lbText;
	ParaIndex paraIndex;
	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_BUSINESS);
	CComboBox* pSelBox = (CComboBox*)GetDlgItem(IDC_SELNUMBER);
	pSelBox->ResetContent();
	sel = pBox->GetCurSel();
	pBox->GetLBText(sel,lbText);

	switch (m_nIndex)
	{
	case 0:
		pos = m_ParaLicList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaLicList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	case 1:
		pos = m_ParaOemPOList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaOemPOList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	case 2:
		pos = m_ParaOemPartList.GetHeadPosition();
		while (pos)
		{
			paraIndex = m_ParaOemPartList.GetNext(pos);
			if (wcscmp(paraIndex.Name,lbText) == 0 && wcslen(paraIndex.Parameter))
			{
				pSelBox->AddString(paraIndex.Parameter);
			}
		}
		pSelBox->SetCurSel(0);
		break;
	default:
		break;
	}
}

void CDisConfigDlg::OnCbnSelchangeType()
{
	// TODO: Add your control notification handler code here
	int sel;
	CComboBox* pBoxType = (CComboBox*)GetDlgItem(IDC_TYPE);
	CComboBox* pBoxSubType = (CComboBox*)GetDlgItem(IDC_SUBTYPE);
	pBoxSubType->ResetContent();
	sel = pBoxType->GetCurSel();
	for (int i=0;i<3;i++)
	{
		if (wcslen(subTypes[sel][i]))
		{
			pBoxSubType->AddString(subTypes[sel][i]);
		}
	}
	pBoxSubType->SetCurSel(0);
}
