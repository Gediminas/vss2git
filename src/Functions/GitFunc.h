#ifndef __GitFunc_H__
#define __GitFunc_H__
#pragma once


namespace git
{
	void CreateDB(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szEmail);
	void Commit(LPCTSTR szOutputDir, LPCTSTR szWorkingDir, LPCTSTR szTime, LPCTSTR szUser, LPCTSTR szEmail, int nNr);
};
//namespace git



#endif //__GitFunc_H__
