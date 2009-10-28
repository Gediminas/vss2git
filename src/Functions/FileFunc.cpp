#include "StdAfx.h"
#include "FileFunc.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//inline bool RunProgramJono(LPCTSTR szCmdLine, const bool bWaitForExit = true)
//{
//	STARTUPINFO   si;
//	PROCESS_INFORMATION pi;
//
//	ZeroMemory(&si, sizeof(si));
//	si.cb = sizeof(si);
//
//	CString strTotal = szCmdLine;
//
//	const bool bRet = (TRUE == ::CreateProcess(NULL, strTotal.GetBuffer(strTotal.GetLength() + 1), 
//				  NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi));
//
//	if(bWaitForExit)
//	{
//		HANDLE hProc  = pi.hProcess;
//		DWORD dwExitCode = NULL;
//
//		while(::GetExitCodeProcess(hProc, &dwExitCode) && STILL_ACTIVE == dwExitCode)
//			WaitForSingleObject(hProc, 100);
////			::SleepEx(100, FALSE); // sleep for 0.1 sec for checking again
//	}
//
//	return(bRet);
//}


void RUN(const CString& cmd)
{
	system(cmd);
	//file::RunCommand(cmd);
}

bool file::RunCommand(const CString& cmd, bool bRetrying /*=false*/)
{
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = {0};
    si.cb = sizeof(si);

    //numCommands++;

//    if(bDebug)
//        printf("Running [%s]\n", cmd);

    CString tmp = cmd;
    if(FALSE == CreateProcess(NULL, tmp.LockBuffer(), NULL, NULL, TRUE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
    {
        printf("Failed to start %s.  LastError=%d\n", cmd, GetLastError());
        return false;
    }
    tmp.UnlockBuffer();

    ResumeThread(pi.hThread);
    if(WAIT_OBJECT_0 != WaitForSingleObject(pi.hProcess, 300000))
    {
        printf("Error waiting for %s to launch and finish--will continue anyway\n", cmd);
    }
    DWORD exit = 0;
    GetExitCodeProcess(pi.hProcess, &exit);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    if(0 != exit)
    {
        printf("Command failed: %s\n", cmd);
//        if(bRetry && (false == bRetrying))
//        {
//            printf("Will auto-retry in 10 seconds...\n");
//            Sleep(10000);
//            return RunCommand(cmd, true);
//        }

		ASSERT(FALSE);

 //       printf("Press Y to try again, or N to fail\n");
//        char ch = _getch();
//        if(('Y' == ch) || ('y' == ch))
//            return RunCommand(cmd, true);

        return false;
    }

    return true;
}

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




bool file::StartJob(LPCTSTR szFilePath, bool bAppend /*=false*/)
{
	printf(">> generating '%s'\n", szFilePath);

	if (bAppend)
	{
		printf(">> resuming\n");
		return true;
	}
	
	if (!file::DoesFileExist(szFilePath) || !file::DoesFileExist(CString(szFilePath) + ".DONE"))
	{
		CleanupJob(szFilePath);
		return true;
	}

	printf(">> already exist\n");
	return false;
}

void file::CleanupJob(LPCTSTR szFilePath)
{
	::DeleteFile(szFilePath);
	::DeleteFile(CString(szFilePath) + ".DONE");
}

void file::MarkJobDone(LPCTSTR szFilePath)
{
	RUN(FormatStr("ECHO DONE. >> %s.DONE", szFilePath));

	CString sText(' ', 55);
	sText = "\r>> done" + sText + "\n";
	printf(sText);
}




