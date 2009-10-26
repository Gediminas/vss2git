#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"
#include "FileFunc.h"

//#include "Tools/FileUtil.h"
//#include "Tools/FileUtilEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static void Initialize()
{
	printf(">> create folder '..\\tmp\\Working'\n");
	::CreateDirectory("..\\tmp", NULL);
	::CreateDirectory("..\\tmp\\Working", NULL);
	printf("\n");

	vss::init_root_workfolder("..\\tmp\\Working");

	//printf(">> generate - 'history.txt' (for StdAfx.h)\n");
	//system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");
}

static void Collect(SDataVect &vect)
{

	vss::get_all();


	//printf("\n");

};


void processor::Run()
{
	Initialize();

	SDataVect vect;
	Collect(vect);

}



