#include "StdAfx.h"
#include "VssFunc.h"

#include "FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//const CString ss_exe = "\"c:\\Program Files\\Microsoft Visual Studio\\Common\\VSS\\win32\\ss.exe\"";
const CString ss_exe = "..\\binSS\\ss.exe";


void vss::init_root_workfolder(LPCTSTR szWorkFolder)
{
	printf(">> set root working folder\n");

	RUN(FormatStr(ss_exe + " workfold $/ %s", szWorkFolder));
	
	printf("\n");
	printf("\n");

	printf(">> set active root '$/'\n");
	RUN(ss_exe + " Project $/");
	printf("\n");
}


void vss::list_all_files(LPCTSTR szOutputFile)
{
	if (file::StartJob(szOutputFile))
	{
		file::CleanupJob(szOutputFile);
		
		RUN(FormatStr("%s Dir -R -E >> %s", ss_exe, szOutputFile));
		
		file::MarkJobDone(szOutputFile);
	}
}

void vss::list_file_versions(LPCTSTR szVssFile, LPCTSTR szOutputFile)
{
	::DeleteFile(szOutputFile);

	CString sCommand;
	sCommand.Format(FormatStr("%s History \"%s\" >> %s", ss_exe, szVssFile, szOutputFile));
	RUN(sCommand);
}

void vss::get_file(LPCTSTR szVssFile, int nVssFileVersion, LPCTSTR szWorkingDir, LPCTSTR szOutputFile)
{
	CString sTo = szVssFile;
	sTo.Replace("$", szWorkingDir);
	sTo.Replace("/", "\\");
	const int nCut = sTo.ReverseFind('\\');
	ASSERT(0 < nCut);
	sTo = sTo.Left(nCut);

	file::CreateDirectoryRecursive(sTo, NULL);

	CString sCommand;
	sCommand.Format(FormatStr("%s Get \"%s\" -V%d -GL\"%s\" >> %s", ss_exe, szVssFile, nVssFileVersion, sTo, szOutputFile));
	RUN(sCommand);
}
