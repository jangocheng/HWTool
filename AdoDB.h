#ifndef __ADO_DB__
#define __ADO_DB__
#pragma once

class CAdoDB
{
public:
	CAdoDB();
	virtual ~CAdoDB();
protected:
	int AdoInit();
	int AdoUninit();
private:
	_ConnectionPtr m_pConn;
	_RecordsetPtr m_pRds;
public:
	int Connect(_bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options);
	int Open(const _variant_t & Source);
	int GetRecordsetPtr() const;

};

#endif//__ADO_DB__