#include "StdAfx.h"
#include "Data.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


bool data::compare_by_time_user(const void* ppData1, const void* ppData2)
{
	const SData *pData1 = (const SData*) ppData1;
	const SData *pData2 = (const SData*) ppData2;

	ASSERT_POINTER(pData1, SData);
	ASSERT_POINTER(pData2, SData);

	if (pData1 == pData2)
	{
		return false;
	}

	CString sx = pData1->time + " " + pData1->user;
	CString sy = pData2->time + " " + pData2->user;
	
	if (sx == sy)
	{
		return false;
	}

	if (-1 != sx.CompareNoCase(sy))
	{
		return false;
	}

	return true;
}



template<typename T>
class TDeletor 
{
public:
	void operator()(const T *ptr)
	{
		delete ptr;
	}
};



SDataVect::SDataVect()
{

}

SDataVect::~SDataVect()
{
	TDeletor<SData> del;
	std::for_each(begin(), end(), del);
}



CStoreData::CStoreData(CStdioFile &file, int nProgressSize)
: m_file(file),
  m_nProgressSize(nProgressSize),
  m_nProgressCurrent(0)
{

}
	
CStoreData::~CStoreData()
{

}
	
CStoreData::operator () (SData* pData)
{
	CString s;
	s += pData->time;
	s += "\t";
	s += pData->user;
	s += "\t";
	s += FormatStr("%d", pData->version);
	s += "\t";
	s += pData->file;
	s += "\n";

	m_file.WriteString(s);

	++ m_nProgressCurrent;
	if (0 == m_nProgressCurrent % 1000)
	{
		printf("\r>> %d%%", 100 * m_nProgressCurrent / m_nProgressSize);
	}
}

