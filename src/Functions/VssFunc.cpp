#include "StdAfx.h"
#include "VssFunc.h"

#include "FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const CString ss_exe = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";


void vss::init_root_workfolder(LPTSTR szWorkFolder)
{
	printf(">> set root working folder\n");

	system(ss_exe + " workfold $/ ..\\tmp\\Working");
	
	printf("\n");
	printf("\n");

	printf(">> set active root '$/'\n");
	system(ss_exe + " Project $/");
	printf("\n");
}


void vss::get_all()
{
	printf(">> generate 'all_files.txt'\n");
	if (!file::DoesFileExist("..\\tmp\\all_files.txt"))
	{
		system(ss_exe + " Dir -R -E >> ..\\tmp\\all_files.txt");
		printf("     - finished\n");
	}
	else
	{
		printf("     - skipping (already exist)\n");
	}
}
