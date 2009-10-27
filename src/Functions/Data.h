#ifndef __Data_H__
#define __Data_H__
#pragma once


struct SData
{
	SData() : version(0) { }
	CString file;
	CString time;
	CString user;
	CString comment;
	CString label;
	CString label_comment;
	int     version;
};


class SDataVect : public std::vector<SData*>
{
public:
	SDataVect();
	~SDataVect();
};


class CStoreData
{
public:
	CStoreData(CStdioFile &file, int nProgressSize);
	~CStoreData();
	operator () (SData* pData);

private:
	CStdioFile &m_file;
	int         m_nProgressSize;
	int         m_nProgressCurrent;
};


namespace data
{
	int compare_by_time_user(const SData* x, const SData* y);
};
//namespace data


#endif //__Data_H__
