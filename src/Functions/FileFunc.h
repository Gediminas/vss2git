#ifndef __FileFunc_H__
#define __FileFunc_H__
#pragma once


void RUN(const CString& cmd);
void RUN2(const CString& cmd);


namespace file
{
	bool RunCommand(const CString& cmd, bool bRetrying = false);
	
	bool DoesFileExist(LPCTSTR szFilePath);
	bool CreateDirectoryRecursive(const CString& sDirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	bool DeleteRecursiveAll(LPCSTR sPath);
	
	bool StartJob(LPCTSTR szFilePath, bool bAppend = false);
	void CleanupJob(LPCTSTR szFilePath);
	void MarkJobDone(LPCTSTR szFilePath);
};
//namespace file


#endif //__FileFunc_H__
