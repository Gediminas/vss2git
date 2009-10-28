#include "StdAfx.h"
#include "GitFunc.h"

#include "Functions/FileFunc.h"
#include "Functions/Config.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void git::Create(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail)
{
	CString sOriginalDir;
	GetCurrentDirectory(2000, sOriginalDir.GetBufferSetLength(2000));
	SetCurrentDirectory(szWorkingDir);
	
	if (!file::DoesFileExist(".git"))
	{
		CString sOutputDir = szOutputDir;
		sOutputDir.Replace("../", "../../"); //workaround

		CString sCommand;
		sCommand.Format("git init >> %s", sOutputDir);
		RUN(sCommand);

		sCommand.Format("ECHO. >> %s", sOutputDir);
		RUN(sCommand);

		sCommand.Format("ECHO. >> %s", sOutputDir);
		RUN(sCommand);

		git::Commit(szOutputDir, szWorkingDir, szTime, szUser, szEmail, 0);
	}

	SetCurrentDirectory(sOriginalDir);
}

void git::Commit(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail, int nNr)
{
	CString sOriginalDir;
	GetCurrentDirectory(2000, sOriginalDir.GetBufferSetLength(2000));
	SetCurrentDirectory(szWorkingDir);
	
	CString sOutputDir = szOutputDir;
	sOutputDir.Replace("../", "../../"); //workaround

	CString sCommand;
	sCommand.Format("git add -A >> %s", sOutputDir);
	RUN(sCommand);

	sCommand.Format("git config user.name %s >> %s", szUser, sOutputDir);
	RUN(sCommand);

	sCommand.Format("git config user.email %s >> %s", szEmail, sOutputDir);
	RUN(sCommand);

	sCommand.Format("env GIT_AUTHOR_DATE=\"%s 0 %s\" git commit -m 'vss2git: %d' >> %s", szTime, config::szTimeZone, nNr, sOutputDir);
	RUN(sCommand);

	SetCurrentDirectory(sOriginalDir);
}

