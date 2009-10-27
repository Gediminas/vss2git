#ifndef __Data_H__
#define __Data_H__
#pragma once


struct SData
{
	CString file;
	CString time;
	CString user;
	CString comment;
	CString label;
	CString label_comment;
	int     version;

	SData();
	~SData();
};

class SDataVect : public std::vector<SData*>
{
public:
	SDataVect();
	~SDataVect();
};


struct SGroupData
{
	CString    time;
	CString    user;
	SDataVect *data_vect;

	SGroupData();
	~SGroupData();
};

class SGroupDataVect : public std::vector<SGroupData*>
{
public:
	SGroupDataVect();
	~SGroupDataVect();
};


class CDataVectGrouping
{
public:
	CDataVectGrouping(SGroupDataVect &group_vect);
	void operator () (SData* pData);
	~CDataVectGrouping();

	void ValidateLastGroup();

private:
	CString sLastTime;
	CString sLastUser;
	SGroupData *m_pLastGroupData;
	SGroupDataVect &m_group_vect;
};


class CStoreGroupData
{
public:
	CStoreGroupData(CStdioFile &file, int nProgressSize);
	~CStoreGroupData();
	void operator () (SGroupData* pGroupData);

private:
	CStdioFile &m_file;
	int         m_nProgressSize;
	int         m_nProgressCurrent;
};


namespace data
{
	bool compare_by_time_user(const void* ppData1, const void* ppData2);
};
//namespace data


#endif //__Data_H__
