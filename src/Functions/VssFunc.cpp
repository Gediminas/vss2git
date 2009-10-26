#include "StdAfx.h"
#include "VssFunc.h"

#include "FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


const CString ss_exe = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";


void vss::init_root_workfolder(LPCTSTR szWorkFolder)
{
	printf(">> set root working folder\n");

	system(FormatStr(ss_exe + " workfold $/ %s", szWorkFolder));
	
	printf("\n");
	printf("\n");

	printf(">> set active root '$/'\n");
	system(ss_exe + " Project $/");
	printf("\n");
}


void vss::list_all_files(LPCTSTR szOutputFile)
{
	if (!file::StartJob(szOutputFile))
	{
		file::CleanupJob(szOutputFile);
		
		system(FormatStr("%s Dir -R -E >> %s", ss_exe, szOutputFile));
		
		file::MarkJobDone(szOutputFile);
	}
}
