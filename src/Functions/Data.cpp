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



SData::SData()
: version(0)
{

}

SData::~SData()
{

}



SDataVect::SDataVect()
{

}

SDataVect::~SDataVect()
{
	TDeletor<SData> del;
	std::for_each(begin(), end(), del);
}



SGroupData::SGroupData()
{
	data_vect = new SDataVect;
}

SGroupData::~SGroupData()
{
	data_vect->clear(); //to avoid destruction of not owned data!
	delete data_vect;
	data_vect = NULL; //just in case
}



SGroupDataVect::SGroupDataVect()
{

}

SGroupDataVect::~SGroupDataVect()
{
	TDeletor<SGroupData> del;
	std::for_each(begin(), end(), del);
}


CStoreGroupData::CStoreGroupData(CStdioFile &file, int nProgressSize)
: m_file(file),
  m_nProgressSize(nProgressSize),
  m_nProgressCurrent(0)
{

}
	
CStoreGroupData::~CStoreGroupData()
{

}
	
void CStoreGroupData::operator () (SGroupData* pGroupData)
{
	ASSERT(pGroupData);
	ASSERT(pGroupData->data_vect);

	m_file.WriteString("**********\n");
	m_file.WriteString(pGroupData->time + "\n");
	m_file.WriteString(pGroupData->user + "\n");

	for (SDataVect::iterator it = pGroupData->data_vect->begin(); pGroupData->data_vect->end() != it; ++ it)
	{
		SData *&data = (*it);
		m_file.WriteString(FormatStr("%s\n%d\n", data->file, data->version));
	}

	m_file.WriteString("\n");

	++ m_nProgressCurrent;
	if (0 == m_nProgressCurrent % 1000)
	{
		printf("\r>> %d%%", 100 * m_nProgressCurrent / m_nProgressSize);
	}
}



CDataVectGrouping::CDataVectGrouping(SGroupDataVect &group_vect)
: m_group_vect(group_vect),
  m_pLastGroupData(NULL)
{
	ASSERT(0 == m_group_vect.size());

}

CDataVectGrouping::~CDataVectGrouping()
{

}

void CDataVectGrouping::operator () (SData* pData)
{
	ASSERT_POINTER(pData, SData);

	const CString sLastUser = (NULL != m_pLastGroupData) ? m_pLastGroupData->user : "";
	const CString sLastTime = (NULL != m_pLastGroupData) ? m_pLastGroupData->time : "";
	const CString sLastDate = sLastTime.Left(10);
	const CString sThisDate = pData->time.Left(10);
	

	if (sLastUser != pData->user || sLastDate != sThisDate)
	{
		ValidateLastGroup();
		m_group_vect.push_back(m_pLastGroupData = new SGroupData);
	}

	ASSERT(NULL != m_pLastGroupData->data_vect);
	
	m_pLastGroupData->time = pData->time;
	m_pLastGroupData->user = pData->user;
	m_pLastGroupData->data_vect->push_back(pData);
}

inline bool FindGreaterVersion(SDataVect &vect, SData *pByData)
{
	SData *pTmpData;

	for (SDataVect::iterator it = vect.begin(); vect.end() != it; ++ it)
	{
		pTmpData = *it;

		if (pByData->file == pTmpData->file)
		{
			if (pByData->version < pTmpData->version)
			{
				ASSERT(1 != pByData->time.Compare(pTmpData->time));
				return true;
			}
		}
	}

	return false;
}


void CDataVectGrouping::ValidateLastGroup()
{
	if (NULL == m_pLastGroupData)
	{
		return;
	}

	ASSERT(NULL != m_pLastGroupData->data_vect);
	SDataVect &vect = *(m_pLastGroupData->data_vect);

	for (SDataVect::iterator it = vect.begin(); vect.end() != it; ++ it)
	{
		if (FindGreaterVersion(vect, *it))
		{
			vect.erase(it);
			--it;
		}
   	}
}
