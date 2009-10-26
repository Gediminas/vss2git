#include "StdAfx.h"
#include "Processor.h"

#include "Data.h"
#include "VssFunc.h"
#include "GitFunc.h"

//#include "Tools/FileUtil.h"
//#include "Tools/FileUtilEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//static CString GetBasePath()
//{
//	CString sPath;
//	::GetCurrentDirectory(1000, sPath.GetBufferSetLength(1000));
//	return sPath;
//}

static bool DoesFileExist(LPTSTR szFilePath)
{
	CFileFind ff;
	return ff.FindFile(szFilePath);
}

const CString ss = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";


static void InitVss()
{
	printf(">> create folder '..\\tmp\\Working'\n");
	::CreateDirectory("..\\tmp", NULL);
	::CreateDirectory("..\\tmp\\Working", NULL);
	printf("\n");

	printf(">> set working folder\n");
	system(ss + " workfold $/ ..\\tmp\\Working");
	printf("\n");
	printf("\n");

	printf(">> set active root '$/'\n");
	system(ss + " Project $/");
	printf("\n");

	//printf(">> generate - 'history.txt' (for StdAfx.h)\n");
	//system(ss + " History $/Wood/MatrixKozijn/StdAfx.h >>..\\tmp\\history.txt");

	printf(">> generate 'all_files.txt'\n");
	if (!::DoesFileExist("..\\tmp\\all_files.txt"))
	{
		system(ss + " Dir -R -E >> ..\\tmp\\all_files.txt");
		printf("     - finished\n");
	}
	else
	{
		printf("     - skipping (already exist)\n");
	}

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



