#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



const CString ss = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";


static void InitVss()
{
	printf(">> create folder '..\\tmp\\Working'\n");
	::CreateDirectory("..\\tmp", NULL);
	::CreateDirectory("..\\tmp\\Working", NULL);

	printf(">> set as working folder\n");
	system(ss + " workfold $/ ..\\tmp\\Working");
	printf("\n");

	printf(">> Generating 'history.txt' for StdAfx.h\n");
	system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");

	printf(">> Generating 'all_files.txt'\n");
	system(ss + " Dir -R -E >> ..\\tmp\\all_files.txt");
	//printf("\n");

}

static void Collect(SDataVect &vect)
{

	vss::get_all();

};


void processor::Run()
{
	InitVss();

	SDataVect vect;
	Collect(vect);

}



