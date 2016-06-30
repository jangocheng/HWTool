#include "stdafx.h"
#include "AdoDB.h"

CAdoDB::CAdoDB()
{
	AdoInit();
}

CAdoDB::~CAdoDB()
{
	AdoUninit();
}

int CAdoDB::AdoInit()
{
	CoInitialize(NULL);
	try
	{
		m_pConn.CreateInstance(__uuidof(Connection));
		m_pRds.CreateInstance(__uuidof(Recordset));
	}
	catch (_com_error e)
	{
		return -1;
	}
	return 0;
}

int CAdoDB::AdoUninit()
{
	int ret = 0;
	try
	{
		if (m_pConn->GetState() & adStateOpen) 
		{
			m_pConn->Close();
		}
		if (m_pRds->GetState() & adStateOpen) 
		{
			m_pRds->Close();
		}
		m_pRds.Release();
		m_pConn.Release();
	}
	catch (_com_error e)
	{
		ret = -1;
	}
	CoUninitialize();
	return ret;
}

int CAdoDB::Connect(_bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options)
{
	int ret = 0;
	HRESULT hr = 0;
	try
	{
		hr = m_pConn->Open(ConnectionString, UserID, Password, Options);
	}
	catch (_com_error e)
	{
		ret = -1;
	}
	if (FAILED(hr))
	{
		ret = -1;
	}
	return ret;
}

int CAdoDB::Open(const _variant_t & Source)
{
	int ret = 0;
	HRESULT hr = 0;
	try
	{
		hr = m_pRds->Open(Source, m_pConn.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdTable);
	}
	catch (_com_error e)
	{
		ret = -1;
	}
	if (FAILED(hr))
	{
		ret = -1;
	}
	ret = m_pRds->Fields->Count;
	return ret;
}

int CAdoDB::GetRecordsetPtr() const
{
	return m_pRds;
}