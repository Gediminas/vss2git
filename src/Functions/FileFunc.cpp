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
//	CFileFind ff;
//	return TRUE == ff.FindFile(szFilePath);
	return (GetFileAttributes(szFilePath) != 0xffffffff);
}

bool file::CreateDirectoryRecursive(const CString& sDirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	// _splitpath treats network computer name as directory, don't use it

	int nIdx = sDirName.Find(":");
	if(nIdx == -1)
	{
		nIdx = sDirName.Find("\\\\");
		if(nIdx != -1)
			nIdx = sDirName.Find('\\', nIdx + 2);
	}
	else
		nIdx = sDirName.Find('\\', nIdx + 1);

	const int nLength = sDirName.GetLength();
	if(nIdx == -1 && nLength != 0)
		return false;

	CStringArray arrCreatedDirs;
	int nIdx2(0);
	BOOL bOk = TRUE;

	while(nIdx != -1)
	{
		if(nIdx2 == (nLength - 1)/*ends with slash*/ || nIdx2  == nLength/*ends without slash*/)
			break;

		nIdx2 = sDirName.Find('\\', nIdx + 1);
		if(nIdx2 == -1)
			nIdx2 = nLength; // path doesn't end with slash symbol

		CString sDir = sDirName.Left(nIdx2);
		if(!DoesFileExist(sDir))
		{
			bOk = CreateDirectory(sDir, lpSecurityAttributes);
			if(bOk)
				arrCreatedDirs.Add(sDir);
			else
				break;
		}

		nIdx = nIdx2;
	}

	if(!bOk)
	{
		// cleanup
		const int nSize = arrCreatedDirs.GetSize();
		for(int i = nSize - 1; i >= 0; --i)
		{
			VERIFY(RemoveDirectory(arrCreatedDirs[i]));
		}
	}

	return (bOk == TRUE);
}

bool file::DeleteRecursiveAll(LPCSTR sPath)
{
	ASSERT(FALSE);
//	CStringArray dirs;
//
//	CString sDirFind = sPath;
//	sDirFind += _T("\\*");
//	GetDirNames(sDirFind, dirs);
//
//	int nCnt = dirs.GetSize();
//
//	CString sDelDir;
//	CString sRemoveFile;
//
//	for (int i1 = 0; i1 < nCnt; ++i1)
//	{
//		sDelDir = sPath;
//		sDelDir += _T("\\");
//		sDelDir += dirs[i1];
//
//		if ( !DeleteRecursiveAll(sDelDir) )
//		{
//			return false;
//		}
//
//		DeleteDirFiles(sDelDir);
//
//		RemoveDirectory(sDelDir);
//	}

	return true;
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




