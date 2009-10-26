#include "StdAfx.h"
#include "Data.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


template<typename T>
class TDeletor 
{
// Operations
public:
//	template <typename T>
//	void operator()(const T *ptr)
//	{
//		delete ptr;
//	}
};



SDataVect::SDataVect()
{

}

SDataVect::~SDataVect()
{
//	TDeletor<SData> del;
//	std::for_each(begin(), end(), del);

	SData *pData = new SData;
	delete pData;
}

