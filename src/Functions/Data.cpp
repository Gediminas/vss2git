#include "StdAfx.h"
#include "Data.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int data::compare_by_time_user(const SData* x, const SData* y)
{
	ASSERT_POINTER(x, SData);
	ASSERT_POINTER(y, SData);
//
//	if (x == y)
//	{
		return 0;
//	}
//
//	CString sx = x->time + " " + x->user;
//	CString sy = y->time + " " + y->user;
//	
//	//TRACE("'%s' <> '%s'\n", sx, sy);
//
//	if (sx == sy)
//	{
//		return 0;
//	}
//
//	return sx.CompareNoCase(sy);
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

