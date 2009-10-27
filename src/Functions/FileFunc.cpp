#include "StdAfx.h"
#include "FileFunc.h"


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

bool file::DoesFileExist(LPCTSTR szFilePath)
{
	CFileFind ff;
	return TRUE == ff.FindFile(szFilePath);
}

bool file::StartJob(LPCTSTR szFilePath)
{
	printf(">> generating '%s'\n", szFilePath);
	
	if (!file::DoesFileExist(szFilePath) || !file::DoesFileExist(CString(szFilePath) + ".DONE"))
	{
		CleanupJob(szFilePath);
		return false;
	}

	printf(">> already exist\n");
	return true;
}

void file::CleanupJob(LPCTSTR szFilePath)
{
	::DeleteFile(szFilePath);
	::DeleteFile(CString(szFilePath) + ".DONE");
}

void file::MarkJobDone(LPCTSTR szFilePath)
{
	system(FormatStr("ECHO DONE. >> %s.DONE", szFilePath));

	CString sText(' ', 55);
	sText = "\r>> done" + sText + "\n";
	printf(sText);
}




