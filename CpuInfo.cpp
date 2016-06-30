#include "stdafx.h"
#include "CpuInfo.h"

CCpuInfo::CCpuInfo()
{
	memset(m_szVid,0,sizeof(m_szVid));
	memset(m_szBrand,0,sizeof(m_szBrand));
}

CCpuInfo::~CCpuInfo()
{
}

void CCpuInfo::QueryCpuInfo(DWORD veax)
{
	DWORD deax,debx,decx,dedx;

	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}
	m_eax = deax;
	m_ebx = debx;
	m_ecx = decx;
	m_edx = dedx;
}

char* CCpuInfo::GetVid()
{
	QueryCpuInfo(0);
	memset(m_szVid,0,sizeof(m_szVid));
	memcpy(m_szVid,&m_ebx,4);
	memcpy(m_szVid+4,&m_edx,4);
	memcpy(m_szVid+8,&m_ecx,4);
	return m_szVid;
}

char* CCpuInfo::GetBrand()
{
	memset(m_szBrand,0,sizeof(m_szBrand));
	for (DWORD i = 0; i< 3; i++)
	{
		QueryCpuInfo(i+0x80000002);
		memcpy(m_szBrand+i*16,&m_eax,16);
	}
	return m_szBrand;
}
