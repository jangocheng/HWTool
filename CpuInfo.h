#pragma once

class CCpuInfo
{
public:
	CCpuInfo();
	virtual ~CCpuInfo();
	char* GetVid();
	char* GetBrand();
private:
	void QueryCpuInfo(DWORD veax);
	DWORD m_eax;
	DWORD m_ebx;
	DWORD m_ecx;
	DWORD m_edx;
	char  m_szVid[13];
	char  m_szBrand[49];
};
