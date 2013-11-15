#ifndef __Dest_H__
#define __Dest_H__
#pragma once


namespace destination
{
	void Create(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail, LPCTSTR szComment);
	void Commit(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail, LPCTSTR szComment);

	void GetLastComment(LPCTSTR szWorkingDir, CString &sComment);
	LPCTSTR GetCommentPrefix();
};
//namespace git


#endif //__Dest_H__
