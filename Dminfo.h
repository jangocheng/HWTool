#ifndef __DMINFO__
#define __DMINFO__
#pragma once

typedef struct _dmi_header  
{  
    BYTE type;  
    BYTE length;  
    WORD handle;  
}dmi_header;  
  
typedef struct _RawSMBIOSData  
{  
    BYTE    Used20CallingMethod;  
    BYTE    SMBIOSMajorVersion;  
    BYTE    SMBIOSMinorVersion;  
    BYTE    DmiRevision;  
    DWORD   Length;  
    BYTE    SMBIOSTableData[1];  
}RawSMBIOSData;  

typedef struct _dm_infoA_
{
	char dmi_bios_vendor[64];	//BIOS vendor name.
	char dmi_bios_version[64];	//BIOS version
	char dmi_bios_release_date[64];	//BIOS release date
	char dmi_sys_manufacture[64];	//System manufacture
	char dmi_sys_product[64];	//System product
	char dmi_sys_version[64];	//System version
	char dmi_sys_serial_number[64];	//System Serial number
	char dmi_sys_uuid[64];	//System UUID
	char dmi_sys_sku[64];	//System SKU number
	char dmi_sys_family[64];	//System Family
	char dmi_bsbrd_manufacture[64];	//Baseboard manufacture
	char dmi_bsbrd_product[64];	//Baseboard product
	char dmi_bsbrd_version[64];	//Baseboard version
	char dmi_bsbrd_serial[64];	//Baseboard Serial number
	char dmi_chassis_type[64];
	char dmi_oem_table_id[64];
	char dmi_oem_id[64];
}dm_infoA;

typedef struct _dm_infoW_
{
	wchar_t dmi_bios_vendor[64];	//BIOS vendor name.
	wchar_t dmi_bios_version[64];	//BIOS version
	wchar_t dmi_bios_release_date[64];	//BIOS release date
	wchar_t dmi_sys_manufacture[64];	//System manufacture
	wchar_t dmi_sys_product[64];	//System product
	wchar_t dmi_sys_version[64];	//System version
	wchar_t dmi_sys_serial_number[64];	//System Serial number
	wchar_t dmi_sys_uuid[64];	//System UUID
	wchar_t dmi_sys_sku[64];	//System SKU number
	wchar_t dmi_sys_family[64];	//System Family
	wchar_t dmi_bsbrd_manufacture[64];	//Baseboard manufacture
	wchar_t dmi_bsbrd_product[64];	//Baseboard product
	wchar_t dmi_bsbrd_version[64];	//Baseboard version
	wchar_t dmi_bsbrd_serial[64];	//Baseboard Serial number
	wchar_t dmi_chassis_type[64];
	wchar_t dmi_oem_table_id[64];
	wchar_t dmi_oem_id[64];
}dm_infoW;

class CDminfo
{
public:
	CDminfo();
	virtual ~CDminfo();
	int GetDMInfo(dm_infoA* dminfo);
protected:
	void dmi_system_uuid(const BYTE *p, short ver,void* uuid);
	const char* dmi_string(const dmi_header *dm, BYTE s);
	const char* dmi_chassis_type(BYTE code);
};

#endif//__DMINFO__