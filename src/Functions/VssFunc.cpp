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


void vss::list_all_files(LPTSTR szOutputFile)
{
	printf(">> generate 'all_files.txt'\n");
	if (!file::DoesFileExist(szOutputFile))
	{
		CString sCommand;
		sCommand.Format("%s Dir -R -E >> %s", ss_exe, szOutputFile);
		system(sCommand);

		printf("     - finished\n");
	}
	else
	{
		printf("     - skipping (already exist)\n");
	}
}
