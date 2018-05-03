#include "stdafx.h"
#include "Dminfo.h"

CDminfo::CDminfo()
{

}

CDminfo::~CDminfo()
{

}

const char* CDminfo::dmi_chassis_type(BYTE code)  
{  
    /* 7.4.1 */  
    static const char *type[] = {  
        "Other", /* 0x01 */  
        "Unknown",  
        "Desktop",  
        "Low Profile Desktop",  
        "Pizza Box",  
        "Mini Tower",  
        "Tower",  
        "Portable",  
        "Laptop",  
        "Notebook",  
        "Hand Held",  
        "Docking Station",  
        "All In One",  
        "Sub Notebook",  
        "Space-saving",  
        "Lunch Box",  
        "Main Server Chassis", /* CIM_Chassis.ChassisPackageType says "Main System Chassis" */  
        "Expansion Chassis",  
        "Sub Chassis",  
        "Bus Expansion Chassis",  
        "Peripheral Chassis",  
        "RAID Chassis",  
        "Rack Mount Chassis",  
        "Sealed-case PC",  
        "Multi-system",  
        "CompactPCI",  
        "AdvancedTCA",  
        "Blade",  
        "Blade Enclosing", /* 0x1D */  
		"Tablet",
		"Convertible",
		"Detachable",
		"IoT Gateway",
		"Embedded PC",
		"Mini PC",
		"Stick PC"
    };  
  
    code &= 0x7F; /* bits 6:0 are chassis type, 7th bit is the lock bit */  
  
    if (code >= 0x01 && code <= 0x1D)  
        return type[code - 0x01];  
    return "unknown";  
}  
  
void CDminfo::dmi_system_uuid(const BYTE *p, short ver,void* uuid)  
{  
    int only0xFF = 1, only0x00 = 1;  
    int i;  

	if (uuid == NULL || IsBadReadPtr(uuid,36))
	{
		return;
	}
  
    for (i = 0; i < 16 && (only0x00 || only0xFF); i++)  
    {  
        if (p[i] != 0x00) only0x00 = 0;  
        if (p[i] != 0xFF) only0xFF = 0;  
    }  
      
    if (only0xFF)  
    {  
        printf("Not Present");  
        return;  
    }  
    if (only0x00)  
    {  
        printf("Not Settable");  
        return;  
    }  
  
    /* 
     * As of version 2.6 of the SMBIOS specification, the first 3 
     * fields of the UUID are supposed to be encoded on little-endian. 
     * The specification says that this is the defacto standard, 
     * however I've seen systems following RFC 4122 instead and use 
     * network byte order, so I am reluctant to apply the byte-swapping 
     * for older versions. 
     */  
    char ch[256];memset(ch,0,sizeof(ch));  
    if (ver >= 0x0206)  
        sprintf(ch,"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",  
            p[3], p[2], p[1], p[0], p[5], p[4], p[7], p[6],  
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);  
    else  
        sprintf(ch,"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",  
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],  
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);  
    printf("%s\n",ch);  
    strcpy((char*)uuid,ch);  
}  
  
const char* CDminfo::dmi_string(const dmi_header *dm, BYTE s)  
{  
    char *bp = (char *)dm;  
    size_t i, len;  
  
    if (s == 0)  
        return "Not Specified";  
  
    bp += dm->length;  
    while (s > 1 && *bp)  
    {  
        bp += strlen(bp);  
        bp++;  
        s--;  
    }  
  
    if (!*bp)  
        return "BAD_INDEX";  
  
    /* ASCII filtering */  
    len = strlen(bp);  
    for (i = 0; i < len; i++)  
        if (bp[i] < 32 || bp[i] == 127)  
            bp[i] = '.';  
  
    return bp;  
}  
  
int CDminfo::GetDMInfo(dm_infoA* dminfo)  
{  
    int ret = 0;  
    RawSMBIOSData *Smbios;  
    dmi_header *h = NULL;  
	//char uuid[64];
    int flag = 1;  

	memset(dminfo,0,sizeof(dm_infoA));
 
	char buff[4096]={0};
	ret = GetSystemFirmwareTable('ACPI','TDSX',0,0);
	if (ret <= 0)
	{
		return 0;
	}
	GetSystemFirmwareTable('ACPI','TDSX',buff,ret);
	memcpy(dminfo->dmi_oem_id,buff+10,6);
	memcpy(dminfo->dmi_oem_table_id,buff+16,8);

    ret = GetSystemFirmwareTable('RSMB', 0, 0, 0);  
    if (!ret)  
    {  
        printf("Function failed!\n");  
        return 1;  
    }  
  
    DWORD bufsize = ret;  
    char *buf = new char[bufsize];  
    memset(buf,0,bufsize); 
  
    ret = GetSystemFirmwareTable('RSMB', 0, buf, bufsize);  
    if (!ret)  
    {  
        printf("Function failed!\n");  
        delete []buf;  
        return 1;  
    }  
  
    Smbios = (RawSMBIOSData *)buf;  
  
    BYTE *p = Smbios->SMBIOSTableData;  
	BYTE *end = p + Smbios->Length;
  
    if (Smbios->Length != bufsize -8 )  
    {  
        printf("Smbios length error\n");  
        delete []buf;  
        return 1;  
    }  
  
    while (p < end){  
        h = (dmi_header *)p;  
		switch (h->type)
		{
		case 0://bios information
			strcpy(dminfo->dmi_bios_vendor,dmi_string(h, p[0x4]));
			strcpy(dminfo->dmi_bios_version,dmi_string(h, p[0x5]));
			strcpy(dminfo->dmi_bios_release_date,dmi_string(h, p[0x8]));
			break;
		case 1://system information
			sprintf(dminfo->dmi_sys_uuid,"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15],  
            p[16], p[17], p[18], p[19], p[20], p[21], p[22], p[23]);  

			//dmi_system_uuid(p+0x8, Smbios->SMBIOSMajorVersion*0x100+Smbios->SMBIOSMinorVersion,&dminfo->dmi_sys_uuid); 
			strcpy(dminfo->dmi_sys_manufacture,dmi_string(h, p[0x4]));
			strcpy(dminfo->dmi_sys_product,dmi_string(h, p[0x5]));
			strcpy(dminfo->dmi_sys_version,dmi_string(h, p[0x6]));
			strcpy(dminfo->dmi_sys_serial_number,dmi_string(h, p[0x7]));
			strcpy(dminfo->dmi_sys_sku,dmi_string(h, p[0x19]));
			strcpy(dminfo->dmi_sys_family,dmi_string(h, p[0x1a]));
			break;
		case 2://baseboard information
			strcpy(dminfo->dmi_bsbrd_manufacture,dmi_string(h, p[0x4]));
			strcpy(dminfo->dmi_bsbrd_product,dmi_string(h, p[0x5]));
			strcpy(dminfo->dmi_bsbrd_version,dmi_string(h, p[0x6]));
			strcpy(dminfo->dmi_bsbrd_serial,dmi_string(h, p[0x7]));
			break;
		case 3://system enclosure or chassis
			strcpy(dminfo->dmi_chassis_type,dmi_chassis_type(p[0x5]));
			break;
		case 4://processor information
			break;
		case 5://memory controller information
			break;
		case 6://memory module information
			break;
		case 7://cache information
			break;
		case 8://port connector information
			break;
		case 9://system slots
			break;
		case 10://on board devices information
			break;
		case 11://oem strings
			break;
		case 12://system configuration
			break;
		case 13://bios language information
			break;
		case 14://group associations
			break;
		case 15://system event log
			break;
		case 16://physical memory array
			break;
		case 17://memory devices
			break;
		case 18://32-bit memory error information
			break;
		case 19://memory array mapped address
			break;
		case 20://memory device mapped address
			break;
		case 21://built-in pointing device
			break;
		case 22://portable battery
			break;
		case 23://system reset
			break;
		case 24://hardware security
			break;
		case 25://system power controls
			break;
		case 26://voltage probe
			break;
		case 27://colling device
			break;
		case 28://temperature probe
			break;
		case 29://electrical current probe
			break;
		case 30://out-of-band remote access
			break;
		case 31://boot integrity services(BIS) entry point
			break;
		case 32://system boot information
			break;
		case 33://64-bit memory error information
			break;
		case 34://management device
			break;
		case 35://management device component
			break;
		case 36://management device threshold data
			break;
		case 37://memory channel
			break;
		case 38://ipmi device information
			break;
		case 39://system power supply
			break;
		case 40://additional information
			break;
		case 41://onboard devices extended information
			break;
		case 42://management controller host interface
			break;
		case 43://tpm device
			break;
		case 126://inactive
			break;
		case 127://end-of-table
			break;
		default:
			break;
		}

		p += h->length; 
		if (*p == 0)
		{
			p += 2;
			continue;
		}
        while(*p) p += strlen((char*)p) + 1;  
        p ++;  
    }  

	delete []buf;  
    return 0;  
}  
