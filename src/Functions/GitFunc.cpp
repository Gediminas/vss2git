#include "StdAfx.h"
#include "GitFunc.h"

#include "Functions/FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void git::CreateDB(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szEmail)
{
	CString sOriginalDir;
	GetCurrentDirectory(2000, sOriginalDir.GetBufferSetLength(2000));
	SetCurrentDirectory(szWorkingDir);
	
	CString sOutputDir = "../Step4_Import.txt";

	CString sCommand;
	sCommand.Format("git init >> %s", sOutputDir);
	RUN(sCommand);

	git::Commit(szOutputDir, szWorkingDir, "1998-10-01 16:00", "Admin", szEmail, 0);

	SetCurrentDirectory(sOriginalDir);
}

void git::Commit(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail, int nNr)
{
	CString sOriginalDir;
	GetCurrentDirectory(2000, sOriginalDir.GetBufferSetLength(2000));
	SetCurrentDirectory(szWorkingDir);
	
	CString sOutputDir = "../Step4_Import.txt";

	CString sCommand;
	sCommand.Format("git add -A >> %s", sOutputDir);
	//sCommand = "git add -A";
	RUN(sCommand);

	sCommand.Format("git config user.name %s >> %s", szUser, sOutputDir);
	//sCommand.Format("git config user.name %s", szUser);
	RUN(sCommand);

	sCommand.Format("git config user.email %s >> %s", szEmail, sOutputDir);
	//sCommand.Format("git config user.email %s", szEmail);
	RUN(sCommand);

	sCommand.Format("git commit -m\"vss2git: %d\" >> %s", nNr, sOutputDir);
	//sCommand.Format("git commit -m\"vss2git: %d\"", nNr);
	RUN(sCommand);

	//sCommand.Format("git commit -m\"vss2git: %d\" >> %s", nNr, sOutputDir);
	//RUN(sCommand);

	//git add -A
	//git config --global user.name "UserNameFromVSS"
	//git config --global user.email "UserNameFromVSS@matrix-software.lt"
	//env GIT_AUTHOR_DATE='Wed Dec 19 15:14:05 203 -0800'git commit -m 'future!'


	SetCurrentDirectory(sOriginalDir);
}
