#ifndef __FileFunc_H__
#define __FileFunc_H__
#pragma once


namespace file
{
	bool DoesFileExist(LPCTSTR szFilePath);
	
	bool StartJob(LPCTSTR szFilePath);
	void CleanupJob(LPCTSTR szFilePath);
	void MarkJobDone(LPCTSTR szFilePath);
};
//namespace file


#endif //__FileFunc_H__
